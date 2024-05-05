#include "Texture2D.h"
#include "Texture2D.h"
#include "Texture2D.h"
#include "Textures/Texture2D.h"
#include <vector>
#include "Device.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture2D::Texture2D(uint32_t width, uint32_t height)
{
    SetDescriptor(width, height);

	m_desc.format = WGPUTextureFormat_RGBA8Unorm;
	m_desc.usage = WGPUTextureUsage_TextureBinding | WGPUTextureUsage_CopyDst;

    m_texture = wgpuDeviceCreateTexture(Device::Get(), &m_desc);

    std::vector<uint8_t> pixels(4 * m_desc.size.width * m_desc.size.height);
    for (uint32_t i = 0; i < m_desc.size.width; i += 2) {
        for (uint32_t j = 0; j < m_desc.size.height; j+=2) {
            uint8_t* p = &pixels[4 * (j * m_desc.size.width + i)];
            p[0] = (uint8_t)i; // r
            p[1] = (uint8_t)j; // g
            p[2] = 128; // b
            p[3] = 255; // a
        }
    }

    WGPUImageCopyTexture destination = {};
    destination.texture = m_texture;
    destination.mipLevel = 0;
    destination.origin = { 0, 0, 0 }; // equivalent of the offset argument of Queue::writeBuffer
    destination.aspect = WGPUTextureAspect_All; // only relevant for depth/Stencil textures

    WGPUTextureDataLayout source = {};

    source.offset = 0;
    source.bytesPerRow = 4 * m_desc.size.width;
    source.rowsPerImage = m_desc.size.height;

    wgpuQueueWriteTexture(Device::GetQueue(), &destination, pixels.data(), pixels.size(), &source, &m_desc.size);

    CreateView();
    
}

Texture2D::Texture2D(const std::filesystem::path& path)
{
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    char* data = (char*)stbi_load(((path).generic_string()).data(), &width, &height, &channels, 4);

    SetDescriptor(width, height);

    m_desc.format = WGPUTextureFormat_RGBA8Unorm;
    m_desc.usage = WGPUTextureUsage_TextureBinding | WGPUTextureUsage_CopyDst;

    m_texture = wgpuDeviceCreateTexture(Device::Get(), &m_desc);

    WGPUImageCopyTexture destination = {};
    destination.texture = m_texture;
    destination.mipLevel = 0;
    destination.origin = { 0, 0, 0 }; // equivalent of the offset argument of Queue::writeBuffer
    destination.aspect = WGPUTextureAspect_All; // only relevant for depth/Stencil textures

    WGPUTextureDataLayout source = {};

    source.offset = 0;
    source.bytesPerRow = 4 * m_desc.size.width;
    source.rowsPerImage = m_desc.size.height;

    wgpuQueueWriteTexture(Device::GetQueue(), &destination, data, width * height * channels, &source, &m_desc.size);

    CreateView();
    stbi_image_free(data);
}

Texture2D::~Texture2D()
{
    wgpuTextureDestroy(m_texture);
    wgpuTextureRelease(m_texture);
    if (m_view) wgpuTextureViewRelease(m_view);
}

void Texture2D::SetDescriptor(uint32_t width, uint32_t height)
{
    m_desc.nextInChain = nullptr;
    m_desc.dimension = WGPUTextureDimension_2D;
    m_desc.size = { width, height, 1 };

    m_desc.mipLevelCount = 1;
    m_desc.sampleCount = 1;

    m_desc.viewFormatCount = 0;
    m_desc.viewFormats = nullptr;
}

void Texture2D::CreateView()
{
    if (m_view) wgpuTextureViewRelease(m_view);

    WGPUTextureViewDescriptor textureViewDesc = {};
    textureViewDesc.aspect = WGPUTextureAspect_All;
    textureViewDesc.baseArrayLayer = 0;
    textureViewDesc.arrayLayerCount = 1;
    textureViewDesc.baseMipLevel = 0;
    textureViewDesc.mipLevelCount = 1;
    textureViewDesc.dimension = WGPUTextureViewDimension_2D;
    textureViewDesc.format = m_desc.format;
    m_view = wgpuTextureCreateView(m_texture, &textureViewDesc);
}
