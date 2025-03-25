#include "MVPShader.hlsli"

struct VS_INPUT
{
    float3 Position : POSITION; // Input position from vertex buffer
    float4 Color : COLOR; // Input color from vertex buffer
};

struct PS_INPUT
{
    float4 Position : SV_POSITION; // Transformed position to pass to the pixel shader
    float4 Color : COLOR; // Color to pass to the pixel shader
};

PS_INPUT mainVS(VS_INPUT Input)
{ 
    PS_INPUT Output = (PS_INPUT) 0.0f;
    
    Output.Position = float4(Input.Position, 1.0f);
    Output.Position = mul(Output.Position, WorldMatrix); // 오브젝트 스페이스 -> 월드 스페이스
    Output.Position = mul(Output.Position, ViewMatrix); // 월드 스페이스 -> 뷰 스페이스
    Output.Position = mul(Output.Position, ProjectionMatrix); // 뷰 스페이스 -> NDC
    
    if (ColorFlag == true)
        Output.Color = Input.Color + float4(0.5f, 0.5f, 0.5f, 0.0f);
    else
        Output.Color = Input.Color;

    return Output;
}

float4 mainPS(PS_INPUT Input) : SV_TARGET
{
    // Output the color directly
    return Input.Color;
}