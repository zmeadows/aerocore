#pragma once

#include <iostream>

#include <GL/glew.h>

class PolygonShader {
    GLuint m_programID;

public:
    PolygonShader();
    ~PolygonShader();

    bool loadProgram(void);

    bool bind(void);
    void unbind(void);

    GLuint getProgramID(void);
};
