#ifndef _CHARACTER_H
#define _CHARACTER_H

#include "MovingObject.h"
#include "../Display/DisplayEngine.h"
#include "../../../joson/JoSon.h"
#include "InventoryObject.h"

#define TRAIT_MAX_VALUE			5

#define INVENTORY_TYPE_FOOD		"food"

//	"speed" stands for the speed characteristic; it must be relativised to board metrics
#define SPEED_CONVERT(s)	(2.0f + (double)s / 2.0f)

class Character : public MovingObject
{
public:
	Character(const JoS_Element& json);
    virtual ~Character();

    virtual string toString() const;
    virtual void update(double delta);

    virtual bool isCharacter() { return true; };

    // Getters / setters
    long_hash * getTraits() { return &mapTraits; };
    string getName() { return name; };
    double getSpeed() { return speed; };
    void setSpeed(double speed) { this->speed = speed; };
    double getLife() { return life; };
    void setLife(double life) { this->life = life; };
    double getMaxLife() { return maxLife; };
    void setMaxLife(double maxlife) { this->maxLife = maxLife; };
    double getEnergy() { return energy; };
    void setEnergy(double energy) { this->energy = energy; };
    double getMaxEnergy() { return maxEnergy; };
    void setMaxEnergy(double maxEnergy) { this->maxEnergy = maxEnergy; };
    bool canMove() { return bCanMove; };
    void setCanMove(bool bCanMove) { this->bCanMove = bCanMove; };
    list<InventoryObject*>& getInventory() { return inventory; };

    // Gameplay methods
    void say(string);
    void goToGround(f3d);
    void goToGround(double, double);
    virtual bool isMoving() { return MovingObject::isMoving() || bHasMoveTarget; };
    bool hasMoveTarget() { return bHasMoveTarget; };
    int getLifePlusEnergy() { return life + energy; };
    int restoreLife(int);
    int restoreEnergy(int);
    int restoreLifeOrEnergy(int);
    int loseEnergy(int);
    void addToInventory(InventoryObject*);
    bool isHungry();
    bool isStarving();
    bool isTired();
    void resetHungryState() { hungryWorldTimer = -1; };

    // Static stuff
    static void initData();
    static void releaseData();
    static JoS_Element& getCommonDialogs(string name);
    static float getTraitsRelation(string from, string towards);

protected:
    static JoSon * TraitsRelations;

private:
    void doMove(double);
    void checkHungry();

    string name;
    int speed;	// arbitrary scale; convert it to display/physic engine using macro "SPEED_CONVERT"

    /*
     * About life and energy:
     * Total hit points = life + energy.
     * When tired or starving, character loses energy ; not life
     * When hitten, character loses first energy, then life. Energy is like a shield.
     * When eating or sleeping, character regains energy ; not life
     * To gain life, the character must be healed (potion, medicine, etc.)
     */
    int life;
    int maxLife;
    int energy;
    int maxEnergy;

    long_hash mapTraits;
    bool bCanMove;
    bool bHasMoveTarget;
    f3d moveTarget;
    list<InventoryObject*> inventory;
    long hungryWorldTimer;
    double hungryRealTimer;	// just not to check hungryness at each frame
};

#endif
