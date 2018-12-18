#include "../image.h"
#include "../operations.h"
#include <gtest/gtest.h>

using namespace Eigen;

//void comparaMtype(Mtype const & a, Mtype const & b) {
//    ASSERT_EQ(a.size(),     b.size());
//    ASSERT_EQ(a.rows(),     b.rows());
//    ASSERT_EQ(a.cols(),     b.cols());
//    for (int i=0; i!= a.rows(); ++i) {
//        for (int j=0; j!= a.rows(); ++j) {
//            EXPECT_EQ(a(i,j),   b(i, j)) << "i:j is: " << i < ' ' << j << '\n';
//        }
//    }
//}

TEST(PaddingReflectTest, PositiveNos) {
    Mtype a1(3,3);
    for (int i=0; i!= a1.size(); ++i) {
        a1(i) = i + 1;
    }
    Mtype a = a1.transpose();
    Mtype kern(3, 2);
    Mtype padded = make_padding(a, kern, Reflect); 
    Mtype padded_expect(5, 4);
    ASSERT_EQ(padded.size(),       padded_expect.size());
    ASSERT_EQ(padded.cols(),       padded_expect.cols());
    ASSERT_EQ(padded.rows(),       padded_expect.rows());
    padded_expect<< 1, 2, 3, 3,
                    1, 2, 3, 3,
                    4, 5, 6, 6,
                    7, 8, 9, 9,
                    7, 8, 9, 9;
    for (int i = 0; i!= padded.rows(); ++i) {
        for (int j = 0; j!= padded.cols(); ++j) {
            EXPECT_EQ(padded(i,j),  padded_expect(i,j)) << i << ' ' << j << '\n';
        }
    }
}

//TEST(ConvModeTest, PositiveNos) {
//    Mtype kern(3, 2);
//    convolve(kern, kern, (ConvMode)(-1));
//}

//
// Conv test of *ZeroPad* mode
//
TEST(ConvZeroPadTest, PositiveNos) {
    Mtype a = Mtype::Ones(3,3);
    Mtype kern_a = Mtype::Ones(3,3);

    Mtype convolved = convolve(a ,kern_a, ZeroPad);
    Mtype ep1(3, 3);
    ep1.row(0) << 4, 6, 4;
    ep1.row(1) << 6 ,9, 6;
    ep1.row(2) << 4, 6, 4;

    ASSERT_EQ(convolved.size(),     ep1.size());
    ASSERT_EQ(a.block(0,0,3,3).array().sum(),  9);
    for (int i = 0; i != 3; ++i) {
        for (int j = 0; j != 3; ++j) {
            EXPECT_EQ(ep1(i,j),      convolved(i,j)) << i << ' ' << j << '\n';
        }
    }
    ASSERT_EQ((convolved - ep1).isZero(), true);

}

//
// Conv test of Reflect mode 1
//
TEST(ConvRefPadTest1, PositiveNos) {
    Mtype a1(3,3);
    for (int i=0; i!= a1.size(); ++i) {
        a1(i) = i;
    }
    Mtype a = a1.transpose();
    Mtype kern  = Mtype::Ones(3,2);
    Mtype conved = convolve(a, kern);
    Mtype conved_exp(3, 3);
    conved_exp << 9, 15, 18,
                  21, 27, 30,
                  33, 39, 42;
    ASSERT_EQ(conved_exp.size(),       conved.size());
    ASSERT_EQ(conved_exp.cols(),       conved.cols());
    ASSERT_EQ(conved_exp.rows(),       conved.rows());
    for (int i = 0; i != 3; ++i) {
        for (int j = 0; j != 3; ++j) {
            EXPECT_EQ(conved(i,j),      conved_exp(i,j)) << i << ' ' << j << '\n';
        }
    }
}

//
// Conv test of Reflect mode 2
//
TEST(ConvRefPadTest2, PositiveNos) {
    Mtype mat(4,5);
    for (int i = 0; i != 4; ++i ) {
        for (int j = 0; j != 5; ++j) {
            mat(i, j) = (i * 5 + j);
            ASSERT_EQ(mat(i, j), i * 5 + j) << i << ' ' << j << '\n';
        }
    }
    mat(2, 2) = 50.5;
    ASSERT_EQ(mat(2,2),     50.5);
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
    ep2.row(1) << 40.0, 78.5, 117,  78.5,   40;
    ep2.row(2) << 40.0, 40.0, 40.0, 40.0, 40;
    ep2.row(3) << 20,  -18.5, -57, -18.5, 20;

    for (int i = 0; i != 4; ++i) {
        for (int j = 0; j != 5; ++j) {
            EXPECT_EQ(ep2(i,j), mat_convolved(i,j)) << "i:j is " << i << ' ' << j << '\n';
        }
    }
}
