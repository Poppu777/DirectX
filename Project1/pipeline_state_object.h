// パイプラインステートオブジェクトクラス
#pragma once

#include"DirectX.h"
#include"shader.h"
#include"root_signature.h"

// パイプラインステートオブジェクトクラス
class PipelineStateObject
{
public:
	PipelineStateObject() = default;

	~PipelineStateObject();

	/*
	* @brief	パイプラインステートオブジェクトを作成する
	* @param	device(DirectX)		デバイスクラスのインスタンス
	* @param	shader				シェーダクラスのインスタンス
	* @param	rootSingnature		ルートシグネチャクラスのインスタンス
	*/
	[[nodiscard]] bool create(const DirectX& directx, const shader& shader, const root_signature& rootsignature) noexcept;

	/*
	* @brief	パイプラインステートを取得する
	* @return	パイプラインステートポインタ
	*/
	[[nodiscard]] ID3D12PipelineState* get() const noexcept;

private:
	ID3D12PipelineState* pipelinelineState_ = {};	///< パイプラインステート
};

