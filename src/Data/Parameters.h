#ifndef _PARAMETERS_H
#define _PARAMETERS_H

#include "../Utils/utils.h"

class Parameters
{
public:
    // Singleton / destructor
    ~Parameters();
    static Parameters * getInstance()
    {
        if (m_pInstance == NULL) m_pInstance = new Parameters();
        return m_pInstance;
    };

    void load();
    void save();

    u16 getScreenXSize() { return m_screenXSize; };
    u16 getScreenYSize() { return m_screenYSize; };
    u16 getWinXPos() { return m_winXPos; };
    u16 getWinYPos() { return m_winYPos; };
    u16 getWinWidth() { return m_winWidth; };
    u16 getWinHeight() { return m_winHeight; };
    bool isFullscreen() { return m_bFullscreen; };
    string getGameModeString() { return m_sGameModeString; };
    int getLogLevel() { return m_iLogLevel; };
    int getGameLogsLifetime() { return m_iGameLogsLifetime; };
    int getSoundVolume() { return m_iSoundVolume; };
    int getMusicVolume() { return m_iMusicVolume; };
    string getLanguage() { return m_sLanguage; };

    void setScreenXSize(u16 w) { m_screenXSize = w; };
    void setScreenYSize(u16 h) { m_screenYSize = h; };
    void setWinXPos(u16 x) { m_winXPos = x; };
    void setWinYPos(u16 y) { m_winYPos = y; };
    void setWinWidth(u16 w) { m_winWidth = w; };
    void setWinHeight(u16 h) { m_winHeight = h; };
    void setFullscreen(bool f) { m_bFullscreen = f; };
    void setGameModeString(string s) { m_sGameModeString = s; };
    void setLogLevel(int l) { m_iLogLevel = l; };
    void setGameLogsLifetime(int l) { m_iGameLogsLifetime = l; };
    void setSoundVolume(int v) { m_iSoundVolume = v; };
    void setMusicVolume(int v) { m_iMusicVolume = v; };
    void setLanguage(string l) { m_sLanguage = l; };

private:
    Parameters();
    static Parameters * m_pInstance;

    u16 m_screenXSize, m_screenYSize;
    u16 m_winXPos, m_winYPos;
    u16 m_winWidth, m_winHeight;
    bool m_bFullscreen;
    string m_sGameModeString;
    string m_sLanguage;
    int m_iLogLevel;
    int m_iGameLogsLifetime;
    int m_iSoundVolume;
    int m_iMusicVolume;
};

// Helper alias
#define _params Parameters::getInstance()

#endif
