#include "DisplayEngine.h"
#include "TextureEngine.h"
#include "../Geometries/Geometry.h"
#include "../Managers/DebugManager.h"
#include "../Data/Parameters.h"

#define BASE_WIDTH    1024.0f
#define BASE_HEIGHT   576.0f

#define PICKING_ACCURACY			5	// Size of the picking area in pixels ; 1 would be the most accurate but could be hard to pick small objects
#define PICKING_HITDATA_NBNAMES		0
#define PICKING_HITDATA_MINZ		1
#define PICKING_HITDATA_MAXZ		2
#define PICKING_HITDATA_FIRSTNAME	3

DisplayEngine * DisplayEngine::m_pInstance = NULL;

// ------------------------------------------------------------------
// Name : DisplayEngine
//  Constructor
// ------------------------------------------------------------------
DisplayEngine::DisplayEngine()
{
    m_bReady = false;
    m_iStencilState = 0;
    m_iWindow = -1;
    m_bAdditive = false;
    m_iStencilDepth = 0;
    m_dScreenRatio = 1;
    m_bIgnoreNextResize = false;
    m_lookAtDelta = f3d(-0.05f, -0.25f, 0);
}

// ------------------------------------------------------------------
// Name : ~DisplayEngine
//  Destructor
// ------------------------------------------------------------------
DisplayEngine::~DisplayEngine()
{
	// Everything must have been deleted using Geometry::~Geometry, which calls DisplayEngine::unregisterGeometry
	assert(m_pRegisteredGeometries.empty());
}

// ------------------------------------------------------------------
// Name : init
// ------------------------------------------------------------------
void DisplayEngine::init()
{
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL | GLUT_ALPHA);
    m_f3CamPos = f3d(0.0f, 0.0f, 0.0f);

    initGlutWindow();
}

// ------------------------------------------------------------------
// Name : initGlutWindow
// ------------------------------------------------------------------
void DisplayEngine::initGlutWindow()
{
	m_bIgnoreNextResize = true;

    if (m_iWindow == -2) {  // was fullscreen
        glutLeaveGameMode();
    } else if (m_iWindow >= 0) {  // was windowed
    	_params->setWinXPos(glutGet(GLUT_WINDOW_X));
    	_params->setWinYPos(glutGet(GLUT_WINDOW_Y));
        glutDestroyWindow(m_iWindow);
    }

    if (_params->isFullscreen()) {
        glutGameModeString(_params->getGameModeString().c_str());
        glutEnterGameMode();
        _params->setScreenXSize(glutGameModeGet(GLUT_GAME_MODE_WIDTH));
        _params->setScreenYSize(glutGameModeGet(GLUT_GAME_MODE_HEIGHT));
        m_iWindow = -2;
    } else {
        glutInitWindowPosition(_params->getWinXPos(), _params->getWinYPos());
        _params->setScreenXSize(_params->getWinWidth());
        _params->setScreenYSize(_params->getWinHeight());
        glutInitWindowSize(_params->getScreenXSize(), _params->getScreenYSize());
        m_iWindow = glutCreateWindow("jotak");
    }
    resizeWindow();

    glShadeModel(GL_SMOOTH);
    glDisable(GL_BLEND);
    glAlphaFunc(GL_GREATER, 0.0001f);
    glEnable(GL_ALPHA_TEST);

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.05f, 0.1f, 0.0f);
    glClearDepth(1.0f);
    glClearStencil(0);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        _debug->error(string("Error in glewInit: ") + (char*) glewGetErrorString(err) + ".\n");
    }
    if (!glewIsSupported("GL_ARB_shading_language_100")) {
    	_debug->error("Warning: extension GL_ARB_shading_language_100 not supported.");
    }
    if (!glewIsSupported("GL_ARB_shader_objects")) {
    	_debug->error("Warning: extension GL_ARB_shader_objects not supported.");
    }
    if (!glewIsSupported("GL_ARB_vertex_shader")) {
    	_debug->error("Warning: extension GL_ARB_vertex_shader not supported.");
    }
    if (!glewIsSupported("GL_ARB_fragment_shader")) {
    	_debug->error("Warning: extension GL_ARB_fragment_shader not supported.");
    }

	for (Geometry* &pGeo : m_pRegisteredGeometries) {
        pGeo->reload();
    }

    _tex->reloadAllTextures();
}

// ------------------------------------------------------------------
// Name : canResize
// ------------------------------------------------------------------
bool DisplayEngine::canResize()
{
	if (m_bIgnoreNextResize) {
		m_bIgnoreNextResize = false;
		return false;
	} else {
		return true;
	}
}

// ------------------------------------------------------------------
// Name : resizeWindow
// ------------------------------------------------------------------
void DisplayEngine::resizeWindow()
{
    glViewport(0, 0, _params->getScreenXSize(), _params->getScreenYSize());
    m_dScreenRatio = BASE_WIDTH / BASE_HEIGHT;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(50.0, m_dScreenRatio, NEARPLANE, FARPLANE);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    moveCameraTo(m_f3CamPos);
}

// ------------------------------------------------------------------
// Name : beginDisplay
// ------------------------------------------------------------------
void DisplayEngine::beginDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_iStencilDepth = 0;
    enableBlending();
}

// ------------------------------------------------------------------
// Name : endDisplay
// ------------------------------------------------------------------
void DisplayEngine::endDisplay()
{
    glutSwapBuffers();
}

// ------------------------------------------------------------------
// Name : begin2D
// ------------------------------------------------------------------
void DisplayEngine::begin2D()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0f, (GLdouble)_params->getScreenXSize(), (GLdouble)_params->getScreenYSize(), 0.0f, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
}

// ------------------------------------------------------------------
// Name : end2D
// ------------------------------------------------------------------
void DisplayEngine::end2D()
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

// ------------------------------------------------------------------
// Name : begin3D
// ------------------------------------------------------------------
void DisplayEngine::begin3D()
{
}

// ------------------------------------------------------------------
// Name : end3D
// ------------------------------------------------------------------
void DisplayEngine::end3D()
{
}

// ------------------------------------------------------------------
// Name : moveCameraBy
// ------------------------------------------------------------------
void DisplayEngine::moveCameraBy(f3d d3Delta)
{
    moveCameraTo(m_f3CamPos + d3Delta);
}

// ------------------------------------------------------------------
// Name : moveCameraTo
// ------------------------------------------------------------------
void DisplayEngine::moveCameraTo(f3d d3Pos)
{
    m_f3CamPos = d3Pos;
    glLoadIdentity();
    gluLookAt(m_f3CamPos.x, m_f3CamPos.y, m_f3CamPos.z, m_f3CamPos.x - m_lookAtDelta.x, m_f3CamPos.y - m_lookAtDelta.y, 0, 0, 1, 0);
}

// ------------------------------------------------------------------
// Name : getGUI3D
// ------------------------------------------------------------------
f3d DisplayEngine::getGUI3D(i3d screenCoords)
{
	return f3d((double)screenCoords.x, (double)screenCoords.y, screenCoords.z);
}

// ------------------------------------------------------------------
// Name : unproject
//	Retrieve mouse 3D coordinates on screen's plane
// ------------------------------------------------------------------
f3d DisplayEngine::unproject(i3d mouse)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLint winX, winY;
    GLfloat winZ;
    GLdouble posX, posY, posZ;

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    winX = mouse.x;
    winY = viewport[3] - mouse.y;
    winZ = 0;
//    glReadPixels(winX, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
    gluUnProject((float)winX, (float)winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    return f3d(posX, posY, posZ);
}

// ------------------------------------------------------------------
// Name : getBoard3
// ------------------------------------------------------------------
f3d DisplayEngine::getBoard3D(i3d mouse)
{
    // unproject gives the 3D coords on "virtual" screen.
    f3d d3u = unproject(mouse);
    // Knowing this and the camera position, we can deduce the line equation
    // and retrieve the intersection of this line with the board plane, that is, the 3D clicked coords on ground
    double t = (NEARPLANE - m_f3CamPos.z) / (m_f3CamPos.z - d3u.z);
    return m_f3CamPos + (m_f3CamPos - d3u) * t;
}

// ------------------------------------------------------------------
// Name : setStencilState
//  return previous state
// ------------------------------------------------------------------
int DisplayEngine::setStencilState(int iState)
{
    int previous = m_iStencilState;
    m_iStencilState = iState;
    switch (iState)
    {
    case 0: // no stencil
        glDisable(GL_STENCIL_TEST);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glEnable(GL_DEPTH_TEST);
        break;
    case 1: // fill stencil
        glEnable(GL_STENCIL_TEST);
        // Increase stencil to a higher depth when it's filled for current depth
        glStencilFunc(GL_EQUAL, m_iStencilDepth, 0xffffffff);
        glStencilOp(GL_KEEP, GL_INCR, GL_INCR);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glDisable(GL_DEPTH_TEST);
        m_iStencilDepth++;
        break;
    case 2: // write with stencil
        glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_EQUAL, m_iStencilDepth, 0xffffffff);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glEnable(GL_DEPTH_TEST);
        break;
    case 3: // unfill stencil
        glEnable(GL_STENCIL_TEST);
        // Decrease stencil to lower depth when it's filled for current depth
        glStencilFunc(GL_EQUAL, m_iStencilDepth, 0xffffffff);
        glStencilOp(GL_KEEP, GL_DECR, GL_DECR);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glDisable(GL_DEPTH_TEST);
        m_iStencilDepth--;
        break;
    }
    return previous;
}

// ------------------------------------------------------------------
// Name : enableBlending
// ------------------------------------------------------------------
void DisplayEngine::enableBlending()
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDepthMask(false);
}

// ------------------------------------------------------------------
// Name : disableBlending
// ------------------------------------------------------------------
void DisplayEngine::disableBlending()
{
    glDepthMask(true);
    glDisable(GL_BLEND);
}

// ------------------------------------------------------------------
// Name : setMaskBlending
// ------------------------------------------------------------------
void DisplayEngine::setMaskBlending(bool bGreyDarken)
{
//  glBlendEquation(GL_MIN);
//  glBlendFunc(GL_DST_COLOR, GL_ZERO);
    if (bGreyDarken) {
        glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
    } else {
        glBlendFuncSeparate(GL_ZERO, GL_ONE, GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
    }
}

// ------------------------------------------------------------------
// Name : startMaskBlending
// ------------------------------------------------------------------
void DisplayEngine::startMaskBlending()
{
//  glBlendEquation(GL_FUNC_ADD);
//  glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);
//  glBlendFunc(GL_ONE, GL_ONE);
    glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA);
}

// ------------------------------------------------------------------
// Name : stopMaskBlending
// ------------------------------------------------------------------
void DisplayEngine::stopMaskBlending()
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// ------------------------------------------------------------------
// Name : registerGeometry
// ------------------------------------------------------------------
void DisplayEngine::registerGeometry(Geometry * pGeometry)
{
    m_pRegisteredGeometries.push_back(pGeometry);
}

// ------------------------------------------------------------------
// Name : unregisterGeometry
// ------------------------------------------------------------------
void DisplayEngine::unregisterGeometry(Geometry * pGeometry)
{
	m_pRegisteredGeometries.remove(pGeometry);
}

// ------------------------------------------------------------------
// Name : saveWinPos
// ------------------------------------------------------------------
void DisplayEngine::saveWinPos()
{
    if (m_iWindow >= 0)  // windowed
    {
        _params->setWinXPos(glutGet(GLUT_WINDOW_X));
        _params->setWinYPos(glutGet(GLUT_WINDOW_Y));
    }
}

// ------------------------------------------------------------------
// Name : setAdditiveMode
// ------------------------------------------------------------------
bool DisplayEngine::setAdditiveMode(bool bAdd)
{
    bool bPrev = m_bAdditive;
    if (!bPrev && bAdd)
    {
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
        m_bAdditive = true;
    }
    else if (bPrev && !bAdd)
    {
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        m_bAdditive = false;
    }
    return bPrev;
}

// ------------------------------------------------------------------
// Name : loadShader
// ------------------------------------------------------------------
bool DisplayEngine::loadShader(GLuint * uShader, GLenum type, string sShader)
{
    *uShader = glCreateShader(type);
    if (*uShader == 0)
    {
        _debug->error("Error in loadShader: can't create shader " + sShader);
        return false;
    }

    // Open source file and read it
    string sFilePath = SHADERS_PATH + sShader;
    ifstream file(sFilePath.c_str());

    if (!file.is_open()) {
        glDeleteShader(*uShader);
        *uShader = 0;
        _debug->error("Error in loadShader: can't read source file " + sShader);
        return false;
    }

    string sContent((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    // Compile shader
    GLint status = GL_TRUE;
    glShaderSource(*uShader, 1, (const GLchar**) (sContent.c_str()), NULL);
    glCompileShader(*uShader);
    glGetShaderiv(*uShader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE)
    {
        char sLog[1024] = "";
        GLint size = 1024;  // wtf?
        glGetShaderInfoLog(*uShader, size, &size, sLog);
        _debug->error("Error in loadShader: can't compile " + sShader + ".\n" + sLog);
        glDeleteShader(*uShader);
        *uShader = 0;
        return false;
    }
    return true;
}

// ------------------------------------------------------------------
// Name : linkShaders
// ------------------------------------------------------------------
bool DisplayEngine::linkShaders(GLuint * uProgram, GLuint uVxShader, GLuint uPxShader)
{
    if (uVxShader == 0 && uPxShader == 0)
    {
    	_debug->error("Error in linkShaders: invalid shaders.");
        return false;
    }
    *uProgram = glCreateProgram();
    if (uVxShader != 0) {
        glAttachShader(*uProgram, uVxShader);
    }
    if (uPxShader != 0) {
        glAttachShader(*uProgram, uPxShader);
    }

    GLint status = GL_TRUE;
    glLinkProgram(*uProgram);
    glGetProgramiv(*uProgram, GL_LINK_STATUS, &status);
    if (status != GL_TRUE)
    {
        char sLog[1024] = "";
        GLint size = 1024;  // wtf?
        glGetProgramInfoLog(*uProgram, size, &size, sLog);
        _debug->error(string("Error in linkShaders: can't link program.\n") + sLog);
        glDeleteProgram(*uProgram);
        *uProgram = 0;
        return false;
    }
    return true;
}

// ------------------------------------------------------------------
// Name : startPicking
//	Get object below mouse
// ------------------------------------------------------------------
void DisplayEngine::startPicking(int x, int y)
{
	GLint viewport[4];
	glSelectBuffer(PICKING_BUFSIZE, m_PickingBuffer);
	glRenderMode(GL_SELECT);

	// Push & reset projection matrix
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	// Get viewport and define picking region
	glGetIntegerv(GL_VIEWPORT, viewport);
	gluPickMatrix(x, viewport[3] - y/*invert y*/, PICKING_ACCURACY, PICKING_ACCURACY, viewport);
    gluPerspective(50.0, m_dScreenRatio, NEARPLANE, FARPLANE);

    // Prepare drawing in selection mode
    glMatrixMode(GL_MODELVIEW);
    glInitNames();
    glPushName(0);	// Push empty name to enable using glLoadName later
}

// ------------------------------------------------------------------
// Name : endPicking
// ------------------------------------------------------------------
unsigned int DisplayEngine::endPicking()
{
	// Pop out projection
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glFlush();

	// Return back to RENDER mode, and retrieve SELECT mode information
	int nbHits = glRenderMode(GL_RENDER);

	if (nbHits > 1) {
		int tott = 0;
		tott++;
	}
	// If there are hits process them
	unsigned int picked = 0;
	if (nbHits != 0) {
		GLuint iCurrent, minZ;

		// The buffer (m_PickingBuffer) contains a list of hits data
		//	Each hit data consists of:
		//	- Numbers of names involved
		//	- Min z-depth
		//	- Max z-depth
		//	- Name 1 involved
		//	- Name 2 involved
		//	- ...
		minZ = 0xffffffff;
		iCurrent = 0;
		for (int i = 0; i < nbHits; i++) {
			if (m_PickingBuffer[iCurrent + PICKING_HITDATA_NBNAMES] > 0
					&& m_PickingBuffer[iCurrent + PICKING_HITDATA_MINZ] < minZ) {
				minZ = m_PickingBuffer[iCurrent + PICKING_HITDATA_MINZ];
				picked = m_PickingBuffer[iCurrent + PICKING_HITDATA_FIRSTNAME];
			}
			// Increase iCurrent ; "3" stands for nbnames, minz and maxz
			iCurrent += 3 + m_PickingBuffer[iCurrent + PICKING_HITDATA_NBNAMES];
		}
	}

	return picked;
}
