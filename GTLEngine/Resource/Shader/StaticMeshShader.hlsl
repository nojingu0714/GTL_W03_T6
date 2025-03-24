#include "MVPShader.hlsli"

// Texture 슬롯.
Texture2D g_DiffuseMap : register(t0);
Texture2D g_SpecularMap : register(t1);
Texture2D g_NormalMap : register(t2);
SamplerState g_Sampler : register(s0);

cbuffer MaterialCB : register(b1)
{
    float3 Ambient;
    float3 Diffuse;
    float3 Specular;
    float3 Emissive;
    float SpecularExponent;
    float Opacity;
    float Transparency;
    float RefractiveIndex;
    int IlluminationModel;
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
    float4 Color : COLOR; // Color to pass to the pixel shader
    float2 Tex : TEXCOORD;
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
    float4 TextureColor = g_DiffuseMap.Sample(g_Sampler, Input.Tex);

// 디버깅용: 색상이 제대로 나오는지 확인
    return TextureColor;
}