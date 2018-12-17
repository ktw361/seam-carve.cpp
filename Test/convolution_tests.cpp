#include "../image.h"
//#include "../image.cpp"
#include "../processing.h"
//#include "../processing.cpp"
#include <gtest/gtest.h>

using namespace Eigen;

TEST(ConvolutionTest, PositiveNos) {
    Mtype a = Mtype::Ones(3,3);
    Mtype kern_a = Mtype::Ones(3,3);

    Mtype convolved = convolve(a ,kern_a);
    Mtype ep1(3, 3);
    ep1.row(0) << 4, 6, 4;
    ep1.row(1) << 6 ,9, 6;
    ep1.row(2) << 4, 6, 4;

    ASSERT_EQ(convolved.size(),     ep1.size());
    ASSERT_EQ(a.block(0,0,3,3).array().sum(),  9);
    for (int i = 0; i != 3; ++i) {
        for (int j = 0; j != 3; ++j) {
            ASSERT_EQ(ep1(i,j),      convolved(i,j));
        }
    }
    ASSERT_EQ((convolved - ep1).isZero(), true);

}

TEST(ConvMtypeTest2, PositiveNos) {
    //
    // Conv test 2
    //
    Mtype mat(4,5);
    for (int i = 0; i != 4; ++i ) {
        for (int j = 0; j != 5; ++j) {
            mat(i, j) = (i * 5 + j);
            ASSERT_EQ(mat(i, j), i * 5 + j) << i << ' ' << j << '\n';
        }
    }
    mat(2, 2) = 50.5;
    ASSERT_EQ(mat(0,0),     0.);
    ASSERT_EQ(mat(2,3),     13.);
    ASSERT_EQ(mat(mat.rows() - 1, mat.cols() - 1),  19);
    Mtype kern(3,3);
    kern.row(0) << 1.,  2.,  1.;        
    kern.row(1) << 0.,  0.,  0.;
    kern.row(2) << -1, -2, -1;
    ASSERT_EQ(kern(0,1),    2.);
    ASSERT_EQ(kern(2,0),    -1.0);
    Mtype mat_convolved = convolve(mat, kern);
    Mtype ep2(4,5); 
    ep2.row(0) << 20.0, 20.0, 20.0, 20.0, 20.0;
    ep2.row(1) << 40.0, 78.5, 116,  78.5,   40;
    ep2.row(2) << 40.0, 40.0, 40.0, 40.0, 40;
    ep2.row(3) << 20,  -18.5, -56, -18.5, 20;
    for (int i = 0; i != 4; ++i) {
        for (int j = 0; j != 5; ++j) {
            EXPECT_EQ(ep2(i,j), mat_convolved(i,j)) << "i:j is " << i << ' ' << j << '\n';
        }
    }
}
