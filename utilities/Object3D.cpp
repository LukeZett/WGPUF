#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "Object3D.h"
#include "Logging.h"
#include "glm/glm.hpp"
#include <unordered_set>


Object3D::Object3D(std::filesystem::path objFile) : m_indexBuffer(0, Index), m_vertexBuffer(0, Vertex)
{
    std::string inputfile = objFile.generic_string();
    tinyobj::ObjReaderConfig reader_config;
    tinyobj::ObjReader reader;


    if (!reader.ParseFromFile(inputfile, reader_config)) {
        if (!reader.Error().empty()) {
            LOG_ERROR("TinyObjReader:\n");
            LOG_ERROR(reader.Error());
        }
        return;
    }

    if (!reader.Warning().empty()) {
        LOG_WARN("TinyObjReader: ");
        LOG_WARN(reader.Warning());
    }
    uint16_t i = 0;

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    for (size_t s = 0; s < shapes.size(); s++) {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {

                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

                VertexData vertex(glm::vec3(vx, vy, vz));

                // Check if `normal_index` is zero or positive. negative = no normal data
                if (idx.normal_index >= 0) {
                    tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
                    tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
                    tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
                    vertex.normal = { nx, ny, nz };
                }

                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if (idx.texcoord_index >= 0) {
                    tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                    tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                    vertex.uv = { tx, ty };
                }

                m_vertices.push_back(vertex);
                m_indices.push_back(i);
                i++;

            }
        }
    }

    m_indexBuffer.Allocate(sizeof(uint16_t) * m_indices.size());
    m_indexBuffer.Upload(m_indices);
    m_vertexBuffer.Allocate(sizeof(Vertex) * m_vertices.size());
    m_vertexBuffer.Upload(m_vertices);
}
