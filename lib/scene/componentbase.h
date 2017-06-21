#ifndef COMPONENTBASE_H
#define COMPONENTBASE_H

#include "core/objectbase.h"

class Node;

class ComponentBase : public ObjectBase
{
public:
    ComponentBase(SystemManager *sm, Node *owner) : ObjectBase(sm), node_(owner){}
    virtual ~ComponentBase(){}

private:
    Node *node_;
};

#endif
