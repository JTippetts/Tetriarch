#ifndef GLFWWRAPPER_H
#define GLFWWRAPPER_H

#include <deque>
#include <GLFW/glfw3.h>

#include "core/logging.h"

struct InputEvent
{
    int type_;

};

void InitializeGLFWWrapping(Logging *logger);


#endif
