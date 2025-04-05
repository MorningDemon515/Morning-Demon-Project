#include "Main.h"

extern HWND hwnd;
extern bool run;

ID3D11Device* Device;
ID3D11DeviceContext* Context;
IDXGISwapChain* SwapChain;

ID3D11RenderTargetView* RenderTargetView;
//ID3D11DepthStencilView* DepthStencilView;

ID3D11Buffer* VertexBuffer;

ID3D11VertexShader* VertexShader;
ID3D11PixelShader* PixelShader;

ID3D11InputLayout* InputLayout;

struct Vertex {
	float x, y, z;
	float r, g, b, a;
};

D3D11_INPUT_ELEMENT_DESC ied[] = {
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

void InitGraphics(int windowed)
{
	SetWindowText(hwnd,L"Game App - Direct3D 11");

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd,sizeof(DXGI_SWAP_CHAIN_DESC));

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_TEXTURE2D_DESC));

	int ScreenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
    int ScreenHeight = GetSystemMetrics(SM_CYFULLSCREEN);

	if (windowed == 1)
	{
		sd.BufferCount = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.Width = WINDOW_WIDTH;
		sd.BufferDesc.Height = WINDOW_HEIGHT;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hwnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		depthStencilDesc.Width = WINDOW_WIDTH;
		depthStencilDesc.Height = WINDOW_HEIGHT;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

	}
	else if (windowed == 0)
	{
		sd.BufferCount = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.Width = ScreenWidth;
		sd.BufferDesc.Height = ScreenHeight;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hwnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = FALSE;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		depthStencilDesc.Width = ScreenWidth;
		depthStencilDesc.Height = ScreenHeight;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;
	}
	else
	{
		ErrorMessage("UnKown Window Mode");
		run = false;
	}

	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&sd,
		&SwapChain,
		&Device,
		NULL,
		&Context);

	ID3D11Texture2D* BackBuffer;
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),(LPVOID*)&BackBuffer);
	Device->CreateRenderTargetView(BackBuffer,NULL, &RenderTargetView);
	BackBuffer->Release();

	/*
	ID3D11Texture2D* DepthStencilBuffer;
	Device->CreateTexture2D(&depthStencilDesc,NULL,&DepthStencilBuffer);
	Device->CreateDepthStencilView(DepthStencilBuffer, NULL, &DepthStencilView);
	DepthStencilBuffer->Release();

	Context->OMSetRenderTargets(1,&RenderTargetView, DepthStencilView);
	*/

	D3D11_VIEWPORT vp = { 0 };
	vp.Width = (windowed == 1) ? WINDOW_WIDTH : (FLOAT)ScreenWidth;
	vp.Height = (windowed == 1) ? WINDOW_HEIGHT : (FLOAT)ScreenHeight;
	vp.MaxDepth = 1.0f;
	Context->RSSetViewports(1, &vp);

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

	ID3DBlob* pVSBlob = nullptr;
	D3DCompileFromFile(L"hlsl/VertexShader.txt", nullptr, nullptr, "VS", "vs_5_0", 0, 0, &pVSBlob, nullptr);
	Device->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &VertexShader);

	ID3DBlob* pPSBlob = nullptr;
	D3DCompileFromFile(L"hlsl/PixelShader.txt", nullptr, nullptr, "PS", "ps_5_0", 0, 0, &pPSBlob, nullptr);
	Device->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &PixelShader);

	Device->CreateInputLayout(ied, 2, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &InputLayout);
	pVSBlob->Release();
}

void DrawScene()
{

	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000 )
	{
		run = false;
	}

	float color[4] = {0.0f,0.0f,0.0f,1.0f};
	Context->OMSetRenderTargets(1, &RenderTargetView, NULL);
	Context->ClearRenderTargetView(RenderTargetView,color);
	//Context->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f,0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	Context->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);
	Context->IASetInputLayout(InputLayout);
	Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Context->VSSetShader(VertexShader, nullptr, 0);
	Context->PSSetShader(PixelShader, nullptr, 0);

	Context->Draw(3, 0);

	SwapChain->Present(0,0);
}

void CleanGraphics()
{

	Device->Release();
	Context->Release();
	SwapChain->Release();

	RenderTargetView->Release();
	//DepthStencilView->Release();

	VertexBuffer->Release();

	VertexShader->Release();
	PixelShader->Release();

	InputLayout->Release();
}