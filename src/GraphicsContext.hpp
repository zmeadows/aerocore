#pragma once

#include <utility>

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "Base.hpp"
#include "PolygonShader.hpp"

class GraphicsContext {

    SDL_GLContext m_GLcontext;
    PolygonShader pgs;

    // GLuint m_vertexArrayID;
    // GLfloat m_triangleVBO[];

    bool initSDL(void);
    bool initOpenGL(void);

public:
    GraphicsContext(void);
    ~GraphicsContext(void);

    const int m_screenWidth;
    const float m_screenWidthFloat;
    SDL_Window* window;

    Sint16 toScreenSpan(const float gridSpan) const;
    ScreenCoordinates toScreenCoordinates(const Position& pos) const;
};
