#include "Triangle.h"

extern ID3D11Device* Device;
extern ID3D11DeviceContext* Context;

void InitScene()
{
	
}

void Draw_Scene()
{

	Triangle t1(Device, 
		-0.4f, 0.0f, 0.0f,
		0.5f,0.5f,0.5f,
		1.0f,0.0f,0.0f);

	t1.Draw(Device,Context);

	Triangle t2(Device,
		0.7f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f,
		0.0f, 0.0f, 1.0f);

	t2.Draw(Device, Context);
}

void CleanScene()
{
	
}