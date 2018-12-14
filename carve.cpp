#include "carve.h"
#include "inc/Eigen/Dense"

using namespace Eigen;

MatrixXf calc_energy(Image img)
{
    Matrix3f kern_du;
    kern_du << 1.0 << 2.0 << 1.0;
    kern_du << 0.0 << 0.0 << 0.0;
    kern_du << -1. << -2. << -1.;

    Matrix3f kern_dv = kern_du.transpose();

    MAT convolved_u(3);
    MAT convolved_v(3);
    for (int i=0; i!= 3; i++) {
        convolved_u[i] = convolve(img.mat[i], kern_du);
        convolved_v[i] = convolve(img.mat[i], kern_dv);
    }

}
