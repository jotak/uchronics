// This class provides an interface to inputs. It must be overriden by subclasses,
//  which will deal with specific controllers (PC, Wii, PS3 etc.)
#include "InputEngine.h"
#include "../Managers/DebugManager.h"

#define DOUBLECLICK_DETECT_TIME       0.2f  // seconds
#define DOUBLECLICK_MAX_MOVE          5     // pixels (if the mouse moves 5 pixels or more after the first click, then double-click in ignored)

InputEngine * InputEngine::m_pInstance = NULL;

// -----------------------------------------------------------------
// Name : InputEngine
// Constructor
// -----------------------------------------------------------------
InputEngine::InputEngine()
{
	m_pKeyboardListener = NULL;
    m_iCursorX = m_iCursorY = 100;

    for (int i = 0; i < NbButtons; i++) {
        m_AllButtons[i].eButton = (InputButton)i;
        m_AllButtons[i].eEvent = Event_None;
        m_AllButtons[i].clickTime = 0;
        m_AllButtons[i].xPos = 0;
        m_AllButtons[i].yPos = 0;
        m_AllButtons[i].xPosInit = 0;
        m_AllButtons[i].yPosInit = 0;
        m_AllButtons[i].xOffset = 0;
        m_AllButtons[i].yOffset = 0;
        m_AllButtons[i].dragDistance = 0;
        m_AllButtons[i].pTargetListener = NULL;
    }

    for (int i = 0; i < MAX_CURSORED_EVENT_LISTENERS; i++) {
        m_pCursoredEventListeners[i] = NULL;
    }

    m_cZ = 'w'; // TODO : do it better
    m_cX = 'x';
    m_cN = 'n';
    m_cB = 'b';
    m_iModifiers = 0;
}

// -----------------------------------------------------------------
// Name : ~InputEngine
// Destructor
// -----------------------------------------------------------------
InputEngine::~InputEngine()
{
    while (!m_pUncursoredEventListeners.empty()) {
        m_pUncursoredEventListeners.pop();
    }
}

// -----------------------------------------------------------------
// Name : addCursoredEventListener
// -----------------------------------------------------------------
void InputEngine::addCursoredEventListener(EventListener * pListener)
{
    for (int i = 0; i < MAX_CURSORED_EVENT_LISTENERS; i++) {
        if (m_pCursoredEventListeners[i] == NULL) {
            m_pCursoredEventListeners[i] = pListener;
            return;
        }
    }
    // No free slot for listener found => error
    _debug->error("InputEngine error: max number of EventListener cursored reached.");
}

// -----------------------------------------------------------------
// Name : removeCursoredEventListener
// -----------------------------------------------------------------
void InputEngine::removeCursoredEventListener(EventListener * pListener)
{
    for (int i = 0; i < MAX_CURSORED_EVENT_LISTENERS; i++) {
        if (m_pCursoredEventListeners[i] == pListener) {
            m_pCursoredEventListeners[i] = NULL;
            return;
        }
    }
}

// -----------------------------------------------------------------
// Name : pushUncursoredEventListener
// -----------------------------------------------------------------
void InputEngine::pushUncursoredEventListener(EventListener * pListener)
{
    m_pUncursoredEventListeners.push(pListener);
    for (int i = (int) ButtonStart; i < (int) NbButtons; i++) {
        m_AllButtons[i].pTargetListener = pListener;
    }
}

// -----------------------------------------------------------------
// Name : popUncursoredEventListener
// -----------------------------------------------------------------
EventListener * InputEngine::popUncursoredEventListener()
{
    if (m_pUncursoredEventListeners.size() == 0) {
        return NULL;
    }
    EventListener * p = m_pUncursoredEventListeners.top();
    m_pUncursoredEventListeners.pop();
    EventListener * p2 = (m_pUncursoredEventListeners.size() > 0) ? m_pUncursoredEventListeners.top() : NULL;
    for (int i = (int) ButtonStart; i < (int) NbButtons; i++) {
        m_AllButtons[i].pTargetListener = p2;
    }
    return p;
}

// -----------------------------------------------------------------
// Name : update
// -----------------------------------------------------------------
void InputEngine::update(double delta)
{
    // Submit mouse move event
    submitEvent(-1);

    for (int i = 0; i < NbButtons; i++) {
        if (m_AllButtons[i].eEvent == Event_Click) {
            m_AllButtons[i].clickTime += delta;
            if (m_AllButtons[i].clickTime >= DOUBLECLICK_DETECT_TIME
                    || std::abs(m_iCursorX - m_AllButtons[i].xPosInit) > DOUBLECLICK_MAX_MOVE
                    || std::abs(m_iCursorY - m_AllButtons[i].yPosInit) > DOUBLECLICK_MAX_MOVE) {
                submitEvent(i);
                m_AllButtons[i].eEvent = Event_None;
                if (i < (int) ButtonStart) {
                    m_AllButtons[i].pTargetListener = NULL;
                }
            }
        }
    }
}

// -----------------------------------------------------------------
// Name : submitEvent
// -----------------------------------------------------------------
void InputEngine::submitEvent(int iEvent)
{
    if (iEvent >= 0 && m_AllButtons[iEvent].pTargetListener != NULL) {
        m_AllButtons[iEvent].pTargetListener->onCatchButtonEvent(&(m_AllButtons[iEvent]));
        return;
    } else if (iEvent >= (int) ButtonStart) {
        return;
    }

    for (int iPriority = LISTENER_MAX_PRIORITY; iPriority >= 0; iPriority--) {
        for (int i = 0; i < MAX_CURSORED_EVENT_LISTENERS; i++) {
            if (m_pCursoredEventListeners[i] != NULL
                    && m_pCursoredEventListeners[i]->m_iEventListenerPriority == iPriority) {
                if (iEvent < 0) { // cursor move
                    if (m_pCursoredEventListeners[i]->onCursorMoveEvent(m_iCursorX, m_iCursorY)) {
                        return;
                    }
                } else {
                    if (m_pCursoredEventListeners[i]->onCatchButtonEvent(&(m_AllButtons[iEvent]))) {
                        m_AllButtons[iEvent].pTargetListener = m_pCursoredEventListeners[i];
                        return;
                    }
                }
            }
        }
    }
}

// -----------------------------------------------------------------
// Name : getCurrentCursorPosition
// -----------------------------------------------------------------
CoordsScreen InputEngine::getCurrentCursorPosition()
{
    return CoordsScreen(m_iCursorX, m_iCursorY);
}

// -----------------------------------------------------------------
// Name : onPressButton
// -----------------------------------------------------------------
void InputEngine::onPressButton(InputButton eButton)
{
    // If we had previously clicked AND if we are still in time THEN it's a double click
    if (m_AllButtons[eButton].eEvent == Event_Click
    		&& (m_AllButtons[eButton].clickTime < DOUBLECLICK_DETECT_TIME)) {

        m_AllButtons[eButton].eEvent = Event_DoubleClick;
        m_AllButtons[eButton].xPos = m_iCursorX;
        m_AllButtons[eButton].yPos = m_iCursorY;
        submitEvent(eButton);
        m_AllButtons[eButton].eEvent = Event_None;
        if (eButton < ButtonStart) {
            m_AllButtons[eButton].pTargetListener = NULL;
        }
    } else {
        // In every other cases, deal it as a new mouse down
        m_AllButtons[eButton].eEvent = Event_Down;
        m_AllButtons[eButton].xPosInit = m_AllButtons[eButton].xPos = m_iCursorX;
        m_AllButtons[eButton].yPosInit = m_AllButtons[eButton].yPos = m_iCursorY;
        m_AllButtons[eButton].xOffset = 0;
        m_AllButtons[eButton].yOffset = 0;
        submitEvent(eButton);
        // And init drag data
        m_AllButtons[eButton].dragDistance = 0;
        m_AllButtons[eButton].eEvent = Event_Drag;
    }
}

// -----------------------------------------------------------------
// Name : onReleaseButton
// -----------------------------------------------------------------
void InputEngine::onReleaseButton(InputButton eButton)
{
    m_AllButtons[eButton].xPos = m_iCursorX;
    m_AllButtons[eButton].yPos = m_iCursorY;

    // If we have dragged for a negligeable distance, we consider it's a click or a future double-click
    if (m_AllButtons[eButton].eEvent == Event_Drag && m_AllButtons[eButton].dragDistance < 8) {
        m_AllButtons[eButton].clickTime = 0;
        m_AllButtons[eButton].eEvent = Event_Up;
        submitEvent(eButton);
        // Prepare for click
        m_AllButtons[eButton].eEvent = Event_Click;
    } else {
        // Else, we just reset data.
        if (m_AllButtons[eButton].eEvent != Event_DoubleClick) {
            m_AllButtons[eButton].eEvent = Event_Up;
            submitEvent(eButton);
        }
        m_AllButtons[eButton].eEvent = Event_None;
        if (eButton < ButtonStart) {
            m_AllButtons[eButton].pTargetListener = NULL;
        }
    }
}

// -----------------------------------------------------------------
// Name : onAnalogicMove
// -----------------------------------------------------------------
void InputEngine::onAnalogicMove(int x, int y)
{
    // Update mouse position
    m_iCursorX = x;
    m_iCursorY = y;

    // Check drags
    for (int i = 0; i < NbButtons; i++) {
        if (m_AllButtons[i].eEvent == Event_Drag) {
            m_AllButtons[i].dragDistance += std::abs(m_AllButtons[i].xPos - x) + std::abs(m_AllButtons[i].yPos - y);
            m_AllButtons[i].xPosInit = m_AllButtons[i].xPos;
            m_AllButtons[i].yPosInit = m_AllButtons[i].yPos;
            m_AllButtons[i].xPos = x;
            m_AllButtons[i].yPos = y;
            submitEvent(i);
        }
    }
}

// -----------------------------------------------------------------
// Name : onKeyboard
// -----------------------------------------------------------------
void InputEngine::onKeyboard(unsigned char key, int x, int y)
{
    m_iModifiers = glutGetModifiers();
    if (m_pKeyboardListener != NULL) {
        if (m_pKeyboardListener->onKeyDown(key)) {
            return;
        }
    }
    InputButton eButton;
    if (key == '\n' || key == 13) {
        eButton = ButtonStart;
    } else if (key == 27) {
        eButton = ButtonBack;
    } else if (key == m_cZ) {
        eButton = ButtonZ;
    } else if (key == m_cX) {
        eButton = ButtonX;
    } else if (key == m_cN) {
        eButton = ButtonNext;
    } else if (key == m_cB) {
        eButton = ButtonPrev;
    } else {
    	// This button isn't used
        return;
    }
    onPressButton(eButton);
    onReleaseButton(eButton);
}

// -----------------------------------------------------------------
// Name : onSpecialKeyboard
// -----------------------------------------------------------------
void InputEngine::onSpecialKeyboard(int key, int x, int y)
{
    m_iModifiers = glutGetModifiers();
    InputButton eButton = (InputButton)0;
    int speckey = -1;
    switch (key)
    {
    case GLUT_KEY_LEFT:
        eButton = ButtonLeft;
        speckey = SPECKEY_LEFT;
        break;
    case GLUT_KEY_RIGHT:
        eButton = ButtonRight;
        speckey = SPECKEY_RIGHT;
        break;
    case GLUT_KEY_UP:
        eButton = ButtonUp;
        speckey = SPECKEY_UP;
        break;
    case GLUT_KEY_DOWN:
        eButton = ButtonDown;
        speckey = SPECKEY_DOWN;
        break;
    case GLUT_KEY_HOME:
        speckey = SPECKEY_HOME;
        break;
    case GLUT_KEY_END:
        speckey = SPECKEY_END;
        break;
    }
    if (m_pKeyboardListener != NULL && speckey != -1) {
        if (m_pKeyboardListener->onSpecialKeyDown(speckey)) {
            return;
        }
    }
    if (eButton != (InputButton)0) {
        onPressButton(eButton);
        onReleaseButton(eButton);
    }
}

// -----------------------------------------------------------------
// Name : onMouseClick
// -----------------------------------------------------------------
void InputEngine::onMouseClick(int button, int state, int x, int y)
{
    m_iModifiers = glutGetModifiers();
    InputButton eButton;
    switch (button)
    {
    case GLUT_LEFT_BUTTON:
        eButton = Button1;
        break;
    case GLUT_RIGHT_BUTTON:
        eButton = Button2;
        break;
    case GLUT_WHEEL_UP:
        onPressButton(ButtonZ);
        onReleaseButton(ButtonZ);
        return;
    case GLUT_WHEEL_DOWN:
        onPressButton(ButtonX);
        onReleaseButton(ButtonX);
        return;
    default:  // This button isn't used
        return;
    }
    if (state == GLUT_DOWN) {
        onPressButton(eButton);
    } else if (state == GLUT_UP) {
        onReleaseButton(eButton);
    }
}

// -----------------------------------------------------------------
// Name : onMouseMove
// -----------------------------------------------------------------
void InputEngine::onMouseMove(int x, int y)
{
    onAnalogicMove(x, y);
}

// -----------------------------------------------------------------
// Name : isShiftPressed
// -----------------------------------------------------------------
bool InputEngine::isShiftPressed()
{
    return ((m_iModifiers & GLUT_ACTIVE_SHIFT) != 0);
}

// -----------------------------------------------------------------
// Name : isCtrlPressed
// -----------------------------------------------------------------
bool InputEngine::isCtrlPressed()
{
    return ((m_iModifiers & GLUT_ACTIVE_CTRL) != 0);
}
