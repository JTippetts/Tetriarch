#ifndef OBJECTBASE_H
#define OBJECTBASE_H

#include "core/messaging.h"

class SystemManager;

class ObjectBase
{
public:
    ObjectBase(SystemManager *sm):sm_(sm){}
    virtual ~ObjectBase();

    virtual void HandleEvent(StringHash msg, AnyMap &args)=0;
    virtual void HandleEvent(std::string str, AnyMap &args)
    {
        HandleEvent(StringHasher{}(str), args);
    }
    virtual void HandleEvent(ObjectBase *sender, StringHash msg, AnyMap &args)=0;
    virtual void HandleEvent(ObjectBase *sender, std::string str, AnyMap &args)
    {
        HandleEvent(sender, StringHasher{}(str), args);
    }

    void SubscribeEvent(StringHash msg);
    void SubscribeEvent(std::string msg);
    void SubscribeEvent(ObjectBase *sender, StringHash msg);
    void SubscribeEvent(ObjectBase *sender, std::string msg);
    void UnsubscribeEvent(StringHash msg);
    void UnsubscribeEvent(std::string msg);
    void UnsubscribeEvent(ObjectBase *sender, StringHash msg);
    void UnsubscribeEvent(ObjectBase *sender, std::string msg);

    void SendEvent(StringHash event, AnyMap &args);
    void SendEvent(std::string event, AnyMap &args){SendEvent(StringHasher{}(event), args);}

protected:
    SystemManager *sm_;
};

#endif // OBJECTBASE_H
