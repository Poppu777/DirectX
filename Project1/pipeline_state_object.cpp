// パイプラインステートオブジェクトクラス

#include "pipeline_state_object.h"
#include<cassert>

PipelineStateObject::~PipelineStateObject() {

	if (pipelineState_) {
		pipelineState_->Release();
		pipelineState_ = nullptr;
	}
}

[[nodidscard]] bool PipelineStateObject::create(const DirectX& device, const Shader& shader, const RootSignature& rootSignature) noexcept {

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = {
		{"POSITION", 0,DXGI_FORMAT_R32G32B32_FLOAT,0,0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,0,12,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
	};
}

D3D12_RASTERIZER_DESC rasterizerDesc{};
rasterizerDesc.FillMode					= D3D12_FILL_MODE_SOLID;
rasterizerDesc.CullMode					= D3D12_CULL_MODE_BACK;
rasterizerDesc.FrontCounterClockwise	= false;
rasterizerDesc.DepthBias				= D3D12_DEFAULT_DEPTH_BIAS;
rasterizerDesc.DepthBiasClamp			= D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
rasterizerDesc.SlopeScaledDepthBias		= D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
rasterizerDesc.DepthClipEnable			= true;
rasterizerDesc.MultisampleEnable		= false;
rasterizerDesc.AntialiasedLineEnable	= false;
rasterizerDesc.ForcedSampleCount		= 0;
rasterizerDesc.ConservativeRaster		= D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;


const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc = {
	FALSE,
	FALSE,
	D3D12_BLEND_ONE,
	D3D12_BLEND_ZERO,
	D3D12_BLEND_OP_ADD,
	D3D12_BLEND_ONE,
	D3D12_BLEND_ZERO,
	D3D12_BLEND_OP_ADD,
	D3D12_LOGIC_OP_NOOP,
	D3D12_COLOR_WRITE_ENABLE_ALL,
};

D3D12_BLEND_DESC blendDesc{};
blendDesc.AlpahToCoverageEnable = false;
blendDesc.IndependentBlendEnable = false;
for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
{
	blendDesc.RenderTarget[i] = defaultRenderTagetBlendDesc;
}