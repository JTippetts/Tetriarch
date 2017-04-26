#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "resource/resourcecache.h"
#include "resource/image.h"
#include "resource/yamlfile.h"
#include <GL/gl.h>

class Texture2D : public ResourceBase
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
