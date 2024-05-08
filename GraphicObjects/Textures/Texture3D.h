#pragma once
#include "Texture.h"

class Texture3D : public Texture
{
public:
	Texture3D() {
		SetDefaultDescriptor<3>();
		SetDefaultViewDescriptor<3>();
	};
	
	void Init(uint32_t width, uint32_t height, uint32_t depth);

	inline void Write(void* bytes, const glm::vec<3, uint32_t>& offset, const glm::vec<3, uint32_t>& size, size_t pixelSize, uint32_t miplevel = 0);
};



#pragma region Definitions

inline void Texture3D::Init(uint32_t width, uint32_t height, uint32_t depth)
{
	i_Init(width, height, depth);
	CreateView();
}

inline void Texture3D::Write(void* bytes, const glm::vec<3, uint32_t>& offset, const glm::vec<3, uint32_t>& size, size_t pixelSize, uint32_t miplevel)
{
	i_Write(bytes, offset, size, pixelSize, miplevel);
}

#pragma endregion