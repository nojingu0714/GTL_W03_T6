#include "MVPShader.hlsli"
Texture2D ViewportTexture : register(t0);
SamplerState ViewportSampler : register(s0);

cbuffer CViewportRatio : register(b4)
{
    float OffsetX;
    float OffsetY;
    float ExtentX;
    float ExtentY;
}

    struct VS_INPUT
{
    float3 Position : POSITION;
    float2 TexCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
};

VS_OUTPUT mainVS(VS_INPUT Input)
{
    VS_OUTPUT Output;
    Output.Position = float4(Input.Position, 1.0f);
    //Output.Position = Output.Position * float4(ExtentX, ExtentY, 0, 1) + float4(OffsetX, OffsetY, 0, 1);
    Output.TexCoord = Input.TexCoord;
    return Output;
}

float4 mainPS(PS_INPUT Input) : SV_TARGET
{
    return ViewportTexture.Sample(ViewportSampler, Input.TexCoord);
}


