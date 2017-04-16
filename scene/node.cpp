#include "scene/node.h"

Node::Node(SystemManager *sm) : ObjectBase(sm),lastposition_(0,0), position_(0,0),
    lastscale_(1,1), scale_(1,1), parent_(nullptr)
{
    lastorientation_=glm::angleAxis(0.0f, glm::vec3(0,0,1));
    orientation_=lastorientation_;
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
    p->SetParent(this);
    children_.push_back(p);
    return p;
}

std::shared_ptr<Node> Node::CreateChild(std::string name)
{
    auto p=std::make_shared<Node>(sm_);
    p->SetName(name);
    p->SetParent(this);
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

    p->SetParent(nullptr);
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
    p->SetParent(nullptr);
}

