// -----------------------------------------------------------------
// CHARACTER
// -----------------------------------------------------------------
#include "Character.h"
#include "../Physics/MovesHelper.h"
#include "../Managers/DebugManager.h"
#include "../Data/JSonUtil.h"
#include "../World/WorldTime.h"

JoSon * Character::TraitsRelations = NULL;

#define HUNGRY_LOSE_ENERGY_EVERY_X_HOURS		3
#define DELTA_HUNGRY_CHECK						5.0f

// -----------------------------------------------------------------
// Name : Character
// -----------------------------------------------------------------
Character::Character(const JoS_Element &json)
{
	name = JSonUtil::getString(json["name"], "Dude");
	speed = JSonUtil::getInt(json["speed"], 1);
	maxLife = life = JSonUtil::getInt(json["life"], 1);
	maxEnergy = energy = JSonUtil::getInt(json["energy"], 1);

	const JoS_Element& jsTraits = json["traits"];
	int nbTraits = (*TraitsRelations)["_list_"].size();
	for (int i = 0; i < nbTraits; i++) {
		string trait = (*TraitsRelations)["_list_"][i].toString();
		mapTraits[trait] = JSonUtil::getCappedInt(jsTraits[trait], 0, TRAIT_MAX_VALUE);
	}

    bCanMove = true;
    bHasMoveTarget = false;
    resetHungryState();
    hungryRealTimer = 0;
}

// -----------------------------------------------------------------
// Name : ~Character
// -----------------------------------------------------------------
Character::~Character()
{
	FREEVEC(inventory);
}

// -----------------------------------------------------------------
// Name : toString
// -----------------------------------------------------------------
string Character::toString() const {
	std::ostringstream oss;
	oss << GameObject::toString();
	oss << " | speed: " << speed;
	for (pair<string, long> entry : mapTraits) {
		if (entry.second != 0) {
			oss << " | " << entry.first << ": " << entry.second;
		}
	}
	return oss.str();
}

// -----------------------------------------------------------------
// Name : update
// -----------------------------------------------------------------
void Character::update(double delta)
{
	MovingObject::update(delta);
	if (bHasMoveTarget && bCanMove) {
		doMove(delta * SPEED_CONVERT(speed));
	}
	if (hungryRealTimer <= 0) {
		checkHungry();
		hungryRealTimer = DELTA_HUNGRY_CHECK;
	} else {
		hungryRealTimer -= delta;
	}
}

// -----------------------------------------------------------------
// Name : doMove
// -----------------------------------------------------------------
void Character::doMove(double step)
{
	// TODO: pathfinder
    f3d pos = getPosition();
    f3d globalMove = moveTarget - pos;
    double distanceToGoal = globalMove.getSize();
    if (distanceToGoal <= step) {
    	// Character has arrived
    	setPosition(moveTarget);
    	bHasMoveTarget = false;
    } else {
    	setPosition(pos + globalMove.getUnitVector(distanceToGoal) * step);
    }
}

// -----------------------------------------------------------------
// Name : goToGround
// -----------------------------------------------------------------
void Character::goToGround(f3d pos2D)
{
	bHasMoveTarget = true;
	moveTarget = pos2D;
	moveTarget.z = BOARDPLANE;
}

// -----------------------------------------------------------------
// Name : goToGround
// -----------------------------------------------------------------
void Character::goToGround(double x, double y)
{
	bHasMoveTarget = true;
	moveTarget = f3d(x, y, BOARDPLANE);
}

// -----------------------------------------------------------------
// Name : say
// -----------------------------------------------------------------
void Character::say(string sentence)
{
	cout << name << ": " << sentence << endl;
}

// -----------------------------------------------------------------
// Name : restoreLife
// 	Returns unused points
// -----------------------------------------------------------------
int Character::restoreLife(int points)
{
	int usedPoints = min(points, maxLife - life);
	if (usedPoints > 0) {
		life += usedPoints;
		cout << name << "'s life is now " << life << endl;
	}
	return points - usedPoints;
}

// -----------------------------------------------------------------
// Name : restoreEnergy
// 	Returns unused points
// -----------------------------------------------------------------
int Character::restoreEnergy(int points)
{
	int usedPoints = min(points, maxEnergy - energy);
	if (usedPoints > 0) {
		energy += usedPoints;
		cout << name << "'s energy is now " << energy << endl;
	}
	return points - usedPoints;
}

// -----------------------------------------------------------------
// Name : restoreLifeOrEnergy
// 	Returns unused points
// -----------------------------------------------------------------
int Character::restoreLifeOrEnergy(int points)
{
	int unused = restoreLife(points);
	if (unused > 0) {
		unused = restoreEnergy(unused);
	}
	return unused;
}

// -----------------------------------------------------------------
// Name : loseEnergy
// 	Returns unused points
// -----------------------------------------------------------------
int Character::loseEnergy(int points)
{
	int unused = 0;
	if (energy > points) {
		energy -= points;
		cout << name << "'s energy is now " << energy << endl;
	} else if (energy > 0) {
		unused = points - energy;
		energy = 0;
		cout << name << "'s energy is now 0" << endl;
	}
	return unused;
}

// -----------------------------------------------------------------
// Name : addToInventory
// -----------------------------------------------------------------
void Character::addToInventory(InventoryObject * obj)
{
	inventory.push_back(obj);
	say("I've found a " + obj->getName());
}

// -----------------------------------------------------------------
// Name : isHungry
//	Character is hungry when he lost more than 50% of its energy
// -----------------------------------------------------------------
bool Character::isHungry()
{
	return (energy * 2 <= maxEnergy);
}

// -----------------------------------------------------------------
// Name : isStarving
//	0 energy
// -----------------------------------------------------------------
bool Character::isStarving()
{
	return (energy == 0);
}

// -----------------------------------------------------------------
// Name : isTired
//	Character is hungry when he lost more than 50% of its energy
//	(this is the same as "isHungry" for now... we'll have to see if it's ok with the gameplay...)
// -----------------------------------------------------------------
bool Character::isTired()
{
	return (energy * 2 <= maxEnergy);
}

// -----------------------------------------------------------------
// Name : checkHungry
// -----------------------------------------------------------------
void Character::checkHungry()
{
	// Get "now - hungryTimer"
	long nowSeconds = _time->getTime().getSecondsBased();

	if (hungryWorldTimer < 0) {
		hungryWorldTimer = nowSeconds;
	} else {
		if (nowSeconds - hungryWorldTimer > SECONDS_PER_MINUTE * MINUTES_PER_HOUR * HUNGRY_LOSE_ENERGY_EVERY_X_HOURS) {
			// More than X hours
			loseEnergy(1);
			hungryWorldTimer = nowSeconds;
		}
	}
}

// -----------------------------------------------------------------
// Name : initData
//	static
// -----------------------------------------------------------------
void Character::initData()
{
    // Build file name
    string jsonDesc = string(AI_PATH) + "traitsRelations.json";
    string err;
    TraitsRelations = JoSon::fromFile(jsonDesc, &err);
    if (TraitsRelations == NULL) {
    	_debug->error(err);
    }
}

// -----------------------------------------------------------------
// Name : releaseData
//	static
// -----------------------------------------------------------------
void Character::releaseData()
{
	FREE(TraitsRelations);
}

// -----------------------------------------------------------------
// Name : getTraitsRelation
// Relation is not comutable: getTraitsRelation(A, B) != getTraitsRelation(B, A)
//	static
// -----------------------------------------------------------------
float Character::getTraitsRelation(string from, string towards)
{
	if (TraitsRelations != NULL && !(*TraitsRelations)[from][towards].isNull()) {
		float value = (float) (*TraitsRelations)[from][towards].toDouble();
		// Cap value in [-1,1]
		if (value > 1.0f) {
			value = 1.0f;
		} else if (value < -1.0f) {
			value = -1.0f;
		}
		return value;
	}
	// Value not found => neutral relation
	return 0.0f;
}
