#include "Vec2D.h"
#include "Math.h"
const double PI = 4.0*atan(1.0);

Vec2D::Vec2D(void)
{
	x = 0;
	y = 0;
}

Vec2D::Vec2D(float _x, float _y)
{
	x = _x;
	y = _y;
}

void Vec2D::add(Vec2D v)
{
	x += v.x;
	y += v.y;
}

void Vec2D::sub(Vec2D v)
{
	x -= v.x;
	y -= v.y;
}

void Vec2D::scale(float m)
{
	x *= m;
	y *= m;
}

float Vec2D::dot(Vec2D v)
{
	return (x*v.x + y*v.y);
}

void Vec2D::normalize()
{
	float m = getMag();
	if(m != 0.0f)
	{
		x /= m;
		y /= m;
	}
}

void Vec2D::project(Vec2D v)
{
	setMag(getMag()*cos(angleBetween(v)));
	setDirection(v.getDirection());
}

void Vec2D::limit(float l)
{
	if (getMag() > l)
		setMag(l);
}
float Vec2D::angleBetween(Vec2D v)
{
	return acos(dot(v));
}

float Vec2D::getDirection()
{
	return atan2(y, x);
}

void Vec2D::setDirection(float dir)
{
	float m = getMag();
	x = m*cos(dir);
	y = m*sin(dir);
}

float Vec2D::getMag()
{
	return sqrt(x*x + y*y);
}

void Vec2D::setMag(float m)
{
	// backwards way of doing normalize() followed with scale(m);
	float d = getDirection();
	x = m*cos(d);
	y = m*sin(d);
}

Vec2D Vec2D::add(Vec2D a, Vec2D b) const
{
	return Vec2D(a.x + b.x, a.y + b.y);
}
void Vec2D::add(float a, float b)
{
	x += a;
	y += b;
}

Vec2D Vec2D::sub(Vec2D a, Vec2D b) const
{
	return Vec2D(a.x - b.x, a.y - b.y);
}

void Vec2D::sub(float a, float b)
{
	x -= a;
	y -= b;
}

Vec2D Vec2D::get()
{
	return Vec2D(x, y);
}

void Vec2D::set(float _x, float _y)
{
	x = _x;
	y = _y;
}

void Vec2D::set_x(float _x)
{
	x = _x;
}

void Vec2D::set_y(float _y)
{
	y = _y;
}

string Vec2D::getCoords()
{
	char _x[64], _y[64];
	sprintf_s(_x, 64, "%.*f", 2, x);
	sprintf_s(_y, 64, "%.*f", 2, y);
	string s = string (_x) + ", " + string (_y);
	return s;
}
string Vec2D::getMagString()
{
	char c[256];
	sprintf_s(c, 256, "%.*f", 2, getMag());
	return c;
}

Vec2D Vec2D::operator-() const
{
	Vec2D neg;
	neg.x = -this->x;
	neg.y = -this->y;
	return neg;
}

Vec2D Vec2D::operator+(const Vec2D& right) const
{
	Vec2D sum;
	sum.x = this->x + right.x;
	sum.y = this->y + right.y;
	return sum;
}

Vec2D Vec2D::operator+=(const Vec2D& right)
{
	(*this) = (*this) + right;
	return (*this);
}

Vec2D Vec2D::operator/(const float& right) const
{
	Vec2D quotient = (*this);
	quotient.x /= right;
	quotient.y /= right;
	return quotient;
}

Vec2D Vec2D::operator*(const float& right) const
{
	Vec2D product = (*this);
	product.x *= right;
	product.y *= right;
	return product;
}

Vec2D Vec2D::operator*=(const float& right)
{
	this->x *= right;
	this->y *= right;
	return (*this);
}

Vec2D Vec2D::operator -= (const Vec2D& right)
{
	(*this) = (*this) - right;
	return *this;
}

Vec2D Vec2D::operator-(const Vec2D& right) const
{
	return Vec2D(this->x - right.x, this->y - right.y);
}

void Vec2D::zero()
{
	x = 0.0f;
	y = 0.0f;
}

Vec2D Vec2D::operator=(const Vec2D& right)
{
	x = right.x;
	y = right.y;
	return (*this);
}

Vec2D Vec2D::operator*(const Vec2D& right) const
{
	Vec2D product;
	product.x = this->x * right.x;
	product.y = this->y * right.y;
	return product;
}

/*float operator*(Vec2D u, Vec2D v)	//now use dot() for dot product, this function is ambiguous with Vec2D::operator*
{
	return (u.x * v.x + u.y * v.y);
}*/