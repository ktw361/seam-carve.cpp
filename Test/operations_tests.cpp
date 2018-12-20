#include "../image.h"
#include "../operations.h"
#include <gtest/gtest.h>

using namespace Eigen;

//TEST(AbsTest, PositiveNos) {
//
//}
//

TEST(ChannelSumTest, PositiveNos) {
    //
    // make linspace
    //
    Mtype mat1(4, 3);
    for (int i = 0; i != 12; ++i) {
        mat1(i) = i;
    }
    Mtype mat = mat1.array().transpose();
    Image img = Mrepeat(mat);
    
    Image sum = channel_sum(img);
    for (int i = 0; i!= 3; ++i) {
        for (int j = 0; j != 4; ++j) {
            EXPECT_EQ(sum(i, j),    (i * 4 + j) * 3);
        }
    }
}

//
//TEST(RotTest, PositiveNos) {
//
//}
//
//TEST(MrepTest, PositiveNos) {
//
//}

TEST(asMtypeTest, PositiveNos) {
    std::vector<Indtype> vec { 1,2,3,4,5,6 };
    MIndtype ind_mat = asMtype(vec, 3, 2);
    int cnt = 1;
    for (int j = 0; j !=2; ++j) {
        for (int i = 0; i!= 3; ++i) {
            EXPECT_EQ(ind_mat(i,j ),   cnt++) << i << ' '<<  j << '\n' ;
        }
    }
}
