#include <d3d9.h>
#include <d3dx9.h>

struct Camera
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 lookat;
	D3DXVECTOR3 up;
	float targetZ;

	Camera()
	{
		targetZ = -250.0f;
		position = D3DXVECTOR3 (0.0f, 0.0f, -450.0f);
		lookat = D3DXVECTOR3 (0.0f, 0.0f, 0.0f);
		up = D3DXVECTOR3 (0.0f, 1.0f, 0.0f);
	}
};