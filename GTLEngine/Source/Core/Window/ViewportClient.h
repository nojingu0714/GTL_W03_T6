#pragma once

class FViewportClient
{
private:
	ID3D11SamplerState* SamplerState;
	ID3D11Buffer* QuadVertexBuffer;
	

public:
	void Init();
	void Draw(const FString& ViewportName);
};

