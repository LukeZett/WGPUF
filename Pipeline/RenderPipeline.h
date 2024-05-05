#pragma once
#include "webgpu/webgpu.h"
#include "Shader.h"

class RenderPipelineBuilder;

class RenderPipeline
{
	friend RenderPipelineBuilder;

	RenderPipeline(std::filesystem::path shaderSource) : 
		m_shadermodule(shaderSource) {
	};

	RenderPipeline() {
	}

	RenderPipeline(RenderPipeline&& other) noexcept : m_shadermodule(std::move(other.m_shadermodule)),
		m_pipeline(std::move(other.m_pipeline)) {
	}
private:
	uint16_t m_bindEntryCount = 0;
	WGPUBindGroupLayout m_layout = {};

	void Init(WGPURenderPipelineDescriptor& descriptor);

	WGPURenderPipeline m_pipeline = nullptr;
	Shader m_shadermodule;

public:
	WGPUBindGroupLayout GetBindGroupLayout(uint16_t group) const {
		return m_layout;
	}

	uint16_t GetBindingCount(uint16_t group) const {
		return m_bindEntryCount;
	}

	WGPURenderPipeline Get() const {
		return m_pipeline;
	}

	~RenderPipeline() {
		if (m_pipeline == nullptr) return;
		wgpuRenderPipelineRelease(m_pipeline);
		wgpuBindGroupLayoutRelease(m_layout);
	}
};