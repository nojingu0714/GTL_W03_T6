#include "MVPShader.hlsli"

// Texture 슬롯.
cbuffer ColorCB : register(b3)
{
    float4 Color;
};

// StaticMeshShader
struct VS_INPUT
{
    float4 Pos : POSITION; // Input position from vertex buffer
    float3 Nor : NORMAL;
    float4 Color : COLOR; // Input color from vertex buffer
    float2 Tex : TEXCOORD;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION; // Transformed position to pass to the pixel shader
};

PS_INPUT mainVS(VS_INPUT Input)
{
    Input.Pos.w = 1.f;
    
    PS_INPUT Output = (PS_INPUT) 0.f;
    
    Output.Pos = Input.Pos;
    Output.Pos = mul(Output.Pos, WorldMatrix); // 오브젝트 스페이스 -> 월드 스페이스
    Output.Pos = mul(Output.Pos, ViewMatrix); // 월드 스페이스 -> 뷰 스페이스
    Output.Pos = mul(Output.Pos, ProjectionMatrix); // 뷰 스페이스 -> NDC
    
    return Output;
}

float4 mainPS(PS_INPUT Input) : SV_TARGET
{    
    return Color;
}