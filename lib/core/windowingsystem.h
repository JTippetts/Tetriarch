/*
WindowingSystem

This is the system that 'owns' everything to do with the application window. It includes functionality to set the video mode,
functionality to handle events, and implements the main game loop. The main loop will execute until the method EndLoop() is called.

Notice that WindowingSystem derives from SystemBase, so that it can be managed by SystemManager. The interface is rudimentary right now, but will grow as needed.
*/

#ifndef WINDOWINGSYSTEM_H
#define WINDOWINGSYSTEM_H

#include "core/systemmanager.h"
#include <GLFW/glfw3.h>

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
    GLFWwindow *window_;
    bool loopexecuting_=false;
    bool resetloop_=false;
};

#endif
