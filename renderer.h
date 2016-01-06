#ifndef RENDERER_H
#define RENDERER_H
#include "systemmanager.h"

class Renderer : public SystemBase
{
public:
    Renderer(SystemManager *mgr) : SystemBase(mgr){}
    ~Renderer(){}

    void Render(float percent);

private:
};

#endif
