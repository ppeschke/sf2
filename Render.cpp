#include "Global.h"
#include "Color.h"
#include "Ship.h"
#include "Player.h"
#include "Drone.h"
#include "Sprite.h"
#include "Team.h"
#include <fstream>
using namespace std;

ofstream outlog;

void LoadMesh(const char* filename, renderableType mT, int scale);
void LoadTexture(LPCWSTR filename, renderableType mT, int r, int g, int b);
void LoadSprite(LPCWSTR filename, renderableType mt);

void LoadGraphics(Game* thegame)
{
	InitDirect3D(thegame);
	//load graphics
	LoadMesh("meshes\\drone.mesh", drone, 3);
	LoadMesh("meshes\\scout.mesh", scout, 8);
	LoadMesh("meshes\\pod.mesh", pod, 4);
	LoadMesh("meshes\\recon.mesh", recon, 20);
	LoadMesh("meshes\\ewar.mesh", ewar, 5);
	LoadMesh("meshes\\ewar2.mesh", ewar2, 15);
	LoadMesh("meshes\\sniper.mesh", sniper, 15);
	LoadMesh("meshes\\logistics.mesh", logistics, 7);
	LoadMesh("meshes\\logistics2.mesh", logistics2, 15);
	LoadMesh("meshes\\logistics3.mesh", logistics3, 15);
	LoadMesh("meshes\\flag.mesh", flag, 15);
	LoadMesh("meshes\\target.mesh", target, 3);
	LoadMesh("meshes\\directionalindicator.mesh", directionalIndicator, 15);
	LoadMesh("meshes\\warp.mesh", warp, 20);
	LoadMesh("meshes\\heavy.mesh", heavy, 16);
	LoadMesh("meshes\\missile.mesh", missile, 15);
	LoadMesh("meshes\\mf.mesh", mf, 10);
	LoadMesh("meshes\\bullet.mesh", bullet, 15);
	LoadMesh("meshes\\sse.mesh", sse, 15);
	LoadMesh("meshes\\spawnPoint.mesh", spawnPoint, 10);
	LoadMesh("meshes\\hostileDrone.mesh", _hostileDrone, 3);
	LoadMesh("meshes\\cloak.mesh", cloak, 250);
	LoadMesh("meshes\\heal.mesh", heal, 25);
	LoadMesh("meshes\\empbomb.mesh", empbomb, 100);

	LoadTexture(L"textures\\fire.png", orangeCircle, 118, 89, 55);
	LoadTexture(L"textures\\blueflame.png", blueCircle, 118, 89, 55);
	LoadTexture(L"textures\\smoke.png", darkBlueCircle, 118, 89, 55);
	LoadTexture(L"textures\\star.png", star, 255, 255, 255);
													/*
													IF YOUR TEXTURES ARE NOT IN SIZES OF POWERS OF TWO, DIRECTX WILL STRETCH THEM
													IT'S NOT WORTH IT!! MAKE IT A POWER OF TWO
													*/
	LoadSprite(L"textures\\HealthBarOutline.png", healthBarOutline);
	LoadSprite(L"textures\\HealthBar.png", healthBar);
	LoadSprite(L"textures\\SpeedBar.png", speedBar);
	LoadSprite(L"textures\\SpeedBarOutline.png", speedBarOutline);
	LoadSprite(L"textures\\HealthBarOrange.png", healthBarOrange);
	LoadSprite(L"textures\\HealthBarRed.png", healthBarRed);
	LoadSprite(L"textures\\OtherPlayerHealthOutline.png", otherPlayerHealthOutline);
	LoadSprite(L"textures\\OtherPlayerHealthBar.png", otherPlayerHealthBar);
	LoadSprite(L"textures\\OtherPlayerHealthBarOrange.png", otherPlayerHealthBarOrange);
	LoadSprite(L"textures\\OtherPlayerHealthBarRed.png", otherPlayerHealthBarRed);
	LoadSprite(L"textures\\RadarOutline.png", radarOutline);
	LoadSprite(L"textures\\RadarBackground.png", radarBackground);
	LoadSprite(L"textures\\AllyBlip.png", allyBlip);
	LoadSprite(L"textures\\EnemyBlip.png", enemyBlip);
	LoadSprite(L"textures\\ObjectiveBlip.png", objectiveBlip);
	LoadSprite(L"textures\\ButtonOutline.png", buttonOutline);
	LoadSprite(L"textures\\MousePointer.png", mousePointer);
	LoadSprite(L"textures\\blank.png", blank);
	LoadSprite(L"textures\\SeperatorBar.png", seperatorBar);
	LoadSprite(L"textures\\upArrow.png", upArrow);
	LoadSprite(L"textures\\downArrow.png", downArrow);
}

void Render(Game* thegame)
{
	switch(thegame->state)
	{
	case debugHolding:
	case debugMoving:
	case running:
		thegame->debug.EnterSubProcess("Render", 18);
		StartRender();
		AdjustCamera(thegame->pc->ship->loc.x, thegame->pc->ship->loc.y, thegame);

		//arena boundary
		thegame->arena->mesh->Render(&thegame->arena->loc, &thegame->arena->dir);

		if(thegame->sf)
			thegame->sf->mesh->Render(&Vec2D(), &Vec2D());

		//sensorStrength
		//Draw2DCircle(thegame->pc->ship->loc.x, thegame->pc->ship->loc.y, thegame->pc->ship->sensorStrength, D3DCOLOR_XRGB(255, 255, 0));

		for(unsigned int index = 0; index <= thegame->lastIndex; ++index)
		{
			if(thegame->objects[index] != NULL && thegame->objects[index]->drawMe)
			{
				thegame->objects[index]->mesh->Render(&thegame->objects[index]->loc, &thegame->objects[index]->dir);
				//Drone flightTarget
				/*if(typeid(*thegame->objects[index]) == typeid(Drone))
				{
					Draw2DLine(((Drone*)(thegame->objects[index]))->circle.x - 5, ((Drone*)(thegame->objects[index]))->circle.y - 5, ((Drone*)(thegame->objects[index]))->circle.x + 5, ((Drone*)(thegame->objects[index]))->circle.y + 5, D3DCOLOR_XRGB(255, 0, 0));
					Draw2DLine(((Drone*)(thegame->objects[index]))->circle.x - 5, ((Drone*)(thegame->objects[index]))->circle.y + 5, ((Drone*)(thegame->objects[index]))->circle.x + 5, ((Drone*)(thegame->objects[index]))->circle.y - 5, D3DCOLOR_XRGB(255, 0, 0));
				}*/

				//bounding box
				/*if(thegame->objects[index]->bb != NULL)
					DrawRect(thegame->objects[index]->bb->x, thegame->objects[index]->bb->y,thegame->objects[index]->bb->length, thegame->objects[index]->bb->length, D3DCOLOR_XRGB(255, 255, 255));*/
				if(typeid(*thegame->objects[index]) == typeid(Ship))
				{
					//velocity
					//Draw2DLine(thegame->objects[index]->loc.x, thegame->objects[index]->loc.y, thegame->objects[index]->loc.x + thegame->objects[index]->vel.x * 20, thegame->objects[index]->loc.y + thegame->objects[index]->vel.y * 20, D3DCOLOR_XRGB(255, 255, 0));

					//Draw2DCircle(thegame->objects[index]->loc.x, thegame->objects[index]->loc.y, 100.0f, D3DCOLOR_XRGB(255, 0, 0));	//drone control range
					//Draw2DCircle(thegame->objects[index]->loc.x, thegame->objects[index]->loc.y, 300.0f, D3DCOLOR_XRGB(150, 0, 0));	//drone outter range

					Draw2DCircle(thegame->objects[index]->loc.x, thegame->objects[index]->loc.y, thegame->objects[index]->mesh->radius, D3DCOLOR_XRGB(255, 255, 0));	//shields
					//targeting square
					if(thegame->pc->ship->targetObj == thegame->objects[index])
						DrawRect(thegame->objects[index]->bb->x, thegame->objects[index]->bb->y,thegame->objects[index]->bb->length, thegame->objects[index]->bb->length, D3DCOLOR_XRGB(255, 255, 255));
				
				}
			}
		}

		//thegame->collisionDetection.Render();

		thegame->messages.displayMessages();

		StartSpriteRender();
		if(thegame->pc != NULL)
		{
			int hp;
			//Other Player's Health Bars and radar blips
			for(unsigned int index = 0; index <= thegame->lastIndex; ++index)
			{
				if(thegame->objects[index] != NULL && typeid(*thegame->objects[index]) == typeid(Ship) && thegame->objects[index] != thegame->pc->ship && Distance(thegame->pc->ship->loc, thegame->objects[index]->loc) <= thegame->pc->ship->sensorStrength * 1.7f)
				{
					Ship* s = (Ship*)(thegame->objects[index]);
					Color c = thegame->gametype->teams[thegame->objects[index]->owner->team].color;
					string n = thegame->objects[index]->owner->name;
					Vec2D screencoords = GetScreenCoords(s->loc + Vec2D(0.0f, s->mesh->radius));	//top of ship

					screencoords += Vec2D(-50.0f, -15.0f);

					//player name
					drawText(n.c_str(), (int)screencoords.x + 50 - (10 * n.length()/2), (int)screencoords.y - 25, int(c.red*255), int(c.green*255), int(c.blue*255));
					hp = (int)((float)(s->hitpoints)/s->maxHitpoints * 100);
					for(int index = 0; index < hp; ++index)
					{
						if(hp < 25)
							DrawSprite((Sprite*)(thegame->getRenderable(otherPlayerHealthBarRed)), (int)screencoords.x + (index), (int)screencoords.y);
						else if(hp < 50)
							DrawSprite((Sprite*)(thegame->getRenderable(otherPlayerHealthBarOrange)), (int)screencoords.x + (index), (int)screencoords.y);
						else
							DrawSprite((Sprite*)(thegame->getRenderable(otherPlayerHealthBar)), (int)screencoords.x + (index), (int)screencoords.y);
					}
					//outline for other player's health bar
					DrawSprite((Sprite*)(thegame->getRenderable(otherPlayerHealthOutline)), (int)screencoords.x, (int)screencoords.y);
				}	//end if
			}	//end for (health bars and names)

			//health bar
			hp = (int)((float)(thegame->pc->ship->hitpoints)/thegame->pc->ship->maxHitpoints * 100);
			int speed = (int)((float)(thegame->pc->ship->vel.getMag())/thegame->pc->ship->maxSpeed * 100);
			for(int index = 0; index < hp; ++index)
			{
				if(hp < 25)
					DrawSprite((Sprite*)(thegame->getRenderable(healthBarRed)), thegame->gWindow.Width/2-98 + (index * 2), thegame->gWindow.Height-76);
				else if(hp < 50)
					DrawSprite((Sprite*)(thegame->getRenderable(healthBarOrange)), thegame->gWindow.Width/2-98 + (index * 2), thegame->gWindow.Height- 76);
				else
					DrawSprite((Sprite*)(thegame->getRenderable(healthBar)), thegame->gWindow.Width/2-98 + (index * 2), thegame->gWindow.Height- 76);
			}
			//speedbar
			for(int index = 0; index < speed; ++index)
				DrawSprite((Sprite*)(thegame->getRenderable(speedBar)), thegame->gWindow.Width/2-98 + (index * 2), thegame->gWindow.Height- 38);

			//outlines for health and speed bar
			DrawSprite((Sprite*)(thegame->getRenderable(healthBarOutline)), thegame->gWindow.Width/2-98, thegame->gWindow.Height-76);
			DrawSprite((Sprite*)(thegame->getRenderable(speedBarOutline)), thegame->gWindow.Width/2-98, thegame->gWindow.Height-38);

			//radar background
			DrawSpriteTransparent((Sprite*)(thegame->getRenderable(radarBackground)), thegame->gWindow.Width - 192, thegame->gWindow.Height - 192, 127);

			//radar
			for(unsigned int index = 0; index <= thegame->lastIndex; ++index)
			{
				if(thegame->objects[index] != NULL)
				{
					if(typeid(*thegame->objects[index]) == typeid(SpawnPoint) || typeid(*thegame->objects[index]) == typeid(Flag) && Distance(thegame->pc->ship->loc, thegame->objects[index]->loc) <= thegame->pc->ship->sensorStrength * 10.0f)
					{
						Vec2D midpoint(float(thegame->gWindow.Width - 96), float(thegame->gWindow.Height - 96));
						Vec2D diff = thegame->objects[index]->loc - thegame->pc->ship->loc;
						diff.y = -diff.y;
						diff.x /= (thegame->pc->ship->sensorStrength * 10.0f);
						diff.y /= (thegame->pc->ship->sensorStrength * 10.0f);
						diff.scale(90);
						DrawSprite((Sprite*)(thegame->getRenderable(objectiveBlip)), (int)midpoint.x + (int)diff.x - 3, (int)midpoint.y + (int)diff.y - 3);
					}
					if(typeid(*thegame->objects[index]) == typeid(Ship) && thegame->objects[index] != thegame->pc->ship && Distance(thegame->pc->ship->loc, thegame->objects[index]->loc) <= thegame->pc->ship->sensorStrength * 10.0f)
					{
						Vec2D midpoint(float(thegame->gWindow.Width - 96), float(thegame->gWindow.Height - 96));
						Vec2D diff = thegame->objects[index]->loc - thegame->pc->ship->loc;
						diff.y = -diff.y;
						diff.x /= (thegame->pc->ship->sensorStrength * 10.0f);
						diff.y /= (thegame->pc->ship->sensorStrength * 10.0f);
						diff.scale(90);
						if(thegame->objects[index]->owner->team == thegame->pc->team)
							DrawSprite((Sprite*)(thegame->getRenderable(allyBlip)), (int)midpoint.x + (int)diff.x - 3, (int)midpoint.y + (int)diff.y - 3);
						else
							DrawSprite((Sprite*)(thegame->getRenderable(enemyBlip)), (int)midpoint.x + (int)diff.x - 3, (int)midpoint.y + (int)diff.y - 3);
					}
				}
			}	//end for (radar)
			DrawSprite((Sprite*)(thegame->getRenderable(radarOutline)), thegame->gWindow.Width - 192, thegame->gWindow.Height - 192);
		}	//end if pc != NULL
		EndSpriteRender();

		if(thegame->pc != NULL)
		{
			drawText(thegame->pc->ship->loc.getCoords().c_str(), 5, thegame->gWindow.Height-50, 0, 255, 0);
			//drawText(thegame->pc->ship->vel.getMagString().c_str(), 5, thegame->gWindow.Height-35, 0, 255, 0);
			drawText("Shields:", thegame->gWindow.Width/2 - 185, thegame->gWindow.Height - 81, 0, 255, 0);
			drawText("Speed:", thegame->gWindow.Width/2 - 185, thegame->gWindow.Height - 43, 0, 255, 255);
		}

		EndRender();

		thegame->debug.ExitSubProcess();
		break;
	case paused:
		
		StartRender();
		AdjustCamera(thegame->pc->ship->loc.x, thegame->pc->ship->loc.y, thegame);
		
		//sensorStrength
		//Draw2DCircle(thegame->pc->ship->loc.x, thegame->pc->ship->loc.y, thegame->pc->ship->sensorStrength, D3DCOLOR_XRGB(255, 255, 0));

		if(thegame->sf)
			thegame->sf->mesh->Render(&Vec2D(), &Vec2D());

		//arena boundary
		thegame->arena->mesh->Render(&thegame->arena->loc, &thegame->arena->dir);
		/*Draw2DCircle(0, 0, thegame->arenaRadius, D3DCOLOR_XRGB(255, 102, 0));
		Draw2DCircle(0, 0, thegame->arenaRadius - 5.0f, D3DCOLOR_XRGB(255, 102, 0));*/

		for(unsigned int index = 0; index <= thegame->lastIndex; ++index)
		{
			if(thegame->objects[index] != NULL && thegame->objects[index]->drawMe)
			{
				thegame->objects[index]->mesh->Render(&thegame->objects[index]->loc, &thegame->objects[index]->dir);
				//Drone flightTarget
				/*if(typeid(*thegame->objects[index]) == typeid(Drone))
				{
					Draw2DLine(((Drone*)(thegame->objects[index]))->circle.x - 5, ((Drone*)(thegame->objects[index]))->circle.y - 5, ((Drone*)(thegame->objects[index]))->circle.x + 5, ((Drone*)(thegame->objects[index]))->circle.y + 5, D3DCOLOR_XRGB(255, 0, 0));
					Draw2DLine(((Drone*)(thegame->objects[index]))->circle.x - 5, ((Drone*)(thegame->objects[index]))->circle.y + 5, ((Drone*)(thegame->objects[index]))->circle.x + 5, ((Drone*)(thegame->objects[index]))->circle.y - 5, D3DCOLOR_XRGB(255, 0, 0));
				}*/

				//bounding box
				/*if(thegame->objects[index]->bb != NULL)
					DrawRect(thegame->objects[index]->bb->x, thegame->objects[index]->bb->y,thegame->objects[index]->bb->length, thegame->objects[index]->bb->length, D3DCOLOR_XRGB(255, 255, 255));*/
				if(typeid(*thegame->objects[index]) == typeid(Ship))
				{
					//AI target stuff
					/*if(thegame->objects[index] != thegame->pc->ship)
					{
						Ship* x = ((Ship*)thegame->objects[index]);
						Draw2DLine(x->loc.x, x->loc.y, x->loc.x + x->diff.x, x->loc.y + x->diff.y, D3DCOLOR_XRGB(255, 255, 255));
					}*/
					//velocity
					//Draw2DLine(thegame->objects[index]->loc.x, thegame->objects[index]->loc.y, thegame->objects[index]->loc.x + thegame->objects[index]->vel.x * 20, thegame->objects[index]->loc.y + thegame->objects[index]->vel.y * 20, D3DCOLOR_XRGB(255, 255, 0));

					/*Draw2DCircle(thegame->objects[index]->loc.x, thegame->objects[index]->loc.y, 100.0f, D3DCOLOR_XRGB(255, 0, 0));	//drone control range
					Draw2DCircle(thegame->objects[index]->loc.x, thegame->objects[index]->loc.y, 300.0f, D3DCOLOR_XRGB(150, 0, 0));	//drone outter range*/

					Draw2DCircle(thegame->objects[index]->loc.x, thegame->objects[index]->loc.y, thegame->objects[index]->mesh->radius, D3DCOLOR_XRGB(255, 255, 0));	//shields
					//targeting square
					if(thegame->pc->ship->targetObj == thegame->objects[index])
						DrawRect(thegame->objects[index]->bb->x, thegame->objects[index]->bb->y,thegame->objects[index]->bb->length, thegame->objects[index]->bb->length, D3DCOLOR_XRGB(255, 255, 255));
				
				}
			}
		}
		thegame->messages.displayMessages();
		
		StartSpriteRender();
		if(thegame->pc != NULL)
		{
			int hp;
			//Other Player's Health Bars and radar blips
			for(unsigned int index = 0; index <= thegame->lastIndex; ++index)
			{
				if(thegame->objects[index] != NULL && typeid(*thegame->objects[index]) == typeid(Ship) && thegame->objects[index] != thegame->pc->ship && Distance(thegame->pc->ship->loc, thegame->objects[index]->loc) <= thegame->pc->ship->sensorStrength * 1.7f)
				{
					Ship* s = (Ship*)(thegame->objects[index]);
					Color c = thegame->gametype->teams[thegame->objects[index]->owner->team].color;
					string n = thegame->objects[index]->owner->name;
					Vec2D screencoords = GetScreenCoords(s->loc + Vec2D(0.0f, s->mesh->radius));	//top of ship

					screencoords += Vec2D(-50.0f, -15.0f);

					//player name
					drawText(n.c_str(), (int)screencoords.x + 50 - (10 * n.length()/2), (int)screencoords.y - 25, (int)(c.red*255), (int)(c.green*255), (int)(c.blue*255));
					hp = (int)((float)(s->hitpoints)/s->maxHitpoints * 100);
					for(int index = 0; index < hp; ++index)
					{
						if(hp < 25)
							DrawSprite((Sprite*)(thegame->getRenderable(otherPlayerHealthBarRed)), (int)screencoords.x + (index), (int)screencoords.y);
						else if(hp < 50)
							DrawSprite((Sprite*)(thegame->getRenderable(otherPlayerHealthBarOrange)), (int)screencoords.x + (index), (int)screencoords.y);
						else
							DrawSprite((Sprite*)(thegame->getRenderable(otherPlayerHealthBar)), (int)screencoords.x + (index), (int)screencoords.y);
					}
					//outline for other player's health bar
					DrawSprite((Sprite*)(thegame->getRenderable(otherPlayerHealthOutline)), (int)screencoords.x, (int)screencoords.y);
				}	//end if
			}	//end for (health bars and names)

			//health bar
			hp = (int)((float)(thegame->pc->ship->hitpoints)/thegame->pc->ship->maxHitpoints * 100);
			int speed = (int)((float)(thegame->pc->ship->vel.getMag())/thegame->pc->ship->maxSpeed * 100);
			for(int index = 0; index < hp; ++index)
			{
				if(hp < 25)
					DrawSprite((Sprite*)(thegame->getRenderable(healthBarRed)), thegame->gWindow.Width/2-98 + (index * 2), thegame->gWindow.Height-76);
				else if(hp < 50)
					DrawSprite((Sprite*)(thegame->getRenderable(healthBarOrange)), thegame->gWindow.Width/2-98 + (index * 2), thegame->gWindow.Height- 76);
				else
					DrawSprite((Sprite*)(thegame->getRenderable(healthBar)), thegame->gWindow.Width/2-98 + (index * 2), thegame->gWindow.Height- 76);
			}
			//speedbar
			for(int index = 0; index < speed; ++index)
				DrawSprite((Sprite*)(thegame->getRenderable(speedBar)), thegame->gWindow.Width/2-98 + (index * 2), thegame->gWindow.Height- 38);

			//outlines for health and speed bar
			DrawSprite((Sprite*)(thegame->getRenderable(healthBarOutline)), thegame->gWindow.Width/2-98, thegame->gWindow.Height-76);
			DrawSprite((Sprite*)(thegame->getRenderable(speedBarOutline)), thegame->gWindow.Width/2-98, thegame->gWindow.Height-38);

			//radar background
			DrawSpriteTransparent((Sprite*)(thegame->getRenderable(radarBackground)), thegame->gWindow.Width - 192, thegame->gWindow.Height - 192, 127);

			//radar
			for(unsigned int index = 0; index <= thegame->lastIndex; ++index)
			{
				if(thegame->objects[index] != NULL)
				{
					if(typeid(*thegame->objects[index]) == typeid(SpawnPoint) || typeid(*thegame->objects[index]) == typeid(Flag) && Distance(thegame->pc->ship->loc, thegame->objects[index]->loc) <= thegame->pc->ship->sensorStrength * 10.0f)
					{
						Vec2D midpoint((float)(thegame->gWindow.Width - 96), (float)(thegame->gWindow.Height - 96));
						Vec2D diff = thegame->objects[index]->loc - thegame->pc->ship->loc;
						diff.y = -diff.y;
						diff.x /= (thegame->pc->ship->sensorStrength * 10.0f);
						diff.y /= (thegame->pc->ship->sensorStrength * 10.0f);
						diff.scale(90);
						DrawSprite((Sprite*)(thegame->getRenderable(objectiveBlip)), (int)midpoint.x + (int)diff.x - 3, (int)midpoint.y + (int)diff.y - 3);
					}
					if(typeid(*thegame->objects[index]) == typeid(Ship) && thegame->objects[index] != thegame->pc->ship && Distance(thegame->pc->ship->loc, thegame->objects[index]->loc) <= thegame->pc->ship->sensorStrength * 10.0f)
					{
						Vec2D midpoint(float(thegame->gWindow.Width - 96), float(thegame->gWindow.Height - 96));
						Vec2D diff = thegame->objects[index]->loc - thegame->pc->ship->loc;
						diff.y = -diff.y;
						diff.x /= (thegame->pc->ship->sensorStrength * 10.0f);
						diff.y /= (thegame->pc->ship->sensorStrength * 10.0f);
						diff.scale(90);
						if(thegame->objects[index]->owner->team == thegame->pc->team)
							DrawSprite((Sprite*)(thegame->getRenderable(allyBlip)), (int)midpoint.x + (int)diff.x - 3, (int)midpoint.y + (int)diff.y - 3);
						else
							DrawSprite((Sprite*)(thegame->getRenderable(enemyBlip)), (int)midpoint.x + (int)diff.x - 3, (int)midpoint.y + (int)diff.y - 3);
					}
				}
			}	//end for (radar)
			DrawSprite((Sprite*)(thegame->getRenderable(radarOutline)), thegame->gWindow.Width - 192, thegame->gWindow.Height - 192);
		}	//end if pc != NULL
		EndSpriteRender();

		if(thegame->pc != NULL)
		{
			drawText(thegame->pc->ship->loc.getCoords().c_str(), 5, thegame->gWindow.Height-50, 0, 255, 0);
			//drawText(thegame->pc->ship->vel.getMagString().c_str(), 5, thegame->gWindow.Height-35, 0, 255, 0);
			drawText("Shields:", thegame->gWindow.Width/2 - 185, thegame->gWindow.Height - 81, 0, 255, 0);
			drawText("Speed:", thegame->gWindow.Width/2 - 185, thegame->gWindow.Height - 43, 0, 255, 255);
		}
		thegame->gametype->pauseMenu.Render();
		EndRender();

		break;
		case choosingShip:
			StartRender();
			thegame->chooseShipMenu.Render();
			EndRender();
			break;
		case starting:
			StartRender();
			thegame->menu.Render();
			EndRender();
			break;
		case settings:
			StartRender();
			thegame->gametype->settingsMenu.Render();
			EndRender();
			break;
	}
}

void LoadMesh(const char* filename, renderableType mT, int scale)
{
	Mesh* temp = new Mesh(filename, mT, scale);
	getGame()->renderables.insert(getGame()->renderables.end(), temp);
}

void LoadTexture(LPCWSTR filename, renderableType mT, int r, int g, int b)
{
	Texture* t = new Texture;
	LoadTextureFromFile(filename, t->t_vertex, t->texture, r, g, b);
	t->radius = 0.0f;
	t->type = mT;
	getGame()->renderables.insert(getGame()->renderables.end(), t);
}

void LoadSprite(LPCWSTR filename, renderableType mt)
{
	Sprite* s = new Sprite;
	LoadSpriteFromFile(filename, s);
	getGame()->renderables.insert(getGame()->renderables.end(), s);
	s->type = mt;
}

void UnloadGraphics(Game* thegame)
{
	for(vector<Renderable*>::iterator index = thegame->renderables.begin(); index != thegame->renderables.end(); ++index)
	{
		delete (*index);
	}
	thegame->renderables.clear();
}