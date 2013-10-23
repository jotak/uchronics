// -----------------------------------------------------------------
// MASTER MANAGER
// Gere les managers
// -----------------------------------------------------------------
#include "MasterManager.h"
#include "../Data/LocalisationTool.h"
#include "../Data/Parameters.h"

// Low level (engines)
#include "../Display/DisplayEngine.h"
#include "../Display/TextureEngine.h"
#include "../Fonts/FontEngine.h"
#include "../Input/InputEngine.h"
#include "../Utils/GarbageCollector.h"

// High level (managers)
#include "InterfaceManager.h"
#include "DebugManager.h"
#include "WorldManager.h"

// Other
#include "../World/WorldBuilderTest.h"
#include "../World/Character.h"

// -----------------------------------------------------------------
// Name : MasterManager
// -----------------------------------------------------------------
MasterManager::MasterManager()
{
    m_fRestartGlutTimer = 0.0f;
    m_iOldWidth = -1;
    m_iOldHeight = -1;

	// Create singleton instances
    // Engines
    _gc;
	_display;
	_tex;
	_font;
	_input;

    // Managers
	_interface;
	_debug;
	_world;

    // Other modules
	_params;
    i18n;
    _params->setWinXPos(glutGet(GLUT_WINDOW_X));
    _params->setWinYPos(glutGet(GLUT_WINDOW_Y));

    // Other static stuff
    Character::initData();
}

// -----------------------------------------------------------------
// Name : ~MasterManager
// -----------------------------------------------------------------
MasterManager::~MasterManager()
{
    // Some static stuff
	Character::releaseData();

    delete i18n;
    delete _params;
    delete _world;
    delete _interface;
    delete _input;
    delete _font;
    delete _tex;
    delete _debug;
    delete _display;
    delete _gc;
}

// -----------------------------------------------------------------
// Name : init
// -----------------------------------------------------------------
void MasterManager::init()
{
    // Init modules / managers / engines
	_debug->init();
	_params->load();

	DEBUG_MSG(2, "_display->init");
    _display->init();
	DEBUG_MSG(2, "_font->resetAllFonts");
    _font->resetAllFonts();
	DEBUG_MSG(2, "i18n->init");
    i18n->init();
	DEBUG_MSG(2, "guiObject::registerTextures");
    guiObject::registerTextures();
	DEBUG_MSG(2, "_debug->registerTextures");
    _debug->registerTextures();
	DEBUG_MSG(2, "_interface->init");
    _interface->init();
	DEBUG_MSG(2, "_world->init");
    _world->init(new WorldBuilderTest());
	DEBUG_MSG(2, "_display->setReady");
    _display->setReady();

}

// -----------------------------------------------------------------
// Name : update
// -----------------------------------------------------------------
void MasterManager::update(double delta)
{
	DEBUG_MSG(3, "MasterManager::update");
	DEBUG_MSG(3, "_input->update");
    _input->update(delta);
	DEBUG_MSG(3, "_debug->update");
	_debug->update(delta);
	DEBUG_MSG(3, "_interface->update");
	_interface->update(delta);
	DEBUG_MSG(3, "_world->update");
	_world->update(delta);
    _display->saveWinPos();
    _gc->run();
}

// -----------------------------------------------------------------
// Name : display
// -----------------------------------------------------------------
void MasterManager::display()
{
	DEBUG_MSG(3, "MasterManager::display");

    if (_display->isReady()) {

    	DEBUG_MSG(3, "_display->beginDisplay");
        _display->beginDisplay();
    	DEBUG_MSG(3, "_display->begin3D");
        _display->begin3D();
    	DEBUG_MSG(3, "_world->display");
    	_world->display();
    	DEBUG_MSG(3, "_display->end3D");
        _display->end3D();
    	DEBUG_MSG(3, "_display->begin2D");
        _display->begin2D();
    	DEBUG_MSG(3, "_interface->display");
    	_interface->display();
    	DEBUG_MSG(3, "_debug->display");
    	_debug->display();
    	DEBUG_MSG(3, "_display->end2D");
        _display->end2D();
    	DEBUG_MSG(3, "_display->endDisplay");
        _display->endDisplay();
    }
}

// -----------------------------------------------------------------
// Name : onResize
// -----------------------------------------------------------------
void MasterManager::onResize(int w, int h)
{
    if (w <= 0 || h <= 0) {
        return;
    }
    if (_display->canResize()) {
    	_params->setWinWidth(w);
    	_params->setWinHeight(h);
    	m_fRestartGlutTimer = 1.0f;
    }
}

// -----------------------------------------------------------------
// Name : cleanUp
// -----------------------------------------------------------------
void MasterManager::cleanUp()
{
    _params->save();
}

// -----------------------------------------------------------------
// Name : onKeyboard
// -----------------------------------------------------------------
void MasterManager::onKeyboard(int key, int x, int y, bool bSpecialKey)
{
	if (bSpecialKey) {
	    if (key == 1/*F1*/) {
	        _debug->switchShowFPS();
	    } else {
	    	_input->onSpecialKeyboard(key, x, y);
	    }
	} else {
	    if (key == 127/*ESC*/) {
	        _debug->clear();
	    } else {
	    	_input->onKeyboard(key, x, y);
	    }
	}
}

// -----------------------------------------------------------------
// Name : onMouseClick
// -----------------------------------------------------------------
void MasterManager::onMouseClick(int button, int state, int x, int y)
{
	_input->onMouseClick(button, state, x, y);
}

// -----------------------------------------------------------------
// Name : onMouseMove
// -----------------------------------------------------------------
void MasterManager::onMouseMove(int x, int y)
{
	_input->onMouseMove(x, y);
}

// -----------------------------------------------------------------
// Name : checkRestartGlut
// -----------------------------------------------------------------
void MasterManager::checkRestartGlut(double delta)
{
    if (m_fRestartGlutTimer > 0.0f) {
    	m_fRestartGlutTimer -= (float) delta;
        if (m_fRestartGlutTimer <= 0.0f) {
        	// Do restart
        	m_iOldWidth = _params->getScreenXSize();
        	m_iOldHeight = _params->getScreenYSize();
            _display->initGlutWindow();
            // register glut callbacks again
            extern void registerGlutCallbacks();
            registerGlutCallbacks();
            if (m_iOldWidth > 0) {
                _interface->onResize(m_iOldWidth, m_iOldHeight);
            }
        }
    }
}

