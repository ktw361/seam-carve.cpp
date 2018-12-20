#include "image.h"

#include <vector>
#include <iostream>
#include <fstream>
//DEBUG
#include <cstdlib>

#include "inc/Eigen/Dense"

#define STB_IMAGE_IMPLEMENTATION
#include "inc/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "inc/stb_image_write.h"

Image::Image(int t_c): channels(t_c)
{
    arr_ = new Mtype[channels];
}

Image::Image(int const t_h, int const t_w, int const t_c): channels(t_c)
{
    arr_ = new Mtype[channels];
    for (int i = 0; i < channels; ++i) {
        arr_[i] = Mtype::Zero(t_h, t_w);
    }
}

Image::Image(internal_dtype const *img_data, int const t_h, int const t_w, int const t_c): channels(t_c)
{
    //
    // img.data take over, CALLER MUST call stbi_free!
    //
    internal_dtype *data = new unsigned char [t_h * t_w * t_c];
    for (int i=0; i!= t_h * t_w * t_c; ++i) {
        data[i] = img_data[i];
    }

    arr_ = new Mtype[channels];

    Slice _red(data, t_h, t_w);
    Slice _green(data + 1, t_h, t_w);
    Slice _blue(data + 2, t_h, t_w);
    for (int i = 0; i < channels; ++i) {
        arr_[i] = Mtype::Zero(t_h, t_w);
    }
    arr_[0]= _red.cast<Dtype>();
    arr_[1]= _green.cast<Dtype>();
    arr_[2]= _blue.cast<Dtype>();
}

Image::Image(Mtype const & mat): channels(1) {
    arr_ = new Mtype[1];
    arr_[0] = mat;
}

Image::Image(Image const & other)
{
    channels = other.channels;
    arr_ = new Mtype[channels];
    for (int i = 0; i!= channels; ++i) {
        arr_[i] = other.arr_[i];
    }
}

Image::~Image()
{
    delete [] arr_;
}

int
Image::height() const 
{
    return arr_[0].rows();
}

int
Image::width() const 
{
    return arr_[0].cols();
}

int
Image::d_size() const 
{
    return height() * width() * channels;
}

Image & 
Image::operator=(Image const & other)
{
    delete [] arr_;
    channels = other.channels;
    arr_ = new Mtype[channels];
    for (int i = 0; i < channels; ++i) {
        arr_[i] = other[i];
    }
    return *this;
}

const Mtype & 
Image::operator[] (std::size_t i) const 
{
    return this->arr_[i];
}

Mtype & 
Image::operator[] (std::size_t i)
{
    return this->arr_[i];
}

Dtype &
Image::operator() (Indtype i, Indtype j) const
{
    return arr_[0](i, j);
}

Dtype &
Image::operator() (Indtype i, Indtype j) 
{
    return arr_[0](i, j);
}

Image & 
Image::operator+=(Image const & other){
    // TODO assert?
    if ((height() != other.height()) || 
          (width() != other.width()) || 
          (channels != other.channels)
          ) {
        throw "Image size not match\n";
    }
    for (int i = 0; i < channels; ++i) {
        arr_[i] += other[i];  
    }
    return *this;
}

Image 
operator+(Image const & lhs, Image const & rhs)
{
    Image sum = lhs;
    sum += rhs;
    return sum;
}

Image 
imread(char const *filename) 
{
    if (!std::ifstream(filename)) {
        std::cerr << "File not exist\n";
        throw "File not exist";
    }
    int w, h, c;
    unsigned char *data = stbi_load(filename, &w, &h, &c, 0);
    Image img(data, h, w, c);
    stbi_image_free(data);
    return img;
}

void 
imsave(char const *filename, Image const & out)
{
    const int component = out.channels;
    const int quality = 100;
    int h = out.height();
    int w = out.width();
    int c = out.channels;

    internal_dtype *data = new internal_dtype[h * w * c];
    // 用memcpy？
    for (int i = 0; i != h; ++i) {
        for (int j = 0; j != w; ++j) {
            for (int k = 0; k != c; ++k) {
                data[i * w * c + j * c + k] = out[k](i,j);
                // 不要cast， cast图之后反而是灰度图片？？ 
            }
        }
    }
    stbi_write_jpg(filename, w, h, component, data, quality);
}
