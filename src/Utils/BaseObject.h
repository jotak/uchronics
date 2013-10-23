#ifndef _BASE_OBJECT_H
#define _BASE_OBJECT_H

#include "../Utils/utils.h"

#define NAME_MAX_CHARS          64
#define DESCRIPTION_MAX_CHARS   1024

class BaseObject
{
public:
    BaseObject() { m_pAttachment = NULL; };
    virtual ~BaseObject() {};
    void setAttachment(BaseObject * pObj) { m_pAttachment = pObj; };
    BaseObject * getAttachment() { return m_pAttachment; };

protected:
    BaseObject * m_pAttachment;
};

class String : public BaseObject
{
public:
    String(string str) { m_sString = string(str); };
    string m_sString;
};

class Integer : public BaseObject
{
public:
    Integer(int i) { m_i = i; };
    void operator=(int i) { m_i = i; };
    void operator=(Integer i) { m_i = i.intValue(); };
    int intValue() { return m_i; };

private:
    int m_i;
};

class Couple : public BaseObject
{
public:
    Couple(BaseObject * first, BaseObject * second)
    {
        m_pFirst = first;
        m_pSecond = second;
    };

    ~Couple()
    {
        FREE(m_pFirst);
        FREE(m_pSecond);
    };

    BaseObject * getFirst() { return m_pFirst; }
    BaseObject * getSecond() { return m_pSecond; }
    void setFirst(BaseObject * first) { m_pFirst = first; }
    void setSecond(BaseObject * second) { m_pSecond = second; }

private:
    BaseObject * m_pFirst;
    BaseObject * m_pSecond;
};

class CoordsObject : public BaseObject
{
public:
    CoordsObject(Coords3D c)
    {
        m_Coords = c;
    };
    CoordsObject(CoordsScreen c)
    {
        m_Coords.x = c.x;
        m_Coords.y = c.y;
        m_Coords.z = c.z;
    };

    Coords3D getCoords3D()
    {
        return m_Coords;
    };
    CoordsScreen getCoordsScreen()
    {
        return CoordsScreen((int)m_Coords.x, (int)m_Coords.y, m_Coords.z);
    };

    Coords3D m_Coords;
};

#endif
