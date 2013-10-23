// -----------------------------------------------------------------
// WORLD BUILDER TEST
// Construit le monde de test
// -----------------------------------------------------------------
#include "WorldBuilderTest.h"
#include "../Managers/WorldManager.h"
#include "../Display/DisplayEngine.h"
#include "../Geometries/GeometryQuads.h"
#include "../Geometries/ModColorize.h"
#include "../AI/Actions/ErraticMoveAction.h"
#include "../AI/AI.h"
#include "../Managers/DebugManager.h"

// -----------------------------------------------------------------
// Name : WorldBuilderTest
// -----------------------------------------------------------------
WorldBuilderTest::WorldBuilderTest()
{
}

// -----------------------------------------------------------------
// Name : ~WorldBuilderTest
// -----------------------------------------------------------------
WorldBuilderTest::~WorldBuilderTest()
{
}

// -----------------------------------------------------------------
// Name : init
// -----------------------------------------------------------------
void WorldBuilderTest::build(Terrain * pTerrain, list<GameObject*> * pGameObjects)
{
    QuadData quad(0.0f, 10.0f, 0.0f, 10.0f, string("CrackedTerrainTexture"));
    GeometryQuads * pGeometry = new GeometryQuads(&quad, VB_Static);
    pTerrain->addGeometry(pGeometry);
    pTerrain->setDimensions(0, 10, 0, 10);
    pTerrain->addWaterArea(new WaterArea(
    		Polygon::builder()
    			->addPoint(F3DBOARD(7.1, 2.9))
    			->addPoint(F3DBOARD(6.85, 2.5))
    			->addPoint(F3DBOARD(6.85, 1.6))
    			->addPoint(F3DBOARD(7.2, 1.3))
    			->addPoint(F3DBOARD(7.9, 1.3))
    			->addPoint(F3DBOARD(8, 1.6))
    			->addPoint(F3DBOARD(8, 2.7))
    			->addPoint(F3DBOARD(7.5, 2.9))
    			->close()
    , 0.4f/*fishingProbability: % chances per minute*/, 75/*troutsPct*/, 25/*carpsPct*/));

	string sError;
	JoSon * json = JoSon::fromString(string("{speed:3, traits:{friendly:2, funny:1}}"), &sError);
	if (json == NULL) {
		_debug->error(sError);
	} else {
		Character * pCharacter = new Character(*json);
		QuadData quad2(0.0f, 0.5f, 0.0f, 0.5f, string("heroe"));
		pGeometry = new GeometryQuads(&quad2, VB_Static);
		pGeometry->setOffset(CoordsScreen(280, 70));
		pCharacter->setGeometry(pGeometry);
		pCharacter->setPosition(F3DBOARD(2, 2));
		pGameObjects->push_back(pCharacter);
		_world->setActiveCharacter(pCharacter);
	}

	createAI(string("jophur.json"), pGameObjects);
	createAI(string("dude1.json"), pGameObjects);
	createAI(string("dude2.json"), pGameObjects);

	_display->moveCameraTo(f3d(4, 4, 6));
}

// -----------------------------------------------------------------
// Name : createAI
// -----------------------------------------------------------------
void WorldBuilderTest::createAI(string fileJson, list<GameObject*> * pGameObjects)
{
	AI * pAI = AI::buildAI(fileJson);
	if (pAI != NULL) {
	    QuadData quad2(0.0f, 0.5f, 0.0f, 0.5f, string("heroe"));
		GeometryQuads * pGeometry = new GeometryQuads(&quad2, VB_Static);
		pGeometry->setOffset(CoordsScreen(280, 70));
		pGeometry->bindModifier(new ModColorize(Color::red));
		pGeometry->bindModifier(new ModColorize(Color::green));
		pAI->setGeometry(pGeometry);
		pAI->setPosition(F3DBOARD(2 + rand() % 2, 2 + rand() % 2));
		pGameObjects->push_back(pAI);
		cout << "AI created: " << *pAI << endl;
	}
}
