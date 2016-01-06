#ifndef RESOURCECACHE_H
#define RESOURCECACHE_H
#include <memory>
#include <string>
#include <map>

#include "systemmanager.h"
#include "logging.h"

class ResourceBase
{
protected:

    ResourceBase()
    {
    }
public:

    virtual ~ResourceBase()
    {
    }

    virtual void Load(SystemManager *mom, std::string name)
    {
    }
};

class ResourceManager : public SystemBase
{
public:
    ResourceManager(SystemManager *sm) : SystemBase(sm)
    {
        Logging *log=sm->GetSystem<Logging>();
        if(log) log->Log(LOG_INFO, "Starting ResourceManager.");
    }
    ~ResourceManager()
    {
    }

    template<typename ResType> std::shared_ptr<ResType> GetResource(std::string name)
    {
        auto p=resources_.find(name);
        if(p!=resources_.end() && p->second.expired()==false)
        {
            return std::dynamic_pointer_cast<ResType>(std::shared_ptr<ResourceBase>(p->second));
        }
        else
        {
            std::shared_ptr<ResType> ptr=std::make_shared<ResType>();
            if(ptr) ptr->Load(systemmgr_,name);
            auto tp=std::dynamic_pointer_cast<ResourceBase>(ptr);
            resources_[name]=std::weak_ptr<ResourceBase>(tp);
            return ptr;
        }
    }
private:
    std::map<std::string, std::weak_ptr<ResourceBase> > resources_;
};

#endif
