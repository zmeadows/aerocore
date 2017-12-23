#include "GraphicsContext.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>

GraphicsContext::GraphicsContext(void)
    : m_GLcontext(NULL), pgs(), m_screenWidth(800), m_screenWidthFloat(800.0) {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL context!" << std::endl;
        std::cerr << SDL_GetError() << std::endl;
        throw;
    }

    // Tell SDL what version of OpenGL to use/expect
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow("aerocore", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_screenWidth,
                              m_screenWidth, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS | SDL_WINDOW_OPENGL);

    if (!window) {
        std::cerr << "Failed to initialize SDL Window!" << std::endl;
        std::cerr << SDL_GetError() << std::endl;
        throw;
    }

    m_GLcontext = SDL_GL_CreateContext(window);
    if (!m_GLcontext) {
        std::cerr << "Failed to initialize OpenGL context!" << std::endl;
        std::cerr << SDL_GetError() << std::endl;
        throw;
    }

    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();

    if (glewError != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW!" << std::endl;
        std::cerr << glewGetErrorString(glewError) << std::endl;
        throw;
    }

    if (SDL_GL_SetSwapInterval(1) < 0) {
        std::cerr << "Failed to enable VSync!" << std::endl;
        std::cerr << SDL_GetError() << std::endl;
        throw;
    }

    if (!this->initOpenGL()) {
        std::cerr << "Failed to initialize OpenGL!" << std::endl;
        throw;
    }
}

bool GraphicsContext::initOpenGL(void) {
    glGenVertexArrays(1, &m_vertexArrayID);
    glBindVertexArray(m_vertexArrayID);

    // Load basic shader program
    if (!pgs.loadProgram()) {
        printf("Unable to load basic shader!\n");
        return false;
    }

    // Bind basic shader program
    pgs.bind();

    // Check for error
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        printf("Error initializing OpenGL!");
        return false;
    }

    return true;
}

GraphicsContext::~GraphicsContext(void) {
    DEBUG("Destroying GraphicsContext...");
    SDL_DestroyWindow(window);
    SDL_Quit();
}

Sint16 GraphicsContext::toScreenSpan(const float gridSpan) const {
    return static_cast<int>(nearbyint(m_screenWidthFloat * gridSpan / 200.0));
}

ScreenCoordinates GraphicsContext::toScreenCoordinates(const Position& pos) const {
    Sint16 x = static_cast<int>(nearbyint(m_screenWidthFloat * pos.x / 200.0 + m_screenWidthFloat / 2.0));

    Sint16 y = static_cast<int>(nearbyint(m_screenWidthFloat * pos.y / (-200.0) + m_screenWidthFloat / 2.0));

    return {x, y};
}