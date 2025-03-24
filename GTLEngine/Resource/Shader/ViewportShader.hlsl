Texture2D ViewportTexture : register(t0);
SamplerState ViewportSampler : register(s0);

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
    Output.TexCoord = Input.TexCoord;
    return Output;
}

float4 mainPS(PS_INPUT Input) : SV_TARGET
{
    return ViewportTexture.Sample(ViewportSampler, Input.TexCoord);
}


