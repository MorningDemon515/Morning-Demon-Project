#ifndef RECTANGLETEXTURE_H
#define RECTANGLETEXTURE_H

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Shader.h"

struct RectangleTextureVertex {
	float x, y, z;
	float u, v;
};

D3D11_INPUT_ELEMENT_DESC recttexied[] = {
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

class RectangleTexture
{
public:
	RectangleTexture() {};
	RectangleTexture(ID3D11Device* Device,const char* texture,
		float x, float y, float z,
		float sx, float sy, float sz);

	~RectangleTexture();

	void Draw(ID3D11Device* Device, ID3D11DeviceContext* Context);
private:
	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;

	ID3D11SamplerState* TextureSamplerState;
	ID3D11Texture2D* Texture;
	ID3D11ShaderResourceView* TextureSRV;
};

RectangleTexture::RectangleTexture(ID3D11Device* Device,const char* texture,
	float x, float y, float z,
	float sx, float sy, float sz)
{
	RectangleTextureVertex vertices[] = {
	{ sx * (x + -0.5f), sy * (y + -0.5f), sz * (z + 0.0f), 0.0f, 1.0f }, 
	{ sx * (x + -0.5f), sy * (y + 0.5f),  sz * (z + 0.0f), 0.0f, 0.0f }, 
	{ sx * (x + 0.5f),  sy * (y + 0.5f),  sz * (z + 0.0f), 1.0f, 0.0f },
	{ sx * (x + 0.5f),  sy * (y + -0.5f), sz * (z + 0.0f), 1.0f, 1.0f } 
	};

	DWORD indices[] = {
	0, 1, 2,
	0, 2, 3,
	};

	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(D3D11_BUFFER_DESC));
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

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(D3D11_SAMPLER_DESC));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;//D3D11_TEXTURE_ADDRESS_WRAP
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;//
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;//
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	Device->CreateSamplerState(&sampDesc, &TextureSamplerState);

	int width, height, channels;
	unsigned char* image_data = stbi_load(texture, &width, &height, &channels, 0);

	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc,sizeof(D3D11_TEXTURE2D_DESC));
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; 
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_IMMUTABLE;       
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA texData;
	ZeroMemory(&texData,sizeof(D3D11_SUBRESOURCE_DATA));
	texData.pSysMem = image_data;
	texData.SysMemPitch = width * 4;

	Device->CreateTexture2D(&texDesc, &texData, &Texture);

	Device->CreateShaderResourceView(Texture, nullptr, &TextureSRV);

	stbi_image_free(image_data);
}

RectangleTexture::~RectangleTexture()
{
	VertexBuffer->Release();
	IndexBuffer->Release();
	TextureSamplerState->Release();
	TextureSRV->Release();
	Texture->Release();
}

void RectangleTexture::Draw(ID3D11Device* Device, ID3D11DeviceContext* Context)
{
	VertexShader vshader(Device, L"hlsl/RectangleTexture.VertexShader", "Main");
	PixelShader pshader(Device, L"hlsl/RectangleTexture.PixelShader", "Main");

	UINT stride = sizeof(RectangleTextureVertex);
	UINT offset = 0;
	Context->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);
	Context->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	vshader.SetInputLayout(Device, Context, recttexied, 2);

	vshader.Set(Context);
	pshader.Set(Context);

	Context->PSSetShaderResources(0, 1, &TextureSRV);
	Context->PSSetSamplers(0, 1, &TextureSamplerState);

	Context->DrawIndexed(6, 0, 0);
}

#endif // !RECTANGLETEXTURE_H
