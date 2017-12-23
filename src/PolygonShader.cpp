#include "PolygonShader.hpp"

PolygonShader::PolygonShader(void) : m_programID(0) {}

PolygonShader::~PolygonShader(void) {
    glDeleteProgram(m_programID);
}

bool PolygonShader::bind(void) {
    glUseProgram(m_programID);

    // Check for error
    const GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        printf("Error binding shader! %s\n", gluErrorString(error));
        return false;
    }

    return true;
}

GLuint PolygonShader::getProgramID() {
    return m_programID;
}

void PolygonShader::unbind(void) {
    gUseProgram(NULL);
}

bool PolygonShader::loadProgram() {
    // Success flag
    GLint programSuccess = GL_TRUE;

    // Generate program
    m_programID = glCreateProgram();

    // Create vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Get vertex source
    const GLchar* vertexShaderSource[] = {
        "#version 140\nin vec2 LVertexPos2D; void main() { gl_Position = vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 ); }"};

    // Set vertex source
    glShaderSource(vertexShader, 1, vertexShaderSource, NULL);

    // Compile vertex source
    glCompileShader(vertexShader);

    // Check vertex shader for errors
    GLint vShaderCompiled = GL_FALSE;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
    if (vShaderCompiled != GL_TRUE) {
        printf("Unable to compile vertex shader %d!\n", vertexShader);
        return false;
    }

    // Attach vertex shader to program
    glAttachShader(m_programID, vertexShader);

    // Create fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Get fragment source
    const GLchar* fragmentShaderSource[] = {
        "#version 140\nout vec4 LFragment; void main() { LFragment = vec4( 1.0, 1.0, 1.0, 1.0 ); }"};

    // Set fragment source
    glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);

    // Compile fragment source
    glCompileShader(fragmentShader);

    // Check fragment shader for errors
    GLint fShaderCompiled = GL_FALSE;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
    if (fShaderCompiled != GL_TRUE) {
        printf("Unable to compile fragment shader %d!\n", fragmentShader);
        return false;
    }

    // Attach fragment shader to program
    glAttachShader(m_programID, fragmentShader);

    // Link program
    glLinkProgram(m_programID);

    // Check for errors
    glGetProgramiv(m_programID, GL_LINK_STATUS, &programSuccess);
    if (programSuccess != GL_TRUE) {
        printf("Error linking program %d!\n", m_programID);
        return false;
    }

    return true;
}