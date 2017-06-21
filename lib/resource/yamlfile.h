#ifndef YAMLFILE_H
#define YAMLFILE_H
#include "resource/resourcecache.h"
#include <yaml-cpp/yaml.h>

class YAMLFile : public ResourceBase
{
public:
    YAMLFile(SystemManager *mom);
    ~YAMLFile();

    virtual void Load(std::string name);

    YAML::Node &GetNode(){return node_;}

private:
    YAML::Node node_;

};


#endif
