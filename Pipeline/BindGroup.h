#pragma once
#include "webgpu/webgpu.h"
#include "Device.h"
#include <vector>
#include "Buffer.h"
#include "Textures/Texture.h"
#include <unordered_map>

class BindGroup
{
public:
	BindGroup();
	~BindGroup();

	WGPUBindGroup& Init(WGPUBindGroupLayout layout);

	WGPUBindGroup& Reload();

	void AddUniform(uint32_t binding, const Buffer& uniformBuffer);

	void AddTexture(uint32_t binding, const Texture& texture);

private:
	void Delete();

	WGPUBindGroupEntry* GetBindingsIndex(uint32_t binding);

	WGPUBindGroupDescriptor m_desc = {};
	WGPUBindGroup m_bindGroup = nullptr;
	std::vector<WGPUBindGroupEntry> m_bgEntries;
	std::unordered_map<uint16_t, uint16_t> m_bindingToIndex;
};






inline BindGroup::BindGroup()
{
	m_desc.nextInChain = nullptr;
}

inline BindGroup::~BindGroup()
{
	Delete();
}

inline WGPUBindGroup& BindGroup::Init(WGPUBindGroupLayout layout)
{
	m_desc.layout = layout;
	return Reload();
}

inline WGPUBindGroup& BindGroup::Reload()
{
	Delete();

	m_desc.entries = &m_bgEntries[0];
	m_desc.entryCount = m_bgEntries.size();
	m_bindGroup = wgpuDeviceCreateBindGroup(Device::Get(), &m_desc);
	return m_bindGroup;
}

inline void BindGroup::AddUniform(uint32_t binding, const Buffer& uniformBuffer)
{
	auto entry = GetBindingsIndex(binding);
	entry->nextInChain = nullptr;
	entry->binding = binding;
	entry->buffer = uniformBuffer.Get();
	entry->offset = 0;
	entry->size = uniformBuffer.Size();

}

inline void BindGroup::AddTexture(uint32_t binding, const Texture& texture)
{
	auto entry = GetBindingsIndex(binding);
	entry->nextInChain = nullptr;
	entry->binding = binding;
	entry->textureView = texture.GetView();
}

inline void BindGroup::Delete()
{
	if (m_bindGroup != nullptr) wgpuBindGroupRelease(m_bindGroup);
}

inline WGPUBindGroupEntry* BindGroup::GetBindingsIndex(uint32_t binding)
{
	if (m_bindingToIndex.contains(binding))
	{
		return &m_bgEntries[m_bindingToIndex[binding]];
	}
	m_bgEntries.push_back({});
	m_bindingToIndex[binding] = m_bgEntries.size() - 1;
	return &m_bgEntries[m_bindingToIndex[binding]];
}
