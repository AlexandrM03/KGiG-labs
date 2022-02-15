#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct character {
    unsigned int TextureID; // ID текстуры глифа
    glm::ivec2 Size; // размер глифа
    glm::ivec2 Bearing; // смещение от линии шрифта до верхнего/левого угла глифа
    unsigned int Advance; // смещение до следующего глифа
};