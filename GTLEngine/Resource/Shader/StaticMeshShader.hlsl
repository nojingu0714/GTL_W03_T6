#include "MVPShader.hlsli"

Texture2D g_txColor : register(t0);
SamplerState g_Sample : register(s0);

// StaticMeshShader
struct VS_INPUT
{
    float3 p : POSITION; // Input position from vertex buffer
    float3 n : NORMAL;
    float4 c : COLOR; // Input color from vertex buffer
    float2 t : TEXTURE;
};
