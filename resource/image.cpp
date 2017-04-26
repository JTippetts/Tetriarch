#include "resource/Image.h"

#include "stb/stb_image.h"

Image::Image(SystemManager *mom) : ResourceBase(mom), width_(0), height_(0), depth_(0){}
Image::~Image(){}

void Image::Load(std::string name)
{
    unsigned char *data=stbi_load(name.c_str(), &width_, &height_, &depth_,0);
    if(!data)
    {
        Logging *log=systemmanager_->GetSystem<Logging>();
        log->Log(LOG_ERROR, "Unable to load image "+name);
    }
    else
    {
        Logging *log=systemmanager_->GetSystem<Logging>();
        log->Log(LOG_INFO, "Loading image "+name+": %dx%dx%d", width_, height_, depth_);
        Create(width_, height_, depth_, data);
    }
    stbi_image_free(data);
}

void Image::Create(unsigned int w, unsigned int h, unsigned int d, unsigned char *data)
{
    width_=w;
    height_=h;
    depth_=d;

    pixels_.resize(w*h*d);
    memcpy(data, &pixels_[0], w*h*d);
}

unsigned char *Image::GetData()
{
    if(pixels_.size()>0) return &pixels_[0];
    return nullptr;
}
