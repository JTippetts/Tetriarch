#include "shader.h"
#include "../core/logging.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

VertexShader::VertexShader() : id_(0), systemmanager_(nullptr)
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

FragmentShader::FragmentShader() : id_(0), systemmanager_(nullptr)
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


ShaderProgram::ShaderProgram() :id_(0)
{
}

ShaderProgram::~ShaderProgram()
{
}

void ShaderProgram::Load(SystemManager *mom, std::string name)
{
    // Load definition from YAML File
    ResourceCache *cache=mom->GetSystem<ResourceCache>();
    Logging *log=mom->GetSystem<Logging>();
    systemmanager_=mom;

    definition_=cache->GetResource<YAMLFile>(name);
    YAML::Node &yaml=definition_->GetNode();
    if (yaml.Type() != YAML::NodeType::Map)
    {
        log->Log(LOG_ERROR, "Malformed shader definition (bad definition format): "+name);
        return;
    }

    // For now, just get vertexshader and fragmentshader names and load
    std::string vs=yaml["vertexshader"].as<std::string>();
    std::string fs=yaml["fragmentshader"].as<std::string>();

    if(vs=="")
    {
        log->Log(LOG_ERROR, "Malformed shader definition (missing vertex shader name): "+name);
        return;
    }

    if(fs=="")
    {
        log->Log(LOG_ERROR, "Malformed shader definition (missing fragment shader name): "+name);
        return;
    }

    vertexshader_=cache->GetResource<VertexShader>(vs);
    if(vertexshader_->getId()==0)
    {
        log->Log(LOG_ERROR, "Could not load vertex shader "+vs);
        return;
    }

    fragmentshader_=cache->GetResource<FragmentShader>(fs);
    if(fragmentshader_->getId()==0)
    {
        log->Log(LOG_ERROR, "Could not load fragment shader "+fs);
        return;
    }

    id_=glCreateProgram();
    glAttachShader(id_, vertexshader_->getId());
    glAttachShader(id_, fragmentshader_->getId());
    glLinkProgram(id_);

    GLint linkStatus;
    glGetProgramiv(id_, GL_LINK_STATUS, &linkStatus);
    if(linkStatus==GL_FALSE)
    {
        log->Log(LOG_ERROR, "Shader program linking failed.");
        return;
    }
    else log->Log(LOG_INFO, "Shader program linked.");

    // Setup attributes and uniforms
    const YAML::Node &att=yaml["attributes"];
    if(att.Type()!=YAML::NodeType::Sequence)
    {
        log->Log(LOG_INFO, "No attributes defined.");
    }
    else
    {
        for(std::size_t i=0; i<att.size(); ++i)
        {
            std::string aname=att[i].as<std::string>();
            GLint loc=glGetAttribLocation(id_, aname.c_str());
            if(loc==-1)
            {
                log->Log(LOG_ERROR, "Program does not have attribute "+aname);
            }
            else
            {
                attributes_[aname]=loc;
                log->Log(LOG_INFO, "Added attribute location "+aname);
            }
        }
    }

    const YAML::Node &uni=yaml["uniforms"];
    if(uni.Type()!=YAML::NodeType::Sequence)
    {
        log->Log(LOG_INFO, "No uniforms defined.");
    }
    else
    {
        for(std::size_t i=0; i<uni.size(); ++i)
        {
            std::string uname=uni[i].as<std::string>();
            GLint loc=glGetAttribLocation(id_, uname.c_str());
            if(loc==-1)
            {
                log->Log(LOG_ERROR, "Program does not have uniform "+uname);
            }
            else
            {
                uniforms_[uname]=loc;
                log->Log(LOG_INFO, "Added uniform location "+uname);
            }
        }
    }

}

GLint ShaderProgram::attribute(std::string name)
{
    auto it=attributes_.find(name);
    if(it!=attributes_.end()) return attributes_[name];
    else
    {
        if(systemmanager_) systemmanager_->GetSystem<Logging>()->Log(LOG_ERROR, "Requested undefined attribute "+name);
        return -1;
    }
}

GLint ShaderProgram::uniform(std::string name)
{
    auto it=uniforms_.find(name);
    if(it!=uniforms_.end()) return uniforms_[name];
    else
    {
        if(systemmanager_) systemmanager_->GetSystem<Logging>()->Log(LOG_ERROR, "Requested undefined uniform "+name);
        return -1;
    }
}
