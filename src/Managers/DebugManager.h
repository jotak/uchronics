#ifndef _DEBUG_MANAGER_H
#define _DEBUG_MANAGER_H

#include "../Utils/TimeController.h"
#include "../Geometries/GeometryText.h"

#define DBG_MAX_LINES       40
#define DBG_MAX_CHARS       128
#define ERROR_MESSAGE_SIZE  128

class DebugManager
{
public:
    // Destructor
    ~DebugManager();
    static DebugManager * getInstance()
    {
        if (m_pInstance == NULL) m_pInstance = new DebugManager();
        return m_pInstance;
    };

    // Texture(s) registration
    void registerTextures();

    // Manager functions
    void init();
    void display();
    void update(double delta);

    // Other function
    void log(string msg);
    void error(string msg);
    void notifyLoadingMessage(string msg);
    void clear();
    void switchShowFPS()
    {
        m_bShowFPS = !m_bShowFPS;
    };
    void autoStartGame();

private:
    DebugManager();
    static DebugManager * m_pInstance;

    void addCustomeLine(string msg);

    TimeController m_refreshTC;
    GeometryText * m_pFPSGeometry;
    GeometryText * m_pGeometries[DBG_MAX_LINES];
    short m_iCustomInfoNbLines;
    int m_iFontId;
    bool m_bShowFPS;
    string m_sLogFile;
};

// Helper alias
#define _debug DebugManager::getInstance()

#ifdef DEBUG
	#define DEBUG_MSG(level, msg) { if (_params->getLogLevel() >= level) _debug->log(msg); }
#else
	#define DEBUG_MSG(level, msg) {}
#endif

#endif
