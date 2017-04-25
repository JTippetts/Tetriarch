#include "core/glfwwrapper.h"

#include "core/logging.h"

#define ANL_IMPLEMENTATION
//#define IMPLEMENT_STB
#include <anl.h>

Logging *glfwlogger=nullptr;
void GLFWErrorCallback(int error, const char *description)
{
    if(glfwlogger)
    {
        glfwlogger->Log(LOG_ERROR, "GLFW Error: %s", description);
    }
}


void InitializeGLFWWrapping(Logging *logger)
{
    logger->Log(LOG_INFO, "Initializing GLFW.");
    glfwlogger=logger;
    glfwSetErrorCallback(GLFWErrorCallback);
}
