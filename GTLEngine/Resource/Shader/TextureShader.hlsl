#include "MVPShader.hlsli"

Texture2D AtlasTexture : register(t0);
SamplerState AtlasSampler : register(s0);

struct VS_INPUT
{
    float4 position : POSITION; // Input position from vertex buffer
    float2 texCoord : TEXCOORD; // Input texture coordinate from vertex buffer
};

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

PS_INPUT mainVS(VS_INPUT Input)
{
    Input.position.w = 1.f;
    
    PS_INPUT Output = (PS_INPUT) 0.0f;
    // Output.Position = mul(Input.Position, MVP); // 오브젝트 스페이스 -> NDC
    
    Output.position = Input.position;
    Output.position = mul(Output.position, WorldMatrix); // 오브젝트 스페이스 -> 월드 스페이스
    Output.position = mul(Output.position, ViewMatrix); // 월드 스페이스 -> 뷰 스페이스
    Output.position = mul(Output.position, ProjectionMatrix); // 뷰 스페이스 -> NDC
    
    //int a = 65 / 16;
    //int b = 65 % 16;
    //Output.texCoord = (Input.texCoord + float2(b , a)) / 16.0f;
    Output.texCoord = Input.texCoord;
    
    return Output;
}

float4 mainPS(PS_INPUT Input) : SV_TARGET
{
    
    float4 Texture = AtlasTexture.Sample(AtlasSampler, Input.texCoord);
    
    if (Texture.r <= 0.1f && Texture.g <= 0.1f && Texture.b <= 0.1f)
    {
        discard;
    }
    return Texture;
}