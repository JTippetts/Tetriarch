#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "resource/resourcecache.h"
#include "resource/image.h"
#include "resource/yamlfile.h"
#include <GL/gl.h>

class Texture : public ResourceBase
{
public:
    Texture(SystemManager *mom) : ResourceBase(mom){}
    virtual ~Texture(){}

    virtual void Load(std::string name)=0;
    virtual void bind()=0;

protected:

};

class Texture2D : public Texture
{
public:
    Texture2D(SystemManager *mom);
    ~Texture2D();

    GLuint GetId(){return id_;}

    void Load(std::string name);
    void Create(int w, int h, int d, unsigned char *data);
    void Create(Image *i);
    void LoadFromDefinition(const YAML::Node &yaml);
    void bind();
private:
    GLuint id_;
};

#endif
