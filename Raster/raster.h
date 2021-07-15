#pragma once
#include <map>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SDL/SDL.h>


class Rasterizer
{
public:
    Rasterizer() {};
    ~Rasterizer() {};

    int AllocateBuffer(const size_t size);        ////return idx of buffer
    void WriteBuffer(const int idx, void* ptr, const size_t size);
    void SetVertexBuffer(const int idx);
    void SetIndexBuffer(const int idx);
    void SetModelViewProjection(const glm::mat4 model, const glm::mat4 view, const glm::mat4 projection);
    void ConstructViewPortMatrix(const int width, const int height);

    void Draw(size_t idxAmount, SDL_Renderer* sdlPtr);

public:
    int m_IndxBuff;
    int m_VertBuff;

    glm::mat4 m_MatrixModel;
    glm::mat4 m_MatrixView;
    glm::mat4 m_MatrixProjection;
    glm::mat4 m_MatrixViewPortTransform;

    std::vector <std::uint8_t*> m_Buffers;
};