#ifndef SHADER_H
#define SHADER_H

#include "../resource/resourcecache.h"
#include <GL/glew.h>
#include <GL/GL.h>

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



#endif
