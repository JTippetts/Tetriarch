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
private:
    std::shared_ptr<ShaderProgram> shader_;
    std::map<std::string, std::shared_ptr<Texture2D>> textures_;
    std::string name_;
};

#endif
