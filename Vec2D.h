#ifndef VEC2D_H
#define VEC2D_H

/*
SPECIAL THANKS TO: John "Zach" Tucker for his early work on the Vec2D class. Some corrections and additions have been made,
but most of the credit for this class goes to him.
*/

#include <string>
using namespace std;

#include "Polygon.h"

class Vec2D
{
public:
	float x;
	float y;
//	float magnitude;		// magnitude. Necessary, given mag()?
//	float direction;	// direction (heading), either in degrees or radians

public:
	Vec2D(void);
	Vec2D(float _x, float _y);
	Vec2D(sf2::CUSTOMVERTEX);

	void add(Vec2D v);
	void sub(Vec2D v);				// subtract v from this vector, see the other version
	void scale(float m);			// scale the length by m
	float dot(Vec2D v);				// the dot product
	void normalize(void);
	void project(Vec2D v);			// project this vector onto and in the direction of v
	void limit(float l);
	void zero();

	float angleBetween(Vec2D v);	// return angle between this vector and v
	float getDirection(void);		// return heading (in radians)
	void setDirection(float d);
	float getMag(void);
	void setMag(float m);		// set the magnitude (length) of the vector
	
	Vec2D add(Vec2D a, Vec2D b) const;	// add 2 vectors but return the result
	void add(float a, float b);
	Vec2D sub(Vec2D a, Vec2D b) const;
	void sub(float a, float b);
	Vec2D operator-() const;
	Vec2D operator+(const Vec2D& right) const;
	Vec2D operator+=(const Vec2D& right);
	Vec2D operator-(const Vec2D& right) const;
	Vec2D operator-= (const Vec2D& right);
	Vec2D operator/(const float& right) const;
	Vec2D operator*(const float& right) const;
	Vec2D operator*=(const float& right);
	Vec2D operator=(const Vec2D& right);
	Vec2D operator*(const Vec2D& right) const;

	Vec2D get(void);			// return a copy of this vector
	void set(float _x, float _y);
	void set_x(float _x);
	void set_y(float _y);

	string getCoords();
	string getMagString();
};

//float operator*(Vec2D u, Vec2D v);	//use .dot for the dot product, * for multiplication (above)
Vec2D operator/(const float& left, const Vec2D& right);

#endif