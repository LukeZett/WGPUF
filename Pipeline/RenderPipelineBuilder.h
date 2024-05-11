#pragma once
#include "RenderPipeline.h"
#include "webgpu/webgpu.h"
#include "Device.h"
#include "Shader.h"
#include "Buffer.h"
#include "AppWindow.h"

class WGPUFramework;

void setDefault(WGPUStencilFaceState& stencilFaceState);
void setDefault(WGPUDepthStencilState& depthStencilState);


enum ShaderVisibility {
	VertexShader = WGPUShaderStage_Vertex,
	FragmentShader = WGPUShaderStage_Fragment,
	All = WGPUShaderStage_Vertex | WGPUShaderStage_Fragment
};

class RenderPipelineBuilder
{
	friend WGPUFramework;

public:
	RenderPipeline Build();

	RenderPipelineBuilder(const RenderPipelineBuilder& other) : m_window(other.m_window) {
		SetDescPtrs();
	}

	RenderPipelineBuilder& AddBufferLayout(BufferLayout& BufferLayout);

	RenderPipelineBuilder& SetShader(const std::filesystem::path& path);

	template<typename T>
	RenderPipelineBuilder& AddUniformBinding(uint16_t group, uint16_t binding, ShaderVisibility visibility);

	RenderPipelineBuilder& AddTextureBinding(uint16_t group, uint16_t binding, ShaderVisibility visibility, WGPUTextureViewDimension dimension = WGPUTextureViewDimension_2D);

	RenderPipelineBuilder& SetDepthState(WGPUCompareFunction comparator, bool depthWrite);

	~RenderPipelineBuilder();

private:
	void SetDescPtrs() {
		m_descriptor.fragment = &m_fragmentState;
		m_fragmentState.targets = &m_colorTargetState;
		m_colorTargetState.blend = &m_blendState;
		if (m_descriptor.depthStencil) m_descriptor.depthStencil = &m_depthStencilState;
	}

	RenderPipelineBuilder(WGPUTextureFormat targetFormat, AppWindow& window);


private:
	void SetDefaultBindingLayout(WGPUBindGroupLayoutEntry& bindingLayout);

	WGPURenderPipelineDescriptor m_descriptor = {};
	WGPUFragmentState m_fragmentState = {};
	WGPUBlendState m_blendState = {};
	WGPUDepthStencilState m_depthStencilState = {};
	WGPUColorTargetState m_colorTargetState = {};
	AppWindow& m_window;
	std::filesystem::path m_shaderSource;
	bool customShader = false;

	std::vector<WGPUVertexBufferLayout> m_vbLayouts;
	std::vector<WGPUVertexAttribute*> m_vertexAttribs;

	std::vector<WGPUBindGroupLayoutEntry> m_bindingEntries;
	std::vector<uint16_t> m_bindingsGroups;
};



template<typename T>
inline RenderPipelineBuilder& RenderPipelineBuilder::AddUniformBinding(uint16_t group, uint16_t binding, ShaderVisibility visibility)
{
	m_bindingEntries.push_back({});
	m_bindingsGroups.push_back(group);
	auto& bindingEntry = m_bindingEntries.back();
	SetDefaultBindingLayout(bindingEntry);
	bindingEntry.binding = binding;
	bindingEntry.visibility = visibility;
	bindingEntry.buffer.type = WGPUBufferBindingType_Uniform;
	bindingEntry.buffer.minBindingSize = sizeof(T);
	return *this;
}

inline RenderPipelineBuilder& RenderPipelineBuilder::AddTextureBinding(uint16_t group, uint16_t binding, ShaderVisibility visibility, WGPUTextureViewDimension dimension)
{
	m_bindingEntries.push_back({});
	m_bindingsGroups.push_back(group);
	auto& bindingEntry = m_bindingEntries.back();
	SetDefaultBindingLayout(bindingEntry);
	bindingEntry.binding = binding;
	bindingEntry.visibility = visibility;
	bindingEntry.texture.sampleType = WGPUTextureSampleType_Float;
	bindingEntry.texture.viewDimension = dimension;
	return *this;
}

inline RenderPipelineBuilder& RenderPipelineBuilder::SetDepthState(WGPUCompareFunction comparator, bool depthWrite)
{
	setDefault(m_depthStencilState);

	m_depthStencilState.depthCompare = comparator;
	m_depthStencilState.depthWriteEnabled = depthWrite;
	m_depthStencilState.format = static_cast<WGPUTextureFormat>(m_window.GetDepthFormat());
	m_depthStencilState.stencilReadMask = 0;
	m_depthStencilState.stencilWriteMask = 0;
	m_descriptor.depthStencil = &m_depthStencilState;
	return *this;
}

inline RenderPipelineBuilder::~RenderPipelineBuilder()
{
	for (auto& ptr : m_vertexAttribs) {
		delete ptr;
	}
}


inline void setDefault(WGPUStencilFaceState& stencilFaceState) {
	stencilFaceState.compare = WGPUCompareFunction_Always;
	stencilFaceState.failOp = WGPUStencilOperation_Keep;
	stencilFaceState.depthFailOp = WGPUStencilOperation_Keep;
	stencilFaceState.passOp = WGPUStencilOperation_Keep;
}

inline void setDefault(WGPUDepthStencilState& depthStencilState) {
	depthStencilState.format = WGPUTextureFormat_Undefined;
	depthStencilState.depthWriteEnabled = false;
	depthStencilState.depthCompare = WGPUCompareFunction_Always;
	depthStencilState.stencilReadMask = 0xFFFFFFFF;
	depthStencilState.stencilWriteMask = 0xFFFFFFFF;
	depthStencilState.depthBias = 0;
	depthStencilState.depthBiasSlopeScale = 0;
	depthStencilState.depthBiasClamp = 0;
	setDefault(depthStencilState.stencilFront);
	setDefault(depthStencilState.stencilBack);
}
