
#include "RectangleTexture.h"
#include "Cube.h"

extern ID3D11Device* Device;
extern ID3D11DeviceContext* Context;

void InitScene()
{
	
}

void Draw_Scene()
{
	Cube cube1(Device, 1.0f,0.0f,0.0f,1.0f);
	cube1.Draw(Device,Context);
}

void CleanScene()
{
	
}