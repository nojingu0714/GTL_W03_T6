#include "MVPShader.hlsli"

// Texture 슬롯.
Texture2D TexAmbientMap : register(t0);
Texture2D TexDiffuseMap : register(t1);
Texture2D TexSpecularMap : register(t2);
Texture2D TexEmissiveMap : register(t3);
Texture2D TexBumpMap : register(t4);
Texture2D TexDisplacementMap : register(t5);
Texture2D TexNormalMap : register(t6);

SamplerState MatSampler : register(s0);

cbuffer MaterialCB : register(b4)
{
    float3 Ambient;
    float Opacity;
    
    
    float3 Diffuse;
    float Transparency;
    
    
    float3 Specular;
    float RefractiveIndex;
    
    
    float3 Emissive;
    float SpecularExponent;
    

    
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
    
    float4 AmbientTex = TexAmbientMap.Sample(MatSampler, Input.Tex);
    float4 DiffuseTex = TexDiffuseMap.Sample(MatSampler, Input.Tex);
    float4 SpecularTex = TexSpecularMap.Sample(MatSampler, Input.Tex);
    float4 EmissiveTex = TexEmissiveMap.Sample(MatSampler, Input.Tex);
    float4 BumpMapTex = TexBumpMap.Sample(MatSampler, Input.Tex);
    float4 DisplacementMapTex = TexDisplacementMap.Sample(MatSampler, Input.Tex);
    float4 NormalMapTex = TexNormalMap.Sample(MatSampler, Input.Tex);
    
    float4 AmbientColor = AmbientTex * float4(Ambient, 1.0f);
    float4 DiffuseColor = DiffuseTex * float4(Diffuse, 1.0f);
    float4 SpecularColor = SpecularTex * float4(Specular, 1.0f);
    float4 EmissiveColor = float4(Emissive, 1.0f);
    
    float4 TextureColor = DiffuseColor * 0.5 + AmbientColor * 0.1 + SpecularColor * 0.1 + SpecularExponent * 0.1 + BumpMapTex * 0.05 + RefractiveIndex * 0.8 + EmissiveColor;
    
    // 하드코딩으로 조명 생성.
    float3 gLightDir = normalize(float3(-1.0f, 0.0f, 1.0f));
    float3 gLightColor = float3(1.0f, 0.0f, 0.0f);
    float3 gAmbientColor = float3(0.1f, 0.1f, 0.1f);
    
    float3 Norm = normalize(Input.Nor);
    float Diff = saturate(dot(Norm, -gLightDir));
    float shadowFactor = smoothstep(0.3, 0.6, Diff);
    float3 AmbientLight = gAmbientColor * Ambient;
    
    float3 DiffuseLight = Diff * gLightColor * Diffuse * (0.2 + 1.0 * shadowFactor);
    
    float3 Lighting = AmbientLight + DiffuseLight;
    
    float4 FinalColor = float4(TextureColor.rgb * Lighting, TextureColor.a);
    FinalColor.a *= Opacity;

    return float4(Diffuse, 1.0f);
    
    return FinalColor; // 알파는 1.0f로 설정 (불투명)
}