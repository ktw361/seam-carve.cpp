#include "image.h"

#include <vector>
#include "inc/Eigen/Dense"

#define STB_IMAGE_IMPLEMENTATION
#include "inc/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "inc/stb_image_write.h"

Image::Image(): h(0), w(0), c(0),data(nullptr), arr_(nullptr)
{
}

Image::Image(unsigned char const *img_data, int const data_w, int const data_h, int const data_c) : h(data_h), w(data_w), c(data_c)
{
    // img.data take over, CALLER MUST call stbi_free!
    data = new unsigned char [h*w*c];
    for (int i=0; i!= h*w*c; ++i) {
        data[i] = img_data[i];
    }

    Slice _red(data, h, w), _green(data + 1, h, w), _blue(data + 2, h, w);
    red = _red.cast<float>();
    green = _green.cast<float>();
    blue = _blue.cast<float>();

    arr_ = new Mtype[3];
    arr_[0] = red;
    arr_[1] = green;
    arr_[2] = blue;
}

Image::Image(Image const & other): h(other.h), w(other.w), c(other.c)
{
    data = new unsigned char[w*h*c];
    for (int i = 0; i< w*h*c; ++i) {
        data[i] = other.data[i];
    }

    arr_ = new Mtype[3];
    for (int i = 0; i!= 3; ++i) {
        arr_[i] = other.arr_[i];
    }
    red = arr_[0];
    green = arr_[1];
    blue = arr_[2];
}

Image::~Image()
{
    delete [] data;
    delete [] arr_;
}

Image 
imread(char const *filename) 
{
    int w, h, c;
    unsigned char *data = stbi_load(filename, &w, &h, &c, 0);
    Image img(data, w, h, c);
    stbi_image_free(data);
    return img;
}

void 
imsave(char const *filename, Image const & out)
{
    const int component = 3;
    const int quality = 100;
    stbi_write_jpg(filename, out.w, out.h, component, out.data, quality);
}
