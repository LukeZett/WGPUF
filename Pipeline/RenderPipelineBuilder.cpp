#include "RenderPipelineBuilder.h"
#include "RenderPipelineBuilder.h"
#include "RenderPipelineBuilder.h"

void RenderPipelineBuilder::AddBufferLayout(BufferLayout& bufferLayout)
{
	m_descriptor.vertex.bufferCount++;
	m_vbLayouts.push_back({});
	m_vertexAttribs.push_back(nullptr);
	bufferLayout.getVertexBufferLayout(m_vbLayouts.back(), m_vertexAttribs.back());
	m_descriptor.vertex.buffers = &m_vbLayouts[0];
}

void RenderPipelineBuilder::SetShader(const std::filesystem::path& path)
{
	customShader = true;
	m_shaderSource = path;
}

RenderPipelineBuilder::RenderPipelineBuilder(WGPUTextureFormat targetFormat)
{
	m_descriptor.nextInChain = nullptr;
	
	m_descriptor.vertex.bufferCount = 0;
	m_descriptor.vertex.buffers = nullptr;
	
	m_descriptor.vertex.entryPoint = "vs_main";
	m_descriptor.vertex.constantCount = 0;
	m_descriptor.vertex.constants = nullptr;
	
	m_descriptor.primitive.topology = WGPUPrimitiveTopology_TriangleList;
	m_descriptor.primitive.stripIndexFormat = WGPUIndexFormat_Undefined;
	m_descriptor.primitive.frontFace = WGPUFrontFace_CCW;
	m_descriptor.primitive.cullMode = WGPUCullMode_None;
	
	m_descriptor.depthStencil = nullptr;
	
	m_descriptor.multisample.count = 1;
	m_descriptor.multisample.mask = ~0u;
	m_descriptor.multisample.alphaToCoverageEnabled = false;

	m_fragmentState.entryPoint = "fs_main";
	m_fragmentState.nextInChain = nullptr;
	m_fragmentState.constantCount = 0;
	m_fragmentState.constants = nullptr;
	
	m_blendState.color.srcFactor = WGPUBlendFactor_SrcAlpha;
	m_blendState.color.dstFactor = WGPUBlendFactor_OneMinusSrcAlpha;
	m_blendState.color.operation = WGPUBlendOperation_Add;
	
	m_blendState.alpha.srcFactor = WGPUBlendFactor_Zero;
	m_blendState.alpha.dstFactor = WGPUBlendFactor_One;
	m_blendState.alpha.operation = WGPUBlendOperation_Add;
	
	
	m_colorTargetState.nextInChain = nullptr;
	m_colorTargetState.format = targetFormat;
	m_colorTargetState.writeMask = WGPUColorWriteMask_All;
	
	m_fragmentState.targetCount = 1;
	
	m_descriptor.layout = nullptr;
	
	SetDescPtrs();
}

RenderPipeline RenderPipelineBuilder::Build() {
	RenderPipeline pipeline = customShader ? RenderPipeline(m_shaderSource) : RenderPipeline();

	if (m_bindingEntries.size() > 0) {
		WGPUBindGroupLayoutDescriptor bindGroupLayoutDesc{};
		bindGroupLayoutDesc.nextInChain = nullptr;
		bindGroupLayoutDesc.entryCount = m_bindingEntries.size();
		bindGroupLayoutDesc.entries = &m_bindingEntries[0];
		pipeline.m_bindEntryCount = m_bindingEntries.size();
		pipeline.m_layout = wgpuDeviceCreateBindGroupLayout(Device::Get(), &bindGroupLayoutDesc);

		WGPUPipelineLayoutDescriptor layoutDesc{};
		layoutDesc.nextInChain = nullptr;
		layoutDesc.bindGroupLayoutCount = 1;
		layoutDesc.bindGroupLayouts = &pipeline.m_layout;
		WGPUPipelineLayout layout = wgpuDeviceCreatePipelineLayout(Device::Get(), &layoutDesc);
		m_descriptor.layout = layout;
	}

	m_fragmentState.module = pipeline.m_shadermodule.Get();
	m_descriptor.vertex.module = pipeline.m_shadermodule.Get();
	pipeline.Init(m_descriptor);
	return pipeline;
}



void RenderPipelineBuilder::SetDefaultBindingLayout(WGPUBindGroupLayoutEntry& bindingLayout) {
	bindingLayout.buffer.nextInChain = nullptr;
	bindingLayout.buffer.type = WGPUBufferBindingType_Undefined;
	bindingLayout.buffer.hasDynamicOffset = false;

	bindingLayout.sampler.nextInChain = nullptr;
	bindingLayout.sampler.type = WGPUSamplerBindingType_Undefined;

	bindingLayout.storageTexture.nextInChain = nullptr;
	bindingLayout.storageTexture.access = WGPUStorageTextureAccess_Undefined;
	bindingLayout.storageTexture.format = WGPUTextureFormat_Undefined;
	bindingLayout.storageTexture.viewDimension = WGPUTextureViewDimension_Undefined;

	bindingLayout.texture.nextInChain = nullptr;
	bindingLayout.texture.multisampled = false;
	bindingLayout.texture.sampleType = WGPUTextureSampleType_Undefined;
	bindingLayout.texture.viewDimension = WGPUTextureViewDimension_Undefined;
}
