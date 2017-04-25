#include "shader.h"
#include "../core/logging.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

VertexShader::VertexShader() : systemmanager_(nullptr)
{
    id_=glCreateShader(GL_VERTEX_SHADER);
}

VertexShader::~VertexShader()
{
    glDeleteShader(id_);
}

void VertexShader::Load(SystemManager* mom, std::string name)
{
    systemmanager_=mom;
    Logging *log=mom->GetSystem<Logging>();
    std::ifstream file;

    file.open( name.c_str() );

    if (!file.good() )
    {
        log->Log(LOG_ERROR, "Failed to open vertex shader"+name);
        return;
    }

    std::stringstream stream;
    stream << file.rdbuf();
    file.close();
    source_ = stream.str();
    const char *sourceChars = source_.c_str();
    glShaderSource(id_, 1, &sourceChars, NULL);
    compile();
}

void VertexShader::compile()
{
    Logging *log=systemmanager_->GetSystem<Logging>();

    glCompileShader(id_);
    GLint shaderStatus;
    glGetShaderiv(id_, GL_COMPILE_STATUS, &shaderStatus);
    if (shaderStatus == GL_FALSE)
    {
        GLint infoLogLength;
        glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(id_, infoLogLength, NULL, strInfoLog);
        log->Log(LOG_ERROR, "Shader compilation failed: %s", strInfoLog);
        delete[] strInfoLog;
    }
    else
    {
        log->Log(LOG_INFO, "Shader compiled successfully.");
    }
}

FragmentShader::FragmentShader() : systemmanager_(nullptr)
{
    id_=glCreateShader(GL_FRAGMENT_SHADER);
}

FragmentShader::~FragmentShader()
{
    glDeleteShader(id_);
}

void FragmentShader::Load(SystemManager* mom, std::string name)
{
    systemmanager_=mom;
    Logging *log=mom->GetSystem<Logging>();
    std::ifstream file;

    file.open( name.c_str() );

    if (!file.good() )
    {
        log->Log(LOG_ERROR, "Failed to open vertex shader"+name);
        return;
    }

    std::stringstream stream;
    stream << file.rdbuf();
    file.close();
    source_ = stream.str();
    const char *sourceChars = source_.c_str();
    glShaderSource(id_, 1, &sourceChars, NULL);
    compile();
}

void FragmentShader::compile()
{
    Logging *log=systemmanager_->GetSystem<Logging>();

    glCompileShader(id_);
    GLint shaderStatus;
    glGetShaderiv(id_, GL_COMPILE_STATUS, &shaderStatus);
    if (shaderStatus == GL_FALSE)
    {
        GLint infoLogLength;
        glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(id_, infoLogLength, NULL, strInfoLog);
        log->Log(LOG_ERROR, "Shader compilation failed: %s", strInfoLog);
        delete[] strInfoLog;
    }
    else
    {
        log->Log(LOG_INFO, "Shader compiled successfully.");
    }
}
