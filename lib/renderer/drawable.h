#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "core/componentbase.h"

class Drawable : public ComponentBase
{
public:
    Drawable(SystemManager *mom, Node *owner) : ComponentBase(mom,owner){}
    ~Drawable(){}

    virtual void render(float t)=0;
protected:

};

#endif
