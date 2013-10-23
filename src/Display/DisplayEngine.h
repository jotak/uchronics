#ifndef _DISPLAY_ENGINE_H
#define _DISPLAY_ENGINE_H

#include "../Geometries/Geometry.h"

#define NEARPLANE                   1.0f
#define FARPLANE                    15.0f
#define GUIPLANE                    50.0f	// anywhere in ]15,100] actually
#define BOARDPLANE                  14.0f
#define BUFFER_OFFSET(i)            ((char*)NULL + (i))
#define PICKING_BUFSIZE				 512

#define F3DBOARD(x, y)				(f3d(x, y, BOARDPLANE-0.001f))

class DisplayEngine
{
public:
    // Singleton / destructor
    ~DisplayEngine();
    static DisplayEngine * getInstance()
    {
        if (m_pInstance == NULL) m_pInstance = new DisplayEngine();
        return m_pInstance;
    };

    void init();
    void initGlutWindow();

    // Other functions
    void beginDisplay();
    void endDisplay();
    void begin2D();
    void end2D();
    void begin3D();
    void end3D();
    bool isReady() { return m_bReady; };
    void setReady() { m_bReady = true; };
    void resizeWindow();
    f3d getGUI3D(i3d screenCoords);
    f3d getBoard3D(i3d screenCoords);
    f3d getCamera() { return m_f3CamPos; };
    void moveCameraBy(f3d d3Delta);
    void moveCameraTo(f3d d3Delta);
    int setStencilState(int iState);
    void enableBlending();
    void disableBlending();
    void setMaskBlending(bool bGreyDarken);
    void startMaskBlending();
    void stopMaskBlending();
    void registerGeometry(Geometry * pGeometry);
    void unregisterGeometry(Geometry * pGeometry);
    void saveWinPos();
    bool setAdditiveMode(bool bAdd);
    bool canResize();

    // GL Picking
    void startPicking(int x, int y);
    void pickNext(unsigned int name) { glLoadName(name); };
    unsigned int endPicking();

    // Shaders
    bool loadShader(GLuint * uShader, GLenum type, string sShader);
    bool linkShaders(GLuint * uProgram, GLuint uVxShader, GLuint uPxShader);

private:
    DisplayEngine();
    static DisplayEngine * m_pInstance;

    f3d unproject(i3d mouse);

    f3d m_f3CamPos;
    bool m_bReady;
    int m_iStencilState;  // 0 = no stencil, 1 = fill stencil, 2 = write with stencil
    int m_iStencilDepth;
    int m_iWindow;
    list<Geometry*> m_pRegisteredGeometries;
    double m_dScreenRatio;
    bool m_bAdditive;
    bool m_bIgnoreNextResize;
    f3d m_lookAtDelta;
    GLuint m_PickingBuffer[PICKING_BUFSIZE];
};

// Helper alias
#define _display DisplayEngine::getInstance()

#endif
