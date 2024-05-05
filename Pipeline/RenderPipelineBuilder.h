#pragma once
#include "RenderPipeline.h"
#include "webgpu/webgpu.h"
#include "Device.h"
#include "Shader.h"
#include "Buffer.h"

class WGPUFramework;

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

	RenderPipelineBuilder(const RenderPipelineBuilder& other) {
		SetDescPtrs();
	}

	void AddBufferLayout(BufferLayout& BufferLayout);

	void SetShader(const std::filesystem::path& path);

	template<typename T>
	void AddUniformBinding(uint16_t group, uint16_t binding, ShaderVisibility visibility);

	void AddTextureBinding(uint16_t group, uint16_t binding, ShaderVisibility visibility);

	~RenderPipelineBuilder();

private:
	void SetDescPtrs() {
		m_descriptor.fragment = &m_fragmentState;
		m_fragmentState.targets = &m_colorTargetState;
		m_colorTargetState.blend = &m_blendState;
	}

	RenderPipelineBuilder(WGPUTextureFormat targetFormat);


private:
	void SetDefaultBindingLayout(WGPUBindGroupLayoutEntry& bindingLayout);

	WGPURenderPipelineDescriptor m_descriptor = {};
	WGPUFragmentState m_fragmentState = {};
	WGPUBlendState m_blendState = {};
	WGPUColorTargetState m_colorTargetState = {};
	std::filesystem::path m_shaderSource;
	bool customShader = false;

	std::vector<WGPUVertexBufferLayout> m_vbLayouts;
	std::vector<WGPUVertexAttribute*> m_vertexAttribs;

	std::vector<WGPUBindGroupLayoutEntry> m_bindingEntries;
	std::vector<uint16_t> m_bindingsGroups;
};



template<typename T>
inline void RenderPipelineBuilder::AddUniformBinding(uint16_t group, uint16_t binding, ShaderVisibility visibility)
{
	m_bindingEntries.push_back({});
	m_bindingsGroups.push_back(group);
	auto& bindingEntry = m_bindingEntries.back();
	SetDefaultBindingLayout(bindingEntry);
	bindingEntry.binding = binding;
	bindingEntry.visibility = visibility;
	bindingEntry.buffer.type = WGPUBufferBindingType_Uniform;
	bindingEntry.buffer.minBindingSize = sizeof(T);
}

inline void RenderPipelineBuilder::AddTextureBinding(uint16_t group, uint16_t binding, ShaderVisibility visibility)
{
	m_bindingEntries.push_back({});
	m_bindingsGroups.push_back(group);
	auto& bindingEntry = m_bindingEntries.back();
	SetDefaultBindingLayout(bindingEntry);
	bindingEntry.binding = binding;
	bindingEntry.visibility = visibility;
	bindingEntry.texture.sampleType = WGPUTextureSampleType_Float;
	bindingEntry.texture.viewDimension = WGPUTextureViewDimension_2D;
}

inline RenderPipelineBuilder::~RenderPipelineBuilder()
{
	for (auto& ptr : m_vertexAttribs) {
		delete ptr;
	}
}
