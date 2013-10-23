// main.cpp : définit le point d'entrée pour l'application console.
//
#ifdef linux
#include "../Utils/utils.h"
#include "../Managers/MasterManager.h"
#include <ctime>

#define UNIT_TEST

#ifdef UNIT_TEST
#include "../Tests/Test.h"
#endif

MasterManager * g_pMaster = NULL;
clock_t g_iLastUpdateTime = 0;

// glut callbacks / system callbacks
void registerGlutCallbacks();
void cleanUp();

int main(int argc, char *argv[])
{
    // First init functions
    glutInit(&argc, argv);
    atexit(cleanUp);
    g_iLastUpdateTime = clock();
    srand(time(NULL));

#ifdef UNIT_TEST
    runAllTests();
#endif

    // Game root creation
    g_pMaster = new MasterManager();
    g_pMaster->init();

    // register glut callbacks
    registerGlutCallbacks();

    // Start
    glutMainLoop();

    return EXIT_SUCCESS;
}

//**************************************************************************
// GLUT/system CALLBACKS FUNCTIONS
//**************************************************************************

void glutResize(int w, int h)
{
	g_pMaster->onResize(w, h);
}

void glutDisplay()
{
	g_pMaster->display();
}

void cleanUp()
{
	g_pMaster->cleanUp();
	delete g_pMaster;
}

void glutKeyboard(unsigned char key, int x, int y)
{
	g_pMaster->onKeyboard(key, x, y, false);
}

void glutSpecialKeyboard(int key, int x, int y)
{
	g_pMaster->onKeyboard(key, x, y, true);
}

void glutMouseClick(int button, int state, int x, int y)
{
    g_pMaster->onMouseClick(button, state, x, y);
}

void glutMouseMove(int x, int y)
{
	g_pMaster->onMouseMove(x, y);
}

void glutIdle()
{
    clock_t now = clock();
    // Notes: in case of troubles with CLOCKS_PER_SEC not recognized, try to change indexing order in your IDE to put ctime first
    // Example Eclipse CDT: Window > Preferences > C/C++ > Indexer > Files to index up-front
    // Example Code::Blocks
    double delta = (double)(now - g_iLastUpdateTime) / (double)CLOCKS_PER_SEC;
    if (delta > 0) {
    	g_pMaster->update(delta);
    	glutPostRedisplay();
    	g_iLastUpdateTime = now;
    	g_pMaster->checkRestartGlut(delta);
    }
}

void registerGlutCallbacks()
{
    glutReshapeFunc(glutResize);
    glutDisplayFunc(glutDisplay);
    glutIdleFunc(glutIdle);
    glutKeyboardFunc(glutKeyboard);
    glutSpecialFunc(glutSpecialKeyboard);
    glutMouseFunc(glutMouseClick);
    glutMotionFunc(glutMouseMove);
    glutPassiveMotionFunc(glutMouseMove);
}

#endif
