#ifndef BASE_H
#define BASE_H

class Renderable;
#include "Vec2D.h"
#include "BoundingBox.h"
class Player;
struct Color;
class ParticleEffect;
class Mesh;

enum renderableType;

class Base
{
public:
	Base(unsigned int index, Player*, Vec2D location, Vec2D direction, bool collides = true, bool draw = true);
	virtual ~Base(void);
	virtual void run(float deltaTime) = 0;
	Mesh* CopyMesh(renderableType mT, const Color& C);
	ParticleEffect* CopyParticleEffect(renderableType mt);
	virtual void Kill();

	Player* owner;
	Renderable* mesh;

	Vec2D loc;
	Vec2D vel;
	Vec2D acc;
	Vec2D dir;
	Vec2D turn;
	BoundingBox* bb;
	int index;

	bool collidable;
	bool dead;
	bool drawMe;
};

#endif