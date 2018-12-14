#pragma once

#include <vector>
#include "inc/Eigen/Dense"

using namespace Eigen;

typedef Eigen::Map<Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>, Eigen::Unaligned, Eigen::InnerStride<3> > Slice;

struct Image {
    typedef Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> Mtype;

    int h, w, c;
    unsigned char *data;
    Mtype red;
    Mtype green;
    Mtype blue;
    Mtype *arr_;

    Image();
    Image(unsigned char const *data, int const data_w, int const data_h, int const data_c);
    Image(Image const &);
    ~Image();
};

Image imread(char const *filename);
void imsave(char const *filename, Image const & out);
