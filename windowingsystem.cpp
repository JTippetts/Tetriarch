#include "windowingsystem.h"
#include "logging.h"
#include "renderer.h"

#include <SDL.h>



WindowingSystem::WindowingSystem(SystemManager *mgr) : SystemBase(mgr), window_(0)
{
    Logging *log=systemmgr_->GetSystem<Logging>();
    if(log) log->Log(LOG_INFO, "Starting WindowingSystem.");
}
WindowingSystem::~WindowingSystem()
{
    SDL_GL_DeleteContext(context_);
    SDL_Quit();
}

bool WindowingSystem::SetVideoMode(int width, int height, bool fullscreen)
{
    Logging *log=systemmgr_->GetSystem<Logging>();
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)<0)
    {
        log->Log(LOG_ERROR, "Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    log->Log(LOG_INFO, "Setting video mode to %dx%d, fullscreen = %d",width,height,fullscreen);
    if(window_)
    {
    }

    unsigned int flags=SDL_WINDOW_OPENGL;
    if(fullscreen) flags|=SDL_WINDOW_FULLSCREEN;

    window_= SDL_CreateWindow("Tetriarch",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          width, height,
                          flags );
    if(!window_)
    {
        log->Log(LOG_ERROR, "Could not set requested video mode: %s", SDL_GetError());
        return false;
    }

    context_ = SDL_GL_CreateContext(window_);

    return true;
}

void WindowingSystem::ExecuteMainLoop(unsigned int updatesPerSecond)
{
    loopexecuting_=true;
    resetloop_=false;
    bool purgeEvents=false;

    unsigned int oldTime=SDL_GetTicks();
    unsigned int curTime=0;
    unsigned int logicCounter=0;
    SDL_Event event;
    unsigned int frameDelta;
    unsigned int curFrame=0;
    unsigned int logicStep=1000/updatesPerSecond;
    float dt=1.0/(float)updatesPerSecond;

    while(loopexecuting_)
    {
        if(resetloop_)
        {
            oldTime=SDL_GetTicks();
            logicCounter=0;
            resetloop_=false;
            purgeEvents=true;
        }

        while(SDL_PollEvent(&event))
        {
            if(!purgeEvents)
            {
                HandleEvent(&event);
            }
            if(!loopexecuting_) return;
        }

        curTime=SDL_GetTicks();
        frameDelta=curTime-oldTime;
        logicCounter=logicCounter + frameDelta;
        while(logicCounter>=logicStep)
        {
            systemmgr_->Update(dt);
            logicCounter-=logicStep;
            if(resetloop_) logicCounter=0;
            curFrame++;
        }

        float percentWithinTick=(float)logicCounter / (float)logicStep;
        Renderer *renderer=systemmgr_->GetSystem<Renderer>();
        if(renderer) renderer->Render(percentWithinTick);

    }
}

void WindowingSystem::HandleEvent(SDL_Event *event)
{
    switch(event->type)
    {
        case SDL_KEYDOWN: EndLoop(); break;
        default: break;
    }
}

void WindowingSystem::Flip()
{
    SDL_GL_SwapWindow(window_);
}
