struct PSInput
{
    float4 position : SV_POSITION; // 入力：スクリーン座標（ピクセル単位）
    float4 color : COLOR;        // 入力：補間された色
};

float4 ps(PSInput input) : SV_TARGET
{
    // 補間された頂点色をそのまま出力
    return input.color;

// より複雑な処理の例：
// return input.color * 0.8f;           // 暗くする
// return float4(1.0f, 0.0f, 0.0f, 1.0f); // 全て赤色
}