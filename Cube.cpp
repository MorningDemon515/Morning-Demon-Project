#include "Cube.h"
#include "Main.h"

#include <DirectXMath.h>
using namespace DirectX;

struct MatrixBufferType {
    XMMATRIX World;
    XMMATRIX View;
    XMMATRIX Projection;
};

struct CubeVertex {
	float x, y, z;
	float r, g, b, a;
};

D3D11_INPUT_ELEMENT_DESC cubeied[] = {
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

Cube::Cube(ID3D11Device* Device,
	float r, float g, float b,float a)
{
    CubeVertex vertices[] = {
        {-0.5f, -0.5f, -0.5f, r, g, b, a},
        {0.5f, -0.5f, -0.5f, r, g, b, a},
        {0.5f, 0.5f, -0.5f, r, g, b, a},
        {0.5f, 0.5f, -0.5f, r, g, b, a},
        {-0.5f, 0.5f, -0.5f, r, g, b, a},
        {-0.5f, -0.5f, -0.5f, r, g, b, a},

        {-0.5f, -0.5f, 0.5f, r, g, b, a},
        {0.5f, -0.5f, 0.5f, r, g, b, a},
        {0.5f, 0.5f, 0.5f, r, g, b, a},
        {0.5f, 0.5f, 0.5f, r, g, b, a},
        {-0.5f, 0.5f, 0.5f, r, g, b, a},
        {-0.5f, -0.5f, 0.5f, r, g, b, a},

        {-0.5f, 0.5f, 0.5f, r, g, b, a},
        {-0.5f, 0.5f, -0.5f, r, g, b, a},
        {-0.5f, -0.5f, -0.5f, r, g, b, a},
        {-0.5f, -0.5f, -0.5f, r, g, b, a},
        {-0.5f, -0.5f, 0.5f, r, g, b, a},
        {-0.5f, 0.5f, 0.5f, r, g, b, a},

        {0.5f, 0.5f, 0.5f, r, g, b, a},
        {0.5f, 0.5f, -0.5f, r, g, b, a},
        {0.5f, -0.5f, -0.5f, r, g, b, a},
        {0.5f, -0.5f, -0.5f, r, g, b, a},
        {0.5f, -0.5f, 0.5f, r, g, b, a},
        {0.5f, 0.5f, 0.5f, r, g, b, a},

        {-0.5f, -0.5f, -0.5f, r, g, b, a},
        {0.5f, -0.5f, -0.5f, r, g, b, a},
        {0.5f, -0.5f, 0.5f, r, g, b, a},
        {0.5f, -0.5f, 0.5f, r, g, b, a},
        {-0.5f, -0.5f, 0.5f, r, g, b, a},
        {-0.5f, -0.5f, -0.5f, r, g, b, a},

        {-0.5f, 0.5f, -0.5f, r, g, b, a},
        {0.5f, 0.5f, -0.5f, r, g, b, a},
        {0.5f, 0.5f, 0.5f, r, g, b, a},
        {0.5f, 0.5f, 0.5f, r, g, b, a},
        {-0.5f, 0.5f, 0.5f, r, g, b, a},
        {-0.5f, 0.5f, -0.5f, r, g, b, a}
    };

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
    bd.ByteWidth = sizeof(vertices);
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA srd = { vertices, 0, 0 };
    Device->CreateBuffer(&bd, &srd, &VertexBuffer);

    D3D11_BUFFER_DESC cbd;
    ZeroMemory(&cbd, sizeof(D3D11_BUFFER_DESC));
    cbd.Usage = D3D11_USAGE_DYNAMIC;
    cbd.ByteWidth = sizeof(MatrixBufferType);
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    Device->CreateBuffer(&cbd, nullptr, &MatrixBuffer);

}

Cube::~Cube()
{
	VertexBuffer->Release();
    MatrixBuffer->Release();
}

void Cube::Draw(ID3D11Device* Device, ID3D11DeviceContext* Context)
{
    VertexShader vshader(Device, L"hlsl/Cube.VertexShader", "Main");
    PixelShader pshader(Device, L"hlsl/Cube.PixelShader", "Main");

    XMMATRIX worldMatrix = XMMatrixRotationY(30.0f) * XMMatrixRotationZ(30.0f);
    XMMATRIX viewMatrix = XMMatrixLookAtLH(
        XMVectorSet(0.0f, 0.0f, -5.0f, 1.0f), 
        XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), 
        XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));

    XMMATRIX projectionMatrix = XMMatrixPerspectiveFovLH(
        XMConvertToRadians(45.0f),
        (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT,
        0.1f,
        100.0f
        );
    D3D11_MAPPED_SUBRESOURCE mappedResource;

    Context->Map(MatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;
    dataPtr->World = XMMatrixTranspose(worldMatrix);
    dataPtr->View = XMMatrixTranspose(viewMatrix);
    dataPtr->Projection = XMMatrixTranspose(projectionMatrix);
    Context->Unmap(MatrixBuffer, 0);

    Context->VSSetConstantBuffers(0, 1, &MatrixBuffer);

    UINT stride = sizeof(CubeVertex);
    UINT offset = 0;
    Context->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);

    vshader.SetInputLayout(Device, Context, cubeied, 2);

    Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    vshader.Set(Context);
    pshader.Set(Context);

    Context->Draw(36, 0);

}