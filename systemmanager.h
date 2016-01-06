#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H
#include <memory>
#include <vector>

class SystemManager;
class SystemBase
{
public:
    virtual ~SystemBase(){}
    virtual void FixedUpdate(float dt){}
protected:
    SystemBase(SystemManager *sm) : systemmgr_(sm){}
    SystemManager *systemmgr_;
};

class SystemManager
{
public:
    // API methods to get the various systems
    SystemManager(){}

    template<typename SysType> SysType *GetSystem()
    {
        for(auto i : systems_)
        {
            std::shared_ptr<SysType> ptr=std::dynamic_pointer_cast<SysType>(i);
            if(ptr) return ptr.get();
        }

        // System does not exist, so create
        std::shared_ptr<SysType> ptr=std::make_shared<SysType>(this);
        if(ptr)
        {
            systems_.push_back(std::dynamic_pointer_cast<SystemBase>(ptr));
            return ptr.get();
        }
        return nullptr;
    }

    ~SystemManager() { systems_.clear();}

    void Update(float dt)
    {
        for(auto i : systems_)
        {
            i->FixedUpdate(dt);
        }
    }


private:
    std::vector<std::shared_ptr<SystemBase> > systems_;
};

#endif
