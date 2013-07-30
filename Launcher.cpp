#include "Global.h"
#include "Launcher.h"
#include "Missile.h"
#include "Projectile.h"
#include "Drone.h"
#include "Ship.h"
#include "Player.h"
#include "CursorTarget.h"

Launcher::Launcher(unsigned int index, Player* o, renderableType mesh, Base* follow, Vec2D loc, Vec2D dir, float Rof, weaponType wT, bool collides, bool draw) : Weapon(index, o, follow, loc, dir, Rof, collides, draw)
{
	//launchers don't have meshhhesss or bounding boxesss does they preciouss?
	if(this->owner == getGame()->pc && getGame()->pc->ship)
		getGame()->CreateCursor();

	shootWhat = wT;
}

Launcher::~Launcher(void)
{
}

void Launcher::Kill()
{
	dead = true;
	//launcher has no mesh
}

void Launcher::shoot()
{
	if(Timer <= 0.0f)
	{
		getGame()->debug.AddEvent("Shooting");
		Timer = (owner->ship->abilityTimer > 0.0f? owner->ship->abilityROF:ROF);
		Base* temp;
		switch(shootWhat)
		{
		case shootMissile:
			temp = new Missile(getGame()->getNextIndex(), owner, missile, loc, dir);
			PlayThatSound(getGame()->getSound(launch));
			break;
		case shootProjectile:
			temp = new Projectile(getGame()->getNextIndex(), owner, bullet, this->loc, dir);
			PlayThatSound(getGame()->getSound(pew));
			break;
		case shootDrone:
			temp = new Drone(getGame()->getNextIndex(), owner, drone, this->loc, this->dir, true);
			break;
		default:
			exit(EXIT_FAILURE);
		}
		getGame()->insertObject(temp);
	}
}

void Launcher::run(float deltaTime)
{	
}

void Launcher::endStep(float deltaTime)
{
	Vec2D temp = coords;
	temp.setDirection(((Ship*)follow)->dir.getDirection() - 3.14159f/2.0f + temp.getDirection());
	this->loc = ((Ship*)follow)->loc + temp;
	this->dir = ((Ship*)follow)->dir;
	if(Timer > 0.0f)
		Timer -= deltaTime;
}