#ifndef MATERIAL_H
#define MATERIAL_H

#include "resource/resourcecache.h"
#include "renderer/shader.h"
#include "resource/yamlfile.h"
#include "renderer/texture2d.h"

#include "GL/gl.h"
#include <vector>
#include <map>

class Material : public ResourceBase
{
public:
    Material(SystemManager *mom);
    ~Material();

    void Load(std::string name);
    void LoadFromDefinition(const YAML::Node &yaml);

    void enable();

    std::shared_ptr<Texture> GetTexture(std::string bindpoint);
    std::shared_ptr<Texture> ParseTexture(const YAML::Node &def, std::string bindpoint);
private:
    std::shared_ptr<ShaderProgram> shader_;
    std::map<std::string, std::shared_ptr<Texture>> textures_;
    std::string name_;
};

#endif
