#include "resource/yamlfile.h"

YAMLFile::YAMLFile(SystemManager *mom) : ResourceBase(mom), node_()
{
}

YAMLFile::~YAMLFile()
{
}

void YAMLFile::Load(std::string name)
{
    Logging *log=systemmanager_->GetSystem<Logging>();
    log->Log(LOG_INFO, std::string("Loading YAML File ")+name);
    node_=YAML::LoadFile(name);

    if(!node_) log->Log(LOG_ERROR, std::string("Could not load YAML File ")+ name);
    else log->Log(LOG_INFO, std::string("Loaded YAML File resource ")+ name);
}
