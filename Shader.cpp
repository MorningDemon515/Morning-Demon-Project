#include "Shader.h"

VertexShader::VertexShader(ID3D11Device* device, const wchar_t* vss, const char* Main)
{
	D3DCompileFromFile(vss, nullptr, nullptr, Main, "vs_5_0", 0, 0, &pVSBlob, nullptr);
	device->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &pVertexShader);
}

VertexShader::~VertexShader()
{
	pVSBlob->Release();
	pVertexShader->Release();
	InputLayout->Release();
}

void VertexShader::Set(ID3D11DeviceContext* context)
{
	context->VSSetShader(pVertexShader, nullptr, 0);
}

void VertexShader::SetInputLayout(ID3D11Device* device, ID3D11DeviceContext* context, D3D11_INPUT_ELEMENT_DESC* ied,UINT size)
{
	device->CreateInputLayout(ied, size, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &InputLayout);
	context->IASetInputLayout(InputLayout);
}

PixelShader::PixelShader( ID3D11Device* device, const wchar_t* pss, const char* Main)
{
	D3DCompileFromFile(pss, nullptr, nullptr, Main, "ps_5_0", 0, 0, &pPSBlob, nullptr);
	device->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &pPixelShader);
}

PixelShader::~PixelShader()
{
	pPixelShader->Release();
	pPSBlob->Release();
}

void PixelShader::Set(ID3D11DeviceContext* context)
{
	context->PSSetShader(pPixelShader, nullptr, 0);
}