// -----------------------------------------------------------------
// PARAMETERS
// -----------------------------------------------------------------
#include "Parameters.h"
#include "../Managers/DebugManager.h"
#include "IniFileWriter.h"
#include "IniFileReader.h"
#include <locale.h>
#include <stdio.h>

Parameters * Parameters::m_pInstance = NULL;

// -----------------------------------------------------------------
// Name : Parameters
// -----------------------------------------------------------------
Parameters::Parameters()
{
    m_iLogLevel = 1;
    m_screenXSize = 800;
    m_screenYSize = 600;
    m_iGameLogsLifetime = -1;
    m_iSoundVolume = 10;
    m_iMusicVolume = 10;
}

// -----------------------------------------------------------------
// Name : ~Parameters
// -----------------------------------------------------------------
Parameters::~Parameters()
{
}

// -----------------------------------------------------------------
// Name : load
// -----------------------------------------------------------------
void Parameters::load()
{
    IniFileReader reader;
    if (reader.read(string("config.ini"))) {
        m_bFullscreen = reader.findBoolValue("Fullscreen", false);
        m_sGameModeString = reader.findCharValue("GameModeString", "1280x1024:32");
        m_winWidth = reader.findIntValue("WindowWidth", 1024);
        m_winHeight = reader.findIntValue("WindowHeight", 768);
        m_winXPos = reader.findIntValue("WindowX", 50);
        m_winYPos = reader.findIntValue("WindowY", 50);
        m_sLanguage = reader.findCharValue("Language", "EN");
        m_iLogLevel = reader.findIntValue("LogLevel", 1);
        m_iGameLogsLifetime = reader.findIntValue("GameLogsLifetime", -1);
        m_iSoundVolume = reader.findIntValue("SoundVolume", 10);
        m_iMusicVolume = reader.findIntValue("MusicVolume", 10);
    } else {
        _debug->error("Can't read file config.ini");
    }
}

// -----------------------------------------------------------------
// Name : save
// -----------------------------------------------------------------
void Parameters::save()
{
    IniFileWriter writer;
    writer.set("Fullscreen", m_bFullscreen);
    writer.set("GameModeString", m_sGameModeString);
    writer.set("WindowWidth", m_winWidth);
    writer.set("WindowHeight", m_winHeight);
    writer.set("WindowX", m_winXPos);
    writer.set("WindowY", m_winYPos);
    writer.set("Language", m_sLanguage);
    writer.set("LogLevel", m_iLogLevel);
    writer.set("GameLogsLifetime", m_iGameLogsLifetime);
    writer.set("SoundVolume", m_iSoundVolume);
    writer.set("MusicVolume", m_iMusicVolume);

    if (!writer.write("config.ini")) {
        _debug->error("Can't write config.ini");
    }
}
