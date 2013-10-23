#ifndef _INTERFACEMANAGER_H
#define _INTERFACEMANAGER_H

#include <jogy.h>
#include "../Utils/BaseObject.h"
#include "../Input/EventListener.h"

#define SMALL_ICON_SIZE       32
#define SMALL_ICON_SPACING    4

class InterfaceManager : public EventListener
{
public:
    class TopDisplayObject : public BaseObject
    {
    public:
    	void display() {
    		pObj->displayAt(iX, iY, *cpntColor, *docColor);
    	};
        jogy::Component * pObj;
        int iX, iY;
        Color * cpntColor;
        Color * docColor;
    };

    // Singleton / destructor
    ~InterfaceManager();
    static InterfaceManager * getInstance()
    {
        if (m_pInstance == NULL) m_pInstance = new InterfaceManager();
        return m_pInstance;
    };

    // Manager functions
    void init();
    void initMenu();
    void update(double delta);
    void display();

    // Input functions
    bool onCatchButtonEvent(ButtonAction * pEvent);
    bool onCursorMoveEvent(int xPxl, int yPxl);

    // Frame management
    void registerFrame(jogy::Frame * pFrm);
    jogy::Frame * findFrame(string frmId);
    jogy::Frame * findFrameFromDoc(jogy::Document * pDoc);
    void deleteFrame(jogy::Frame * pFrm);
    jogy::Frame * deleteCurrentFrame(list<jogy::Frame*>::iterator it);
    void bringFrameAbove(jogy::Frame * pFrm);
    jogy::Component * getObjectAt(int xPxl, int yPxl, int * xoffset, int * yoffset);

    void topDisplay(jogy::Component * pObj, int iX, int iY, Color * cpntColor, Color * docColor);
    void cancelTopDisplay(jogy::Component * pObj);
    void onResize(int oldw, int oldh);
    bool onClickStart();
    void resetSharedPointers(jogy::Object * pObj = NULL);
    void getRichText(jogy::Document * pDest, CoordsScreen offset, string sSource);

    void setUniqueDialog(jogy::Document * pDoc);

private:
    InterfaceManager();
    static InterfaceManager * m_pInstance;

    void deleteAllFrames();

    list<jogy::Frame*> m_pFrameList;

    jogy::Popup * m_pTooltip;
    float m_fTooltipTime;
    BaseObject * m_pTargetedObject;
    jogy::Object * m_pClickedObjects[2];
    jogy::Object * m_pPointedObject;
    list<TopDisplayObject*> m_pTopDisplayList;
};

// Helper alias
#define _interface InterfaceManager::getInstance()

#endif
