#include "../image.h"
#include "../operations.h"
#include "../processing.h"
#include <iostream>
#include <gtest/gtest.h>

class ProcessingTest: public ::testing::Test {
protected:
    void SetUp() override {
        img_lin_3_4 = make_lin_img(3, 4);
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
                        ) << i << ' ' << j << '\n';
                }
            }
        }
    } // void
    void print_mat(Mtype const & mat) {
        std::cerr << mat << std::endl;
    }

    Image img_lin_3_4;
};

TEST_F(ProcessingTest, calc_energy) {
    Image emap = calc_energy(img_lin_3_4);
    ASSERT_EQ(emap.height(),    3);
    ASSERT_EQ(emap.width(),     4);
    ASSERT_EQ(emap.channels,    1);
    std::vector<Dtype> epct = {180.0,216.0,216.0,180.0,324.0,360.0,360.0,324.0,180.0,216.0,216.0,180.0,};
    compare_row_and_col(emap, epct);
}

TEST_F(ProcessingTest, minimal_seam) {
    Image emap = calc_energy(img_lin_3_4);
    auto seam_info = minimal_seam(emap);

    std::vector<Dtype> exp_info_M = {
        180.0,216.0,216.0,180.0,
        504.0,540.0,540.0,504.0,
        684.0,720.0,720.0,684.0,};
    std::vector<Dtype> exp_info_track = {
        0,0,0,0,0,0,3,3,0,0,3,3,
    };

    auto M = seam_info.first;
    auto track = seam_info.second;
    compare_row_and_col(M, exp_info_M);
    compare_row_and_col(track, exp_info_track);
}

TEST_F(ProcessingTest, carve_one_column) {
    auto out = carve_one_column(img_lin_3_4);
    auto img = out.first;
    auto pick_list = out.second;
    std::vector<Dtype> ep_out_1 = {
        1.0,1.0,1.0,2.0,2.0,2.0,3.0,3.0,3.0,5.0,5.0,5.0,6.0,6.0,6.0,7.0,7.0,7.0,9.0,9.0,9.0,10.0,10.0,10.0,11.0,11.0,11.0,
    };
    compare_row_and_col(img, ep_out_1);
}

TEST_F(ProcessingTest, process_seam) {
    TODO
}
