#ifndef CUBE_H
#define CUBE_H

#include "Shader.h"

class Cube
{
public:
	Cube() {};
	Cube(ID3D11Device* Device,
	     float r,float g,float b,float a
	);
	~Cube();

	void Draw(ID3D11Device* Device, ID3D11DeviceContext* Context);

private:
	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* MatrixBuffer;
};

#endif // !CUBE_H
