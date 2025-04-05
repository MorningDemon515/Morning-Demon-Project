#include "Shader.h"

extern ID3D11Device* Device;
extern ID3D11DeviceContext* Context;

ID3D11Buffer* VertexBuffer;

struct Vertex {
	float x, y, z;
	float r, g, b, a;
};

D3D11_INPUT_ELEMENT_DESC ied[] = {
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

void InitScene()
{
	Vertex vertices[] = {
	{ 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f },
	{ 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f },
	{-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f }
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(vertices);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA srd = { vertices, 0, 0 };
	Device->CreateBuffer(&bd, &srd, &VertexBuffer);
}

void Draw_Scene()
{
	VertexShader vshader(Device, L"hlsl/VertexShader.txt", "Main");
	PixelShader pshader(Device, L"hlsl/PixelShader.txt", "Main");

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	Context->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);

	vshader.SetInputLayout(Device, Context, ied, 2);

	Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	vshader.Set(Context);
	pshader.Set(Context);

	Context->Draw(3, 0);
}

void CleanScene()
{
	VertexBuffer->Release();
}