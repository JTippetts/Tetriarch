#include "scene/node.h"

Node::Node(SystemManager *sm) : ObjectBase(sm)
{
    SubscribeEvent("PreUpdate");
}
Node::~Node(){}
void Node::HandleEvent(StringHash msg, AnyMap &args)
{
    static StringHash PreUpdate=StringHasher{}("PreUpdate");

    if(msg==PreUpdate)
    {
        lastposition_=position_;
        lastscale_=scale_;
        lastorientation_=orientation_;
    }
}
void Node::HandleEvent(ObjectBase *sender, StringHash msg, AnyMap &args){}
