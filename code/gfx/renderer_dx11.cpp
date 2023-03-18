#include "renderer.hpp"
#include "dx.hpp"
#include "dx11.hpp"


namespace gfx {
namespace dx11 {

GLOBAL char const shader_source[] = R"HLSL(
cbuffer VsConstantBuffer : register(b0)
{
    float4x4 model_matrix;
    float4x4 view_matrix;
    float4x4 projection_matrix;
};

struct VS_Input
{
    float3 position : POSITION;
    float4 color    : COLOR;
};

struct VS_Output
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VS_Output VShader(VS_Input input)
{
    float4 p = mul(projection_matrix, mul(view_matrix, mul(model_matrix, float4(input.position, 1.0))));

    VS_Output result;
    result.position = p;
    result.color = input.color;

    return result;
}

float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
    return color;
}
)HLSL";






} // namespace dx11
} // namespace gfx
