/*****************************************************************************
 *  \author 
 *  	Erwin Aertbelien, Div. PMA, Dep. of Mech. Eng., K.U.Leuven
 *
 *  \version 
 *		ORO_Geometry V0.2
 *
 *	\par History
 *		- $log$
 *
 *	\par Release
 *		$Id: motionprofile_traphalf.cpp,v 1.1.1.1.2.5 2003/07/24 13:26:15 psoetens Exp $
 *		$Name:  $ 
 ****************************************************************************/


//#include "error.h"
#include "geometry/motionprofile_traphalf.h"

#ifdef USE_NAMESPACE
namespace ORO_Geometry {
#endif


MotionProfile_TrapHalf::MotionProfile_TrapHalf(double _maxvel,double _maxacc,bool _starting):
		  maxvel(_maxvel),maxacc(_maxacc),starting(_starting) {}

void MotionProfile_TrapHalf::PlanProfile1(double v,double a) {
	a3 = 0;
	a2 = 0;
	a1 = startpos;
	b3 = a/2;
	b2 = -a*t1;
	b1 = startpos + a2*t1*t1/2;
	c3 = 0;
	c2 = v;
	c1 = endpos - v*duration;
}

void MotionProfile_TrapHalf::PlanProfile2(double v,double a) {
	a3 = 0;
	a2 = v;
	a1 = startpos;
	b3 = -a/2;
	b2 = a*t2;
	b1 = endpos - a*t2*t2/2;
	c3 = 0;
	c2 = 0;
	c1 = endpos;
}

void MotionProfile_TrapHalf::SetProfile(double pos1,double pos2) {
	startpos        = pos1;
	endpos          = pos2;
	double s        = sign(endpos-startpos);
	duration		= s*(endpos-startpos)/maxvel+maxvel/maxacc/2.0;
	if (starting) {
		t1 = 0;
		t2 = maxvel/maxacc;
		PlanProfile1(maxvel*s,maxacc*s);
	} else {
		t1 = duration-maxvel/maxacc;
		t2 = duration;
		PlanProfile2(s*maxvel,s*maxacc);
	}
}

void MotionProfile_TrapHalf::SetProfileDuration(
	double pos1,double pos2,double newduration) 
{
    SetProfile(pos1,pos2);
    double factor = duration/newduration;

    if ( factor > 1 )
        return;

	double s        = sign(endpos-startpos);
	double tmp      = 2.0*s*(endpos-startpos)/maxvel;
	double v        = s*maxvel;
	duration        = newduration;
	if (starting) {
		if (tmp > duration) {
			t1 = 0;
			double a = v*v/2.0/(v*duration-(endpos-startpos));
			t2 = v/a;
			PlanProfile1(v,a);
		} else {
			t2 = duration;
			double a = v*v/2.0/(endpos-startpos);
			t1 = t2-v/a;
			PlanProfile1(v,a);
		}
	} else {
		if (tmp > duration) {
			t2 = duration;
			double a = v*v/2.0/(v*duration-(endpos-startpos));
			t1 = t2-v/a;
			PlanProfile2(v,a);
		} else {
			double a = v*v/2.0/(endpos-startpos);
			t1 = 0;
			t2 = v/a;
			PlanProfile2(v,a);
		}
	}
}

double MotionProfile_TrapHalf::Duration() const {
	return duration;
}

double MotionProfile_TrapHalf::Pos(double time) const {
	if (time < 0) {
		return startpos;
	} else if (time<t1) {
		return a1+time*(a2+a3*time);
	} else if (time<t2) {
		return b1+time*(b2+b3*time);
	} else if (time<=duration) {
		return c1+time*(c2+c3*time);
	} else {
		return endpos;
	}
}
double MotionProfile_TrapHalf::Vel(double time) const {
	if (time < 0) {
		return 0;
	} else if (time<t1) {
		return a2+2*a3*time;
	} else if (time<t2) {
		return b2+2*b3*time;
	} else if (time<=duration) {
		return c2+2*c3*time;
	} else {
		return 0;
	}
}

double MotionProfile_TrapHalf::Acc(double time) const {
	if (time < 0) {
		return 0;
	} else if (time<t1) {
		return 2*a3;
	} else if (time<t2) {
		return 2*b3;
	} else if (time<=duration) {
		return 2*c3;
	} else {
		return 0;
	}
}

MotionProfile* MotionProfile_TrapHalf::Clone() {
	return new MotionProfile_TrapHalf(maxvel,maxacc,starting);
}

MotionProfile_TrapHalf::~MotionProfile_TrapHalf() {}


#ifdef OROINT_OS_STDIOSTREAM
void MotionProfile_TrapHalf::Write(ostream& os) const {
	os << "TRAPEZOIDALHALF[" << maxvel << "," << maxacc << "," << starting << "]";
}
#endif





#ifdef USE_NAMESPACE
}
#endif

