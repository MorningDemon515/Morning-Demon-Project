#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Shader.h"

struct RectangleVertex {
	float x, y, z;
	float r, g, b, a;
};

D3D11_INPUT_ELEMENT_DESC rectied[] = {
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

class Rectangle_Class
{
public:
	Rectangle_Class() {};
	Rectangle_Class(ID3D11Device* Device,
		float x, float y, float z,
		float sx, float sy, float sz,
		float r, float g, float b);

	~Rectangle_Class();

	void Draw(ID3D11Device* Device, ID3D11DeviceContext* Context);

private:
	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;
};

Rectangle_Class::Rectangle_Class(ID3D11Device* Device,
	float x, float y, float z,
	float sx, float sy, float sz,
	float r, float g, float b)
{
	RectangleVertex vertices[] = {
	{ sx * (x + -0.5f),  sy * (y + -0.5f), sz * (z + 0.0f), r, g, b, 1.0f },
	{ sx * (x + -0.5f), sy * (y + 0.5f), sz * (z + 0.0f), r, g, b, 1.0f},
	{ sx * (x + 0.5f), sy * (y + 0.5f), sz * (z + 0.0f), r, g, b, 1.0f},
	{ sx * (x + 0.5f), sy * (y + -0.5f), sz * (z + 0.0f), r, g, b, 1.0f}
	};

	DWORD indices[] = {
	0, 1, 2,
	0, 2, 3,
	};

	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd,sizeof(D3D11_BUFFER_DESC));
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(indices);
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA isrd = { indices, 0, 0 };
	Device->CreateBuffer(&ibd, &isrd, &IndexBuffer);

	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(D3D11_BUFFER_DESC));
	vbd.ByteWidth = sizeof(vertices);
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vsrd = { vertices, 0, 0 };
	Device->CreateBuffer(&vbd, &vsrd, &VertexBuffer);

}

Rectangle_Class::~Rectangle_Class()
{
	VertexBuffer->Release();
	IndexBuffer->Release();
}

void Rectangle_Class::Draw(ID3D11Device* Device, ID3D11DeviceContext* Context)
{
	VertexShader vshader(Device, L"hlsl/Rectangle.VertexShader", "Main");
	PixelShader pshader(Device, L"hlsl/Rectangle.PixelShader", "Main");

	UINT stride = sizeof(RectangleVertex);
	UINT offset = 0;
	Context->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);
	Context->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	vshader.SetInputLayout(Device, Context, rectied, 2);

	vshader.Set(Context);
	pshader.Set(Context);

	Context->DrawIndexed(6, 0,0);
}

/*
class Rectangle_Class
{
public:
	Rectangle_Class() {};
	Rectangle_Class(ID3D11Device* Device,
		float x, float y, float z,
		float sx, float sy, float sz,
		float r, float g, float b);

	~Rectangle_Class();

	void Draw(ID3D11Device* Device, ID3D11DeviceContext* Context);

private:
	ID3D11Buffer* VertexBuffer;
};

Rectangle_Class::Rectangle_Class(ID3D11Device* Device,
	float x, float y, float z,
	float sx, float sy, float sz,
	float r, float g, float b)
{
	RectangleVertex vertices[] = {
	{ sx * (x + -0.5f),  sy * (y + 0.0f), sz * (z + 0.0f), r, g, b, 1.0f },
	{ sx * (x + -0.5f), sy * (y + 0.5f), sz * (z + 0.0f), r, g, b, 1.0f },
	{ sx * (x + 0.0f), sy * (y + 0.0f), sz * (z + 0.0f), r, g, b, 1.0f },

	{ sx * (x + 0.0f),  sy * (y + 0.5f), sz * (z + 0.0f), r, g, b, 1.0f },
	{ sx * (x + 0.0f), sy * (y + 0.0f), sz * (z + 0.0f), r, g, b, 1.0f },
	{ sx * (x + -0.5f), sy * (y + 0.5f), sz * (z + 0.0f), r, g, b, 1.0f }
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(vertices);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA srd = { vertices, 0, 0 };
	Device->CreateBuffer(&bd, &srd, &VertexBuffer);

}

Rectangle_Class::~Rectangle_Class()
{
	VertexBuffer->Release();
}

void Rectangle_Class::Draw(ID3D11Device* Device, ID3D11DeviceContext* Context)
{
	VertexShader vshader(Device, L"hlsl/Rectangle.VertexShader", "Main");
	PixelShader pshader(Device, L"hlsl/Rectangle.PixelShader", "Main");

	UINT stride = sizeof(RectangleVertex);
	UINT offset = 0;
	Context->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);

	vshader.SetInputLayout(Device, Context, rectied, 2);

	Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	vshader.Set(Context);
	pshader.Set(Context);

	Context->Draw(6, 0);
}*/


#endif // !TRIANGLE_H
