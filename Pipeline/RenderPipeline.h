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


public:
	RenderPipeline(RenderPipeline&& other) noexcept : m_shadermodule(std::move(other.m_shadermodule)),
		m_pipeline(std::move(other.m_pipeline)) {
		m_layout = other.m_layout;
		m_bindEntryCount = other.m_bindEntryCount;
		other.m_pipeline = nullptr;
	}

	inline RenderPipeline& operator =(RenderPipeline && other) noexcept {
		if (this != &other) {
			m_pipeline = std::move(other.m_pipeline);
			m_shadermodule = std::move(other.m_shadermodule);
			m_layout = other.m_layout;
			m_bindEntryCount = other.m_bindEntryCount;
			other.m_pipeline = nullptr;
		}
		return *this;
	}

private:
	uint16_t m_bindEntryCount = 0;
	WGPUBindGroupLayout m_layout = nullptr;

	void Init(WGPURenderPipelineDescriptor& descriptor);

	WGPURenderPipeline m_pipeline = nullptr;
	Shader m_shadermodule;

public:
	RenderPipeline() {
	}

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
		if (m_layout == nullptr) return;
		wgpuBindGroupLayoutRelease(m_layout);
	}
};