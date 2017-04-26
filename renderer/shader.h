#ifndef SHADER_H
#define SHADER_H

#include "../resource/resourcecache.h"
#include "../resource/yamlfile.h"
//#include <GL/glew.h>
#include <GL/GL.h>

#include <map>

class ShaderProgram : public ResourceBase
{
public:
    ShaderProgram();
    ~ShaderProgram();

    virtual void Load(SystemManager *mom, std::string name);

    GLuint getId(){return id_;}
    void bind();
    void disable();

    GLint attribute(std::string att);
    GLint uniform(std::string uni);
protected:
    SystemManager *systemmanager_;
    GLuint id_;

    std::map<std::string, GLint> attributes_;
    std::map<std::string, GLint> uniforms_;

    GLuint compileShader(std::string shaderSource, GLenum shaderType);
    std::string loadShaderFromFile(const std::string filename);

    std::string getShaderInfoLog(int id);
    std::string getProgramInfoLog(int id);
};

#endif
