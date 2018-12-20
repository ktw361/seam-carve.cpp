#include "../image.h"
#include "../operations.h"
#include <gtest/gtest.h>

using namespace Eigen;

class ConvolveTest: public ::testing::Test {
protected:
    void SetUp() override {
    }

    Image make_lin_img(int h, int w) {
        // make linear , and stack to 3 channels
    	Mtype mat1(w, h);
    	for (int i = 0; i != h * w; ++i) {
    	    mat1(i) = i;
    	}
    	Mtype mat = mat1.array().transpose();
		return Mrepeat(mat);
    }
 
    void compare_row_and_col(Image const & a, Image const & b) {
        ASSERT_EQ(a.height(),   b.height());
        ASSERT_EQ(a.width(),    b.width());
        ASSERT_EQ(a.channels,   b.channels);
        for (int i = 0; i != a.height(); ++i) {
            for (int j = 0; j != a.width(); ++j) {
                for (int k = 0; k != a.channels; ++k) {
                    EXPECT_EQ(a[k](i, j),   b[k](i,j)) << i << ' ' << j << '\n';
                }
            }
        }
    } // void

    template<typename MT>
    void compare_row_and_col(MT const & a, std::vector<Dtype> & vec) {
        ASSERT_EQ(a.size(),   vec.size());
        for (int i = 0; i != a.rows(); ++i) {
            for (int j = 0; j != a.cols(); ++j) {
                    EXPECT_EQ(
                            a(i, j),   
                            vec[i * a.cols() + j]
                        ) << i << ' ' << j << '\n';
            }
        }
    } // void

    void compare_row_and_col(Image const & a, std::vector<Dtype> & vec) {
        ASSERT_EQ(a.d_size(),   vec.size());
        for (int i = 0; i != a.height(); ++i) {
            for (int j = 0; j != a.width(); ++j) {
                for (int k = 0; k != a.channels; ++k) {
                    EXPECT_EQ(
                            a[k](i, j),   
                            vec[i * a.width() * a.channels + j * a.channels + k]
                        ) 
                        << "i,j,k= "<< i << ',' << j << ',' << k << '\n';
                }
            }
        }
    } // void

    template <typename T>
    void compare_vec(std::vector<T> const & a, std::vector<T> const & b) {
        ASSERT_EQ(a.size(),     b.size());
        for (std::size_t i = 0; i != a.size(); ++i) {
            EXPECT_EQ(a[i],     b[i]);
        }
    }
    void print_mat(Mtype const & mat) {
        std::cerr << mat << std::endl;
    }


    Image img_lin_3_4;
};

TEST_F(ConvolveTest, PaddingReflect) {
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

TEST_F(ConvolveTest, ConvOnImage) {
    //
    // make linspace
    //
    Mtype mat1(4, 3);
    for (int i = 0; i != 12; ++i) {
        mat1(i) = i;
    }
    Mtype mat = mat1.array().transpose();
    Image img = Mrepeat(mat);

    Mtype kern_I = Mtype::Zero(3,3);
    kern_I(1,1) = 1.;
    Image filter_I = Mrepeat(kern_I);

    Image conved = convolve(img, filter_I);
    ASSERT_EQ(conved.height(),    3);
    ASSERT_EQ(conved.width(),     4);
    ASSERT_EQ(conved.channels,    3);
    Dtype epct[] = {0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33};;
    for (int i = 0; i != 3; ++i) {
        for (int j = 0; j != 4; ++j) {
            // 故意在第二片上测！, 这是实现
            EXPECT_EQ(conved[2](i, j),   epct[i * 4 + j]) << i << ' ' << j << '\n';
        }
    }
}

//
// Conv test of *ZeroPad* mode
//
TEST_F(ConvolveTest, ConvZeroPad) {
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
TEST_F(ConvolveTest, ConvRefPad) {
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
TEST_F(ConvolveTest, ConvRefPad2) {
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
