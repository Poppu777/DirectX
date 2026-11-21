// パイプラインステートオブジェクトクラス
#pragma once

#include"DirectX.h"
#include"shader.h"
#include"root_signature.h"

class PipelineStateObject
{
public:
	PipelineStateObject() = default;

	~PipelineStateObject();

	[[nodiscard]]bool create(const DirectX& directx,const shader& shader, const root_signature& rootsignature)
};

