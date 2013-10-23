#ifndef _COORDS_H
#define _COORDS_H


#define XYZ_EMPTY_CONSTRUCTOR(T)  T() { x = 0; y = 0; z = 0; };
#define XYZ_CONSTRUCTOR(T, T2)    T(T2 x, T2 y, double z = 0.0f) { this->x = x; this->y = y; this->z = z; };
#define XYZ_OPDIF(T)              bool operator!= (T other) { return (x != other.x || y != other.y || z != other.z); };
#define XYZ_OPEQ(T)               bool operator== (T other) { return (x == other.x && y == other.y && z == other.z); };
#define XYZ_OPPLUS(T)             T operator+ (T other) { return T(x + other.x, y + other.y, z + other.z); };
#define XYZ_OPPLUSEQ(T)           void operator+= (T other) { x += other.x; y += other.y; z += other.z; };
#define XYZ_OPMINUS(T)            T operator- (T other) { return T(x - other.x, y - other.y, z - other.z); };
#define XYZ_OPMINUSEQ(T)          void operator-= (T other) { x -= other.x; y -= other.y; z -= other.z; };
#define XYZ_OPMULT(T)          	   T operator* (T other) { return T(x * other.x, y * other.y, z * other.z); };
#define XYZ_OPPLUS2(T, T2)        T operator+ (T2 k) { return T(x + k, y + k, z + (double)k); };
#define XYZ_OPMULT2(T, T2)        T operator* (T2 k) { return T(x * k, y * k, z * (double)k); };
#define XYZ_OPDIV(T, T2)          T operator/ (T2 k) { return T(x / k, y / k, z / (double)k); };
#define XYZ_DECLAREVAR(T)         T x, y; double z;

#define XY_EMPTY_CONSTRUCTOR(T)   T() { x = 0; y = 0; };
#define XY_CONSTRUCTOR(T, T2)     T(T2 x, T2 y) { this->x = x; this->y = y; };
#define XY_OPDIF(T)               bool operator!= (T other) { return (x != other.x || y != other.y); };
#define XY_OPEQ(T)                bool operator== (T other) { return (x == other.x && y == other.y); };
#define XY_OPPLUS(T)              T operator+ (T other) { return T(x + other.x, y + other.y); };
#define XY_OPPLUSEQ(T)            void operator+= (T other) { x += other.x; y += other.y; };
#define XY_OPMINUS(T)             T operator- (T other) { return T(x - other.x, y - other.y); };
#define XY_OPMINUSEQ(T)           void operator-= (T other) { x -= other.x; y -= other.y; };
#define XY_OPMULT(T, T2)         T operator* (T2 k) { return T(x * k, y * k); };
#define XY_OPDIV(T, T2)           T operator/ (T2 k) { return T(x / k, y / k); };
#define XY_DECLAREVAR(T)          T x, y;

class Coords {};
class CoordsScreen : public Coords
{
public:
    XYZ_EMPTY_CONSTRUCTOR(CoordsScreen)
    XYZ_CONSTRUCTOR(CoordsScreen, int)
    XYZ_OPDIF(CoordsScreen)
    XYZ_OPEQ(CoordsScreen)
    XYZ_OPPLUS(CoordsScreen)
    XYZ_OPPLUSEQ(CoordsScreen)
    XYZ_OPMINUS(CoordsScreen)
    XYZ_OPMINUSEQ(CoordsScreen)
    XYZ_OPMULT2(CoordsScreen, int)
    XYZ_OPDIV(CoordsScreen, int)
    XYZ_DECLAREVAR(int)
};
class Coords3D : public Coords
{
public:
    XYZ_EMPTY_CONSTRUCTOR(Coords3D)
    XYZ_CONSTRUCTOR(Coords3D, double)
    XYZ_OPDIF(Coords3D)
    XYZ_OPEQ(Coords3D)
    XYZ_OPPLUS(Coords3D)
    XYZ_OPPLUSEQ(Coords3D)
    XYZ_OPMINUS(Coords3D)
    XYZ_OPMINUSEQ(Coords3D)
    XYZ_OPMULT(Coords3D)
    XYZ_OPMULT2(Coords3D, double)
    XYZ_OPDIV(Coords3D, double)
    XYZ_OPPLUS2(Coords3D, double)
    double getSize()
    {
        return sqrt(x*x+y*y+z*z);
    };
    double getSize2()
    {
        return x*x+y*y+z*z;
    };
    double dot(Coords3D op2)
    {
        return ( x * op2.x + y * op2.y + z * op2.z);
    }; // Dot product
    void cross(Coords3D op1, Coords3D op2)
    {
        x=(op1.y*op2.z)-(op1.z*op2.y);
        y=(op1.z*op2.x)-(op1.x*op2.z);
        z=(op1.x*op2.y)-(op1.y*op2.x);
    }; // Cross product
    Coords3D getUnitVector()
    {
        double size = getSize();
        if (size!=0) return (*this/size);
        else return Coords3D();
    };
    Coords3D getUnitVector(double size)
    {
        if (size!=0) return (*this/size);
        else return Coords3D();
    };
    XYZ_DECLAREVAR(double)
};

#define f3d   Coords3D
#define i3d   CoordsScreen

extern f3d sin(const f3d &d3);
extern f3d cos(const f3d &d3);
extern f3d tan(const f3d &d3);
extern f3d atan(const f3d &d3);
extern f3d sqrt(const f3d &d3);
extern f3d closestPoint(f3d &p, f3d &a, f3d &b, bool segment);

#endif
