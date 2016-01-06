#ifndef WINDOWINGSYSTEM_H
#define WINDOWINGSYSTEM_H

#include "systemmanager.h"
#include <SDL.h>

class WindowingSystem : public SystemBase
{
public:
    WindowingSystem(SystemManager *mgr);
    ~WindowingSystem();

    bool SetVideoMode(int width, int height, bool fullscreen);

    void ExecuteMainLoop(unsigned int updatesPerSecond);
    void EndLoop(){loopexecuting_=false;}
    bool LoopRunning(){return loopexecuting_;}
    void ResetLoop(){resetloop_=true;}

    void Flip();
private:
    SDL_Window *window_;
    SDL_GLContext context_;
    bool loopexecuting_=false;
    bool resetloop_=false;

    void HandleEvent(SDL_Event *event);
};

#endif
