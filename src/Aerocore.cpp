#include "Aerocore.hpp"

#include <array>

#include "ComponentManager.hpp"
#include "Generator.hpp"
#include "GraphicsContext.hpp"
#include "InputManager.hpp"
#include "SystemManager.hpp"

#include <SDL2/SDL.h>

Aerocore::Aerocore(void)
    : GC(std::make_unique<GraphicsContext>()), CM(std::make_unique<ComponentManager>())
// SM(std::make_unique<SystemManager>()),
// IM(std::make_unique<InputManager>(CM.get())) {
{
    // SM->addSystem(new CleanupSystem(CM.get()));
    // SM->addSystem(new TranslationSystem(CM.get()));
    // SM->addSystem(new RotationSystem(CM.get()));
    // SM->addSystem(new CollisionSystem(CM.get()));
    // SM->addSystem(new ParticleSystem(CM.get()));
    // SM->addSystem(new DrawSystem(CM.get(), GC.get()));
    // generate<EntityType::Player>(CM.get());
    // generate<EntityType::Enemy>(CM.get());
}

bool Aerocore::tick(void) {
    bool quitting = processInput();

    static Uint64 t0 = SDL_GetPerformanceCounter();
    static Uint64 t1 = 0;

    // Clear color buffer
    glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Reset transformations
    glLoadIdentity();

    // VBO data
    // GLfloat vertexData[] = {-0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f};

    // GLuint vertexbuffer;
    // glGenBuffers(1, &vertexbuffer);
    // glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    // t1 = SDL_GetPerformanceCounter();
    // const float tmpdt = (float)(t1 - t0) / SDL_GetPerformanceFrequency();
    // SM->runSystems(tmpdt);
    // t0 = SDL_GetPerformanceCounter();

    SDL_GL_SwapWindow(GC->window);

    // if (!quitting) {
    //     quitting = !CM->has<Position>(UUID::playerUUID);
    // }

    return quitting;
}

bool Aerocore::processInput(void) {
    SDL_Event e;

    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            return true;
        } else if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
            // IM->processInput(e.key.keysym.sym, false);
            return false;
        } else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
            // IM->processInput(e.key.keysym.sym, true);
            return false;
        }
    }

    return false;
}