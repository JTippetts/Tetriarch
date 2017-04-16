/*
ResourceCache

The ResourceCache is a system, deriving from SystemBase so that it can be managed by SystemManager. The purpose of ResourceCache is to manage the creation
and storage of resources. Resources are any external data: images, sounds, etc...

The way ResourceCache operates is that a resources must inherit from the ResourceBase class, and implement the Load() method.
ResourceCache maintains an internal map that maps from the filename (std::string) of the resource to a weak_ptr holding a ResourceBase pointer.
Similarly to how SystemManager works, the guts of GetResource<Type> use dynamic_pointer_cast to convert the pointer to a ResourceBase pointer for storage, and
convert back to Type when returning a shared_ptr to the resources.

The idiom of using a weak_ptr for storage internally is so that the resource will be 'automatically' unloaded whenever all external shared_ptrs to it are destroyed.
A weak pointer will not keep an object alive if no shared_ptrs exist that point to the object. So when a resource is requested, a shared_ptr is returned to the resource. If that
shared_ptr is then destroyed (the object no longer needs it, the object itself is destroyed, whatever) then, if no other pointers to it exist, the resource is destroyed. It
can be loaded again by subsequent calls to GetResource.

*/

#ifndef RESOURCECACHE_H
#define RESOURCECACHE_H
#include <memory>
#include <string>
#include <map>

#include "core/systemmanager.h"
#include "core/logging.h"

// The base class for Resources. All Resources must inherit from ResourceBase in order to be managed by the cache.
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

    // Resource types MUST implement Load().
    virtual void Load(SystemManager *mom, std::string name)=0;
};

// The resource cache. Note it inherits from SystemBase, so it can be managed by the ResourceManager.
class ResourceCache : public SystemBase
{
public:
    ResourceCache(SystemManager *sm) : SystemBase(sm)
    {
        // since it's a system, it has access to the system manager. Here, it requests a pointer to the logging system, and logs some info.
        Logging *log=sm->GetSystem<Logging>();
        if(log) log->Log(LOG_INFO, "Starting ResourceCache.");
    }
    ~ResourceCache()
    {
    }

    // GetResource is templated by the type of resource to load. Specify the filename to load, and the specific resource type will attempt to load it.
    template<typename ResType> std::shared_ptr<ResType> GetResource(std::string name)
    {
        // First, check to see if the resource is already in existence.
        auto p=resources_.find(name);
        if(p!=resources_.end() && p->second.expired()==false)
        {
            // If you get here, then the resource is already stored in the cache and is active, meaning there are
            // other shared_ptrs to it in existence, so it is loaded and ready to go. In this case, cast the stored ResourceBase pointer
            // back to the desired resource typ, and return a shared pointer to it.
            return std::dynamic_pointer_cast<ResType>(std::shared_ptr<ResourceBase>(p->second));
        }
        else
        {
            // If you get here, one of two things happened.
            // Either 1) the resource doesn't exist or
            // 2) the resource 'exists' but all previous shared_ptrs to it have been destroyed, so the resource
            // itself was destroyed. Note that this will destroy the resource, but won't remove the pointer from
            // the map, so even if a pointer is found it must be checked for validity using the expired() method of
            // the weak_ptr.
            // In this case, create (or re-create) the resource, and call Load() on it to load the resource file. Cast
            // the pointer to a ResourceBase pointer to store in the map, and return the shared pointer to the caller.

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
