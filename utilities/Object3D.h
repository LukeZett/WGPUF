#pragma once
#include "Buffer.h"
#include <filesystem>
#include "glm/glm.hpp"

struct VertexData
{
	glm::vec3 pos;
	glm::vec3 normal = { 0,0,0 };
	glm::vec2 uv = { 0,0 };

	VertexData(const glm::vec3& position) : pos(position) {}
};

class Object3D
{
public:
	Object3D(std::filesystem::path objFile);

private:
	Buffer m_vertexBuffer;
	Buffer m_indexBuffer;

	std::vector<uint16_t> m_indices;
	std::vector<VertexData> m_vertices;

};