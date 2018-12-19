#include "../image.h"
//#include "../image.cpp"
#include <gtest/gtest.h>
#include <iostream>

TEST(ImageTest, PositiveNos) {
    //
    // Index type must be the same
    //
    ASSERT_EQ(typeid(Indtype),  typeid(MIndtype::Index));

    const char fname[] = "/home/damon/QtWorkSapce/carveCpp/Test/img_test.jpg";
    const char fname_out[] = "/home/damon/QtWorkSapce/carveCpp/Test/img_out.jpg";
    // r -> w -> r(img1) -> w(img2)
    Image img1 = imread(fname);
    ASSERT_EQ(img1.height(),   480);
    ASSERT_EQ(img1.width(),   720);
    ASSERT_EQ(img1.channels,   3);

    imsave(fname_out, img1);

    img1 = imread(fname_out);
    imsave(fname_out, img1);

    Image img2; // Test operator=
    img2 = imread(fname_out);
    imsave(fname_out, img2);
    Image img3 = imread(fname_out);

    
    EXPECT_EQ(img1[0](0,0),     img2[0](0,0));
    for (int i = 0; i != img1.channels; ++i) {
        EXPECT_EQ(img1[i].data(),   img1.arr_[i].data());

        EXPECT_EQ(img1[i].size(),   img2[i].size());

        EXPECT_NEAR(img1[i].mean(), img2[i].mean(),  1);
        EXPECT_NEAR(img2[i].sum() , img2[i].sum(),   1);
        EXPECT_EQ(img1[i](222, 333), img2[i](222, 333));
    }
    ASSERT_EQ(img1.height(),           img2.height());
    ASSERT_EQ(img1.d_size(),      img2.d_size());

}
