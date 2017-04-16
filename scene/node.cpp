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

std::shared_ptr<Node> Node::CreateChild()
{
    auto p=std::make_shared<Node>(sm_);
    children_.push_back(p);
    return p;
}

std::shared_ptr<Node> Node::CreateChild(std::string name)
{
    auto p=std::make_shared<Node>(sm_);
    p->SetName(name);
    children_.push_back(p);
    return p;
}

void Node::RemoveChild(std::shared_ptr<Node> p)
{
    for(auto &&n=children_.begin(); n!=children_.end(); ++n)
    {
        if((*n)==p)
        {
            children_.erase(n);
            return;
        }
    }
}

void Node::RemoveChild(Node *p)
{
    for(auto &&n=children_.begin(); n!=children_.end(); ++n)
    {
        if((*n).get()==p)
        {
            children_.erase(n);
            return;
        }
    }
}

