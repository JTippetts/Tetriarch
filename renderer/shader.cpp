#include "shader.h"
#include "../core/logging.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

ShaderProgram::ShaderProgram() :id_(0)
{
}

ShaderProgram::~ShaderProgram()
{
}

std::string ShaderProgram::getShaderInfoLog(int id)
{
    GLint infoLogLength;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
    GLchar *infoLog = new GLchar[infoLogLength + 1];
    glGetShaderInfoLog(id, infoLogLength, NULL, infoLog);
    // Convert the info log to a string
    std::string infoLogString(infoLog);

    // Delete the char array version of the log
    delete[] infoLog;

    // Finally, return the string version of the info log
    return infoLogString;
}

std::string ShaderProgram::getProgramInfoLog(int id)
{
    GLint infoLogLength;
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
    GLchar *infoLog = new GLchar[infoLogLength + 1];
    glGetProgramInfoLog(id, infoLogLength, NULL, infoLog);
    std::string infoLogString(infoLog);

    // Delete the char array version of the log
    delete[] infoLog;

    // Finally, return the string version of the info log
    return infoLogString;
}

void ShaderProgram::Load(SystemManager *mom, std::string name)
{
    // Load definition from YAML File
    ResourceCache *cache=mom->GetSystem<ResourceCache>();
    Logging *log=mom->GetSystem<Logging>();
    systemmanager_=mom;

    std::shared_ptr<YAMLFile> definition=cache->GetResource<YAMLFile>(name);
    YAML::Node &yaml=definition->GetNode();
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

    GLuint vertexshader=compileShader(loadShaderFromFile(vs), GL_VERTEX_SHADER);
    if(vertexshader==0)
    {
        log->Log(LOG_ERROR, "Could not load vertex shader "+vs);
        return;
    }

    GLuint fragmentshader=compileShader(loadShaderFromFile(fs), GL_FRAGMENT_SHADER);
    if(fragmentshader==0)
    {
        log->Log(LOG_ERROR, "Could not load fragment shader "+fs);
        return;
    }

    id_=glCreateProgram();
    glAttachShader(id_, vertexshader);
    glAttachShader(id_, fragmentshader);
    glLinkProgram(id_);
    glDetachShader(id_, vertexshader);
    glDetachShader(id_, fragmentshader);

    GLint linkStatus=GL_FALSE;
    glGetProgramiv(id_, GL_LINK_STATUS, &linkStatus);
    if(linkStatus==GL_FALSE)
    {
        log->Log(LOG_ERROR, "Shader program linking failed.");
        return;
    }
    else log->Log(LOG_INFO, "Shader program linked.");

    // Validate
    glValidateProgram(id_);

    // Check the validation status and throw a runtime_error if program validation failed
    GLint programValidatationStatus;
    glGetProgramiv(id_, GL_VALIDATE_STATUS, &programValidatationStatus);
    if (programValidatationStatus == GL_TRUE)
    {
        log->Log(LOG_INFO, "Shader program validated.");
    }
    else
    {
        //throw std::runtime_error("Shader program validation failed: " + getInfoLog(ObjectType::PROGRAM, programId) );
        log->Log(LOG_ERROR, "Shader program validation failed: " + getProgramInfoLog(id_));
        return;
    }

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

GLuint ShaderProgram::compileShader(std::string shaderSource, GLenum shaderType)
{
    Logging *log=systemmanager_->GetSystem<Logging>();

    std::string shaderTypeString;
    switch (shaderType)
    {
    case GL_VERTEX_SHADER:
        shaderTypeString = "GL_VERTEX_SHADER";
        break;
    case GL_FRAGMENT_SHADER:
        shaderTypeString = "GL_FRAGMENT_SHADER";
        break;
    default:
        //throw std::runtime_error("Bad shader type enum in compileShader.");
        log->Log(LOG_ERROR, "Bad shader type in compileShader.");
        return 0;
        break;
    }

    // Generate a shader id
    // Note: Shader id will be non-zero if successfully created.
    GLuint shaderId = glCreateShader(shaderType);
    if (shaderId == 0)
    {
        // Display the shader log via a runtime_error
        //throw std::runtime_error("Could not create shader of type " + shaderTypeString + ": " + getInfoLog(ObjectType::SHADER, shaderId) );
        log->Log(LOG_ERROR, "Could not create shader of type " + shaderTypeString + ": " + getShaderInfoLog(shaderId));
        return 0;
    }

    // Get the source string as a pointer to an array of characters
    const char *shaderSourceChars = shaderSource.c_str();

    // Attach the GLSL source code to the shader
    // Params: GLuint shader, GLsizei count, const GLchar **string, const GLint *length
    // Note: The pointer to an array of source chars will be null terminated, so we don't need to specify the length and can instead use NULL.
    glShaderSource(shaderId, 1, &shaderSourceChars, NULL);

    // Compile the shader
    glCompileShader(shaderId);

    // Check the compilation status and throw a runtime_error if shader compilation failed
    GLint shaderStatus;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &shaderStatus);
    if (shaderStatus == GL_FALSE)
    {
        //throw std::runtime_error(shaderTypeString + " compilation failed: " + getInfoLog(ObjectType::SHADER, shaderId) );
        log->Log(LOG_ERROR, shaderTypeString + " compilation failed: " + getShaderInfoLog(shaderId));
        return 0;
    }
    else
    {
        log->Log(LOG_INFO, shaderTypeString + " compiled successfully.");
    }

    // If everything went well, return the shader id
    return shaderId;
}

std::string ShaderProgram::loadShaderFromFile(const std::string filename)
{
    Logging *log=systemmanager_->GetSystem<Logging>();

    // Create an input filestream and attempt to open the specified file
    std::ifstream file( filename.c_str() );

    // If we couldn't open the file we'll bail out
    if ( !file.good() )
    {
        //throw std::runtime_error("Failed to open file: " + filename);
        log->Log(LOG_ERROR, "Failed to open file " + filename);
        return "";
    }

    // Otherwise, create a string stream...
    std::stringstream stream;

    // ...and dump the contents of the file into it.
    stream << file.rdbuf();

    // Now that we've read the file we can close it
    file.close();

    // Finally, convert the stringstream into a string and return it
    return stream.str();
}
