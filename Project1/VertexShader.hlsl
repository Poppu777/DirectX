struct VSInput
{
    float3 position : POSITION;  // 入力：頂点座標
    float4 color : COLOR;     // 入力：頂点色
};

struct VSOutput
{
    float4 position : SV_POSITION; // 出力：変換後座標
    float4 color : COLOR;        // 出力：頂点色
};

VSOutput vs(VSInput input)
{
    VSOutput output;

    // 3D座標を同次座標に変換
    output.position = float4(input.position, 1.0f);

    // 色情報をそのまま次の段階に渡す
    output.color = input.color;

    return output;
}