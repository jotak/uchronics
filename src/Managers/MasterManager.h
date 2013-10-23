#ifndef _MASTER_MANAGER_H
#define _MASTER_MANAGER_H

class MasterManager
{
public:
    // Constructor / destructor
    MasterManager();
    ~MasterManager();

    // Init / Update / Display
    void init();
    void update(double delta);
    void display();

    // WM handlers
    void onResize(int w, int h);
    void cleanUp();
    void onKeyboard(int key, int x, int y, bool bSpecialKey);
    void onMouseClick(int button, int state, int x, int y);
    void onMouseMove(int x, int y);
    void checkRestartGlut(double delta);

private:
    float m_fRestartGlutTimer;
    int m_iOldWidth;
    int m_iOldHeight;
};

#endif
