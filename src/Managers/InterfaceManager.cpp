// -----------------------------------------------------------------
// INTERFACE MANAGER
// -----------------------------------------------------------------
#include "InterfaceManager.h"
#include <jogy.h>
#include "../Data/LocalisationTool.h"
#include "../Interface/Tooltip.h"
#include "../Input/InputEngine.h"
#include "../Managers/DebugManager.h"
#include "../Display/TextureEngine.h"
#include "../Data/Parameters.h"

InterfaceManager * InterfaceManager::m_pInstance = NULL;

// -----------------------------------------------------------------
// Name : InterfaceManager
// -----------------------------------------------------------------
InterfaceManager::InterfaceManager() : EventListener(7)
{
    m_pTargetedObject = NULL;
    deleteAllFrames();
    guiContainer::initStatic();
}

// -----------------------------------------------------------------
// Name : ~InterfaceManager
// -----------------------------------------------------------------
InterfaceManager::~InterfaceManager()
{
	FREEVEC(m_pFrameList);
	FREEVEC(m_pTopDisplayList);
	Jogy::destroy();
}

// -----------------------------------------------------------------
// Name : deleteAllFrames
// -----------------------------------------------------------------
void InterfaceManager::deleteAllFrames()
{
	FREEVEC(m_pFrameList);
	FREEVEC(m_pTopDisplayList);

    m_pTooltip = NULL;
    m_fTooltipTime = 0;
    m_pClickedObjects[0] = m_pClickedObjects[1] = NULL;
    m_pPointedObject = NULL;
}

// -----------------------------------------------------------------
// Name : Init
// -----------------------------------------------------------------
void InterfaceManager::init()
{
    _tex->loadTexture("gui/interface", true/*composed*/);
    initMenu();
    _input->addCursoredEventListener(this);
    _input->pushUncursoredEventListener(this);
}

// -----------------------------------------------------------------
// Name : initMenu
// -----------------------------------------------------------------
void InterfaceManager::initMenu()
{
    deleteAllFrames();

    // Init tooltips
    m_pTooltip = jogy::Popup::createEmptyPopup();
    m_pTooltip->removeEffect(POPUP_EFFECT_FOCUS);
    m_pTooltip->removeEffect(POPUP_EFFECT_INTRO);
    m_pTooltip->getDocument()->setEnabled(true);
    m_pTooltip->setVisible(false);
    m_pFrameList.push_back(m_pTooltip);
}

// -----------------------------------------------------------------
// Name : update
// -----------------------------------------------------------------
void InterfaceManager::update(double delta)
{
    // Update frames
	for (list<jogy::Frame*>::iterator it = m_pFrameList.begin(); it != m_pFrameList.end(); ++it) {
		jogy::Frame * frm = *it;
        frm->update(delta);
        if (frm->getDocument() == NULL)   // Dead frame
        {
        	it = m_pFrameList.erase(it);
        }
    }

    // Tooltip
    if (m_pPointedObject != NULL && m_fTooltipTime > 0)
    {
        m_fTooltipTime -= delta;
        if (m_fTooltipTime <= 0)
        {
            m_fTooltipTime = 0;
            string sText = m_pPointedObject->getTooltipText();
            if (sText != "")
            {
                m_pTooltip->getDocument()->deleteAllComponents();
                m_pTooltip->getDocument()->setDimensions(0, 0);
                getRichText(m_pTooltip->getDocument(), CoordsScreen(0, 0), sText);
                m_pTooltip->updateSizeFit();
                CoordsScreen cs = _input->getCurrentCursorPosition();
                cs.y += 10;
                int width = m_pTooltip->getDocument()->getWidth() + 2;
                int height = m_pTooltip->getDocument()->getHeight() + 2;
                if (cs.x + width + 10 > _params->getScreenXSize()) {
                    cs.x = _params->getScreenXSize() - width - 10;
                }
                if (cs.y + height + 10 > _params->getScreenYSize()) {
                    cs.y = _params->getScreenYSize() - height - 10;
                }
                m_pTooltip->moveTo(cs.x, cs.y);
                m_pTooltip->setVisible(true);
                bringFrameAbove(m_pTooltip);
            }
        }
    }
}

// -----------------------------------------------------------------
// Name : display
// -----------------------------------------------------------------
void InterfaceManager::display()
{
    FREEVEC(m_pTopDisplayList);
    jogy::Color plain(1, 1, 1, 1);
    jogy::Color alpha(1, 1, 1, 0.65f);

	for (jogy::Frame* &pFrm : m_pFrameList) {
        if (pFrm != m_pTooltip) {
        	pFrm->displayAt(0, 0, plain, alpha);
        }
    }

	for (TopDisplayObject* &pObj : m_pTopDisplayList) {
		pObj->display();
    }

    if (m_pTooltip != NULL) {
        alpha.a = 0.85f;
        m_pTooltip->displayAt(0, 0, plain, alpha);
    }
}

// -----------------------------------------------------------------
// Name : onCatchButtonEvent
//  Called by Input Engine.
//  Must return true if event is consumed ; false to let the event be catched by other modules
//  Transfer the message to top frame under mouse
// -----------------------------------------------------------------
bool InterfaceManager::onCatchButtonEvent(ButtonAction * pEvent)
{
    if (pEvent->eButton == ButtonStart && pEvent->eEvent == Event_Down) {
        return onClickStart();
    }
    if (pEvent->eEvent == Event_Down)
    {
        // Reset clicked objects
        if (pEvent->eButton == Button1) {
            m_pClickedObjects[0] = NULL;
        } else if (pEvent->eButton == Button2) {
            m_pClickedObjects[1] = NULL;
        }

        int xoffset, yoffset;
        jogy::Component * pObj1 = getObjectAt(pEvent->xPos - pEvent->xOffset, pEvent->yPos - pEvent->yOffset, &xoffset, &yoffset);
        pEvent->xOffset += xoffset;
        pEvent->yOffset += yoffset;
        if (pObj1 == NULL) {
            return false; // not concerned
        }
        if (!pObj1->isEnabled()) {
            return true;  // event is for us, but do nothing
        }

        if ((pObj1->getType() & GOTYPE_FRAME) && pEvent->eButton == Button1) {
            bringFrameAbove((jogy::Frame*)pObj1);
        }

        // Send event
        jogy::Object * pObj = pObj1->onButtonEvent(pEvent);
        // Store object for dragging (only for main buttons)
        if (pEvent->eButton == Button1) {
            m_pClickedObjects[0] = pObj;
        } else if (pEvent->eButton == Button2) {
            m_pClickedObjects[1] = pObj;
        }
        // else, don't care about dragging, so just forget pObj.
        return true;
    }
    else
    {
    	jogy::Object ** pObj = (pEvent->eButton == Button1) ? &(m_pClickedObjects[0]) : ((pEvent->eButton == Button2) ? &(m_pClickedObjects[1]) : NULL);
        if (pObj != NULL && *pObj != NULL)
        {
            *pObj = (*pObj)->onButtonEvent(pEvent);
            if (*pObj != NULL && ((*pObj)->getType() & GOTYPE_FRAME) && pEvent->eEvent == Event_Drag) {
                ((jogy::Frame*) (*pObj))->checkPositionIfDragged();
            }
        }
        return true;
    }
}

// -----------------------------------------------------------------
// Name : onCursorMoveEvent
// -----------------------------------------------------------------
bool InterfaceManager::onCursorMoveEvent(int xPxl, int yPxl)
{
    int xoffset, yoffset;
    jogy::Component * pObj = getObjectAt(xPxl, yPxl, &xoffset, &yoffset);
    if (pObj == NULL)
    {
        if (m_pPointedObject != NULL)
        {
            m_pPointedObject->onCursorMoveOutEvent();
            m_pPointedObject = NULL;
            m_pTooltip->setVisible(false);
        }
        return false;
    }
    xPxl -= xoffset;
    yPxl -= yoffset;

    jogy::Object * pPointed = pObj->onCursorMoveEvent(xPxl, yPxl);
    if (pPointed == NULL)
    {
        if (m_pPointedObject != NULL)
        {
            m_pPointedObject->onCursorMoveOutEvent();
            m_pPointedObject = NULL;
            m_pTooltip->setVisible(false);
        }
    }
    else if (pPointed != m_pPointedObject)
    {
        if (m_pPointedObject != NULL) {
            m_pPointedObject->onCursorMoveOutEvent();
        }
        m_pTooltip->setVisible(false);
        m_pPointedObject = pPointed;
        if (m_fTooltipTime > 0) {
            m_fTooltipTime = 0.5f;
        } else {
            m_fTooltipTime = 0.2f;
        }
    }
    return true;
}

// -----------------------------------------------------------------
// Name : getObjectAt
// -----------------------------------------------------------------
jogy::Component * InterfaceManager::getObjectAt(int xPxl, int yPxl, int * xoffset, int * yoffset)
{
    // First look at TopDisplayList
	for (list<TopDisplayObject*>::reverse_iterator it = m_pTopDisplayList.rbegin(); it != m_pTopDisplayList.rend(); ++it) {
		TopDisplayObject * pTop = (*it);
        if (pTop->pObj->isAt(xPxl - pTop->iX, yPxl - pTop->iY)) {
            *xoffset = pTop->iX;
            *yoffset = pTop->iY;
            return pTop->pObj;
        }
    }
    // Then, frames
    *xoffset = 0;
    *yoffset = 0;
	for (list<jogy::Frame*>::reverse_iterator it = m_pFrameList.rbegin(); it != m_pFrameList.rend(); ++it) {
		jogy::Frame * pFrm = (*it);
        if (pFrm != m_pTooltip && pFrm->isAt(xPxl, yPxl)) {
            return pFrm;
        }
    }
    return NULL;
}

// -----------------------------------------------------------------
// Name : findFrame
// -----------------------------------------------------------------
jogy::Frame * InterfaceManager::findFrame(string frmId)
{
	for (jogy::Frame* &pFrm : m_pFrameList) {
        if (pFrm->getId() == frmId) {
            return pFrm;
        }
    }
    return NULL;
}

// -----------------------------------------------------------------
// Name : findFrameFromDoc
// -----------------------------------------------------------------
jogy::Frame * InterfaceManager::findFrameFromDoc(jogy::Document * pDoc)
{
	for (jogy::Frame* &pFrm : m_pFrameList) {
        if (pFrm->getDocument() == pDoc) {
            return pFrm;
        }
    }
    return NULL;
}

// -----------------------------------------------------------------
// Name : registerFrame
// -----------------------------------------------------------------
void InterfaceManager::registerFrame(jogy::Frame * pFrm)
{
    m_pFrameList.push_back(pFrm);
}

// -----------------------------------------------------------------
// Name : deleteFrame
// -----------------------------------------------------------------
void InterfaceManager::deleteFrame(jogy::Frame * pFrm)
{
	for(list<jogy::Frame*>::iterator it = m_pFrameList.begin(); it != m_pFrameList.end(); ++it) {
        if (*it == pFrm) {
        	m_pFrameList.erase(it);
        	resetSharedPointers();
            return;
        }
    }
}

// -----------------------------------------------------------------
// Name : deleteCurrentFrame
// -----------------------------------------------------------------
jogy::Frame * InterfaceManager::deleteCurrentFrame(list<jogy::Frame*>::iterator it)
{
    resetSharedPointers();   // since the frame has probably been closed via its own button, we set ClickedObject to NULL by security. In the worst case it would just cancel a drag&drop or double-click.
	return *(m_pFrameList.erase(it));
}

// -----------------------------------------------------------------
// Name : resetSharedPointers
// -----------------------------------------------------------------
void InterfaceManager::resetSharedPointers(jogy::Object * pObj)
{
    if (pObj == NULL)
    {
        m_pClickedObjects[0] = m_pClickedObjects[1] = NULL;
        m_pPointedObject = NULL;
        m_pTooltip->setVisible(false);
        FREEVEC(m_pTopDisplayList);
    }
    else
    {
        if (pObj == m_pClickedObjects[0]) {
            m_pClickedObjects[0] = NULL;
        }
        if (pObj == m_pClickedObjects[1]) {
            m_pClickedObjects[1] = NULL;
        }
        if (pObj == m_pPointedObject) {
            m_pPointedObject = NULL;
        }
    }
}

// -----------------------------------------------------------------
// Name : bringFrameAbove
// -----------------------------------------------------------------
void InterfaceManager::bringFrameAbove(jogy::Frame * pFrm)
{
	for (list<jogy::Frame*>::iterator it = m_pFrameList.begin(); it != m_pFrameList.end(); ++it) {
		if (*it == pFrm) {
			m_pFrameList.erase(it);
			m_pFrameList.push_back(pFrm);
			if (pFrm != m_pTooltip) {
				// keep tooltips above, in any case
				bringFrameAbove(m_pTooltip);
			}
			break;
		}
	}
}

// -----------------------------------------------------------------
// Name : setUniqueDialog
// -----------------------------------------------------------------
void InterfaceManager::setUniqueDialog(jogy::Document * pDoc)
{
	for (jogy::Frame* &pFrm : m_pFrameList) {
        if (pFrm != m_pTooltip) {
            if (pFrm->getDocument() == pDoc) {
                pFrm->setEnabled(true);
                pFrm->getDocument()->setEnabled(true);
                pFrm->show();
            } else if (pFrm->isVisible()) {
                pFrm->hide();
            }
        }
    }
}

// -----------------------------------------------------------------
// Name : topDisplay
// -----------------------------------------------------------------
void InterfaceManager::topDisplay(jogy::Component * pObj, int iX, int iY, Color * cpntColor, Color * docColor)
{
    TopDisplayObject * obj = new TopDisplayObject();
    obj->pObj = pObj;
    obj->iX = iX;
    obj->iY = iY;
    obj->cpntColor = cpntColor;
    obj->docColor = docColor;
    m_pTopDisplayList.push_back(obj);
}

// -----------------------------------------------------------------
// Name : cancelTopDisplay
// -----------------------------------------------------------------
void InterfaceManager::cancelTopDisplay(jogy::Component * pObj)
{
	for(list<TopDisplayObject*>::iterator it = m_pTopDisplayList.begin(); it != m_pTopDisplayList.end(); ++it) {
        if ((*it)->pObj == pObj)
        {
        	m_pTopDisplayList.erase(it);
            return;
        }
    }
}

// -----------------------------------------------------------------
// Name : onResize
// -----------------------------------------------------------------
void InterfaceManager::onResize(int oldw, int oldh)
{
    float fWCoef = (float) _params->getScreenXSize() / (float) oldw;
    float fHCoef = (float) _params->getScreenYSize() / (float) oldh;

	for (jogy::Frame* &pFrm : m_pFrameList) {
        int xc = pFrm->getXPos() + pFrm->getWidth() / 2;
        int yc = pFrm->getYPos() + pFrm->getHeight() / 2;
        pFrm->moveBy((float)xc * (fWCoef - 1), (float)yc * (fHCoef - 1));
        // Check that it's not out of screen
        if (pFrm->getXPos() + pFrm->getWidth() < 5) {
            pFrm->moveTo(5 - pFrm->getWidth(), pFrm->getYPos());
        } else if (pFrm->getXPos() > _params->getScreenXSize() - 5) {
            pFrm->moveTo(_params->getScreenXSize() - 5, pFrm->getYPos());
        }
        if (pFrm->getYPos() + pFrm->getHeight() < 5) {
            pFrm->moveTo(pFrm->getXPos(), 5 - pFrm->getHeight());
        } else if (pFrm->getYPos() > _params->getScreenYSize() - 5) {
            pFrm->moveTo(pFrm->getXPos(), _params->getScreenYSize() - 5);
        }
    }
}

// -----------------------------------------------------------------
// Name : onClickStart
// -----------------------------------------------------------------
bool InterfaceManager::onClickStart()
{
	return false;
}

// -----------------------------------------------------------------
// Name : getRichText
//  sSource is label, except when found:
//    #n is new line
//    #f is font, followed by font name (TEXT, H1, H2, DARK), until # is found
//    #i is image, followed by image file name, until # is found
// Example:
//    #fH1#Hello#n#ismiley_smile##n#fTEXT#Welcome to this game!
// -----------------------------------------------------------------
void InterfaceManager::getRichText(jogy::Document * pDest, CoordsScreen offset, string sSource)
{
    string sCurrentText("");
    int iSrcLen = sSource.length();
    int iSrc = 0;
    int iX = offset.x;
    int iY = offset.y;
    int maxX = iX;
    char curObject = '0';  // 0 for normal label
    jogy::Component * pLastObj = NULL;
    fontid font = TEXT_FONT;
    Color color = TEXT_COLOR;

    for (iSrc = 0; iSrc < iSrcLen; iSrc++) {
        switch (curObject)
        {
        case '#':  // '#' (new object)
            curObject = sSource[iSrc];
            if (curObject == 'n') {  // Start new normal object
                if (iX > maxX) {
                    maxX = iX;
                }
                iX = offset.x;
                iY += (pLastObj == NULL) ? 4 : pLastObj->getHeight() + 4;
                curObject = '0';
                sCurrentText = string("");
            }
            break;
        case '0':   // normal label
        {
            if (sSource[iSrc] == '#') { // start new object
                curObject = '#'; // new object (#)
                // Register previous object (normal label)
                if (sCurrentText != "") {
                	jogy::Label * pLbl = new jogy::Label();
                    pLbl->init(sCurrentText, font, color, "", iX, iY, 0, 0);
                    pDest->addComponent(pLbl);
                    iX += pLbl->getWidth();
                    pLastObj = pLbl;
                }
                sCurrentText = string("");
            } else {
                sCurrentText += sSource[iSrc];
            }
            break;
        }
        case 'f':
        {
            if (sSource[iSrc] == '#') {  // end FONT object
                if (sCurrentText == "H1") {
                    font = H1_FONT;
                    color = H1_COLOR;
                } else if (sCurrentText == "H2") {
                    font = H2_FONT;
                    color = H2_COLOR;
                } else if (sCurrentText == "DARK") {
                    font = TEXT_FONT;
                    color = TEXT_COLOR_DARK;
                } else {
                    font = TEXT_FONT;
                    color = TEXT_COLOR;
                }
                curObject = '0';
                sCurrentText = string("");
            } else {
                sCurrentText += sSource[iSrc];
            }
            break;
        }
        case 'i':
        {
            if (sSource[iSrc] == '#') { // end IMAGE object
                Texture * pTex = _tex->loadTexture(sCurrentText);
                if (pTex != NULL)
                {
                	jogy::Image * pImg = new jogy::Image();
                    pImg->init(pTex, "", iX, iY, -1, -1);
                    pDest->addComponent(pImg);
                    iX += pImg->getWidth();
                    pLastObj = pImg;
                }
                curObject = '0';
                sCurrentText = string("");
            } else {
                sCurrentText += sSource[iSrc];
            }
            break;
        }
        }
    }

    // Ending normal label
    if (sCurrentText != "")
    {
    	jogy::Label * pLbl = new jogy::Label();
        pLbl->init(sCurrentText, font, color, "", iX, iY, 0, 0);
        pDest->addComponent(pLbl);
        iX += pLbl->getWidth();
        pLastObj = pLbl;
    }

    // Update document size
    if (iX > maxX) {
        maxX = iX;
    }
    if (pDest->getWidth() > maxX) {
        maxX = pDest->getWidth();
    }
    iY += (pLastObj == NULL) ? 4 : pLastObj->getHeight() + 4;
    if (pDest->getHeight() > iY) {
        iY = pDest->getHeight();
    }
    pDest->setDimensions(maxX, iY);
}
