#include "resource/yamlfile.h"

YAMLFile::YAMLFile() : ResourceBase(), node_()
{
}

YAMLFile::~YAMLFile()
{
}

void YAMLFile::Load(SystemManager *mom, std::string name)
{
    Logging *log=mom->GetSystem<Logging>();
    log->Log(LOG_INFO, std::string("Loading YAML File ")+name);
    node_=YAML::LoadFile(name);

    if(!node_) log->Log(LOG_ERROR, std::string("Could not load YAML File ")+ name);
    else log->Log(LOG_INFO, std::string("Loaded YAML File resource ")+ name);
}
