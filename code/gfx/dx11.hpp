#ifndef GFX_DX11_HPP
#define GFX_DX11_HPP

#include <base.hpp>

#include <d3d11.h>
#include <d3dcompiler.h>


namespace gfx {
namespace dx11 {


struct dx11_driver
{
    ID3D11Device *Device;
    ID3D11DeviceContext *DeviceContext;
    IDXGISwapChain *SwapChain;

    ID3D11RasterizerState *RasterizerState;
    ID3D11DepthStencilState *DepthStencilState;

    math::vector4 ClearColor;
};

static_assert(sizeof(dx11_driver) <= sizeof(gfx::driver));

struct render_target
{
    ID3D11RenderTargetView *backbuffer;
    ID3D11DepthStencilView *depth_stencil;

    uint32 width;
    uint32 height;
    int32  num_samples;
};

struct shader
{
    ID3D11VertexShader *VertexShader;
    ID3D11PixelShader *PixelShader;
    ID3DBlob *VertexShaderBytecode;
    ID3DBlob *PixelShaderBytecode;
};


GLOBAL dx11_driver Dx;


bool32 initialize(win32::window *w, driver *d)
{
    HRESULT Result;

    DXGI_SWAP_CHAIN_DESC SwapChainDescription;
    ZeroMemory(&SwapChainDescription, sizeof(SwapChainDescription));

    SwapChainDescription.BufferDesc.Width = 800; // @todo unhardcode
    SwapChainDescription.BufferDesc.Height = 600;
    SwapChainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    SwapChainDescription.BufferDesc.RefreshRate.Numerator = 60;
    SwapChainDescription.BufferDesc.RefreshRate.Denominator = 1;
    SwapChainDescription.SampleDesc.Count = 1;
    SwapChainDescription.SampleDesc.Quality = 0;
    SwapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SwapChainDescription.BufferCount = 2;
    SwapChainDescription.OutputWindow = w->handle;
    SwapChainDescription.Windowed = TRUE;
    // SwapChainDescription.SwapEffect =  //?
    // SwapChainDescription.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // Allow full-screen switching

    D3D_FEATURE_LEVEL FeatureLevel = D3D_FEATURE_LEVEL_11_0;
    D3D_FEATURE_LEVEL CreatedFeatureLevel;
    Result = D3D11CreateDeviceAndSwapChain(
        NULL,
        D3D_DRIVER_TYPE_HARDWARE, // DriverType
        NULL,               // Software
        NULL,               // Flags
        &FeatureLevel,      // FeatureLevels
        1,                  // FeatureLevelCount,
        D3D11_SDK_VERSION,  // SDKVersion
        &SwapChainDescription,
        &Dx.SwapChain,
        &Dx.Device,
        &CreatedFeatureLevel, // OutFeatureLevel
        &Dx.DeviceContext);

    if(FAILED(Result))
    {
        MessageBeep(MB_ICONERROR);
        MessageBoxA(0, "Could not create Direct3D 11 device and swap chain.", "Asuka Error", MB_OK | MB_ICONERROR | MB_TOPMOST);
        return false;
    }

    // Setting the Rasterizer state
    {
        D3D11_RASTERIZER_DESC RasterizerDescription;
        ZeroMemory(&RasterizerDescription, sizeof(RasterizerDescription));
        RasterizerDescription.FillMode = D3D11_FILL_SOLID;
        RasterizerDescription.CullMode = D3D11_CULL_NONE;
        RasterizerDescription.FrontCounterClockwise = TRUE;
        RasterizerDescription.DepthClipEnable = TRUE;
        RasterizerDescription.MultisampleEnable = FALSE;

        Result = Dx.Device->CreateRasterizerState(&RasterizerDescription, &Dx.RasterizerState);
        if (SUCCEEDED(Result))
        {
            Dx.DeviceContext->RSSetState(Dx.RasterizerState);
        }
        else
        {
            return false;
        }
    }

    // Setting the DepthStencil state
    {
        D3D11_DEPTH_STENCIL_DESC DepthStencilDescription;
        ZeroMemory(&DepthStencilDescription, sizeof(DepthStencilDescription));
        DepthStencilDescription.DepthEnable    = TRUE;
        DepthStencilDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        DepthStencilDescription.DepthFunc      = D3D11_COMPARISON_LESS;

        Result = Dx.Device->CreateDepthStencilState(&DepthStencilDescription, &Dx.DepthStencilState);
        if (SUCCEEDED(Result))
        {
            Dx.DeviceContext->OMSetDepthStencilState(Dx.DepthStencilState, 0);
        }
        else
        {
            return false;
        }
    }

    return true;
}

void set_clear_color(math::vector4 color)
{
    Dx.ClearColor = color;
}

void set_clear_color(float32 r, float32 g, float32 b, float32 a)
{
    glClearColor(r, g, b, a);
}

void clear()
{
    // Dx.DeviceContext->ClearRenderTargetView(MainRenderTarget.backbuffer, Dx.ClearColor.get_data());
    // Dx.DeviceContext->ClearDepthStencilView(MainRenderTarget.depth_stencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

shader compile_shader(string_view source_code)
{
    ID3DBlob *ShaderErrors = NULL;

    ID3DBlob *VSByteCode = NULL;
    HRESULT VertexShaderCompilationResult = D3DCompile(
        source_code.data, // SrcData
        source_code.size, // SrcDataSize
        NULL,             // SourceName
        NULL,             // Defines
        NULL,             // Include
        "VShader",        // Entrypoint
        "vs_4_0",         // Target,
        0,                // Flags1
        0,                // Flags2
        &VSByteCode,      // Code
        &ShaderErrors);   // ErrorMsgs

    if (FAILED(VertexShaderCompilationResult))
    {
        // char ErrorBuffer[1024] = {};
        // sprintf(ErrorBuffer, "Vertex shader compilation error:\n\n%.*s",
        //     (int) ShaderErrors->GetBufferSize(), (char *) ShaderErrors->GetBufferPointer());

        MessageBeep(MB_ICONERROR);
        MessageBoxA(0, (char const *) ShaderErrors->GetBufferPointer(), "Asuka Error", MB_OK | MB_ICONERROR | MB_TOPMOST);
        return {};
    }

    RELEASE_COM(ShaderErrors);

    ID3D11VertexShader *VertexShader = NULL;
    Dx.Device->CreateVertexShader(VSByteCode->GetBufferPointer(), VSByteCode->GetBufferSize(), NULL, &VertexShader);

    ID3DBlob *PSBytecode = NULL;
    HRESULT PixelShaderCompilationResult = D3DCompile(
        source_code.data, // SrcData
        source_code.size, // SrcDataSize
        NULL,             // SourceName
        NULL,             // Defines
        NULL,             // Include
        "PShader",        // Entrypoint
        "ps_4_0",         // Target
        0,                // Flags1
        0,                // Flags2
        &PSBytecode,      // Code
        &ShaderErrors);   // ErrorMsgs

    if (FAILED(PixelShaderCompilationResult))
    {
        // char ErrorBuffer[1024] = {};
        // sprintf(ErrorBuffer, "Pixel shader compilation error:\n\n%.*s",
        //     (int) ShaderErrors->GetBufferSize(), (char *) ShaderErrors->GetBufferPointer());

        MessageBeep(MB_ICONERROR);
        MessageBoxA(0, (char const *) ShaderErrors->GetBufferPointer(), "Asuka Error", MB_OK | MB_ICONERROR | MB_TOPMOST);

        RELEASE_COM(VSByteCode);
        RELEASE_COM(VertexShader);
        RELEASE_COM(ShaderErrors);
        return {};
    }

    ID3D11PixelShader *PixelShader = NULL;
    Dx.Device->CreatePixelShader(PSBytecode->GetBufferPointer(), PSBytecode->GetBufferSize(), NULL, &PixelShader);

    shader Result;
    Result.VertexShader = VertexShader;
    Result.PixelShader = PixelShader;
    Result.VertexShaderBytecode = VSByteCode;
    Result.PixelShaderBytecode = PSBytecode;

    return Result;
}


void use_shader(shader s)
{
    Dx.DeviceContext->VSSetShader(s.VertexShader, 0, 0);
    Dx.DeviceContext->PSSetShader(s.PixelShader, 0, 0);
}


void set_viewport(viewport vp)
{
    D3D11_VIEWPORT DxViewport;
    DxViewport.Width = (float32) vp.width;
    DxViewport.Height = (float32) vp.height;
    DxViewport.TopLeftX = (float32) vp.offset_x;
    DxViewport.TopLeftY = (float32) vp.offset_y;
    DxViewport.MinDepth = 0.0f;
    DxViewport.MaxDepth = 1.0f;

    Dx.DeviceContext->RSSetViewports(1, &DxViewport);
}


} // namespace dx11
} // namespace gfx

#endif // GFX_DX11_HPP
