#pragma once
#include "webgpu/webgpu.h"
#include "Device.h"
#include "Logging.h"
#include "Uncopyable.h"
#include "glm/glm.hpp"


/**
* @brief Base class for texture object. 
* Call first Init to (re)initialize texture object on GPU;
* Call CreateView to (re)create texture view;
* Resources are automatically deallocated when object is destructed
*/
class Texture : Uncopyable
{
public:
	inline Texture();

	~Texture() { Delete(); }

	WGPUTextureView GetView() const { return m_view; }

	WGPUTexture GetTexture() const { return m_texture; }

	WGPUTextureDescriptor GetDescriptor() const { return m_desc; }

	WGPUTextureViewDescriptor GetViewDesc() const { return m_viewDesc; }

	inline void Delete();

	inline WGPUTextureView CreateView();

protected:
	inline void i_Write(void* bytes, const glm::vec<3, uint32_t>& offset, const glm::vec<3, uint32_t>& size, size_t pixelSize, uint32_t miplevel = 0);

	inline void i_Init(uint32_t width, uint32_t height, uint32_t depth);

	template<uint8_t dimension>
	void SetDefaultDescriptor() {
		LOG_ERROR("Invalid texture dimension");
	}

	template<>
	void SetDefaultDescriptor<1>() {
		i_SetDefaultDescriptor<WGPUTextureDimension_1D>();
	}

	template<>
	void SetDefaultDescriptor<2>() {
		i_SetDefaultDescriptor<WGPUTextureDimension_2D>();
	}

	template<>
	void SetDefaultDescriptor<3>() {
		i_SetDefaultDescriptor<WGPUTextureDimension_3D>();
	}

	template<uint8_t dimension>
	void SetDefaultViewDescriptor() {
		LOG_ERROR("Invalid texture dimension");
	}

	template<>
	void SetDefaultViewDescriptor<1>() {
		i_SetDefaultViewDescriptor<WGPUTextureViewDimension_1D>();
	}

	template<>
	void SetDefaultViewDescriptor<2>() {
		i_SetDefaultViewDescriptor<WGPUTextureViewDimension_2D>();
	}

	template<>
	void SetDefaultViewDescriptor<3>() {
		i_SetDefaultViewDescriptor<WGPUTextureViewDimension_3D>();
	}

	WGPUTexture m_texture = nullptr;
	WGPUTextureDescriptor m_desc = {};
	
	WGPUTextureView m_view = nullptr;
	WGPUTextureViewDescriptor m_viewDesc = {};

private:
	template<WGPUTextureDimension dim>
	void i_SetDefaultDescriptor();

	template<WGPUTextureViewDimension dim>
	void i_SetDefaultViewDescriptor();
};

#pragma region function definitions

Texture::Texture()
{
	m_desc.nextInChain = nullptr;
	m_viewDesc.nextInChain = nullptr;
};

void Texture::i_Init(uint32_t width, uint32_t height, uint32_t depth) {
	m_desc.size = { width, height, depth };
	if (m_texture) Delete();
	m_texture = wgpuDeviceCreateTexture(Device::Get(), &m_desc);
}

void Texture::Delete() {
	if (m_texture) {
		wgpuTextureDestroy(m_texture);
		wgpuTextureRelease(m_texture);
	}
	if (m_view) wgpuTextureViewRelease(m_view);
}

WGPUTextureView Texture::CreateView() {
	if (m_view) wgpuTextureViewRelease(m_view);
	m_view = wgpuTextureCreateView(m_texture, &m_viewDesc);
	return m_view;
}

inline void Texture::i_Write(void* bytes, const glm::vec<3, uint32_t>& offset, const glm::vec<3, uint32_t>& size, size_t pixelSize, uint32_t miplevel)
{
	WGPUImageCopyTexture destination = {};
	destination.texture = m_texture;
	destination.mipLevel = miplevel;
	destination.origin = { offset.x, offset.y, offset.z };
	destination.aspect = WGPUTextureAspect_All;

	WGPUTextureDataLayout source = {};

	source.offset = 0;
	source.bytesPerRow = pixelSize * size.x;
	source.rowsPerImage = size.y;

	wgpuQueueWriteTexture(Device::GetQueue(), &destination, bytes, size.x * size.y * size.z * pixelSize, &source, &m_desc.size);
}

template<WGPUTextureDimension dim>
void Texture::i_SetDefaultDescriptor()
{
	m_desc.dimension = dim;
	m_desc.mipLevelCount = 1;
	m_desc.sampleCount = 1;
	m_desc.viewFormatCount = 0;
	m_desc.viewFormats = nullptr;
}

template<WGPUTextureViewDimension dim>
inline void Texture::i_SetDefaultViewDescriptor()
{
	m_viewDesc.dimension = dim;
	m_viewDesc.aspect = WGPUTextureAspect_All;
	m_viewDesc.baseArrayLayer = 0;
	m_viewDesc.arrayLayerCount = 1;
	m_viewDesc.baseMipLevel = 0;
	m_viewDesc.mipLevelCount = 1;
	m_viewDesc.format = m_desc.format;
}

#pragma endregion