#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <vector>
#include <fstream>

#include <windows.h>

#include "systemmanager.h"
#include "resourcecache.h"

#include "windowingsystem.h"
#include "logging.h"
#include "renderer.h"

std::fstream out;

class TestResource1 : public ResourceBase
{
public:
    TestResource1() : ResourceBase()
    {
        out<< "TestResource1 created" <<std::endl;
    }
    virtual ~TestResource1()
    {
        out << "TestResource1 destroyed." <<std::endl;
    }

    void Load(SystemManager *mom, std::string name)
    {
        out << "TestResource1 " << name << " loaded" << std::endl;
    }
};


//int main(int argc, char **argv)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    //std::ostream out("log.txt", std::out);
    //out.open("log.txt", std::ios::out);

    // Create a system manager, and instance the basic systems
    SystemManager mom;
    Logging *log=mom.GetSystem<Logging>();
    log->Log(LOG_INFO, "Starting up.");

    mom.GetSystem<ResourceManager>();
    WindowingSystem *gs=mom.GetSystem<WindowingSystem>();
    mom.GetSystem<Renderer>();



    // Set the video mode
    if(gs)
    {
        gs->SetVideoMode(640,480,false);
        gs->ExecuteMainLoop(24);
    }

    return 0;
}
