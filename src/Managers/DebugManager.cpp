// -----------------------------------------------------------------
// DEBUG MANAGER
// Manage and display debug information
// -----------------------------------------------------------------

#include "DebugManager.h"
#include "WorldManager.h"
#include "../Geometries/GeometryText.h"
#include "../Input/InputEngine.h"
#include "../Display/DisplayEngine.h"
#include "../Fonts/FontEngine.h"
#include "../Data/Parameters.h"
#include "../World/WorldTime.h"
#include "../World/Terrain.h"
#include "../World/WaterArea.h"
#include "../Physics/Polygon.h"

DebugManager * DebugManager::m_pInstance = NULL;

// -----------------------------------------------------------------
// Name : DebugManager
// -----------------------------------------------------------------
DebugManager::DebugManager()
{
    for (int i = 0; i < DBG_MAX_LINES; i++)
    {
        m_pGeometries[i] = NULL;
    }
    m_pFPSGeometry = NULL;
	m_iFontId = INVALID_FONTID;
    m_iCustomInfoNbLines = 0;
    m_bShowFPS = true;

    m_sLogFile = string(PATH_LOGS) + "out.log";
    ofstream log;
    log.open(m_sLogFile.c_str(), ios_base::trunc);
    if (log.is_open()) {
        log.close();
    }
}

// -----------------------------------------------------------------
// Name : ~DebugManager
// -----------------------------------------------------------------
DebugManager::~DebugManager()
{
    for (int i = 0; i < DBG_MAX_LINES; i++) {
        FREE(m_pGeometries[i]);
    }
    FREE(m_pFPSGeometry);
}

// -----------------------------------------------------------------
// Name : init
// -----------------------------------------------------------------
void DebugManager::init()
{
    m_refreshTC.start(0.2f);
}

// -----------------------------------------------------------------
// Name : update
// -----------------------------------------------------------------
void DebugManager::update(double delta)
{
    if (m_bShowFPS) {
        m_refreshTC.update(delta);
        if (m_refreshTC.getState() == TC_DelayReached) {
            float fFps = 999;
            if (delta != 0) {
                fFps = 1 / delta;
            }
            CoordsScreen cs = _input->getCurrentCursorPosition();
            cs.z = BOARDPLANE;
            Coords3D c3 = _display->getBoard3D(cs);
            Coords3D cam = _display->getCamera();
            bool inWater = false;
            for (WaterArea * area : _world->getTerrain()->getWaterAreas()) {
            	if (area->getArea()->isInside(c3)) {
            		inWater = true;
            		break;
            	}
            }

            stringstream ss;
            ss << _time->getTime() << endl;
            ss << "ScreenX=" << cs.x << " ; ScreenY=" << cs.y << endl;
            ss << "3dX=" << c3.x << " ; 3dY=" << c3.y << endl;
            ss << "CamX=" << cam.x << " ; CamY=" << cam.y << " ; CamZ=" << cam.z << endl;
            ss << "In water? " << inWater << endl;
            ss << "FPS : " << fFps << endl;
            if (m_pFPSGeometry == NULL) {
                m_pFPSGeometry = new GeometryText(ss.str(), m_iFontId, VB_Static);
            } else {
                m_pFPSGeometry->setText(ss.str(), m_iFontId);
            }
        }
    }
}

// -----------------------------------------------------------------
// Name : display
// -----------------------------------------------------------------
void DebugManager::display()
{
    int yPxl = 0;
    CoordsScreen coords;
    for (int i = 0; i < m_iCustomInfoNbLines; i++)
    {
        yPxl += 15;
        coords = CoordsScreen(5, yPxl, GUIPLANE);
        m_pGeometries[i]->display(coords, Color::white);
    }
    if (m_bShowFPS && m_pFPSGeometry != NULL)
    {
        coords = CoordsScreen(5, _params->getScreenYSize() - 80, GUIPLANE);
        m_pFPSGeometry->display(coords, Color::white);
    }
}

// -----------------------------------------------------------------
// Name : addCustomeLine
// -----------------------------------------------------------------
void DebugManager::addCustomeLine(string msg)
{
    if (_display->isReady() && IS_VALID_FONTID(m_iFontId))
    {
        if (m_iCustomInfoNbLines < DBG_MAX_LINES - 1)
        {
            m_pGeometries[m_iCustomInfoNbLines++] = new GeometryText(msg, m_iFontId, VB_Static);
        }
        else if (m_iCustomInfoNbLines == DBG_MAX_LINES - 1)
        {
            m_pGeometries[m_iCustomInfoNbLines++] = new GeometryText("Max number of lines reached!", m_iFontId, VB_Static);
        }
    }
    else
    {
        cout << msg << endl;
    }
    log(msg);
}

// -----------------------------------------------------------------
// Name : log
// -----------------------------------------------------------------
void DebugManager::log(string msg)
{
    if (_params->getLogLevel() > 0) {
    	ofstream log;
    	log.open(m_sLogFile.c_str(), ios_base::app);
        if (log.is_open()) {
        	log << msg << endl;
        	log.close();
        } else {
            printf("Can't open log file!\n");
        }
    }
}

// -----------------------------------------------------------------
// Name : clear
// -----------------------------------------------------------------
void DebugManager::clear()
{
    m_iCustomInfoNbLines = 0;
    for (int i = 0; i < DBG_MAX_LINES; i++)
    {
        FREE(m_pGeometries[i]);
    }
}

// -----------------------------------------------------------------
// Name : error
// -----------------------------------------------------------------
void DebugManager::error(string msg)
{
    addCustomeLine(msg);
}

// -----------------------------------------------------------------
// Name : notifyLoadingMessage
// -----------------------------------------------------------------
void DebugManager::notifyLoadingMessage(string msg)
{
	cout << "\t" << msg << endl;
}

// -----------------------------------------------------------------
// Name : registerTextures
// -----------------------------------------------------------------
void DebugManager::registerTextures()
{
    m_iFontId = _font->registerFont("BookAntiqua_16");
}
