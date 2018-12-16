#pragma once

#include <vector>
#include "inc/Eigen/Dense"

using namespace Eigen;

#define I_channels  3

typedef float Dtype;
typedef Eigen::Matrix<Dtype, Eigen::Dynamic, Eigen::Dynamic> Mtype;

typedef Eigen::Map<Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>, Eigen::Unaligned, Eigen::InnerStride<3> > Slice;

struct Image {

private:
    typedef unsigned char internal_dtype;

public:

    int h, w, c;
    int d_size;
    internal_dtype * data;
    Mtype * arr_;

    Image();
    Image(int const data_h, int const data_w, int const data_c);
    Image(unsigned char const *data, int const data_h, int const data_w, int const data_c);
    Image(Image const &);
    ~Image();

    Mtype & red();
    Mtype & green();
    Mtype & blue();

    inline const Mtype & operator[] (std::size_t i) const {
        return this->arr_[i];
    }
    inline Mtype & operator[] (std::size_t i) {
        return this->arr_[i];
    }

    Image & operator=(Image const & other);
    Image & operator+=(Image const & other);
};

Image operator+(Image const & lhs, Image const & rhs);

Image imread(char const *filename);
void imsave(char const *filename, Image const & out);

Image Mrepeat(Mtype const & m); 
