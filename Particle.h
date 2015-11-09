#ifndef PARTICLE_H
#define PARTICLE_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3dx9.h>
#include <d3d9.h>
class Vec2D;

class Particle
{
public:
	Particle();
	Particle(float radius, int lifetime);
    void run_particle();
    void reset_particle(Vec2D* loc, Vec2D* vel, Vec2D* acc, int life);

    bool active;

    D3DXVECTOR3 position;
    D3DXVECTOR3 velocity;
    D3DXVECTOR3 acceleration;
    float radius;
    int lifespan;
    int age;
    D3DXMATRIX matRotateX;
    D3DXMATRIX matRotateY;
};

#endif