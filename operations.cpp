#include "operations.h"

#include <vector>
#include <utility>

#include "image.h"
#include "inc/Eigen/Dense"

Image
absolute(Image const & img) 
{
    Image absolute = img;
    for (int i = 0; i != absolute.channels; ++i ) {
        absolute[i] = absolute[i].array().abs();
    }
    return absolute;
}

Image 
channel_sum(Image const & img)
{
    Image ret = img[0];
    for (int i = 1; i != img.channels; ++i) {
        ret[0].array() += img[i].array();
    }
    return ret;
}

Image
rotate90aclk(Image const & img)
{
    Image ret(3);
    for (int i = 0; i != ret.channels; ++i ) {
        ret[i] = img[i].transpose().colwise().reverse();
    }
    return ret;
}

Image
rotate90clk(Image const & img)
{
    Image ret(3);
    for (int i = 0; i != ret.channels; ++i ) {
        ret[i] = img[i].transpose().rowwise().reverse();
    }
    return ret;
}

Image
Mrepeat(Mtype const & mat)
{
    Image img(mat.rows(), mat.cols(), I_channels );
    for (int i = 0; i < img.channels; ++i) {
        img.arr_[i] = mat;
    }
    return img;
}

Image 
Mrepeat(Image const & img)
{
    if (img.channels != 1) {
        throw "Mrepeat: img channel not one, don't know how to repeat";
    }
    return Mrepeat(img[0]);
}

MIndtype 
asMtype(std::vector<Indtype> vec, int h, int w)
{
    Eigen::Map<MIndtype> ret(vec.data(), h, w);
    return ret;
}

Mtype 
make_padding(Mtype const & mat, Mtype const & kern, ConvMode mode) 
{
    Mtype mat2 = mat;
    if (mode == Reflect) {
        ;
    } else if (mode == ZeroPad) {
        mat2 = Mtype::Zero(mat.rows(), mat.cols());
    } else {
        // TODO assert
        throw "ConvMode unknow";
    }

    int h = mat.rows(), w = mat.cols();
    int k_h = kern.rows(), k_w = kern.cols();
    int nl = (k_w - 1) / 2, nr = k_w / 2;
    int nu = (k_h - 1) / 2, nd = k_h / 2; 
    Mtype ret(nu + h + nd, nl + w + nr);
    auto flipdg = [&](Mtype m) {
        return m.colwise().reverse().rowwise().reverse();
    };
    auto flipud = [&](Mtype m) {
        return m.colwise().reverse();
    };
    auto fliplr = [&](Mtype m) {
        return m.rowwise().reverse();
    };
    ret << flipdg(mat2.block(0, 0, nu, nl)), 
           flipud(mat2.block(0, 0, nu, w )), 
           flipdg(mat2.block(0, w - nr, nu, nr)),
           fliplr(mat2.block(0, 0, h, nl)),
           mat,
           fliplr(mat2.block(0, w - nr, h, nr)),
           flipdg(mat2.block(h - nd, 0, nd, nl)),
           flipud(mat2.block(h - nd, 0, nd, w)),
           flipdg(mat2.block(h - nd, w - nr, nd, nr));

    return ret;
}

Mtype 
convolve(Mtype const & img, Mtype const & kern, ConvMode mode)
{
    int rows = img.rows(), cols = img.cols();
    int k_h = kern.rows(), k_w = kern.cols();

    Mtype img_ = make_padding(img, kern, mode);
    Mtype ret(rows, cols);
    auto flipdg = [&](Mtype m) {
        return m.colwise().reverse().rowwise().reverse();
    };
    #pragma omp parallel for
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            ret(i, j) = (
                    img_.block(i, j, k_h, k_w).array() * 
                    flipdg(kern).array()
                    ).sum();
        }
    }
    return ret;
}

Image
convolve(Image const & img, Image const & kern, ConvMode mode )
{
    Image ret(img.height(), img.width(), 1);
//    #pragma omp parallel for
    for (int i = 0; i < img.channels; ++i) {
        ret.arr_[0] += convolve(img.arr_[i], kern.arr_[i], mode);
    }
    Image ret_repeat = Mrepeat(ret);
    return ret_repeat;
}
