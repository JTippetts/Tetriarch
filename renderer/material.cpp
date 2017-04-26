#include <GL/glew.h>
#include <GL/gl.h>

#include "material.h"


Material::Material(SystemManager *mom) : ResourceBase(mom)
{
}

Material::~Material()
{
}

void Material::Load(std::string name)
{
    ResourceCache *cache=systemmanager_->GetSystem<ResourceCache>();

    std::shared_ptr<YAMLFile> yamlfile=cache->GetResource<YAMLFile>(name);
    YAML::Node &yaml=yamlfile->GetNode();
    name_=name;

    LoadFromDefinition(yaml);
}

void Material::LoadFromDefinition(const YAML::Node &yaml)
{
    Logging *log=systemmanager_->GetSystem<Logging>();
    ResourceCache *cache=systemmanager_->GetSystem<ResourceCache>();

    if(yaml.Type()!=YAML::NodeType::Map)
    {
        log->Log(LOG_ERROR, "Material definition not properly formed.");
        return;
    }

    if(!yaml["shader"])
    {
        log->Log(LOG_ERROR, "Material does not specify shader.");
        return;
    }

    shader_=cache->GetResource<ShaderProgram>(yaml["shader"].as<std::string>());
    log->Log(LOG_INFO, "Material loaded shader:"+yaml["shader"].as<std::string>());

    // Textures are defined as a map that maps from bind point to either texture name or definition
    if(yaml["textures"] && yaml["textures"].Type()==YAML::NodeType::Map)
    {
        const YAML::Node &textures=yaml["textures"];
        for(auto i=textures.begin(); i!=textures.end(); ++i)
        {
            // Bind point is denoted by first
            std::string bindpoint=i->first.as<std::string>();
            log->Log(LOG_INFO, "Loading texture for bind point "+bindpoint);

            // Check if second is scalar, representing a string filename
            if(i->second.Type()==YAML::NodeType::Scalar)
            {
                // Load texture file
                log->Log(LOG_INFO, "Loading texture from file "+i->second.as<std::string>());
                std::shared_ptr<Texture2D> tex=cache->GetResource<Texture2D>(i->second.as<std::string>());
                textures_[bindpoint]=tex;
            }
            else if(i->second.Type()==YAML::NodeType::Map)
            {
                std::string texname=name_+"_"+bindpoint;
                std::shared_ptr<Texture2D> tex=cache->CreateResource<Texture2D>(texname);
                log->Log(LOG_INFO, "Loading texture from definition as "+texname);
                if(tex) tex->LoadFromDefinition(i->second);
                textures_[bindpoint]=tex;
            }
            else
            {
                log->Log(LOG_ERROR, "Unrecognized material texture definition.");
            }
        }
    }
}

void Material::enable()
{
    if(shader_) shader_->bind();

    int curtex=0;
    for(auto i=textures_.begin(); i!=textures_.end(); ++i)
    {
        // Get the uniform location from the shader
        // bind that location to curtex
        // bind the texture to curtex
        GLint uniformloc=shader_->uniform((*i).first);
        glUniform1i(uniformloc, curtex);
        glActiveTexture(GL_TEXTURE0+curtex);
        (*i).second->bind();
        ++curtex;
    }
}

