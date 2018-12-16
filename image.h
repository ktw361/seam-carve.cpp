#pragma once

#include <vector>
#include "inc/Eigen/Dense"

using namespace Eigen;

#define I_channels  3

typedef double Dtype;
typedef Eigen::Array<Dtype, Eigen::Dynamic, Eigen::Dynamic> Mtype;

typedef Eigen::Map<Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>, Eigen::Unaligned, Eigen::InnerStride<3> > Slice;

struct Image {

private:
    typedef unsigned char internal_dtype;
    internal_dtype * data; // 干掉这个？

public:

    int h, w, c;
    int d_size;
    Mtype * arr_;

    Image();
    Image(int const data_h, int const data_w, int const data_c);
    Image(unsigned char const *data, int const data_h, int const data_w, int const data_c);
    Image(Image const &);
    ~Image();

    Mtype & red();
    Mtype & green();
    Mtype & blue();

    const Mtype & operator[] (std::size_t i) const;
    Mtype & operator[] (std::size_t i);

    Image & operator=(Image const & other);
    Image & operator+=(Image const & other);

    friend void imsave(char const *filename, Image const & out);
};

Image operator+(Image const & lhs, Image const & rhs);

Image imread(char const *filename);

Image Mrepeat(Mtype const & m); 
