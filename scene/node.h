#ifndef NODE_H
#define NODE_H

#define GLM_ENABLE_EXPERIMENTAL

#include "core/objectbase.h"
#include "core/logging.h"
#include "scene/componentbase.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

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
    void SetOrientation(float angle){orientation_=glm::angleAxis(angle, glm::vec3(0,0,1));}
    void SetOrientation(float vx, float vy){orientation_=glm::angleAxis(glm::atan(vy, vx), glm::vec3(0,0,1));}

    glm::vec2 GetPosition(){return position_;}
    glm::vec2 GetScale(){return scale_;}
    glm::quat GetOrientation(){return orientation_;}

    std::shared_ptr<Node> CreateChild();
    std::shared_ptr<Node> CreateChild(std::string name);
    void RemoveChild(std::shared_ptr<Node> p);
    void RemoveChild(Node *p);
    void SetParent(Node *p){parent_=p;}

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

    template <typename CompType> CompType *GetComponent()
    {
        for(auto i : components_)
        {
            auto p=std::dynamic_pointer_cast<CompType>(i);
            if(p) return p.get();
        }
        return nullptr;
    }

    glm::mat4 GetMatrix(float t)
    {
        Logging *log=sm_->GetSystem<Logging>();

        glm::quat q=InterpOrientation(t);
        glm::mat4 mat=glm::mat4_cast(q);
        glm::vec2 p=InterpPosition(t);
        glm::vec2 s=InterpScale(t);

        glm::mat4 trans=glm::translate(glm::mat4(), glm::vec3(p.x, p.y, 0));
        glm::mat4 scale=glm::scale(glm::mat4(), glm::vec3(s.x, s.y, 1));

        log->Log(LOG_INFO, "Translation: "+glm::to_string(trans));
        log->Log(LOG_INFO, "Rotation: "+glm::to_string(mat));
        log->Log(LOG_INFO, "Scale: "+glm::to_string(scale));

        mat=trans*mat*scale;

        if(parent_)
        {
            glm::mat4 par=parent_->GetMatrix(t);
            mat=par*mat;
            log->Log(LOG_INFO, "Parent: "+glm::to_string(par));
        }

        log->Log(LOG_INFO, "Final: "+glm::to_string(mat));

        return mat;
    }

private:
    glm::vec2 lastposition_, position_;
    glm::vec2 lastscale_, scale_;
    glm::quat lastorientation_, orientation_;

    std::string name_;

    std::vector<std::shared_ptr<Node>> children_;
    std::vector<std::shared_ptr<ComponentBase>> components_;

    Node *parent_;
};
#endif
