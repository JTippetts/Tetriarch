#ifndef IMAGERESOURCE_H
#define IMAGERESOURCE_H

#include "resourcecache.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class ImageResource : public ResourceBase
{
public:
    ImageResource();
    ~ImageResource();

    virtual void Load(SystemManager *mom, std::string name);
private:

};
#endif // IMAGERESOURCE_H
