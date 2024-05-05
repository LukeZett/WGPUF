#pragma once
#include "webgpu/webgpu.h"
#include <filesystem>
#include "Uncopyable.h"



class Texture2D : Uncopyable
{
public:
	Texture2D();
	Texture2D(uint32_t width, uint32_t height);
	Texture2D(const std::filesystem::path& path);
	~Texture2D();


	WGPUTextureView GetView() const {
		return m_view;
	}

private:
	void SetDescriptor(uint32_t width, uint32_t height);
	void CreateView();

	WGPUTextureDescriptor m_desc = {};
	WGPUTexture m_texture = nullptr;
	WGPUTextureView m_view = nullptr;
};


