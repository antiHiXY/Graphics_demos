#include "raster.h"
#include "vertex.h"

int Rasterizer::AllocateBuffer(const size_t size)
{
    m_Buffers.push_back(new std::uint8_t[size]);
    return m_Buffers.size() - 1;
}
void Rasterizer::WriteBuffer(const int idx, void* ptr, const size_t size)
{
    memcpy(m_Buffers[idx], ptr, size);
}
void Rasterizer::SetVertexBuffer(const int idx)
{
    m_VertBuff = idx;
}
void Rasterizer::SetIndexBuffer(const int idx)
{
    m_IndxBuff = idx;
}
void Rasterizer::SetModelViewProjection(const glm::mat4 model, const glm::mat4 view, const glm::mat4 projection)
{
    m_MatrixModel = model;
    m_MatrixView = view;
    m_MatrixProjection = projection;
}
void Rasterizer::ConstructViewPortMatrix(const int width, const int height)
{
    m_MatrixViewPortTransform = glm::mat4({width / 2, 0, 0, width / 2}, {0, height / 2, 0, height / 2}, {0, 0, 0.5f, 0.5f}, {0, 0, 0, 1});
}
float TriangleArea(const glm::vec2 v1, const glm::vec2 v2)
{
    return (v1.x * v2.y - v2.x * v1.y) / 2;
}
glm::vec2 XmaxYmaxAmong3Vectors(const glm::vec2 v1, const glm::vec2 v2, const glm::vec2 v3)
{
    return glm::vec2
                    (
                        std::max(v3.x, std::max(v1.x, v2.x)),
                        std::max(v3.y, std::max(v1.y, v2.y))
                    );
}
glm::vec2 XminYminAmong3Vectors(const glm::vec2 v1, const glm::vec2 v2, const glm::vec2 v3)
{
    return glm::vec2
                    (
                        std::min(v3.x, std::min(v1.x, v2.x)),
                        std::min(v3.y, std::min(v1.y, v2.y))
                    );
}
void Rasterizer::Draw (size_t idxAmount, SDL_Renderer* sdlPtr)
{
    Vertex* vertexBuff = reinterpret_cast <Vertex*> (m_Buffers[m_VertBuff]);
    std::uint8_t* indexBuff= m_Buffers[m_IndxBuff];

    //Vertex* v1 = reinterpret_cast <Vertex*> (vertexBuff);

    for (int k = 0; k < idxAmount / 3; ++k)
    {
        const Vertex v1 = *(vertexBuff + (indexBuff[k]));
        const Vertex v2 = *(vertexBuff + (indexBuff[k + 1]));
        const Vertex v3 = *(vertexBuff + (indexBuff[k + 2]));

        glm::vec4 pos1 = m_MatrixProjection * m_MatrixView * m_MatrixModel * glm::vec4(v1.coordinates, 1) + glm::vec4(1, 1, 0, 0);
        glm::vec4 pos2 = m_MatrixProjection * m_MatrixView * m_MatrixModel * glm::vec4(v2.coordinates, 1) + glm::vec4(1, 1, 0, 0);
        glm::vec4 pos3 = m_MatrixProjection * m_MatrixView * m_MatrixModel * glm::vec4(v3.coordinates, 1) + glm::vec4(1, 1, 0, 0);

        pos1 = m_MatrixViewPortTransform * pos1;
        pos2 = m_MatrixViewPortTransform * pos2;
        pos3 = m_MatrixViewPortTransform * pos3;

        const glm::vec3 colorsv1 = v1.colors;
        const glm::vec3 colorsv2 = v2.colors;
        const glm::vec3 colorsv3 = v3.colors;

        glm::vec2 pos1xy = pos1;
        glm::vec2 pos2xy = pos2;
        glm::vec2 pos3xy = pos3;

        const glm::vec2 XminYmin = XminYminAmong3Vectors(pos1xy, pos2xy, pos3xy);
        const glm::vec2 XmaxYmax = XmaxYmaxAmong3Vectors(pos1xy, pos2xy, pos3xy);

        const float fullTriangle = TriangleArea((pos2xy - pos1xy), (pos3xy - pos1xy));

        for (int i = XminYmin.y; i <= XmaxYmax.y; ++i)
        {
            for (int j = XminYmin.x; j <= XmaxYmax.x; ++j)
            {
                const glm::vec2 point(j, i);
                float firstTriangle = TriangleArea((pos1xy - point), (pos2xy - point));
                float secondTriangle = TriangleArea((pos2xy - point), (pos3xy - point));
                float thirdTriangle = TriangleArea((pos3xy - point), (pos1xy - point));

                float alpha1 = firstTriangle / fullTriangle;
                float alpha2 = secondTriangle / fullTriangle;
                float alpha3 = thirdTriangle / fullTriangle;

                if (alpha1 > 0 && alpha2 > 0 && alpha3 > 0)
                {
                    glm::vec3 pointColor = alpha1 * colorsv1 + alpha2 * colorsv2 + alpha3 * colorsv3;
                    
                    SDL_SetRenderDrawColor(sdlPtr, pointColor.x, pointColor.y, pointColor.z, 255);
                    SDL_RenderDrawPoint(sdlPtr, point.x, point.y);
                }
                else
                    continue;
            }
        }
    }
}