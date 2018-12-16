#include "image.h"

#include <vector>
#include "inc/Eigen/Dense"

#define STB_IMAGE_IMPLEMENTATION
#include "inc/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "inc/stb_image_write.h"

Image::Image(): h(0), w(0), c(0), d_size(0), data(nullptr), arr_(nullptr)
{
}

Image::Image(int const data_h, int const data_w, int const data_c): h(data_h), w(data_w), c(data_c)
{
    d_size = h * w * c;

    data = new unsigned char [h * w * c];
    arr_ = new Mtype[c];
    for (int i = 0; i < c; ++i) {
        arr_[i] = Mtype::Zero(h, w);
    }
}

Image::Image(unsigned char const *img_data, int const data_h, int const data_w, int const data_c) : h(data_h), w(data_w), c(data_c)
{
    d_size = h * w * c;
    //
    // img.data take over, CALLER MUST call stbi_free!
    //
    data = new unsigned char [h*w*c];
    for (int i=0; i!= h*w*c; ++i) {
        data[i] = img_data[i];
    }

    Slice _red(data, h, w), _green(data + 1, h, w), _blue(data + 2, h, w);
    arr_ = new Mtype[c];
    for (int i = 0; i < c; ++i) {
        arr_[i] = Mtype::Zero(h, w);
    }
    arr_[0]= _red.cast<float>();
    arr_[1]= _green.cast<float>();
    arr_[2]= _blue.cast<float>();
}

Image::Image(Image const & other): h(other.h), w(other.w), c(other.c)
{
    d_size = h * w * c;
    data = new unsigned char[w*h*c];
    for (int i = 0; i< w*h*c; ++i) {
        data[i] = other.data[i];
    }

    arr_ = new Mtype[c];
    for (int i = 0; i!= c; ++i) {
        arr_[i] = other.arr_[i];
    }
}

Image::~Image()
{
    delete [] data;
    delete [] arr_;
}

Image & 
Image::operator=(Image const & other)
{
    h = other.h;
    w = other.w;
    c = other.c;
    d_size = other.d_size;
    delete [] data;
    delete [] arr_;
    data = new internal_dtype[d_size]; 
    for (int i = 0; i < d_size; ++i) {
        data[i] = other.data[i];
    }
    for (int i = 0; i < c; ++i) {
        arr_[i] = other.arr_[i];
    }
    return *this;
}

Mtype &
Image::red()
{
    return arr_[0];
}

Mtype &
Image::green()
{
    return arr_[1];
}

Mtype &
Image::blue()
{
    return arr_[2];
}

Image & 
Image::operator+=(Image const & other){
    // TODO
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
    int w, h, c;
    unsigned char *data = stbi_load(filename, &w, &h, &c, 0);
    Image img(data, h, w, c);
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

Image
Mrepreat(Mtype const & m)
{
    Image img(m.rows(), m.cols(), I_channels );
    for (int i = 0; i < img.c; ++i) {
        img.arr_[i] = m;
    }
    return img;
}
