#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Shader.h"

struct TriangleVertex {
	float x, y, z;
	float r, g, b, a;
};

D3D11_INPUT_ELEMENT_DESC triied[] = {
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

class Triangle
{
public:
	Triangle() {};
	Triangle(ID3D11Device* Device ,
		float x, float y, float z,
		float sx, float sy, float sz,
		float r, float g, float b);

	~Triangle();

	void Draw(ID3D11Device* Device, ID3D11DeviceContext* Context);

private:
	ID3D11Buffer* VertexBuffer;
};

Triangle::Triangle(ID3D11Device* Device,
	float x, float y, float z,
	float sx, float sy, float sz,
	float r, float g, float b)
{
	TriangleVertex vertices[] = {
	{ sx*(x + 0.0f),  sy*(y + 0.5f), sz*(z + 0.0f), r, g, b, 1.0f },
	{ sx*(x + 0.5f), sy*(y + -0.5f), sz*(z + 0.0f), r, g, b, 1.0f },
	{ sx*(x + -0.5f), sy*(y + -0.5f), sz*(z + 0.0f), r, g, b, 1.0f }
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(vertices);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA srd = { vertices, 0, 0 };
	Device->CreateBuffer(&bd, &srd, &VertexBuffer);

}

Triangle::~Triangle()
{
	VertexBuffer->Release();
}

void Triangle::Draw(ID3D11Device* Device, ID3D11DeviceContext* Context)
{
	VertexShader vshader(Device, L"hlsl/Triangle.VertexShader", "Main");
	PixelShader pshader(Device, L"hlsl/Triangle.PixelShader", "Main");

	UINT stride = sizeof(TriangleVertex);
	UINT offset = 0;
	Context->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);

	vshader.SetInputLayout(Device, Context, triied, 2);

	Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	vshader.Set(Context);
	pshader.Set(Context);

	Context->Draw(3, 0);
}


#endif // !TRIANGLE_H
