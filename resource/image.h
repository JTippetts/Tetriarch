#ifndef Image_H
#define Image_H

#include "resource/resourcecache.h"

#include <vector>

class Image : public ResourceBase
{
public:
    Image(SystemManager *mom);
    ~Image();

    virtual void Load(std::string name);

    void Create(unsigned int w, unsigned int h, unsigned int d, unsigned char *data);
    unsigned char *GetData();

    int GetWidth(){return width_;}
    int GetHeight(){return height_;}
    int GetDepth(){return depth_;}

private:
    std::vector<unsigned char> pixels_;
    int width_, height_, depth_;
};
#endif // Image_H
