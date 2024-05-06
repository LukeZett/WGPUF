#pragma once
#include "webgpu/webgpu.h"
#include <filesystem>
#include "Texture.h"



class Texture2D : public Texture
{
public:
	Texture2D() {};

	void Initialize(uint32_t width, uint32_t height);

	void Initialize(const std::filesystem::path& path);
};


