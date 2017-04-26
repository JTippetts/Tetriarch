#include <GL/glew.h>
#include <GL/gl.h>

#include "texture2d.h"
#include "core/logging.h"


Texture2D::Texture2D(SystemManager *mom) : ResourceBase(mom), id_(0)
{
    glGenTextures(1, &id_);
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &id_);
}

void Texture2D::bind()
{
    glBindTexture(GL_TEXTURE_2D, id_);
}

void Texture2D::LoadFromDefinition(const YAML::Node &yaml)
{
    Logging *log=systemmanager_->GetSystem<Logging>();
    ResourceCache *cache=systemmanager_->GetSystem<ResourceCache>();

    if(yaml.Type() != YAML::NodeType::Map)
    {
        log->Log(LOG_ERROR, "Invalid texture definition");
        return;
    }

    if(!yaml["image"])
    {
        log->Log(LOG_ERROR, "Image name not defined in texture definition.");
        return;
    }

    std::string imgfile=yaml["image"].as<std::string>();
    auto i=cache->GetResource<Image>(imgfile);
    Create(i.get());

    if(yaml["filtering"])
    {
        std::string filter=yaml["filtering"].as<std::string>();
        if(filter=="nearest")
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            log->Log(LOG_INFO, "Texture "+imgfile+" set to nearest filtering.");
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            log->Log(LOG_INFO, "Texture "+imgfile+" set to linear filtering.");
        }
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
}

void Texture2D::Load(std::string name)
{
    Logging *log=systemmanager_->GetSystem<Logging>();

    log->Log(LOG_INFO, "Loading texture "+name);
    //auto i=systemmanager_->GetSystem<ResourceCache>()->GetResource<Image>(name);
    ResourceCache *cache=systemmanager_->GetSystem<ResourceCache>();

    // Since the image will be named the same as the texture resource, I need to fudge things a little.
    // Create a new image resource with a different name, then load.

    auto yamlfile=cache->GetResource<YAMLFile>(name);

    YAML::Node &yaml=yamlfile->GetNode();
    LoadFromDefinition(yaml);
}

void Texture2D::Create(int w, int h, int d, unsigned char *data)
{
    GLint format;
    if(d==1) format=GL_RED;
    else if(d==2) format=GL_RG;
    else if(d==3) format=GL_RGB;
    else format=GL_RGBA;

    Logging *log=systemmanager_->GetSystem<Logging>();
    log->Log(LOG_INFO, "Creating texture : %dx%dx%d", w, h, d);

    glBindTexture(GL_TEXTURE_2D, id_);
    glTexImage2D(GL_TEXTURE_2D, 0,format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
}

void Texture2D::Create(Image *i)
{
    Logging *log=systemmanager_->GetSystem<Logging>();

    if(!i)
    {
        log->Log(LOG_ERROR, "Could not create texture.");
        return;
    }

    int w=i->GetWidth();
    int h=i->GetHeight();
    int d=i->GetDepth();

    Create(w,h,d,i->GetData());
}
