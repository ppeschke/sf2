#include <cmath>
#include "Global.h"
#include "Color.h"
#include "Particle.h"
#include "Sprite.h"
#include "Polygon.h"
const double PI = 4.0*atan(1.0);
#include <fstream>
using namespace std;

ofstream d3dlog;

LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddev = NULL;
D3DPRESENT_PARAMETERS d3dpp;
LPD3DXSPRITE ddraw;
LPD3DXFONT dxfont;		//10px wide
LPD3DXFONT dxbigfont;	//16px wide

int SCREEN_HEIGHT;
int SCREEN_WIDTH;

// include the Direct3D Library files so you don't have to put them in the project settings
//(the paths to these do have to be included in the lib directories for the compiler though)
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

#include "Polygon.h"
#define TEXTUREFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)

struct TEXTUREVERTEX
{
	FLOAT X, Y, Z;
	DWORD COLOR;
	FLOAT U, V;
};

void InitDirect3D(Game* thegame)
{
	d3dlog.open("d3d.csv");
	d3dlog << "End Process,Present,Total" << endl;

    d3d = Direct3DCreate9(D3D_SDK_VERSION);

    ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = thegame->gWindow.Windowed;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = thegame->gWindow.hWnd;
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferWidth = thegame->gWindow.Width;
	d3dpp.BackBufferHeight = thegame->gWindow.Height;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		thegame->gWindow.hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);
	D3DXCreateFont(d3ddev, 20, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
               PROOF_QUALITY, DEFAULT_PITCH || FF_DONTCARE, L"Courier New", &dxfont);

	D3DXCreateFont(d3ddev, 30, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
               PROOF_QUALITY, DEFAULT_PITCH || FF_DONTCARE, L"Courier New", &dxbigfont);

    d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);
	d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);

	SCREEN_HEIGHT = thegame->gWindow.Height;
	SCREEN_WIDTH = thegame->gWindow.Width;
	SetCamera();
	AdjustCamera(0.0f, 0.0f, thegame);
	d3ddev->SetFVF(CUSTOMFVF);

	D3DXCreateSprite(d3ddev, &ddraw);

	d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
	d3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	d3ddev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD); // set the operation
    return;
}

void CloseDirect3D()
{
	d3dlog.close();
	d3ddev->Release();
    d3d->Release();
    return;
}

// Start rendering
void StartRender()
{
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	d3ddev->BeginScene();

	return;
}

// Stop rendering
void EndRender()
{
	/*DWORD TotalTimer, EndTimer, PresentTimer;
	TotalTimer = GetTickCount();
	EndTimer = GetTickCount();*/
    d3ddev->EndScene();
	/*EndTimer = GetTickCount() - EndTimer;
	PresentTimer = GetTickCount();*/
    d3ddev->Present(NULL, NULL, NULL, NULL);
	/*PresentTimer = GetTickCount() - PresentTimer;
	TotalTimer = GetTickCount() - TotalTimer;
	d3dlog << EndTimer << "," << PresentTimer << "," << TotalTimer << endl;*/

    return;
}

void StartSpriteRender()
{
	//ddraw->Begin(NULL);
	ddraw->Begin(D3DXSPRITE_ALPHABLEND);		// begin sprite drawing with transparency
}

void EndSpriteRender()
{
	ddraw->End();
}

// Set the fixed properties of the 3D camera
void SetCamera()
{
	D3DXMATRIX matProjection;
	float aspect = (float)getGame()->gWindow.Width/getGame()->gWindow.Height;
	float fovy = 2.0f * atan(tan((float)(D3DXToRadian(getGame()->gWindow.fov)) * 0.5f) / (aspect));	//vertical field of view
	D3DXMatrixPerspectiveFovLH(&matProjection,
								fovy,    // the vertical field of view
								aspect, // aspect ratio	//must be in float... duh
								0.0f,    // the near view-plane
								100.0f);    // the far view-plane
							   
	/*D3DXMatrixOrthoLH(&matProjection,
						1024.0f,
						768.0f,
						-1.0f,
						1.0f);*/

    d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection);
    return;
}

void DrawSprite(Sprite* s, int x, int y)
{
	ddraw->Draw(s->sprite, NULL, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &D3DXVECTOR3((float)x, (float)y, 0.0f), D3DCOLOR_XRGB(255, 255, 255)); 
	//s->sprite	is the sprite to draw
	//NULL tells it to draw the whole stinkin' thing (would normally be a pRECT
	//D3DXVECTOR3 is the center point (just setting it to (0, 0)
	//the second D3DVECTOR3 is the point to start drawing it on the screen
	//the color is to draw at full intensity
	/*HRESULT Draw(LPDIRECT3DTEXTURE9 pTexture,
             CONST RECT* pSrcRect,
             CONST D3DXVECTOR3* pCenter,
             CONST D3DXVECTOR3* pPosition,
             D3DCOLOR Color);*/
}

void DrawSpriteTransparent(Sprite* s, int x, int y, int a)
{
	ddraw->Draw(s->sprite, NULL, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &D3DXVECTOR3((float)x, (float)y, 0.0f), D3DCOLOR_ARGB(a, 255, 255, 255)); 
	//s->sprite	is the sprite to draw
	//NULL tells it to draw the whole stinkin' thing (would normally be a pRECT
	//D3DXVECTOR3 is the center point (just setting it to (0, 0)
	//the second D3DVECTOR3 is the point to start drawing it on the screen
	//the color is to draw at full intensity
	/*HRESULT Draw(LPDIRECT3DTEXTURE9 pTexture,
             CONST RECT* pSrcRect,
             CONST D3DXVECTOR3* pCenter,
             CONST D3DXVECTOR3* pPosition,
             D3DCOLOR Color);*/
}

// Set the adjustable properties of the 3D camera
void AdjustCamera(float x, float y, Game* thegame)
{
	thegame->camera.position.x = thegame->camera.lookat.x = x;
	thegame->camera.position.y = thegame->camera.lookat.y = y;
	if(thegame->camera.position.z != thegame->camera.targetZ)
		thegame->camera.position.z += (thegame->camera.targetZ > thegame->camera.position.z? 1.0f:-1.0f);
	//set the view
	D3DXMATRIX matView;    // the view transform matrix
	D3DXMatrixLookAtLH(&matView,
		&thegame->camera.position,    // the camera position
		&thegame->camera.lookat,    // the look-at position
		&thegame->camera.up);    // the up direction
	d3ddev->SetTransform(D3DTS_VIEW, &matView);    // set the view transform to matView
    return;
}

void drawText(const char* text, int x, int y, int r, int g, int b)
{
	static RECT textbox;
	SetRect(&textbox, x, y, SCREEN_WIDTH, SCREEN_HEIGHT); 
	dxfont->DrawTextA(NULL, text, strlen(text), &textbox, DT_LEFT | DT_TOP, D3DCOLOR_ARGB(255, r, g, b));
}

void DrawMesh(Mesh* mesh, Vec2D* loc, Vec2D* dir)
{
	d3ddev->SetFVF(CUSTOMFVF);
	d3ddev->SetTexture(0, NULL);
	//set the translation and rotation
	D3DXMATRIX matTranslate;
	D3DXMATRIX matRotateZ;
	D3DXMatrixTranslation(&matTranslate, loc->x, loc->y, 0.0f);
	D3DXMatrixRotationZ(&matRotateZ, dir->getDirection()-(float)PI/2.0f);
	d3ddev->SetTransform(D3DTS_WORLD, &(matRotateZ * matTranslate));

	for(unsigned int i = 0; i < mesh->polyNum; ++i)
	{
		// select the vertex buffer to display
		d3ddev->SetStreamSource(0, mesh->polys[i].v_buffer, 0, sizeof(sf2::CUSTOMVERTEX));

		//HRESULT DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount);
		d3ddev->DrawPrimitive(D3DPT_LINESTRIP, 0, mesh->polys[i].length - 1);
	}
	//Draw2DLine(100.0f, 0.0f, loc->x, loc->y, D3DCOLOR_XRGB(0, 255, 0));
    return;
}

Mesh* CreateArena(float radius, Color color)
{
	Mesh* m = new Mesh();
	const int NUMPOINTS = 24;
    const float PI = 3.14159f;

	sf2::CUSTOMVERTEX Circle[NUMPOINTS + 1];
	float X;
	float Y;
	float Theta;
	float WedgeAngle;	//Size of angle between two points on the circle (single wedge)

	m->polys = new sf2::Polygon[2];
	m->polyNum = 2;

	//Precompute WedgeAngle
	WedgeAngle = (float)((2*PI) / NUMPOINTS);

	//Set up vertices for a circle
	//Used <= in the for statement to ensure last point meets first point (closed circle)
	for(int t = 0; t < 2; ++t)
	{
		for(int i=0; i <= NUMPOINTS; i++)
		{
			//Calculate theta for this vertex
			Theta = i * WedgeAngle;
		
			//Compute X and Y locations
			X = (float)(radius * cos(Theta));
			Y = (float)(radius * sin(Theta));

			Circle[i].X = X;
			Circle[i].Y = Y;
			Circle[i].Z = 0.0f;
			Circle[i].COLOR = color.returnD3DCOLOR();
		}
		m->polys[t].c = color;
		m->polys[t].length = NUMPOINTS + 1;
		m->polys[t].vertices = new sf2::CUSTOMVERTEX[NUMPOINTS + 1];
		//copy from circle to vertices
		for(int i = 0; i <= NUMPOINTS; ++i)
			m->polys[t].vertices[i] = Circle[i];
		radius -= 5.0f;
	}

	LoadToVRAM(m);
	return m;
}

void Draw2DCircle(float x, float y, float radius, D3DCOLOR color)
{
	LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL;
	VOID* pVoid;    // a void pointer
	const int NUMPOINTS = 24;
    const float PI = 3.14159f;

	sf2::CUSTOMVERTEX Circle[NUMPOINTS + 1];
	int i;
	float X;
	float Y;
	float Theta;
	float WedgeAngle;	//Size of angle between two points on the circle (single wedge)

	//Precompute WedgeAngle
	WedgeAngle = (float)((2*PI) / NUMPOINTS);

	//Set up vertices for a circle
	//Used <= in the for statement to ensure last point meets first point (closed circle)
	for(i=0; i <= NUMPOINTS; i++)
	{
		//Calculate theta for this vertex
		Theta = i * WedgeAngle;
		
		//Compute X and Y locations
		X = (float)(radius * cos(Theta));
		Y = (float)(radius * sin(Theta));

		Circle[i].X = X;
		Circle[i].Y = Y;
		Circle[i].Z = 0.0f;
		Circle[i].COLOR = color;
	}

	d3ddev->CreateVertexBuffer((NUMPOINTS + 1) * sizeof(sf2::CUSTOMVERTEX), 0, CUSTOMFVF, D3DPOOL_MANAGED, &v_buffer, NULL);
	v_buffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, Circle, sizeof(sf2::CUSTOMVERTEX) * (NUMPOINTS + 1));
	v_buffer->Unlock();

	D3DXMATRIX matTranslate;
	D3DXMatrixTranslation(&matTranslate, x, y, 0.0f);
	D3DXMATRIX matRotateZ;
	D3DXMatrixRotationZ(&matRotateZ, 0.0f);
	d3ddev->SetTransform(D3DTS_WORLD, &(matRotateZ * matTranslate));


	//Now draw the circle
    d3ddev->SetFVF(CUSTOMFVF);
	d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(sf2::CUSTOMVERTEX));
	d3ddev->DrawPrimitive(D3DPT_LINESTRIP, 0, NUMPOINTS);

}//Draw2DCircle

void Draw2DLine(float x1, float y1, float x2, float y2, D3DCOLOR color)
{
	d3ddev->SetTexture(0, NULL);
	LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL;
	VOID* pVoid;    // a void pointer
	sf2::CUSTOMVERTEX Line[2];
	Line[0].X = x1;
	Line[0].Y = y1;
	Line[0].Z = 0.0f;
	Line[0].COLOR = color;
	Line[1].X = x2;
	Line[1].Y = y2;
	Line[1].Z = 0.0f;
	Line[1].COLOR = color;

	d3ddev->CreateVertexBuffer(2 * sizeof(sf2::CUSTOMVERTEX), 0, CUSTOMFVF, D3DPOOL_MANAGED, &v_buffer, NULL);
	v_buffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, Line, sizeof(sf2::CUSTOMVERTEX) * 2);
	v_buffer->Unlock();
	
	D3DXMATRIX matTranslate;
	D3DXMatrixTranslation(&matTranslate, 0.0f, 0.0f, 0.0f);
	D3DXMATRIX matRotateZ;
	D3DXMatrixRotationZ(&matRotateZ, 0.0f);
	d3ddev->SetTransform(D3DTS_WORLD, &(matRotateZ * matTranslate));

	//Now draw the line
	d3ddev->SetFVF(CUSTOMFVF);
	d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(sf2::CUSTOMVERTEX));
	d3ddev->DrawPrimitive(D3DPT_LINESTRIP, 0, 1);
}

void LoadToVRAM(Mesh* mesh)
{
	for(unsigned int i = 0; i < mesh->polyNum; ++i)
	{
		//size of the vertex buffer in video memory is important here
		//HRESULT CreateVertexBuffer( UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, LPDIRECT3DVERTEXBUFFER9 ppVertexBuffer, HANDLE* pSharedHandle);
		d3ddev->CreateVertexBuffer(mesh->polys[i].length * sizeof(sf2::CUSTOMVERTEX), 0, CUSTOMFVF, D3DPOOL_MANAGED, &(mesh->polys[i].v_buffer), NULL);

		VOID* pVoid;    // a void pointer

		// lock v_buffer and load the vertices into it
		mesh->polys[i].v_buffer->Lock(0, 0, (void**)&pVoid, 0);
		//sizeof DOESN'T WORK THE SAME WITH DYNAMICALLY ALLOCATED ARRAYS (it returns 4 because all pointers are 4 bytes.)
		//dereference the pointer or send the type to the sizeof function, but don't send just the pointer alone, you'll get 4
		memcpy(pVoid, mesh->polys[i].vertices, sizeof(*(mesh->polys[i].vertices)) * mesh->polys[i].length);
		mesh->polys[i].v_buffer->Unlock();
		mesh->polys[i].VRAMloaded = true;
	}
}

void DrawRect(float x, float y, float length, float height, D3DCOLOR color)
{
	d3ddev->SetTexture(0, NULL);
	LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL;
	VOID* pVoid;    // a void pointer
	const int NUMPOINTS = 4;

	sf2::CUSTOMVERTEX Rectangle[NUMPOINTS + 1];

	Rectangle[0].X = x;
	Rectangle[0].Y = y;
	Rectangle[0].Z = 0.0f;
	Rectangle[0].COLOR = color;

	Rectangle[1].X = x + length;
	Rectangle[1].Y = y;
	Rectangle[1].Z = 0.0f;
	Rectangle[1].COLOR = color;

	Rectangle[2].X = x + length;
	Rectangle[2].Y = y - height;
	Rectangle[2].Z = 0.0f;
	Rectangle[2].COLOR = color;

	Rectangle[3].X = x;
	Rectangle[3].Y = y - height;
	Rectangle[3].Z = 0.0f;
	Rectangle[3].COLOR = color;

	Rectangle[4].X = Rectangle[0].X;
	Rectangle[4].Y = Rectangle[0].Y;
	Rectangle[4].Z = Rectangle[0].Z;
	Rectangle[4].COLOR = Rectangle[0].COLOR;

	d3ddev->CreateVertexBuffer((NUMPOINTS + 1) * sizeof(sf2::CUSTOMVERTEX), 0, CUSTOMFVF, D3DPOOL_MANAGED, &v_buffer, NULL);
	v_buffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, Rectangle, sizeof(sf2::CUSTOMVERTEX) * (NUMPOINTS + 1));
	v_buffer->Unlock();

	D3DXMATRIX matTranslate;
	D3DXMatrixTranslation(&matTranslate, 0.0f, 0.0f, 0.0f);
	D3DXMATRIX matRotateZ;
	D3DXMatrixRotationZ(&matRotateZ, 0.0f);
	d3ddev->SetTransform(D3DTS_WORLD, &(matRotateZ * matTranslate));


	//Now draw the circle
    d3ddev->SetFVF(CUSTOMFVF);
	d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(sf2::CUSTOMVERTEX));
	d3ddev->DrawPrimitive(D3DPT_LINESTRIP, 0, NUMPOINTS);	//apparently it connects to itself at the end?

}//DrawRect

void LoadTextureFromFile(LPCWSTR filename, LPDIRECT3DVERTEXBUFFER9& t_vertex, LPDIRECT3DTEXTURE9& texture, int r, int g, int b)
{
	D3DXCreateTextureFromFile(d3ddev,
                              //LPCWSTR(filename),
							  filename,
                              &texture);

    // create the vertices using the CUSTOMVERTEX
    struct TEXTUREVERTEX t_vert[] =
    {
        {-1.0f, 1.0f, 0.0f, D3DCOLOR_XRGB(r, g, b), 1, 0,},
        {-1.0f, -1.0f, 0.0f, D3DCOLOR_XRGB(r, g, b), 0, 0,},
        {1.0f, 1.0f, 0.0f, D3DCOLOR_XRGB(r, g, b), 1, 1,},
        {1.0f, -1.0f, 0.0f, D3DCOLOR_XRGB(r, g, b), 0, 1,},
    };

    // create a vertex buffer interface called t_vertex
    d3ddev->CreateVertexBuffer(4*sizeof(TEXTUREVERTEX),
                               0,
                               CUSTOMFVF,
                               D3DPOOL_MANAGED,
                               &t_vertex,
                               NULL);

    VOID* pVoid;    // a void pointer

    // lock t_buffer and load the vertices into it
    t_vertex->Lock(0, 0, (void**)&pVoid, 0);
    memcpy(pVoid, t_vert, sizeof(t_vert));
    t_vertex->Unlock();
}

void LoadSpriteFromFile(LPCWSTR filename, Sprite* s)
{
	D3DXCreateTextureFromFile(d3ddev, filename, &s->sprite);
}

void DrawBlendedParticles(Particle* p, int numParticles, Texture* t)
{
	static float camx;
	static float camy;
	static float camz;

	camz = getGame()->camera.position.z;

	
	//turn on alpha blending and turn off z-writing
	d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	d3ddev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	d3ddev->SetFVF(TEXTUREFVF);

	for(int i = 0; i < numParticles; ++i)
	{
		if(p[i].active)
		{
			camx = p[i].position.x;
			camy = p[i].position.y;

			// Before setting the world transform, do the intense mathematics!
			// a. Calculate the Differences
			static float difx, dify, difz;
			difx = camx - p[i].position.x;
			dify = camy - p[i].position.y;
			difz = camz - p[i].position.z;

			// b. Calculate the Distances
			static float FlatDist, TotalDist;
			FlatDist = sqrt(difx * difx + difz * difz);
			TotalDist = sqrt(FlatDist * FlatDist + dify * dify);

			// c. Y Rotation
			D3DXMatrixIdentity(&p[i].matRotateY);
			p[i].matRotateY._11 = p[i].matRotateY._33 = difz / FlatDist;    // cosY
			p[i].matRotateY._31 = difx / FlatDist;    // sinY
			p[i].matRotateY._13 = -p[i].matRotateY._31;    // -sinY

			// d. X Rotation
			D3DXMatrixIdentity(&p[i].matRotateX);
			p[i].matRotateX._22 = p[i].matRotateX._33 = FlatDist / TotalDist;    // cosX
			p[i].matRotateX._32 = dify / TotalDist;    // sinX
			p[i].matRotateX._23 = -p[i].matRotateX._32;    // -sinX

			// e. Tranlation
			static D3DXMATRIX matTranslate;
			D3DXMatrixTranslation(&matTranslate, p[i].position.x, p[i].position.y, p[i].position.z);

			// f. Scaling
			static D3DXMATRIX matScale;
			D3DXMatrixIdentity(&matScale);
			matScale._11 = matScale._22 = matScale._33 = p[i].radius;


			// Now build the world matrix and set it
			d3ddev->SetTransform(D3DTS_WORLD, &(matScale * p[i].matRotateX * p[i].matRotateY * matTranslate));

			d3ddev->SetStreamSource(0, t->t_vertex, 0, sizeof(TEXTUREVERTEX));
			d3ddev->SetTexture(0, t->texture);
			d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
			d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		}
	}

	d3ddev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

Vec2D GetScreenCoords(Vec2D vec)
{
	D3DXVECTOR3 Src(vec.x, vec.y, 0.0f);

	//camera stuff
	D3DXMATRIX View;
	Camera* cam = &getGame()->camera;
	D3DXMatrixLookAtLH( &View, &cam->position, &cam->lookat, &cam->up);

	//build the projection stuff
	D3DXMATRIX Projection;
	D3DXMatrixIdentity(&Projection);
	float aspect = (float)getGame()->gWindow.Width/getGame()->gWindow.Height;
	float fovy = 2.0f * atan(tan(D3DXToRadian(getGame()->gWindow.fov) * 0.5f) / (aspect));	//vertical field of view
	//fovy = 0.4f*3.14f;
	D3DXMatrixPerspectiveFovLH(&Projection, fovy, aspect, 1.0f, 100.0f);

	//and the world stuff
	D3DXMATRIX World;
	D3DXMatrixIdentity(&World);
	//D3DXMatrixTranslation(&World, vec.x, vec.y, 0.0f);
	//d3ddev->GetTransform(D3DTS_WORLD, &World);

	//with our powers combined, we form WVPtron! (the combined matrices are what it goes through to be rendered to the screen
	D3DXMATRIX WVP;
	WVP = World * View * Projection;

	//run it through the WVP transformations
	D3DXVECTOR3 screenCoord = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//will be the position of the coords in screen space
	/*D3DXVec3TransformCoord(&screenCoord, &Src, &WVP);
	//it comes out as Screen Coords ([-1, 1], [-1, 1])
	screenCoord.x = ((screenCoord.x + 1.0f) / 2.0f) * getGame()->gWindow.Width;		//convert from percentage of screen to pixels
	screenCoord.y = ((-screenCoord.y + 1.0f) / 2.0f) * getGame()->gWindow.Height;*/

	D3DVIEWPORT9 vP;
	vP.Width = getGame()->gWindow.Width;
	vP.Height = getGame()->gWindow.Height;
	vP.X = vP.Y = 0;
	vP.MaxZ = 100.0f;
	vP.MinZ = 1.0f;
	D3DXVec3Project(&screenCoord, &Src, &vP, &Projection, &View, &World);

	return Vec2D(screenCoord.x, screenCoord.y);
}

void drawBigText(const char* text, int x, int y, int r, int g, int b)
{
	static RECT textbox;
	SetRect(&textbox, x, y, SCREEN_WIDTH, SCREEN_HEIGHT); 
	dxbigfont->DrawTextA(NULL, text, strlen(text), &textbox, DT_LEFT | DT_TOP, D3DCOLOR_ARGB(255, r, g, b));
}

//this is a heavy function and is only intended to be called once at startup
Vec2D get3DScreenEdges(void)
{
	//TODO: fix this so it calls a function to get a picking ray (should remain static as the view doesn't change,
	//stores this picking ray and uses it when the camera's z-position changes
	//to calculate the x and y intercept with z=0. 
	//I now possess the knowledge to do this more accurately without a loop.
	Vec2D edge(0.0f, 0.0f);
	D3DXVECTOR3 screenCoord = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//will be the position of the coords in screen space
	while(screenCoord.x < getGame()->gWindow.Width)	//while screen coord x is in window
	{
		edge.x += 10.0f;
		D3DXVECTOR3 Src(edge.x, edge.y, 0.0f);
		//camera stuff
		D3DXMATRIX View;
		Camera* cam = &getGame()->camera;
		D3DXMatrixLookAtLH( &View, &D3DXVECTOR3(0.0f, 0.0f, -500.0f), &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &cam->up);

		//build the projection stuff
		D3DXMATRIX Projection;
		D3DXMatrixIdentity(&Projection);
		float aspect = (float)getGame()->gWindow.Width/getGame()->gWindow.Height;
		float fovy = 2.0f * atan(tan(D3DXToRadian(getGame()->gWindow.fov) * 0.5f) / (aspect));	//vertical field of view
		//fovy = 0.4f*3.14f;
		D3DXMatrixPerspectiveFovLH(&Projection, fovy, aspect, 1.0f, 100.0f);

		//and the world stuff
		D3DXMATRIX World;
		D3DXMatrixIdentity(&World);
		//D3DXMatrixTranslation(&World, vec.x, vec.y, 0.0f);
		//d3ddev->GetTransform(D3DTS_WORLD, &World);

		//with our powers combined, we form WVPtron! (the combined matrices are what it goes through to be rendered to the screen
		D3DXMATRIX WVP;
		WVP = World * View * Projection;

		//run it through the WVP transformations
		/*D3DXVec3TransformCoord(&screenCoord, &Src, &WVP);
		//it comes out as Screen Coords ([-1, 1], [-1, 1])
		screenCoord.x = ((screenCoord.x + 1.0f) / 2.0f) * getGame()->gWindow.Width;		//convert from percentage of screen to pixels
		screenCoord.y = ((-screenCoord.y + 1.0f) / 2.0f) * getGame()->gWindow.Height;*/

		D3DVIEWPORT9 vP;
		vP.Width = getGame()->gWindow.Width;
		vP.Height = getGame()->gWindow.Height;
		vP.X = vP.Y = 0;
		vP.MaxZ = 100.0f;
		vP.MinZ = 1.0f;
		D3DXVec3Project(&screenCoord, &Src, &vP, &Projection, &View, &World);
	}

	while(screenCoord.y < getGame()->gWindow.Height)	//while screen coord x is in window
	{
		edge.y -= 10.0f;
		D3DXVECTOR3 Src(edge.x, edge.y, 0.0f);
		//camera stuff
		D3DXMATRIX View;
		Camera* cam = &getGame()->camera;
		D3DXMatrixLookAtLH( &View, &D3DXVECTOR3(0.0f, 0.0f, cam->position.z), &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &cam->up);

		//build the projection stuff
		D3DXMATRIX Projection;
		D3DXMatrixIdentity(&Projection);
		float aspect = (float)getGame()->gWindow.Width/getGame()->gWindow.Height;
		float fovy = 2.0f * atan(tan(D3DXToRadian(getGame()->gWindow.fov) * 0.5f) / (aspect));	//vertical field of view
		//fovy = 0.4f*3.14f;
		D3DXMatrixPerspectiveFovLH(&Projection, fovy, aspect, 1.0f, 100.0f);

		//and the world stuff
		D3DXMATRIX World;
		D3DXMatrixIdentity(&World);
		//D3DXMatrixTranslation(&World, vec.x, vec.y, 0.0f);
		//d3ddev->GetTransform(D3DTS_WORLD, &World);

		//with our powers combined, we form WVPtron! (the combined matrices are what it goes through to be rendered to the screen
		D3DXMATRIX WVP;
		WVP = World * View * Projection;

		//run it through the WVP transformations
		/*D3DXVec3TransformCoord(&screenCoord, &Src, &WVP);
		//it comes out as Screen Coords ([-1, 1], [-1, 1])
		screenCoord.x = ((screenCoord.x + 1.0f) / 2.0f) * getGame()->gWindow.Width;		//convert from percentage of screen to pixels
		screenCoord.y = ((-screenCoord.y + 1.0f) / 2.0f) * getGame()->gWindow.Height;*/

		D3DVIEWPORT9 vP;
		vP.Width = getGame()->gWindow.Width;
		vP.Height = getGame()->gWindow.Height;
		vP.X = vP.Y = 0;
		vP.MaxZ = 100.0f;
		vP.MinZ = 1.0f;
		D3DXVec3Project(&screenCoord, &Src, &vP, &Projection, &View, &World);
	}

	return edge;
}