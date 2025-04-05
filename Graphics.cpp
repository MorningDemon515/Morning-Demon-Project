#include "Main.h"

extern HWND hwnd;
extern bool run;

ID3D11Device* Device;
ID3D11DeviceContext* Context;
IDXGISwapChain* SwapChain;

ID3D11RenderTargetView* RenderTargetView;

void InitGraphics(int windowed)
{
	SetWindowText(hwnd,L"Game App - Direct3D 11");

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd,sizeof(DXGI_SWAP_CHAIN_DESC));

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

	Context->OMSetRenderTargets(1,&RenderTargetView, NULL);
}

void DrawScene()
{

	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000 )
	{
		run = false;
	}

	float color[4] = {0.0f,1.0f,1.0f,1.0f};
	Context->ClearRenderTargetView(RenderTargetView,color);

	SwapChain->Present(0,0);
}

void CleanGraphics()
{

	Device->Release();
	Context->Release();
	SwapChain->Release();

	RenderTargetView->Release();
}