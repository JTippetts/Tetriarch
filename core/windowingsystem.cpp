extern "C"{
#include <GL/glew.h>
}
#include "core/windowingsystem.h"
#include "core/logging.h"
#include "renderer/renderer.h"
#include "core/messaging.h"

#define GLEQ_IMPLEMENTATION
#include "gleq.h"


WindowingSystem::WindowingSystem(SystemManager *mgr) : SystemBase(mgr), window_(nullptr)
{
    Logging *log=systemmgr_->GetSystem<Logging>();
    if(log) log->Log(LOG_INFO, "Starting WindowingSystem.");
}
WindowingSystem::~WindowingSystem()
{
    glfwTerminate();
}

bool WindowingSystem::SetVideoMode(int width, int height, bool fullscreen)
{
    Logging *log=systemmgr_->GetSystem<Logging>();

    if(!glfwInit())
    {
        log->Log(LOG_ERROR, "Unable to initialize GLFW.");
        return false;
    }

    log->Log(LOG_INFO, "Setting video mode to %dx%d, fullscreen = %d",width,height,fullscreen);
    if(window_)
    {
    }

    //unsigned int flags=SDL_WINDOW_OPENGL;
    //if(fullscreen) flags|=SDL_WINDOW_FULLSCREEN;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

    if(fullscreen) window_=glfwCreateWindow(width, height, "Tetriarch", glfwGetPrimaryMonitor(), 0);
    else window_=glfwCreateWindow(width, height, "Tetriarch", 0, 0);

    if(!window_)
    {
        log->Log(LOG_ERROR, "Could not set requested video mode.");
        return false;
    }

    glfwMakeContextCurrent(window_);
    GLenum err = glewInit();
    if(err != GLEW_OK)
    {
        log->Log(LOG_ERROR, "Could not initialize GLEW.");
    }
    else log->Log(LOG_INFO, "Initialized GLEW.");

    glfwGetFramebufferSize(window_, &width, &height);
    glViewport(0, 0, width, height);

    gleqTrackWindow(window_);

    //context_ = SDL_GL_CreateContext(window_);
    glfwMakeContextCurrent(window_);

    return true;
}

void WindowingSystem::ExecuteMainLoop(unsigned int updatesPerSecond)
{
    static StringHash PreUpdate=StringHasher{}("PreUpdate");
    static StringHash Update=StringHasher{}("Update");
    static StringHash PostUpdate=StringHasher{}("PostUpdate");
    static StringHash RenderPreUpdate=StringHasher{}("RenderPreUpdate");
    static StringHash RenderUpdate=StringHasher{}("RenderUpdate");
    static StringHash RenderPostUpdate=StringHasher{}("RenderPostUpdate");

    static StringHash DeltaTime=StringHasher{}("DeltaTime");
    static StringHash FrameTime=StringHasher{}("FrameTime");
    AnyMap am;
    loopexecuting_=true;
    resetloop_=false;
    bool purgeEvents=false;

    double oldTime=glfwGetTime();//SDL_GetTicks();
    double curTime=0;
    double logicCounter=0;

    double frameDelta;
    unsigned int curFrame=0;
    //double logicStep=1000.0/updatesPerSecond;
    double dt=1.0/(double)updatesPerSecond;

    am[FrameTime]=dt;

    while(loopexecuting_ && !glfwWindowShouldClose(window_))
    {
        if(resetloop_)
        {
            oldTime=glfwGetTime();//SDL_GetTicks();
            logicCounter=0;
            resetloop_=false;
            purgeEvents=true;
        }



        /*while(SDL_PollEvent(&event))
        {
            if(!purgeEvents)
            {
                HandleEvent(&event);
            }
            if(!loopexecuting_) return;
        }*/
        glfwPollEvents();

        GLEQevent event;
        while(gleqNextEvent(&event))
        {
            if(!purgeEvents)
            {
                // Handle event
            }
            if(!loopexecuting_) return;
        }

        gleqFreeEvent(&event);

        curTime=glfwGetTime();//SDL_GetTicks();
        frameDelta=curTime-oldTime;
        logicCounter=logicCounter + frameDelta;
        while(logicCounter>=dt)
        {
            systemmgr_->SendEvent(PreUpdate, am);
            systemmgr_->SendEvent(Update, am);
            systemmgr_->SendEvent(PostUpdate, am);
            //systemmgr_->Update(dt);
            logicCounter-=dt;
            if(resetloop_) logicCounter=0;
            curFrame++;
        }

        float percentWithinTick=(float)logicCounter / (float)dt;
        am[DeltaTime]=percentWithinTick;
        systemmgr_->SendEvent(RenderPreUpdate, am);
        systemmgr_->SendEvent(RenderUpdate, am);
        systemmgr_->SendEvent(RenderPostUpdate, am);

        Renderer *renderer=systemmgr_->GetSystem<Renderer>();
        if(renderer) renderer->Render(percentWithinTick);

    }
}

void WindowingSystem::Flip()
{
    //SDL_GL_SwapWindow(window_);
    glfwSwapBuffers(window_);
}
