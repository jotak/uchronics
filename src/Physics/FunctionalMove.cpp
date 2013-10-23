#include "FunctionalMove.h"

#define MIN_SPEED		0.02f

// -----------------------------------------------------------------
// Name : FunctionalMove
// -----------------------------------------------------------------
FunctionalMove::FunctionalMove(f3d vector, double fFullSpeed, MoveFunction func) : Movement()
{
    m_Vector = vector;
    m_fFullSpeed = fFullSpeed / vector.getSize();
    m_Func = func;
    m_f1DPos = 0.0f;
}

// -----------------------------------------------------------------
// Name : ~FunctionalMove
// -----------------------------------------------------------------
FunctionalMove::~FunctionalMove()
{
}

// -----------------------------------------------------------------
// Name : apply
// -----------------------------------------------------------------
Coords3D FunctionalMove::apply(double delta, f3d position)
{
	if (m_State == Inactive) {
		m_State = InProgress;
	}
    double old1DPos = m_f1DPos;
    double funcRes = m_Func(m_f1DPos);
    m_f1DPos += delta * m_fFullSpeed * max(funcRes, MIN_SPEED);
    if (m_f1DPos >= 1.0f) {
    	m_f1DPos = 1.0f;
        m_State = Finished;
    }
    return position + m_Vector * (m_f1DPos - old1DPos);
}
