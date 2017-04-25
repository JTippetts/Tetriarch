#ifndef SHADER_H
#define SHADER_H

#include "../resource/resourcecache.h"
#include "../resource/yamlfile.h"
#include <GL/glew.h>
#include <GL/GL.h>

#include <map>

class VertexShader : public ResourceBase
{
	protected:
		GLuint id_;         // The unique ID / handle for the shader
		std::string source_;     // The shader source code (i.e. the GLSL code itself)
		SystemManager *systemmanager_;

	public:
		// Constructor
		VertexShader();
		~VertexShader();

		GLuint getId()
		{
			return id_;
		}

		std::string getSource()
		{
			return source_;
		}

		virtual void Load(SystemManager *mom, std::string filename);
private:
		void compile();
};

class FragmentShader : public ResourceBase
{
	protected:
		GLuint id_;         // The unique ID / handle for the shader
		std::string source_;     // The shader source code (i.e. the GLSL code itself)
		SystemManager *systemmanager_;

	public:
		// Constructor
		FragmentShader();
		~FragmentShader();

		GLuint getId()
		{
			return id_;
		}

		std::string getSource()
		{
			return source_;
		}

		virtual void Load(SystemManager *mom, std::string filename);
    private:
		void compile();
};

class ShaderProgram : public ResourceBase
{
public:
    ShaderProgram();
    ~ShaderProgram();

    virtual void Load(SystemManager *mom, std::string name);

    GLuint getId(){return id_;}
    void bind(){glUseProgram(id_);}
    void disable(){glUseProgram(0);}

    GLint attribute(std::string att);
    GLint uniform(std::string uni);
protected:
    SystemManager *systemmanager_;
    std::shared_ptr<VertexShader> vertexshader_;
    std::shared_ptr<FragmentShader> fragmentshader_;
    std::shared_ptr<YAMLFile> definition_;

    GLuint id_;

    std::map<std::string, GLint> attributes_;
    std::map<std::string, GLint> uniforms_;
};

#endif
