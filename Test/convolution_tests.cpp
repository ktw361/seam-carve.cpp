#include "../image.h"
#include "../image.cpp"
#include "../processing.h"
#include "../processing.cpp"
#include <gtest/gtest.h>

TEST(ConvolutionTest, PositiveNos) {
    Mtype a = Mtype::Ones(3,3);
    Mtype kern = Mtype::Ones(3,3);

    Mtype convolved = convolve(a ,kern);
    Mtype real(3, 3);
    real << 9, 6, 3,
            6, 4, 2,
            3, 2, 1;

    ASSERT_EQ(convolved.size(),     real.size());
    ASSERT_EQ(a.block(0,0,3,3).array().sum(),  9);
//            Dtype temp = Map<RowVectorXd>(img_.block(i, j, kern_h, kern_w).data(), kern_size) * Map<Eigen::VectorXd>(kern_temp.data(), kern_size);
    for (int i = 0; i != 9; ++i) {
        ASSERT_EQ(real(i),      convolved(i));
    }
    ASSERT_EQ((convolved - real).isZero(), true);

}

int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc , argv);
    return RUN_ALL_TESTS();
}
