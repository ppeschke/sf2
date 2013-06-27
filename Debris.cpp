#include "Debris.h"
#include "Global.h"

Debris::Debris(Ship* s, Player* p, Vec2D location, Vec2D direction, bool collides, bool draw) : ParticleEffect(0, p, none, location, direction, NULL, collides, draw)
{
	this->mesh = this;
	shipFinalDir = s->dir;
	Vec2D rotationpoint;
	Vec2D midpoint;
	Mesh* m = (Mesh*)(s->mesh);
	Mesh* meshpart = NULL;
	for(unsigned int i = 0; i < m->polyNum; ++i)
	{
		for(unsigned int p = 0; p < m->polys[i].length - 1; ++p)
		{
			meshpart = new Mesh();
			meshpart->polyNum = 1;
			meshpart->mountNum = 0;
			meshpart->polys = new sf2::Polygon();
			meshpart->polys->length = 2;
			meshpart->polys->vertices = new sf2::CUSTOMVERTEX[2];
			meshpart->polys->vertices[0] = m->polys[i].vertices[p];
			meshpart->polys->vertices[1] = m->polys[i].vertices[p+1];
			midpoint = Vec2D(meshpart->polys->vertices[1].X + meshpart->polys->vertices[0].X, meshpart->polys->vertices[1].Y + meshpart->polys->vertices[0].Y)/2.0f;
			meshpart->radius = Distance(Vec2D(meshpart->polys->vertices[0].X, meshpart->polys->vertices[0].Y), Vec2D(meshpart->polys->vertices[1].X, meshpart->polys->vertices[1].Y)) / 2.0f;
			Vec2D addedVel = 3.0f/midpoint;	//difference between midpoint and ship's midpoint
			addedVel.scale(meshpart->radius);	//scale by size of part

			//rotationpoint
			Vec2D start(m->polys[i].vertices[p]);
			Vec2D end = Vec2D(m->polys[i].vertices[p+1]);
			rotationpoint = start - end;
			rotationpoint.scale(randomFloat(0.0f, 1.0f));
			rotationpoint += end;

			float limit = 3.14159f * 2 / meshpart->radius;

			float rotationspeed = randomFloat(-limit, limit);
			LoadToVRAM(meshpart);
			parts.insert(parts.end(), new DebrisPart(meshpart, randomFloat(3.0f, 10.0f), s->loc, s->vel + addedVel, rotationpoint, midpoint, rotationspeed));
		}
	}
}

Debris::~Debris(void)
{
}

void Debris::run(float deltaTime)
{
	bool partsdead = true;
	for(list<DebrisPart*>::iterator index = parts.begin(); index != parts.end(); ++index)
	{
		if(!(*index)->dead)
		{
			(*index)->run(deltaTime);
			partsdead = false;
		}
	}
	if(partsdead)
		Kill();
}

void Debris::Render(Vec2D* loc, Vec2D* dir)
{
	for(list<DebrisPart*>::iterator index = parts.begin(); index != parts.end(); ++index)
	{
		if(!(*index)->dead)
		{
			DrawMeshPartWithRotations(&(*index)->m->polys[0], &(*index)->pos, &shipFinalDir, &(*index)->rotationpoint, (*index)->rot);
		}
	}
}