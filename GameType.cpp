#include <typeinfo.h>
using namespace std;
#include "GameType.h"
#include "Ship.h"
#include "Missile.h"
#include "Projectile.h"
#include "Global.h"
#include "MissileExplosion.h"
#include "CursorTarget.h"
#include "Launcher.h"
#include "SpawnPoint.h"
#include "Flag.h"
#include "Player.h"
#include "Team.h"
#include "Game.h"
#include "ObjectiveIndicator.h"
#include "ShipExplosion.h"
#include "StarField.h"
#include "CloakField.h"
//need to include all types of leaf objects in the object tree structure thingy

string meshes[] = {"none", "pod", "scout", "scout2", "scout3", "recon", "recon2", "recon3", "heavy", "heavy2", "heavy3", "sniper", "sniper2", "sniper3", "ewar", "ewar2", "ewar3", "logistics", "logistics2", "logistics3", "drone", "base", "flag", "target", "directionalIndicator", "cloak", "warp", "missile", "bullet", "mf", "sse", "orangecircle", "bluecircle", "darkbluecircle"};

GameType::GameType(void)
{
	Game* thegame = getGame();
	name = "Capture The Flag";
	objectives = "The objective of this game is to\ncapture the enemy's flag and\nbring it back to your base.";
	pointsToWin = 5;
	playersPerTeam = 1;

	teams = new Team[2];
	teams[0].number = 0;
	teams[1].number = 1;

	pauseMenu.backdrop = new _Picture((Sprite*)thegame->getRenderable(blank), Vec2D());
	pauseMenu.headings.insert(pauseMenu.headings.end(), _Heading("Paused", Vec2D(thegame->gWindow.Width/2 - (6/2.0f*16), 50)));
	pauseMenu.pictures.insert(pauseMenu.pictures.end(), _Picture((Sprite*)thegame->getRenderable(seperatorBar), Vec2D(thegame->gWindow.Width - 400.0f, 100.0f)));
	pauseMenu.labels.insert(pauseMenu.labels.end(), _Label(this->objectives, Vec2D(thegame->gWindow.Width - 390.0f, 100.0f)));

	settingsMenu.pointer = new _Pointer((Sprite*)thegame->getRenderable(mousePointer));
	settingsMenu.headings.insert(settingsMenu.headings.end(), _Heading("Capture The Flag", Vec2D(thegame->gWindow.Width/2.0f - (16/2 *16), 50.0f)));
	settingsMenu.buttons.insert(settingsMenu.buttons.end(), _Button("", Vec2D(thegame->gWindow.Width/2.0f - 24, 100.0f), Vec2D(48.0f, 48.0f), &ChangeGameObjective, 1, (Sprite*)thegame->getRenderable(upArrow)));
	settingsMenu.buttons.insert(settingsMenu.buttons.end(), _Button("", Vec2D(thegame->gWindow.Width/2.0f - 24, 200.0f), Vec2D(48.0f, 48.0f), &ChangeGameObjective, -1, (Sprite*)thegame->getRenderable(downArrow)));
	settingsMenu.labels.insert(settingsMenu.labels.end(), _Label("Points To Win:", Vec2D(thegame->gWindow.Width/2.0f - 350.0f, 170.0f)));
	settingsMenu.numbers.insert(settingsMenu.numbers.end(), _DynamicNumber(&this->pointsToWin, Vec2D(thegame->gWindow.Width/2.0f - 4.0f, 170.0f), false));
	settingsMenu.buttons.insert(settingsMenu.buttons.end(), _Button("Start", Vec2D((float)thegame->gWindow.Width - 280.0f, (float)thegame->gWindow.Height - 70.0f), Vec2D(250.0f, 50.0f), &StartGame, NULL, (Sprite*)thegame->getRenderable(buttonOutline)));
	settingsMenu.buttons.insert(settingsMenu.buttons.end(), _Button("", Vec2D(thegame->gWindow.Width/2.0f - 24, 285.0f), Vec2D(48.0f, 48.0f), &ChangeTeamPlayers, 1, (Sprite*)thegame->getRenderable(upArrow)));
	settingsMenu.buttons.insert(settingsMenu.buttons.end(), _Button("", Vec2D(thegame->gWindow.Width/2.0f - 24, 380.0f), Vec2D(48.0f, 48.0f), &ChangeTeamPlayers, -1, (Sprite*)thegame->getRenderable(downArrow)));
	settingsMenu.labels.insert(settingsMenu.labels.end(), _Label("Players Per Team", Vec2D(thegame->gWindow.Width/2.0f - 350.0f, 350.0f)));
	settingsMenu.numbers.insert(settingsMenu.numbers.end(), _DynamicNumber(&this->playersPerTeam, Vec2D(thegame->gWindow.Width/2.0f - 4.0f, 350.0f), false));
}

GameType::~GameType(void)
{
	delete [] teams;
}

void GameType::OnStart(Game* thegame)
{
	thegame->arena = new Arena(0, 1200.0f);
	teams[0].playersAllowed = 2;
	teams[1].playersAllowed = 2;
	/*create a player few player objects based on the stuff the server sent*/
	teams[0].color = Color(0.0f, 1.0f, 0.0f);
	teams[1].color = Color(1.0f, 0.0f, 0.0f);
	teams[0].name = "You";
	teams[1].name = "Computer Team";

	//spawn points
	SpawnPoint* sp = new SpawnPoint(thegame->getNextIndex(), 0, NULL, spawnPoint, Vec2D(-430.0f, 40.0f), Vec2D(0.0f, 1.0f));
	thegame->insertObject(sp);
	teams[0].spawnpoint = sp;
	sp = new SpawnPoint(thegame->getNextIndex(), 1, NULL, spawnPoint, Vec2D(430.0f, -40.0f), Vec2D(0.0f, 1.0f));
	thegame->insertObject(sp);
	teams[1].spawnpoint = sp;

	//objectives
	Flag* f = new Flag(thegame->getNextIndex(), flag, Vec2D(30.0f, 430.0f), 0);
	thegame->insertObject(f);
	teams[0].flag = f;
	f = new Flag(thegame->getNextIndex(), flag, Vec2D(-30.0f, -430.0f), 1);
	thegame->insertObject(f);
	teams[1].flag = f;

	Player* p = new Player(thegame->getNextPlayer(), 0, NULL);
	thegame->players[p->number] = p;

	Player* p2 = new Player(thegame->getNextPlayer(), 1, NULL);
	thegame->players[p2->number] = p2;

	teams[0].addPlayer(p->number);
	teams[1].addPlayer(p2->number);
	p->name = "Player";
	p2->name = "Computer";

	thegame->pc = p;
	/*players should be settled now********************************************************************************************/

	//create other stuff here
	/*****************************************************************************PLAYER CHARACTER IS MADE BY NOW!!!!!****************/
	thegame->ChangeShip(SpawnShip(pod, p->number));
	p2->ship = SpawnShip(pod, p2->number);

	ObjectiveIndicator* oi = new ObjectiveIndicator(thegame->getNextIndex(), thegame->pc, directionalIndicator, Vec2D(), Vec2D(), true);
	thegame->insertObject(oi);
	oi = new ObjectiveIndicator(thegame->getNextIndex(), thegame->pc, directionalIndicator, Vec2D(), Vec2D(), false);
	thegame->insertObject(oi);

	//starfield
	thegame->sf = new StarField(thegame->getNextIndex(), thegame->pc, star, Vec2D(), Vec2D(0.0f, 1.0f), thegame->pc->ship);

	//finish up the menu items with team names
	pauseMenu.labels.insert(pauseMenu.labels.end(), _Label(string("Captures to win: ") + itos(pointsToWin), Vec2D(getGame()->gWindow.Width - 390.0f, 200.0f)));
	pauseMenu.labels.insert(pauseMenu.labels.end(), _Label("Captures", Vec2D(460.0f, 100.0f)));
	int count = 0;
	int tcount = 0;

	float headingTopMargin = 30.0f;
	float headingBottomMargin = 10.0f;
	float teamSize = 22.0f;
	float playerSize = 15.0f;
	float teamPlayerSeperator = 10.0f;
	float playerMargin = 5.0f;
	for(unsigned int t = 0; t < 2; ++t)
	{
		pauseMenu.headings.insert(pauseMenu.headings.end(), _Heading(teams[t].name, Vec2D(25.0f, 125.0f + (tcount * headingTopMargin) + (tcount * teamSize) + (tcount * headingBottomMargin) + (count * playerSize) + (count * playerMargin))));
		pauseMenu.numbers.insert(pauseMenu.numbers.end(), _DynamicNumber(&(teams[t].points), Vec2D(460.0f, 125.0f + (tcount * headingTopMargin) + (tcount * teamSize) + (tcount * headingBottomMargin) + (count * playerSize) + (count * playerMargin)), true));
		for(list<unsigned int>::iterator index = teams[t].players.begin(); index != teams[t].players.end(); ++index)
		{
			pauseMenu.labels.insert(pauseMenu.labels.end(), _Label(getGame()->players[(*index)]->name, Vec2D(45.0f, 125.0f + ((tcount) * headingTopMargin) + ((tcount + 1) * teamSize) + ((tcount + 1) * headingBottomMargin) + (count * playerSize) + (count * playerMargin))));
			pauseMenu.numbers.insert(pauseMenu.numbers.end(), _DynamicNumber(&getGame()->players[(*index)]->points, Vec2D(400.0f, 125.0f + ((tcount) * headingTopMargin) + ((tcount + 1) * teamSize) + ((tcount + 1) * headingBottomMargin) + (count * playerSize) + (count * playerMargin))));
			++count;
		}
		++tcount;
	}
	//LoopSound(thegame->getSound(marker));
	//the first ship was a temporary one (pod) to get things set up. Now the player chooses his/her ship
	thegame->pc->ship->Kill();
	thegame->tempvel = thegame->pc->ship->vel;
	thegame->temploc = thegame->pc->ship->loc;
	thegame->tempdir = thegame->pc->ship->dir;
	changeGameMode(choosingShip);
}

//respawn gives a player back their ship based on their current position
Ship* GameType::RespawnShip(Vec2D loc, Vec2D dir, Vec2D vel, renderableType mT, unsigned int playerNum)
{
	Ship* temp;
	Game* g = getGame();
	//this only works if they're using a regular mesh (won't work for sse or mf)
	int i = (int)mT - 2;
	i /= 3;
	int level = g->players[playerNum]->shipLevels[i];
	mT = (renderableType)(mT + (level - 1));

	switch(mT)
	{
	case pod:
		temp = new Ship(g->getNextIndex(), g->players[playerNum], pod, loc, dir, 132.0f, 100.0f, 30, 5, NULL, NULL);
		break;
	case scout:
		temp = new Ship(g->getNextIndex(), g->players[playerNum], scout, loc, dir, 220.0f, 350.0f, 40, 40, &Cloak, &Uncloak);
		break;
	case scout2:
		temp = new Ship(g->getNextIndex(), g->players[playerNum], scout, loc, dir, 264.0f, 350.0f, 50, 35, NULL, NULL);
		break;
	case scout3:
		temp = new Ship(g->getNextIndex(), g->players[playerNum], scout, loc, dir, 308.0f, 350.0f, 70, 35, NULL, NULL);
		break;
	case recon:
		temp = new Ship(g->getNextIndex(), g->players[playerNum], recon, loc, dir, 264.0f, 450.0f, 50, 60, &SpawnWarpField, NULL);
		break;
	case recon2:
		temp = new Ship(g->getNextIndex(), g->players[playerNum], recon, loc, dir, 308.0f, 450.0f, 80, 40, NULL, NULL);
		break;
	case recon3:
		temp = new Ship(g->getNextIndex(), g->players[playerNum], recon, loc, dir, 352.0f, 450.0f, 100, 40, NULL, NULL);
		break;
	case heavy:
		temp = new Ship(g->getNextIndex(), g->players[playerNum], heavy, loc, dir, 176.0f, 450.0f, 90, 90, NULL, NULL);
		break;
	case heavy2:
		temp = new Ship(g->getNextIndex(), g->players[playerNum], heavy, loc, dir, 176.0f, 450.0f, 120, 70, NULL, NULL);
		break;
	case heavy3:
		temp = new Ship(g->getNextIndex(), g->players[playerNum], heavy, loc, dir, 176.0f, 450.0f, 180, 70, NULL, NULL);
		break;
	case sniper:
		temp = new Ship(g->getNextIndex(), g->players[playerNum], sniper, loc, dir, 220.0f, 500.0f, 50, 60, NULL, NULL);
		break;
	case sniper2:
		temp = new Ship(g->getNextIndex(), g->players[playerNum], sniper, loc, dir, 220.0f, 500.0f, 100, 60, NULL, NULL);
		break;
	case sniper3:
		temp = new Ship(g->getNextIndex(), g->players[playerNum], sniper, loc, dir, 220.0f, 500.0f, 120, 60, NULL, NULL);
		break;
	case ewar:
		temp = new Ship(g->getNextIndex(), g->players[playerNum], ewar, loc, dir, 176.0f, 300.0f, 80, 30, NULL, NULL);
		break;
	case ewar2:
		temp = new Ship(g->getNextIndex(), g->players[playerNum], ewar2, loc, dir, 220.0f, 300.0f, 100, 30, NULL, NULL);
		break;
	case ewar3:
		temp = new Ship(g->getNextIndex(), g->players[playerNum], ewar2, loc, dir, 264.0f, 300.0f, 120, 30, NULL, NULL);
		break;
	case logistics:
		temp = new Ship(g->getNextIndex(), g->players[playerNum], logistics, loc, dir, 308.0f, 400.0f, 145, 45, NULL, NULL);
		break;
	case logistics2:
		temp = new Ship(g->getNextIndex(), g->players[playerNum], logistics2, loc, dir, 308.0f, 400.0f, 240, 45, NULL, NULL);
		break;
	case logistics3:
		temp = new Ship(g->getNextIndex(), g->players[playerNum], logistics3, loc, dir, 308.0f, 400.0f, 330, 45, NULL, NULL);
		break;
	default:
		g->state = over;
		break;
	}
	temp->vel = vel;
	g->insertObject(temp);
	//g->players[playerNum]->ship = temp;
	if(g->players[playerNum] == getGame()->pc)
		getGame()->ChangeShip(temp);	//need this for extra functions (like view zoom and targetCursor generation)
	else
		g->players[playerNum]->ship = temp;
	return temp;
}

Ship* GameType::SpawnShip(renderableType mT, unsigned int playerNum, Vec2D dir, Vec2D vel)
{
	Ship* temp;
	Game* g = getGame();
	switch(mT)
	{
	case pod:
		temp = new Ship(g->getNextIndex(), g->players[playerNum], pod, teams[g->players[playerNum]->team].spawnpoint->loc, dir, 132.0f, 100.0f, 30, 5, NULL, NULL);
		break;
	case scout:
		temp = new Ship(g->getNextIndex(), g->players[playerNum], scout, teams[g->players[playerNum]->team].spawnpoint->loc, dir, 220.0f, 350.0f, 40, 40, NULL, NULL);
		break;
	case scout2:
		temp = new Ship(g->getNextIndex(), g->players[playerNum], scout, teams[g->players[playerNum]->team].spawnpoint->loc, dir, 264.0f, 350.0f, 50, 35, NULL, NULL);
		break;
	case scout3:
		temp = new Ship(g->getNextIndex(), g->players[playerNum], scout, teams[g->players[playerNum]->team].spawnpoint->loc, dir, 308.0f, 350.0f, 70, 35, NULL, NULL);
		break;
	case recon:
		temp = new Ship(g->getNextIndex(), g->players[playerNum], recon, teams[g->players[playerNum]->team].spawnpoint->loc, dir, 264.0f, 450.0f, 50, 60, &SpawnWarpField, NULL);
		break;
	case recon2:
		temp = new Ship(g->getNextIndex(), g->players[playerNum], recon, teams[g->players[playerNum]->team].spawnpoint->loc, dir, 308.0f, 450.0f, 80, 40, NULL, NULL);
		break;
	case recon3:
		temp = new Ship(g->getNextIndex(), g->players[playerNum], recon, teams[g->players[playerNum]->team].spawnpoint->loc, dir, 352.0f, 450.0f, 100, 40, NULL, NULL);
		break;
	case heavy:
		temp = new Ship(g->getNextIndex(), g->players[playerNum], heavy, teams[g->players[playerNum]->team].spawnpoint->loc, dir, 176.0f, 450.0f, 90, 90, NULL, NULL);
		break;
	case heavy2:
		temp = new Ship(g->getNextIndex(), g->players[playerNum], heavy, teams[g->players[playerNum]->team].spawnpoint->loc, dir, 176.0f, 450.0f, 120, 70, NULL, NULL);
		break;
	case heavy3:
		temp = new Ship(g->getNextIndex(), g->players[playerNum], heavy, teams[g->players[playerNum]->team].spawnpoint->loc, dir, 176.0f, 450.0f, 180, 70, NULL, NULL);
		break;
	case sniper:
		temp = new Ship(g->getNextIndex(), g->players[playerNum], sniper, teams[g->players[playerNum]->team].spawnpoint->loc, dir, 220.0f, 500.0f, 50, 60, NULL, NULL);
		break;
	case sniper2:
		temp = new Ship(g->getNextIndex(), g->players[playerNum], sniper, teams[g->players[playerNum]->team].spawnpoint->loc, dir, 220.0f, 500.0f, 100, 60, NULL, NULL);
		break;
	case sniper3:
		temp = new Ship(g->getNextIndex(), g->players[playerNum], sniper, teams[g->players[playerNum]->team].spawnpoint->loc, dir, 220.0f, 500.0f, 120, 60, NULL, NULL);
		break;
	case ewar:
		temp = new Ship(g->getNextIndex(), g->players[playerNum], ewar, teams[g->players[playerNum]->team].spawnpoint->loc, dir, 176.0f, 300.0f, 80, 30, NULL, NULL);
		break;
	case ewar2:
		temp = new Ship(g->getNextIndex(), g->players[playerNum], ewar2, teams[g->players[playerNum]->team].spawnpoint->loc, dir, 220.0f, 450.0f, 100, 30, NULL, NULL);
		break;
	case ewar3:
		temp = new Ship(g->getNextIndex(), g->players[playerNum], ewar2, teams[g->players[playerNum]->team].spawnpoint->loc, dir, 264.0f, 450.0f, 120, 30, NULL, NULL);
		break;
	case logistics:
		temp = new Ship(g->getNextIndex(), g->players[playerNum], logistics, teams[g->players[playerNum]->team].spawnpoint->loc, dir, 308.0f, 400.0f, 145, 45, NULL, NULL);
		break;
	case logistics2:
		temp = new Ship(g->getNextIndex(), g->players[playerNum], logistics2, teams[g->players[playerNum]->team].spawnpoint->loc, dir, 308.0f, 400.0f, 240, 45, NULL, NULL);
		break;
	case logistics3:
		temp = new Ship(g->getNextIndex(), g->players[playerNum], logistics3, teams[g->players[playerNum]->team].spawnpoint->loc, dir, 308.0f, 400.0f, 330, 45, NULL, NULL);
		break;
	default:
		g->state = over;
		break;
	}
	Vec2D placement(0.0f, 1.0f);
	placement.setDirection(randomFloat(0.0f, 3.14159f*2.0f));
	placement.scale(randomFloat(100.0f, 200.0f));
	temp->loc += placement;
	temp->vel = vel;
	g->insertObject(temp);
	g->players[playerNum]->ship = temp;
	return temp;
}

void GameType::onCollision(Base* a, Base* b)
{
	getGame()->debug.AddEvent(string("Collision of ") + typeid(*a).name() + " and " + typeid(*b).name());
	if(a->dead || b->dead)
		return;
	if(typeid((*a)) == typeid(Ship) && typeid((*b)) == typeid(Ship))
	{
		//reverse the last step (or two if necessary) so they're not colliding anymore
		while(Distance(a->loc, b->loc) <= a->mesh->radius + b->mesh->radius)
		{
			a->loc -= a->vel * (float)getGame()->deltaTime/1000.0f;
			b->loc -= b->vel * (float)getGame()->deltaTime/1000.0f;
		}

		float cor = 0.7f;
		Vec2D rVelA, rVelB;
		Ship* sa = (Ship*)(a), *sb = (Ship*)(b);
		Vec2D aCompEffect, aCompDorm, bCompEffect, bCompDorm;
		//comp means component

		//A stuff
		aCompEffect = sb->loc - sa->loc;
		aCompEffect.normalize();
		aCompEffect.scale(sa->vel.dot(aCompEffect));
		
		aCompDorm = sa->vel - aCompEffect;

		//B stuff
		bCompEffect = sa->loc - sb->loc;
		bCompEffect.normalize();
		bCompEffect.scale(sb->vel.dot(bCompEffect));

		bCompDorm = sb->vel - bCompEffect;

		//equation stuff
		rVelA = (aCompEffect*(float)(sa->mass) + bCompEffect*(float)(sb->mass) + (bCompEffect - aCompEffect)*(float)(sb->mass)*cor)/(float)(sa->mass + sb->mass);
		rVelB = (aCompEffect*(float)(sa->mass) + bCompEffect*(float)(sb->mass) + (aCompEffect - bCompEffect)*(float)(sa->mass)*cor)/(float)(sa->mass + sb->mass);
		sa->vel = rVelA + aCompDorm;
		sb->vel = rVelB + bCompDorm;
	}
	else if(typeid(*a) == typeid(Missile) && typeid(*b) == typeid(Ship) || typeid(*a) == typeid(Ship) && typeid(*b) == typeid(Missile))
	{
		if(typeid(*a) == typeid(Missile))
		{
			Base* temp = a;
			a = b;
			b = temp;
		}
		//a is now the ship, no matter what
		b->dead = true;
		Mesh* m = (Mesh*)(b->mesh);
		for(unsigned int i = 0; i < m->mountNum; ++i)
			m->actualMounts[i]->dead = true;
		getGame()->insertObject(new MissileExplosion(getGame()->getNextIndex(), b->loc));
		((Ship*)a)->hitpoints -= 20;
		if(((Ship*)a)->hitpoints <= 0)
		{
			if(a->owner && b->owner)
				getGame()->messages.addMessage((a->owner->name + " has died to " + b->owner->name + "'s missile.").c_str(), teams[b->owner->team].color);
			HandleDeath((Ship*)a);	//ship will take care of its own death, but we need to make sure that we handle messages and flag raising
		}
	}
	else if(typeid(*a) == typeid(Projectile) && typeid(*b) == typeid(Ship) || typeid(*a) == typeid(Ship) && typeid(*b) == typeid(Projectile))
	{
		if(typeid(*a) == typeid(Projectile))
		{
			Base* temp = a;
			a = b;
			b = temp;
		}
		//a is now the ship, no matter what
		b->dead = true;
		//getGame()->insertObject(new MissileExplosion(getGame()->getNextIndex(), b->loc));
		((Ship*)a)->hitpoints -= 3;
		if(((Ship*)a)->hitpoints <= 0)
		{
			if(a->owner && b->owner)
				getGame()->messages.addMessage((a->owner->name + " has died to " + b->owner->name + "'s projectile.").c_str(), teams[b->owner->team].color);
			HandleDeath((Ship*)a);
		}
	}
	else if(typeid(*a) == typeid(CursorTarget) && typeid(*b) == typeid(Ship) || typeid(*b) == typeid(CursorTarget) && typeid(*a) == typeid(Ship))
	{
		if(typeid(*a) == typeid(CursorTarget))
		{
			Base* temp = a;
			a = b;
			b = temp;
		}
		//b is now a CursorTarget, no matter what, and a is the object you pointed at
		if(b->owner != a->owner && !a->dead)
		{
			Game* g = getGame();
			if(!g->pc->ship->targetObj)
			{
				g->messages.addMessage(string("Target set to " + meshes[a->mesh->type]).c_str(), Color(1.0f, 1.0f, 0.0f));
				g->pc->ship->targetObj = a;
			}
		}
	}
	else if(typeid(*a) == typeid(Ship) && typeid(*b) == typeid(CloakField) || typeid(*a) == typeid(CloakField) && typeid(*b) == typeid(Ship))
	{
		if(typeid(*a) == typeid(CloakField))
		{
			Base* temp = a;
			a = b;
			b = temp;
		}
		//a is now pointing to a Ship
		if(b->owner->ship != a)
		{
			b->Kill();
		}
	}
	else if(typeid(*a) == typeid(Ship) && typeid(*b) == typeid(SpawnPoint) || typeid(*a) == typeid(SpawnPoint) && typeid(*b) == typeid(Ship))
	{
		if(typeid(*a) == typeid(SpawnPoint))
		{
			Base* temp = a;
			a = b;
			b = temp;
		}
		//a is now pointing to a Ship
		if(a->mesh->type == pod && a->owner->team == ((SpawnPoint*)(b))->teamNumber)	//if the ship is a pod and it's team is same as spawner
		{
			if(a->owner == getGame()->pc)	//if it's the player character's pod
			{
				getGame()->temploc = a->loc;	//save the ship's state for the next ship
				getGame()->tempdir = a->dir;
				getGame()->tempvel = a->vel;
				a->Kill();
				changeGameMode(choosingShip);
			}
			else
			{
				a->Kill();
				Ship* s = (Ship*)a;
				RespawnShip(s->loc, s->dir, s->vel, scout, a->owner->number);
			}
			//getGame()->state = choosingShip;
		}
		else if(((SpawnPoint*)b)->teamNumber == a->owner->team)	//if Ship is same team as SpawnPoint
		{
			if(teams[a->owner->team?0:1].carryingFlag == a)	//if ship is carrying the other team's flag
			{
				++a->owner->shipLevels[(a->mesh->type-2)/3];	//(- 2) to account for none and pod, then (/ 3) to select the ship type
				++(a->owner->points);
				++(teams[a->owner->team].points);
				getGame()->messages.addMessage((string("Team ") + teams[a->owner->team].name + " has scored a point!").c_str(), teams[a->owner->team].color);
				teams[a->owner->team?0:1].carryingFlag = NULL;
				Flag* f = new Flag(getGame()->getNextIndex(), flag, Vec2D(0.0f, 400.0f * (a->owner->team? 1:-1)), (a->owner->team? 0:1));	//insert opposite team's flag
				getGame()->insertObject(f);
				teams[a->owner->team?0:1].flag = f;
			}
		}
	}
	else if(typeid(*a) == typeid(Ship) && typeid(*b) == typeid(Flag) || typeid(*a) == typeid(Flag) && typeid(*b) == typeid(Ship))
	{
		if(typeid(*a) == typeid(Flag))
		{
			Base* temp = a;
			a = b;
			b = temp;
		}
		//a is now pointing to a Ship

		if(a->mesh->type != pod)
		{
			if(a->owner->team != ((Flag*)(b))->teamNumber)	//if not same team
			{
				b->Kill();
				teams[a->owner->team?0:1].carryingFlag = (Ship*)a;
			}
		}
	}
}

void GameType::HandleDeath(Ship* b)
{
	PlayThatSound(getGame()->getSound(explode));
	if(b->mesh->type == pod)
		if(b->owner->shipLevels[b->owner->lastShip] > 1)
			b->owner->shipLevels[b->owner->lastShip] -= 1;
	if(teams[b->owner->team?0:1].carryingFlag == b)	//if this ship is carrying the other team's flag
	{
		teams[b->owner->team?0:1].carryingFlag = NULL;
		Flag* f = new Flag(getGame()->getNextIndex(), flag, b->loc, (b->owner->team? 0:1));	//insert opposite team's flag
		getGame()->insertObject(f);
		teams[b->owner->team?0:1].flag = f;
	}
}

bool GameType::checkWin()
{
	if(teams[0].points == pointsToWin || teams[1].points == pointsToWin)
		return true;
	return false;
}

void GameType::changeObjective(int points)
{
	if(pointsToWin + points <= 5 && pointsToWin + points >= 1)
		pointsToWin += points;
}

void GameType::changeTeamPlayers(int members)
{
	if(playersPerTeam + members <= 8 && playersPerTeam + members >= 1)
		playersPerTeam += members;
}