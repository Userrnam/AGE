#pragma once


namespace age {

namespace vk {

typedef struct Image_T* Image;
typedef struct ImageView_T* ImageView;
typedef struct Sampler_T* Sampler;

};

class Texture {
    vk::Image image;
    vk::ImageView imageView;
    vk::Sampler sampler;
public:
    void load(const char* filename);
};

}
