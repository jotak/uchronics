#ifndef _FUNCTIONAL_MOVE_H
#define _FUNCTIONAL_MOVE_H

#include "Movement.h"
#include "../Utils/utils.h"

typedef tr1::function<double (double)> MoveFunction;

class FunctionalMove : public Movement
{
public:
	FunctionalMove(f3d vector, double fFullSpeed, MoveFunction func);
    virtual ~FunctionalMove();

    virtual f3d apply(double delta, f3d position);

private:
    f3d m_Vector;
    double m_fFullSpeed;
    MoveFunction m_Func;
    double m_f1DPos;
};

#endif
