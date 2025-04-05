#ifndef SHADER_H
#define SHADER_H

#include <locale>
#include <codecvt>
#include <string>

#include <d3d11.h>
#include <d3dcompiler.h>

class VertexShader
{
public:
	VertexShader() {};
	VertexShader(ID3D11Device* device,const wchar_t* vss, const char* Main);
	~VertexShader();

	void Set(ID3D11DeviceContext* context);
	void SetInputLayout(ID3D11Device* device, ID3D11DeviceContext* context, D3D11_INPUT_ELEMENT_DESC* ied, UINT size);

private:
	ID3DBlob* pVSBlob = nullptr;
	ID3D11VertexShader* pVertexShader;
	ID3D11InputLayout* InputLayout;
};

class PixelShader
{
public:
	PixelShader() {};
	PixelShader(ID3D11Device* device, const wchar_t* pss, const char* Main);
	~PixelShader();

	void Set(ID3D11DeviceContext* context);

private:
	ID3DBlob* pPSBlob = nullptr;
	ID3D11PixelShader* pPixelShader;

};

#endif // !SHADER_H
