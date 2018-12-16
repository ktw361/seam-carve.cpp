#include "processing.h"

#include <vector>

#include "image.h"
#include "inc/Eigen/Dense"

using namespace Eigen;
// 重命名Mtype？
Mtype 
calc_energy(Image const & img)
{
    Mtype kern_du(3,3);
    kern_du << 1., 2., 1.,
               0., 0., 0.,
               -1, -2, -1;
    Mtype kern_dv(kern_du.rows(), kern_du.cols());
    kern_dv = kern_du.transpose();
    Image filter_du = Mrepeat(kern_du);
    Image filter_dv = Mrepeat(kern_dv);

    Image convolved = convolve(img, filter_du) + convolve(img, filter_dv);
    // 
    // absolute
    //
//    convolved = 
//    convol
//    convolved = convolved.absolute();
//    Mtype energy_map = convolved.channel_sum();
//    return energy_map;
}

void
minimal_seam(Mtype const & energy_map, Mtype & M, std::vector<int> track)
{
//    for (int i=0; i!= energy_map.rows(); ++i) {
//        track(i, 1) = argmin(M(i-1))
//    }
    ;
}

Image process_seam(Image const & original, std::vector<std::vector<int> > removed)
{
    ;
}


// Depth One convolution
Mtype 
convolve(Mtype const & img, Mtype const & kern)
{
    int rows = img.rows(), cols = img.cols();
    int kern_h = kern.rows(), kern_w = kern.cols();
    int kern_size = kern_h * kern_w;
    Mtype ret(rows, cols);
    Mtype img_(rows + kern_h - 1, cols +  kern_w - 1);
    img_ << img, Mtype::Zero(rows, kern_w - 1),
            Mtype::Zero(kern_h - 1, cols + kern_w - 1);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            ret(i, j) = (img_.block(i, j, kern_h, kern_w).array() * kern.array()).sum();
//            Mtype kern_temp = kern;
//            Dtype temp = Map<RowVectorXd>(img_.block(i, j, kern_h, kern_w).data(), kern_size) * Map<Eigen::VectorXd>(kern_temp.data(), kern_size);
//            ret(i, j) = temp;
        }
    }
    return ret;
}

Image
convolve(Image const & img, Image const & kern)
{
    Image ret(img.h, img.w, img.c);
    for (int i = 0; i < img.c; ++i) {
        ret.arr_[i] = convolve(img.arr_[i], kern.arr_[i]);
    }
    return ret;
}

Image
horizontal_carving(Image const & image, double const scale)
{
    ;
}

Image
vertical_carving(Image const & image, double const scale)
{
    ;
}
