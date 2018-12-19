#pragma once

#include <vector>
#include "inc/Eigen/Dense"

#define I_channels  3

typedef double Dtype;
typedef Eigen::Array<Dtype, Eigen::Dynamic, Eigen::Dynamic > Mtype;
typedef Mtype::Index Indtype;
typedef Eigen::Array<Indtype, Eigen::Dynamic, Eigen::Dynamic> MIndtype;

//typedef Eigen::Map<Mtype, Eigen::Unaligned, Eigen::InnerStride<3> > Slice;
typedef Eigen::Map<Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>, Eigen::Unaligned, Eigen::InnerStride<3> > Slice;

typedef unsigned char internal_dtype;

struct Image {
    //布局： CWH (按照Eigen 列优先)
    //stbi: HWC

public:

    // 初始化 const？
    int channels; // channels of an image should not change
    // const 对象的成员也是const？
    Mtype * arr_; // 用智能指针？

    Image();
    Image(int const t_h, int const t_w, int const t_c);
    Image(internal_dtype const *data, int const t_h, int const t_w, int const t_c);
    Image(Mtype const &);
    Image(Image const &);
    ~Image();

    // ? 作用于非const时候？
    int height() const;
    int width() const;
    int d_size() const;

    const Mtype & operator[] (std::size_t i) const;
    Mtype & operator[] (std::size_t i);
    // (i, j) 返回第一个维度
    virtual Dtype & operator() (Indtype, Indtype) const;
    Dtype & operator() (Indtype, Indtype);

    Image & operator=(Image const & other);
    Image & operator+=(Image const & other);

    friend void imsave(char const *filename, Image const & out);
};

Image operator+(Image const & lhs, Image const & rhs);

Image imread(char const *filename);
