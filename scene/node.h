#ifndef NODE_H
#define NODE_H

#include "core/objectbase.h"
#include "scene/componentbase.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include <memory>

class Node : public ObjectBase
{
public:
    Node(SystemManager *sm);
    ~Node();

    virtual void HandleEvent(StringHash msg, AnyMap &args);
    virtual void HandleEvent(ObjectBase *sender, StringHash msg, AnyMap &args);

    glm::vec2 InterpPosition(float t){return glm::mix(lastposition_, position_, t);}
    glm::vec2 InterpScale(float t){return glm::mix(lastscale_, scale_, t);}
    glm::quat InterpOrientation(float t){return glm::slerp(lastorientation_, orientation_, t);}

    void SetPosition(glm::vec2 p){position_=p;}
    void SetScale(glm::vec2 s){scale_=s;}
    void SetOrientation(float angle){orientation_=glm::angleAxis(angle, glm::vec3(0,1,0));}
    void SetOrientation(float vx, float vy){orientation_=glm::angleAxis(glm::atan(vy, vx), glm::vec3(0,1,0));}

    glm::vec2 GetPosition(){return position_;}
    glm::vec2 GetScale(){return scale_;}
    glm::quat GetOrientation(){return orientation_;}

    std::shared_ptr<Node> CreateChild();
    std::shared_ptr<Node> CreateChild(std::string name);
    void RemoveChild(std::shared_ptr<Node> p);
    void RemoveChild(Node *p);

    void SetName(std::string name){name_=name;}

    template <typename CompType> std::shared_ptr<CompType> CreateComponent()
    {
        std::shared_ptr<CompType> p=std::make_shared<CompType>(sm_, this);
        components_.push_back(std::dynamic_pointer_cast<ComponentBase>(p));
        return p;
    }

    template <typename CompType> void RemoveComponent(std::shared_ptr<CompType> p)
    {
        for(auto &&i=components_.begin(); i!=components_.end(); ++i)
        {
            if(i->get() == p->get())
            {
                components_.erase(i);
                return;
            }
        }
    }

private:
    glm::vec2 lastposition_, position_;
    glm::vec2 lastscale_, scale_;
    glm::quat lastorientation_, orientation_;

    std::string name_;

    std::vector<std::shared_ptr<Node>> children_;
    std::vector<std::shared_ptr<ComponentBase>> components_;
};
#endif
