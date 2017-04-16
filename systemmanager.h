/*
SystemManager

Serves as the backbone. A pointer to any system in the game can be obtained by calling the GetSystem<Type>() method.

The system works by maintaining a vector of shared_ptr<SystemBase>. All systems must inherit from SystemBase in order to be used with this
class.

At first glance, you might think it would be best to use a unique_ptr instead of a shared_ptr, since GetSystem() returns a raw pointer with no
ownership implications. However, due to the mechanism used to store different systems in a single vector, it's necessary to use a shared_ptr.

The way the mechanism works is that the vector is a vector of shared_ptr<SystemBase>. When GetSystem<Type> is called, the vector is
iterated and each element is checked to see if a dynamic_pointer_cast from the stored pointer to the desired Type is successful.
If it succeeds, we know we've found the system we're looking for.

One caveat of this approach: Do not inherit one system from another system. The way a dynamic cast works is it tries to cast a base pointer (SystemBase) to
the given type. But if the type you are looking for is inherited from another system type, then it can be cast to a pointer of that system type, and so you end up
with a pointer to the wrong system.

Anyway, if the dynamic_pointer_cast succeeds, you have found the system you want, so it returns a raw pointer to it. But if the end of the vector is reached, then
the system hasn't been created yet. In that case, a new instance of the system is created and stored in the vector, then a raw pointer is returned.

All systems are constructed with a pointer to the SystemManager passed to the constructor, so every system can access the SystemManager. This aids in systems requesting
pointers to other systems that they require in order to function. For example, it is common for a system to request a pointer to the Logging system in order to
log information about their operation.
*/

#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H
#include <memory>
#include <vector>
#include <map>
#include "objectbase.h"

// Forward declaration of SystemManager, so that the system can store a pointer to it.
class SystemManager;

// SystemBase
// All Systems must inherit from this class. Some systems respond to the FixedUpdate step of the main loop, so if a system
// wants to perform any tasks on update they must overload FixedUpdate. If not, they can safely ignore it.
// FixedUpdate accepts a parameter representing the length, in seconds, of an update step.
class SystemBase
{
public:
    virtual ~SystemBase(){}
    virtual void FixedUpdate(float dt){}
protected:
    SystemBase(SystemManager *sm) : systemmgr_(sm){}
    SystemManager *systemmgr_;
};

class EventGroup
{
public:
    EventGroup();
    ~EventGroup();

    void Add(ObjectBase *o);
    void Remove(ObjectBase *o);

    void SendEvent(ObjectBase *sender, StringHash msg, AnyMap &args);
private:
    std::vector<ObjectBase *> receivers_;
    std::size_t sending_;
    bool dirty_;

    std::vector<ObjectBase *>::iterator FindObject(ObjectBase *p);
};

typedef std::map<StringHash, EventGroup> EventGroupMap;
typedef std::map<ObjectBase *, EventGroupMap> SpecificEventGroupMaps;

// Implement SystemManager
class SystemManager
{
public:
    SystemManager(){}

    // GetSystem is templated, so that a particular system can be requested by type.
    // For example: Logging *log = systemmanager->GetSystem<Logging>()
    template<typename SysType> SysType *GetSystem()
    {
        // Iterate the systems vector
        for(auto i : systems_)
        {
            // dynamic cast the stored pointer to a pointer of the requested type.
            // If the dynamic cast is wrong (ie, if the stored pointer is not of the requested type)
            // the dynamic cast will fail. If the cast succeeds, then the stored pointer is of the
            // correct type, so return a raw pointer from it.
            std::shared_ptr<SysType> ptr=std::dynamic_pointer_cast<SysType>(i);
            if(ptr) return ptr.get();
        }

        // System does not exist, so create a new on and store it
        std::shared_ptr<SysType> ptr=std::make_shared<SysType>(this);
        if(ptr)
        {
            // dynamic cast the created pointer down to a SystemBase pointer so it can be
            // stored in the vector, and return a raw pointer
            systems_.push_back(std::dynamic_pointer_cast<SystemBase>(ptr));
            return ptr.get();
        }

        // For whatever reason, a system could not be created, so return null
        return nullptr;
    }

    ~SystemManager() { systems_.clear();}

    // During the Update cycle of the main loop, SystemManager::Update will call FixedUpdate on all systems.
    void Update(float dt)
    {
        for(auto i : systems_)
        {
            i->FixedUpdate(dt);
        }
    }

    void SubscribeEvent(ObjectBase *obj, StringHash event);
    void SubscribeEvent(ObjectBase *origin, ObjectBase *obj, StringHash event);
    void UnsubscribeEvent(ObjectBase *obj, StringHash event);
    void UnsubscribeEvent(ObjectBase *origin, ObjectBase *obj, StringHash event);
    void UnsubscribeAllEvents(ObjectBase *obj);
    void RemoveSubscriptions(ObjectBase *obj); // Remove all subscription lists for object

    void SendEvent(StringHash event, AnyMap& args);
    void SendEvent(ObjectBase *sender, StringHash event, AnyMap &args);
    void SendEvent(std::string event, AnyMap& args){SendEvent(StringHasher{}(event), args);}
    void SendEvent(ObjectBase *sender, std::string event, AnyMap &args){SendEvent(sender, StringHasher{}(event), args);}


private:
    std::vector<std::shared_ptr<SystemBase> > systems_;
    EventGroupMap eventgroups_;
    SpecificEventGroupMaps specificeventgroups_;
};

#endif
