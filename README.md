# Tetriarch

Demonstration of concepts:

1) System Management

2) Resource Management

System Management:

Systems are managed through the SystemManager class. All systems must implement the simple interface outlined by SystemBase. After an instance of SystemManager is created, one can call the method SystemManager::GetSystem<Type>() with the system type. If a system of the specified type does not exist, one is created and a pointer is returned. If a system DOES exist, it returns a pointer to it.

Resource Management:

Resources are managed through the ResourceManager, which is implemented as a system (ie, retrievable via SystemManager::GetSystem<ResourceManager>()). Once a ResourceManager system has been created, it can be retreived from the system manager, and used to obtain a pointer to a resource. The method ResourceManager::GetResource<Type>(std::string name) is the method to do so. Internally, the manager keeps a map of weak_ptr<ResourceBase>, and returns shared_ptr<ResourceType>. The use of a weak_ptr means that as soon as all shared_ptrs to a given resource are gone, the resource is destroyed. Calling GetResource for the resource will trigger a reload.