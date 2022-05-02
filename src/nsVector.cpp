#include "nsVector.h"

nsVector& nsVector::set(double x,double y,double z)
{
	pos[0]=x;
	pos[1]=y;
	pos[2]=z;
	pos[3]=0;

	return *this;
}
//ベクトルセット
nsVector& nsVector::set(double *v)
{
	pos[0]=v[0];
	pos[1]=v[1];
	pos[2]=v[2];
	pos[3]=0;

	return *this;
}

nsVector& nsVector::set(unsigned char *v)
{
	pos[0]=v[0];
	pos[1]=v[1];
	pos[2]=v[2];
	pos[3]=0;

	return *this;
}
nsVector nsVector::e()
{
	nsVector buf(*this);
	double  vlen = buf.leng();
	if(isZero() == true)
	{
		buf.set(0,0,0);
		return buf;
	}
	buf *= (1.0 / vlen);

	return buf;
}
double  nsVector::leng()
{
	double ln = sqrt(square());

	return ln;
}

double nsVector::square()
{
	double val;

	val = pos[0]*pos[0] + pos[1]*pos[1] + pos[2]*pos[2];

	return val;
}

nsVector nsVector::comp2()
{
	nsVector val(pos[0]*pos[0],pos[1]*pos[1],pos[2]*pos[2]);

	return val;
}
double nsVector::ave()
{
	double v = (pos[0]+pos[1]+pos[2])/3.0f;

	return v;
}
nsVector nsVector::route()
{
	nsVector buf;

	buf.pos[0] = sqrt(pos[0]);
	buf.pos[1] = sqrt(pos[1]);
	buf.pos[2] = sqrt(pos[2]);
	buf.pos[3] = 0;

	return buf;
}
bool nsVector::isZero()
{
	bool res = (leng() < 0.0001);

	return res;
}

nsVector& nsVector::operator=(const nsVector &vect)
{
	copy(vect);

	return *this;
}
double& nsVector::operator[](int youso)
{
	return pos[youso];
}
nsVector& nsVector::operator*=(double bai)
{
	pos[0] *= bai;
	pos[1] *= bai;
	pos[2] *= bai;

	return *this;
}
nsVector& nsVector::operator+=(const nsVector &vect)
{
	pos[0] += vect.pos[0];
	pos[1] += vect.pos[1];
	pos[2] += vect.pos[2];

	return *this;
}
nsVector nsVector::operator+(const nsVector &vect)
{
	nsVector buf;

	nsVector& v = const_cast<nsVector&>(vect);

	buf.pos[0] = pos[0] + v[0];
	buf.pos[1] = pos[1] + v[1];
	buf.pos[2] = pos[2] + v[2];

	return buf;
}
nsVector& nsVector::operator-=(const nsVector &vect)
{
	nsVector& v = const_cast<nsVector&>(vect);

	pos[0] -= v[0];
	pos[1] -= v[1];
	pos[2] -= v[2];

	return *this;
}
nsVector nsVector::operator-(const nsVector &vect)
{
	nsVector buf;

	nsVector& v = const_cast<nsVector&>(vect);

	buf.pos[0] = pos[0] - v[0];
	buf.pos[1] = pos[1] - v[1];
	buf.pos[2] = pos[2] - v[2];

	return buf;
}
double nsVector::operator*(const nsVector &vect)
{
	double naiseki;
	nsVector& v = const_cast<nsVector&>(vect);
	naiseki = this->pos[0] * v[0]
			+ this->pos[1] * v[1]
			+ this->pos[2] * v[2];

	return naiseki;
}
bool nsVector::operator ==(const nsVector &vect)
{
	bool Chk;

	Chk = (
		pos[0] == vect.pos[0] &&
		pos[1] == vect.pos[1] &&
		pos[2] == vect.pos[2]);

	return Chk;
}
nsVector nsVector::operator / (double v)
{
	nsVector buf;

	buf.pos[0] = pos[0]/v;
	buf.pos[1] = pos[1]/v;
	buf.pos[2] = pos[2]/v;

	return buf;
}
nsVector nsVector::operator/=(double v)
{
	pos[0] /= v;
	pos[1] /= v;
	pos[2] /= v;

	return *this;
}

