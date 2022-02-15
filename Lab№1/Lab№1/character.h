#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct character {
    unsigned int TextureID; // ID �������� �����
    glm::ivec2 Size; // ������ �����
    glm::ivec2 Bearing; // �������� �� ����� ������ �� ��������/������ ���� �����
    unsigned int Advance; // �������� �� ���������� �����
};