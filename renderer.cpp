#include "renderer.h"
#include <GL/GL.h>
#include "windowingsystem.h"

#include "logging.h"

void Renderer::Render(float percent)
{
    glClearColor(0,0,1,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    systemmgr_->GetSystem<WindowingSystem>()->Flip();
}
