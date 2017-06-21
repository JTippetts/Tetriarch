#include "core/objectbase.h"
#include "core/systemmanager.h"

ObjectBase::~ObjectBase()
{
    if(sm_)
    {
        sm_->UnsubscribeAllEvents(this);
        sm_->RemoveSubscriptions(this);
    }
}

void ObjectBase::SubscribeEvent(StringHash msg){if(sm_) sm_->SubscribeEvent(this, msg);}
void ObjectBase::SubscribeEvent(std::string msg){SubscribeEvent(StringHasher{}(msg));}
void ObjectBase::SubscribeEvent(ObjectBase *sender, StringHash msg){if(sm_) sm_->SubscribeEvent(sender, this, msg);}
void ObjectBase::SubscribeEvent(ObjectBase *sender, std::string msg){SubscribeEvent(sender, StringHasher{}(msg));}
void ObjectBase::UnsubscribeEvent(StringHash msg){if(sm_) sm_->UnsubscribeEvent(this, msg);}
void ObjectBase::UnsubscribeEvent(std::string msg){UnsubscribeEvent(StringHasher{}(msg));}
void ObjectBase::UnsubscribeEvent(ObjectBase *sender, StringHash msg){if(sm_) sm_->UnsubscribeEvent(sender, this, msg);}
void ObjectBase::UnsubscribeEvent(ObjectBase *sender, std::string msg){UnsubscribeEvent(sender, StringHasher{}(msg));}
void ObjectBase::SendEvent(StringHash event, AnyMap &args)
{
    if(sm_) sm_->SendEvent(this, event, args);
}
