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
    Image filter_dv = Mrepead(kern_dv);

    Image convolved = 
    
}

void
minimal_seam(Image const & energy_map, Mtype & M, std::vector<int> track)
{
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
            Dtype temp = Map<RowVectorXf>(img_.block<kern_h, kern_w>(i, j).data(), kern_size) * Map<Eigen::VectorXf>(kern.data, kern_size);
            ret(i, j) = temp;
        }
    }
    return ret;
}

Image
convolve(Image const & img, Image const &, kern)
{
    Image ret(img.h, img.w;
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

}
