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

    template<typename MT = Mtype, typename DT = Dtype>
    void compare_row_and_col(MT const & a, std::vector<DT> & vec) {
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

    template<typename DT = Dtype>
    void compare_row_and_col(Image const & a, std::vector<DT> & vec) {
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

    template <typename T = Dtype>
    void compare_vec(std::vector<T> const & a, std::vector<T> const & b) {
        ASSERT_EQ(a.size(),     b.size());
        for (std::size_t i = 0; i != a.size(); ++i) {
            EXPECT_EQ(a[i],     b[i]);
        }
    }

    void print_mat(Mtype const & mat) {
        std::cerr << mat << std::endl;
    }

    void print3d(Image const & img) {
        for (int i = 0; i!= img.channels; ++i) {
            print_mat(img[i]);
        }
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
    std::vector<Indtype> ep_out_2 = {
        0, 0, 0,
    };
    compare_row_and_col(img, ep_out_1);
    // TODO 两个相同模板？就可以不用参数了？
    compare_vec<Indtype>(pick_list,  ep_out_2);
}

TEST_F(ProcessingTest, process_seam) {
    auto out = carve_one_column(img_lin_3_4);
    auto pick_list = out.second;

    decltype(pick_list) pick_list_all;
    pick_list_all.insert(
            pick_list_all.end(),
            std::make_move_iterator(pick_list.begin()),
            std::make_move_iterator(pick_list.end())
            );
    auto removed = asMtype(pick_list_all, 3, 4 - 3);
    auto image_seam = process_seam(img_lin_3_4, removed);
    std::vector<Dtype> ep_seam = {
        255.0, 0.0, 0.0, 1.0, 1.0, 1.0, 2.0, 2.0, 2.0, 3.0, 3.0, 3.0, 255.0, 0.0, 0.0, 5.0, 5.0, 5.0, 6.0, 6.0, 6.0, 7.0, 7.0, 7.0, 255.0, 0.0, 0.0, 9.0, 9.0, 9.0, 10.0, 10.0, 10.0, 11.0, 11.0, 11.0, 
    };
    compare_row_and_col(image_seam,  ep_seam);

    // Do it one more time
    out = carve_one_column(out.first);
    pick_list = out.second;
    pick_list_all.insert(
            pick_list_all.end(),
            std::make_move_iterator(pick_list.begin()),
            std::make_move_iterator(pick_list.end())
            );
    std::vector<Indtype> pick_ep = {
        0, 0, 0, 0, 0, 0,
    };
    compare_vec<Indtype>(pick_list_all, pick_ep);
    removed = asMtype(pick_list_all, 3, 4 - 2);
    compare_row_and_col<MIndtype, Indtype>(removed,    pick_ep);
    image_seam = process_seam(img_lin_3_4, removed);

    ep_seam = {
        255.0, 0.0, 0.0, 1.0, 1.0, 1.0, 2.0, 2.0, 2.0, 3.0, 3.0, 3.0, 255.0, 0.0, 0.0, 5.0, 5.0, 5.0, 6.0, 6.0, 6.0, 7.0, 7.0, 7.0, 255.0, 0.0, 0.0, 9.0, 9.0, 9.0, 10.0, 10.0, 10.0, 11.0, 11.0, 11.0, 
    };
    compare_row_and_col(image_seam, ep_seam);
}

TEST_F(ProcessingTest, horizontal_carving_0_2) {
    auto out = horizontal_carving(img_lin_3_4, 0.2);

    std::vector<Dtype> exp_img = {
        3.0, 3.0, 3.0, 7.0, 7.0, 7.0, 11.0, 11.0, 11.0, 
    };
    std::vector<Dtype> exp_seam = {
        255.0, 0.0, 0.0, 255.0, 0.0, 0.0, 2.0, 2.0, 2.0, 3.0, 3.0, 3.0, 255.0, 0.0, 0.0, 255.0, 0.0, 0.0, 6.0, 6.0, 6.0, 7.0, 7.0, 7.0, 255.0, 0.0, 0.0, 255.0, 0.0, 0.0, 10.0, 10.0, 10.0, 11.0, 11.0, 11.0, 
    };
    compare_row_and_col(out.first, exp_img);
    compare_row_and_col(out.second, exp_seam);
}

TEST_F(ProcessingTest, horizontal_carving_0_5) {
    auto out = horizontal_carving(img_lin_3_4, 0.5);

    std::vector<Dtype> exp_img = {
        2.0, 2.0, 2.0, 3.0, 3.0, 3.0, 6.0, 6.0, 6.0, 7.0, 7.0, 7.0, 10.0, 10.0, 10.0, 11.0, 11.0, 11.0, 
    };
    std::vector<Dtype> exp_seam = {
        255.0, 0.0, 0.0, 1.0, 1.0, 1.0, 2.0, 2.0, 2.0, 3.0, 3.0, 3.0, 255.0, 0.0, 0.0, 5.0, 5.0, 5.0, 6.0, 6.0, 6.0, 7.0, 7.0, 7.0, 255.0, 0.0, 0.0, 9.0, 9.0, 9.0, 10.0, 10.0, 10.0, 11.0, 11.0, 11.0, 
    };
    compare_row_and_col(out.first, exp_img);
    compare_row_and_col(out.second, exp_seam);
}

TEST_F(ProcessingTest, horizontal_carving_0_8) {
    auto out = horizontal_carving(img_lin_3_4, 0.8);

    std::vector<Dtype> exp_img = {
1.0, 1.0, 1.0, 2.0, 2.0, 2.0, 3.0, 3.0, 3.0, 5.0, 5.0, 5.0, 6.0, 6.0, 6.0, 7.0, 7.0, 7.0, 9.0, 9.0, 9.0, 10.0, 10.0, 10.0, 11.0, 11.0, 11.0, 
    };
    std::vector<Dtype> exp_seam = {
255.0, 0.0, 0.0, 1.0, 1.0, 1.0, 2.0, 2.0, 2.0, 3.0, 3.0, 3.0, 255.0, 0.0, 0.0, 5.0, 5.0, 5.0, 6.0, 6.0, 6.0, 7.0, 7.0, 7.0, 255.0, 0.0, 0.0, 9.0, 9.0, 9.0, 10.0, 10.0, 10.0, 11.0, 11.0, 11.0, 
    };
    compare_row_and_col(out.first, exp_img);
    compare_row_and_col(out.second, exp_seam);
}
