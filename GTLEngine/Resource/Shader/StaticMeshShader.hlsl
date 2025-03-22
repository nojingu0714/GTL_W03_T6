#include "MVPShader.hlsli"

Texture2D g_txColor : register(t0);
SamplerState g_Sample : register(s0);

// StaticMeshShader
struct VS_INPUT
{
    float4 Pos : POSITION; // Input position from vertex buffer
    float3 Nor : NORMAL;
    float4 Color : COLOR; // Input color from vertex buffer
    float2 Tex : TEXTURE;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION; // Transformed position to pass to the pixel shader
    float4 Color : COLOR; // Color to pass to the pixel shader
    float2 Tex : TexCoord;
};

PS_INPUT mainVS(VS_INPUT Input)
{
    Input.Pos.w = 1.f;
    
    PS_INPUT Output = (PS_INPUT) 0.0f;
    // Output.Pos = mul(Input.Pos, MVP); // 오브젝트 스페이스 -> NDC
    
    Output.Pos = Input.Pos;
    Output.Pos = mul(Output.Pos, WorldMatrix); // 오브젝트 스페이스 -> 월드 스페이스
    Output.Pos = mul(Output.Pos, ViewMatrix); // 월드 스페이스 -> 뷰 스페이스
    Output.Pos = mul(Output.Pos, ProjectionMatrix); // 뷰 스페이스 -> NDC
    
    Output.Tex = Input.Tex;

    return Output;
}

float4 mainPS(PS_INPUT Input) : SV_TARGET
{
    float Texture = g_txColor.Sample(g_Sample, Input.Tex);
    
    return Texture;
}