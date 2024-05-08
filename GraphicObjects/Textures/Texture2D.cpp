#include "Texture2D.h"
#include "Texture2D.h"
#include "Texture2D.h"
#include "Textures/Texture2D.h"
#include <vector>
#include "Device.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void Texture2D::Init(uint32_t width, uint32_t height)
{
    i_Init(width, height, 1);

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

    i_Write(&pixels[0], { 0,0,0 }, { width, height, 1 }, 4, 0);
    CreateView();
}

void Texture2D::Init(const std::filesystem::path& path)
{
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    char* data = (char*)stbi_load(((path).generic_string()).data(), &width, &height, &channels, 4);

    channels = 4;
    SetDefaultDescriptor<2>();
    SetDefaultViewDescriptor<2>();

    i_Init(width, height, 1);

    i_Write(data, { 0,0,0 }, { width, height, 1 }, channels);

    CreateView();
    stbi_image_free(data);
}

