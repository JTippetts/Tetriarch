#ifndef NODE_H
#define NODE_H

#include "core/objectbase.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Node : public ObjectBase
{
public:
    Node(SystemManager *sm);
    ~Node();

    virtual void HandleEvent(StringHash msg, AnyMap &args);
    virtual void HandleEvent(ObjectBase *sender, StringHash msg, AnyMap &args);

    glm::vec2 InterpPosition(float t);
    glm::vec2 InterpScale(float t);
    glm::quat InterpOrientation(float t);

    void SetPosition(glm::vec2 p);
    void SetScale(glm::vec2 s);
    void SetOrientation(float angle);
    void SetOrientation(float vx, float vy);

private:
    glm::vec2 lastposition_, position_;
    glm::vec2 lastscale_, scale_;
    glm::quat lastorientation_, orientation_;
};
#endif
