#ifndef _KEYBOARD_LISTENER_H
#define _KEYBOARD_LISTENER_H

class KeyboardListener
{
public:
	virtual ~KeyboardListener() {};
    virtual bool onKeyDown(unsigned char c) = 0;
    virtual bool onSpecialKeyDown(int key) = 0;
};

#endif
