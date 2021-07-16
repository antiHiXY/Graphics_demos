#include <SDL/SDL.h>
#undef main
#include <iostream>

#include "raster.h"
#include "vertex.h"

bool ShouldStop = false;

Vertex vertex1;
Vertex vertex2;
Vertex vertex3;

void ProcessInput()
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        switch (e.type)
        {
            case SDL_QUIT: // "Esc" for close program
            ShouldStop = true;
            break;

            case SDL_KEYDOWN:
            switch (e.key.keysym.sym)
            {
                case SDLK_SPACE:
                std::printf("jmyaknul space\n");
                break;
            }
            break;
        }
    }
}

int main()
{
    const size_t width = 800;
    const size_t height = 600;

    SDL_Window* pWindow = nullptr;
    SDL_Renderer* pRender = nullptr;

    SDL_CreateWindowAndRenderer(width, height, 0, &pWindow, &pRender);

    Rasterizer raster;

    vertex1.coordinates = glm::vec3(0.5, 0.3, 0.5);
    vertex2.coordinates = glm::vec3(-0.5, -0.3, 0.5);
    vertex3.coordinates = glm::vec3(0.5, -0.3, 0.5);

    vertex1.colors = glm::vec3(255, 0, 0);
    vertex2.colors = glm::vec3(0, 255, 0);
    vertex3.colors = glm::vec3(0, 0, 255);
    
    std::vector <Vertex> vertices;
    vertices.push_back(vertex1);
    vertices.push_back(vertex2);
    vertices.push_back(vertex3);

    uint8_t indices[] = {0, 1, 2};

    int vertBuff = raster.AllocateBuffer(1024);
    int indxBuff = raster.AllocateBuffer(1024);
    raster.WriteBuffer(vertBuff, vertices.data(), sizeof(Vertex) * vertices.size());
    raster.WriteBuffer(indxBuff, indices, sizeof(indices));
    raster.SetVertexBuffer(vertBuff);
    raster.SetIndexBuffer(indxBuff);
    raster.SetModelViewProjection(glm::mat4(1), glm::mat4(1), glm::mat4(1));
    raster.ConstructViewPortMatrix(width, height);

    while (!ShouldStop)
    {
        ProcessInput();

        SDL_SetRenderDrawColor(pRender, 0, 0, 0, 255);
        SDL_RenderClear(pRender);

        int x = 10, y = 20;
        raster.Draw(3, pRender);
        // SDL_RenderDrawPoint(pRender, x, y);

        SDL_RenderPresent(pRender);
    }
}