#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "Vec2D.h"
#include "Color.h"

enum renderableType {
	none, pod,
	scout, scout2, scout3,
	recon, recon2, recon3,
	heavy, heavy2, heavy3,
	sniper, sniper2, sniper3,
	ewar, ewar2, ewar3,
	logistics, logistics2, logistics3,
	drone, _hostileDrone, spawnPoint, flag, target, directionalIndicator, cloak, warp, heal, empbomb, missile, bullet, mf, sse,
	orangeCircle, blueCircle, darkBlueCircle, star,
	healthBar, healthBarOrange, healthBarRed, healthBarOutline, speedBar, speedBarOutline,
	otherPlayerHealthOutline, otherPlayerHealthBar, otherPlayerHealthBarOrange, otherPlayerHealthBarRed,
	radarOutline, radarBackground, allyBlip, enemyBlip, objectiveBlip,
	buttonOutline, upArrow, downArrow, mousePointer, blank, seperatorBar};

class Game;

class Renderable
{
public:
	Renderable(void);
	virtual ~Renderable(void);

	virtual void Render(Vec2D* loc, Vec2D* dir) = 0;
	renderableType type;
	float radius;
};

#endif