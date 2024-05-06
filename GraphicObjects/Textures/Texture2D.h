#pragma once
#include "webgpu/webgpu.h"
#include <filesystem>
#include "Texture.h"


class Texture2D : public Texture
{
public:
	Texture2D() {};

	void Init(uint32_t width, uint32_t height);

	void Init(const std::filesystem::path & path);

	inline void Write(void* bytes, const glm::vec<2, uint32_t>& offset, const glm::vec<2, uint32_t>& size, size_t pixelSize, uint32_t miplevel = 0);
};

#pragma region Definitions

inline void Texture2D::Write(void* bytes, const glm::vec<2, uint32_t>& offset, const glm::vec<2, uint32_t>& size, size_t pixelSize, uint32_t miplevel)
{
	i_Write(bytes, { offset, 1 }, { size, 1 }, pixelSize, miplevel);
}

#pragma endregion

