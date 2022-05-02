//---------------------------------------------------------------------------

#ifndef nsVectorH
#define nsVectorH
//---------------------------------------------------------------------------
#include <math.h>
//---------------------------------------------------------------------------
class nsVector
{
private:
	double pos[4];
private:
	void copy(const nsVector& him)
	{
		pos[0] = him.pos[0];
		pos[1] = him.pos[1];
		pos[2] = him.pos[2];
		pos[3] = him.pos[3];
	}
public:
	nsVector(){set(0,0,0);}
	nsVector(const nsVector& him){copy(him);}
	nsVector(double x,double y,double z=0.0){set(x,y,z);}
	nsVector(unsigned char *px){set(px[0],px[1],px[2]);}

	~nsVector(){}
public:
	nsVector& set(double x,double y,double z=0.0);
	nsVector& set(double* pix);
	nsVector& set(unsigned char *v);
	nsVector  e();
	double    leng();
	double    square();
	nsVector  comp2();
	double    ave();
	nsVector  route();
	bool      isZero();
public:
	nsVector& operator=(const nsVector &vect);
	nsVector& operator=(double v);
	double&   operator[](int youso);
	nsVector& operator*=(double bai);
	nsVector& operator+=(const nsVector &vect);
	nsVector  operator+ (const nsVector &vect);
	nsVector& operator-=(const nsVector &vect);
	nsVector  operator- (const nsVector &vect);
	double    operator *(const nsVector &vect);
	bool      operator ==(const nsVector &vect);
	nsVector  operator/ (double v);
	nsVector  operator/=(double v);

	double& X(){return pos[0];}
	double& Y(){return pos[1];}
	double& Z(){return pos[2];}
	double& W(){return pos[3];}

};

#endif
