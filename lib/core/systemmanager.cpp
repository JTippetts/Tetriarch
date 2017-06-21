#include "core/systemmanager.h"

EventGroup::EventGroup() : sending_(0), dirty_(false){}
EventGroup::~EventGroup(){}

void EventGroup::Add(ObjectBase *o)
{
    receivers_.push_back(o);
}

void EventGroup::Remove(ObjectBase *o)
{
    std::vector<ObjectBase *>::iterator i=FindObject(o);

    if(i==receivers_.end()) return;

    if(sending_>0)
    {
        if(i!=receivers_.end())
        {
            (*i)=0;
            dirty_=true;
        }
    }
    else receivers_.erase(i);
}

void EventGroup::SendEvent(ObjectBase *sender, StringHash msg, AnyMap &args)
{
    ++sending_;

    for(auto &&i : receivers_)
    {

        if(i)
        {
            if(sender) i->HandleEvent(sender,msg,args);
            else i->HandleEvent(msg,args);
        }
    }

    --sending_;

    if(sending_==0 && dirty_)
    {
        for(auto &&i=receivers_.begin(); i!=receivers_.end(); ++i)
        {
            if((*i)) i=receivers_.erase(i);
        }
        dirty_=false;
    }
}

std::vector<ObjectBase *>::iterator EventGroup::FindObject(ObjectBase *p)
{
    for(auto &&i=receivers_.begin(); i!=receivers_.end(); ++i)
    {
        if ((*i)==p) return i;
    }

    return receivers_.end();
}

void SystemManager::SubscribeEvent(ObjectBase *obj, StringHash event)
{
    auto &&g=eventgroups_[event];
    g.Add(obj);
}

void SystemManager::SubscribeEvent(ObjectBase *origin, ObjectBase *obj, StringHash event)
{
    auto &&sg=specificeventgroups_[origin];
    auto &&g=sg[event];
    g.Add(obj);
}

void SystemManager::UnsubscribeEvent(ObjectBase *obj, StringHash event)
{
    auto &&g=eventgroups_.find(event);
    if(g!=eventgroups_.end()) g->second.Remove(obj);
}

void SystemManager::UnsubscribeEvent(ObjectBase *origin, ObjectBase *obj, StringHash event)
{
    auto &&sg=specificeventgroups_.find(origin);
    if(sg!=specificeventgroups_.end())
    {
        auto &&g=sg->second.find(event);
        if(g!=sg->second.end())
        {
            g->second.Remove(obj);
        }
    }
}

void SystemManager::UnsubscribeAllEvents(ObjectBase *obj)
{
    for(auto &&i : eventgroups_) i.second.Remove(obj);

    for(auto &&i : specificeventgroups_)
    {
        for(auto &&j : i.second) j.second.Remove(obj);
    }
}
void SystemManager::RemoveSubscriptions(ObjectBase *obj)
{
    auto &&i=specificeventgroups_.find(obj);
    if(i!=specificeventgroups_.end()) specificeventgroups_.erase(i);
}

void SystemManager::SendEvent(StringHash event, AnyMap &args)
{
    auto &&i=eventgroups_.find(event);
    if (i!=eventgroups_.end()) i->second.SendEvent(0, event, args);
}

void SystemManager::SendEvent(ObjectBase *sender, StringHash event, AnyMap &args)
{
    auto &&i=eventgroups_.find(event);
    if (i!=eventgroups_.end()) i->second.SendEvent(sender, event, args);

    auto &&g=specificeventgroups_.find(sender);
    if(g!=specificeventgroups_.end())
    {
        auto &&j=g->second.find(event);
        if(j!=g->second.end())
        {
            j->second.SendEvent(sender, event, args);
        }
    }
}
