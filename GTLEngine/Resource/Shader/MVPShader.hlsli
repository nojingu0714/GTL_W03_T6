cbuffer CbChangesOnResize : register(b0)
{
    row_major matrix ProjectionMatrix; // 뷰 스페이스 -> NDC
};

cbuffer CbChangesEveryFrame : register(b1)
{
    row_major matrix ViewMatrix; // 월드 스페이스 -> 뷰 스페이스
};

cbuffer CbChangesEveryObject : register(b2)
{
    row_major matrix WorldMatrix; // 오브젝트 스페이스 -> 월드 스페이스
    int ColorFlag;
    int Padding[3];
};