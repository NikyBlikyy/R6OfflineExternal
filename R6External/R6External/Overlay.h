#pragma once
#include "Includes.h"
#include "SDK.h"
#include "Decrypt.h"
#include "Math.h"
#include "imgui.h"

typedef float D3DVALUE, * LPD3DVALUE;

//every time you call the struct below you need to call it struct _D3DTLVERTEX

typedef struct _D3DTLVERTEX
{
	union
	{
		D3DVALUE    sx;                 //Screen Coords
		D3DVALUE    dvSX;
	};
	
	union
	{
		D3DVALUE    sy;
		D3DVALUE    dvSY;
	};
	
	union
	{
		D3DVALUE    rhw;               //Reciprocal of homogeneous width
		D3DVALUE    dvRHW;
	};
	
	union
	{
		D3DCOLOR    color;            //Vertex Color
		D3DCOLOR    dcColor;
	};
	
	union
	{
		D3DCOLOR    specular;         //Specular component of vertex
		D3DCOLOR    dcSpecular;
	};
	
	union
	{
		D3DVALUE    tu;              //Texture Coordinates
		D3DVALUE    dvTU;
	};
	
	union
	{
		D3DVALUE    tv;
		D3DVALUE    dvTV;
	};

 #if (DIRECT3D_VERSION >= 0x500)
 #if (defined __cplusplus) && (defined D3D_OVERLOADS)
	_D3DTLVERTEX() {}
	_D3DTLVERTEX(const D3DVECTOR& v, float _rhw, D3DCOLOR _color, D3DCOLOR _specular, float _tu, float _tv);
	{
		sx         =     v.x;
		sy         =     v.y;
		sz         =     v.z;
		rhw        =     _rhw;
		color      =     _color;
		specular   =     _specular;
		tu         =     _tu; 
		tv         =     _tv;
	}

#endif
#endif
}D3DTLVERTEX, * LPD3DTLVERTEX;

extern HWND hWnd;
extern IDirect3D9* pD3D;
extern IDirect3DDevice9* pDevice;
extern ID3DXFont* pFont1;
extern ID3DXLine* pLine;


const int Width = GetSystemMetrics(SM_CXSCREEN);
const int Height = GetSystemMetrics(SM_CYSCREEN);
static MARGINS margins = { 0, 0, Width, Height };

//Window Stuff
void Render();

WPARAM OverlayThread();

void InitD3D(HWND hWnd);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void SetupWindow();


namespace Colors
{
	extern D3DCOLOR Red;
	extern D3DCOLOR Orange;
	extern D3DCOLOR Yellow;
	extern D3DCOLOR Green;
	extern D3DCOLOR Turquoise;
	extern D3DCOLOR Blue;
	extern D3DCOLOR Purple;
	extern D3DCOLOR Pink;
	extern D3DCOLOR Black;
	extern D3DCOLOR Gray;
	extern D3DCOLOR White;
}

void DrawLine(int x0, int y0, int x1, int y1, D3DCOLOR color);

void DrawRect(int x, int y, int w, int h, D3DCOLOR color);

void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color);

void DrawHealthbar(int x, int y, int w, int h, int val, int max, D3DCOLOR color);

void DrawString(const ImVec2& aPos, const std::string& aString, ImU32 aColor);

void DrawOutlinedRect(int x, int y, int w, int h, D3DCOLOR color);

void DrawCircle(int x, int y, int r, int numSides, D3DCOLOR color);

void RotatePoint(Vector2 pointToRotate, Vector2 centerPoint, float angle, Vector2& returnVec, bool angleIsInRadians);