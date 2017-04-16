#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <vector>
#include <fstream>

#include <windows.h>

#include "core/systemmanager.h"
#include "resourcecache.h"

#include "windowingsystem.h"
#include "core/logging.h"
#include "renderer.h"

#include "core/messaging.h"

using linb::any_cast;

class TestResource1 : public ResourceBase
{
public:
    TestResource1() : ResourceBase()
    {
    }
    virtual ~TestResource1()
    {
    }

    void Load(SystemManager *mom, std::string name)
    {
    }
};


// TEst object
class TestObject : public ObjectBase
{
public:
    TestObject(SystemManager *sm) : ObjectBase(sm)
    {
    }

    virtual ~TestObject()
    {
    }

    virtual void HandleEvent(StringHash msg, AnyMap &args)
    {
        static StringHash TEST=StringHasher{}("TEST");
        static StringHash Test=StringHasher{}("Test");
        static StringHash UPDATE=StringHasher{}("UPDATE");
        static StringHash GROUPUPDATE=StringHasher{}("GROUPUPDATE");

        Logging *log=sm_->GetSystem<Logging>();
        if (msg==TEST) log->Log(LOG_INFO, "Received a TEST message in TestObject.");
        else if(msg==UPDATE) log->Log(LOG_INFO, "Received an UPDATE message in TestObject.");
        else if(msg==GROUPUPDATE) log->Log(LOG_INFO, "Received a GROUPUPDATE message in TestObject.");

        log->Log(LOG_INFO, "Parameter Test: %f", any_cast<double>(args[Test]));
    }

    virtual void HandleEvent(ObjectBase *sender, StringHash msg, AnyMap &args)
    {
        static StringHash TEST=StringHasher{}("TEST");
        Logging *log=sm_->GetSystem<Logging>();
        if (msg==TEST) log->Log(LOG_INFO, "Received a TEST message in TestObject from %d.",std::size_t(sender));
    }

private:

};


#if defined(WIN32)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
#else
int main(int argc, char **argv)
#endif
{
    // Create a system manager, and instance the basic systems
    // The system manager is the backbone of the whole thing. All systems can be
    // obtained through it, using the GetSystem<Type> method.
    // See systemmanager.h

    SystemManager mom;
    // Create the Logging system.
    Logging *log=mom.GetSystem<Logging>();
    log->Log(LOG_INFO, "Starting up.");

    // Create the ResourceCache system
    // See resourcecache.h

    mom.GetSystem<ResourceCache>();

    // Create the Windowing system. Windowing manages the application window, the main loop and the
    // event handling portion.
    WindowingSystem *gs=mom.GetSystem<WindowingSystem>();

    // Create the Renderer. The Renderer handles drawing everything.
    mom.GetSystem<Renderer>();



    // Set the video mode and, if successful, execute the main loop.
    // The main loop is a Fixed Timestep loop. It accepts a parameter that specifies
    // how many fixed update steps to perform per second. The loop will run until the method
    // WindowingSystem::EndLoop() is called, at which point it will exit.

    //if(gs && gs->SetVideoMode(800,600,false)) gs->ExecuteMainLoop(24);

    // Testing
    //ObjectCollection *c=mom.GetSystem<ObjectCollection>();
    std::shared_ptr<TestObject> to=std::make_shared<TestObject>(&mom);
    to->SubscribeEvent("UPDATE");


    AnyMap am;

    am[StringHasher{}("Test")]=42.0;
    mom.SendEvent(StringHasher{}("UPDATE"), am);

    to->UnsubscribeEvent("UPDATE");

    //to.reset();
    mom.SendEvent(StringHasher{}("UPDATE"), am);

    std::shared_ptr<TestObject> to1=std::make_shared<TestObject>(&mom);

    to->SubscribeEvent(to1.get(), "TEST");

    mom.SendEvent(to1.get(), StringHasher{}("TEST"), am);
    mom.SendEvent("TEST", am);

    to->UnsubscribeEvent(to1.get(), "TEST");
    mom.SendEvent(to1.get(), "TEST", am);

    to->SubscribeEvent(to1.get(), "TEST");
    mom.SendEvent(to1.get(), "TEST", am);
    to1->SendEvent("TEST", am);

    mom.RemoveSubscriptions(to1.get());
    mom.SendEvent(to1.get(), "TEST", am);

    to1->SendEvent("TEST", am);

    return 0;
}
