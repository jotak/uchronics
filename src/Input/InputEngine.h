#ifndef _INPUT_ENGINE_H
#define _INPUT_ENGINE_H

#include "EventListener.h"
#include "KeyboardListener.h"
#include <stack>

#define SPECKEY_HOME    1
#define SPECKEY_END     2
#define SPECKEY_UP      3
#define SPECKEY_DOWN    4
#define SPECKEY_LEFT    5
#define SPECKEY_RIGHT   6

enum ButtonEvent
{
    Event_None = 0,
    Event_Down,
    Event_Up,
    Event_Click,
    Event_DoubleClick,
    Event_Drag
};

enum InputButton
{
    Button1 = 0,
    Button2,
    ButtonZ,
    ButtonX,
    // From this point, ButtonStart, only put "uncursored" buttons
    ButtonStart,
    ButtonLeft,
    ButtonRight,
    ButtonUp,
    ButtonDown,
    ButtonNext,
    ButtonPrev,
    ButtonBack,
    NbButtons
};

#define MAX_CURSORED_EVENT_LISTENERS     2

class ButtonAction
{
public:
    InputButton eButton;
    ButtonEvent eEvent;
    int xPos, yPos;
    int xPosInit, yPosInit;
    int xOffset, yOffset;
    unsigned long dragDistance;
    double clickTime;
    EventListener * pTargetListener;
};

class DebugManager;

class InputEngine
{
public:
    // Singleton / destructor
    ~InputEngine();
    static InputEngine * getInstance()
    {
        if (m_pInstance == NULL) m_pInstance = new InputEngine();
        return m_pInstance;
    };

    void update(double delta);
    CoordsScreen getCurrentCursorPosition();
    void addCursoredEventListener(EventListener * pListener);
    void removeCursoredEventListener(EventListener * pListener);
    void pushUncursoredEventListener(EventListener * pListener);
    EventListener * popUncursoredEventListener();

    // Listeners
    void setKeyboardListener(KeyboardListener * pKeyboardListener)
    {
        m_pKeyboardListener = pKeyboardListener;
    };
    void unsetKeyboardListener(KeyboardListener * pKeyboardListener)
    {
        if (m_pKeyboardListener == pKeyboardListener) m_pKeyboardListener = NULL;
    };

    // Data providing functions
    bool isShiftPressed();
    bool isCtrlPressed();

    // Input glut functions (must only be called from lowlevel)
    void onKeyboard(unsigned char key, int x, int y);
    void onSpecialKeyboard(int key, int x, int y);
    void onMouseClick(int button, int state, int x, int y);
    void onMouseMove(int x, int y);

private:
    InputEngine();
    static InputEngine * m_pInstance;

    void onPressButton(InputButton eButton);
    void onReleaseButton(InputButton eButton);
    void onAnalogicMove(int x, int y);
    void submitEvent(int iEvent);

    ButtonAction m_AllButtons[12];
    EventListener * m_pCursoredEventListeners[MAX_CURSORED_EVENT_LISTENERS];  // Event listener that will be chosen function of cursor position
    std::stack<EventListener*> m_pUncursoredEventListeners;                   // Event listener that will be chosen regardless cursor position
    int m_iCursorX, m_iCursorY;
    KeyboardListener * m_pKeyboardListener;
    int m_iModifiers;
    unsigned char m_cZ, m_cX, m_cN, m_cB; // variable according to keyboard layout
};

// Helper alias
#define _input InputEngine::getInstance()

#endif
