#include "MVPShader.hlsli"

// Texture 슬롯.
Texture2D TexAmbientMap : register(t0);
Texture2D TexDiffuseMap : register(t1);
Texture2D TexSpecularMap : register(t2);
Texture2D TexEmissiveMap : register(t3);
Texture2D TexBumpMap : register(t4);
Texture2D TexDisplacementMap : register(t5);
Texture2D TexNormalMap : register(t6);

SamplerState g_Sampler : register(s0);

cbuffer MaterialCB : register(b3)
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
    float3 Nor : NORMAL;
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
    Output.Color = Input.Color;

    return Output;
}

float4 mainPS(PS_INPUT Input) : SV_TARGET
{
    // 텍스처 색상 샘플링
    float4 TexDiffuseColor = TexDiffuseMap.Sample(g_Sampler, Input.Tex);
    
     // 머티리얼 값
    float3 DiffuseColor = Diffuse;
    float3 AmbientColor = Ambient;
    float3 SpecularColor = Specular;
    float3 EmissiveColor = Emissive;
    
    // // 조명 계산 (단일 광원 예시)
    //Light light;
    //light.Position = float3(0.0f, 10.0f, 0.0f); // 광원 위치 (예: 월드 좌표)
    //light.Color = float3(1.0f, 1.0f, 1.0f); // 광원 색상 (예: 흰색)
    //light.Intensity = 1.0f; // 광원 강도
    
    //float3 lightDir = normalize(light.Position - Input.Pos.xyz);
    //float3 normal = normalize(Input.Nor);
    
    //// Lambertian 조명 모델 적용
    //float lambert = LambertLighting(normal, lightDir);

    // 최종 색상 계산
    //float3 FinalColor = AmbientColor + lambert * DiffuseColor + SpecularColor * pow(lambert, SpecularExponent) + EmissiveColor;

    float3 FinalColor = TexDiffuseColor;
    return TexDiffuseColor; // 알파는 1.0f로 설정 (불투명)
}