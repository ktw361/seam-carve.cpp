#include "../image.h"
//#include "../image.cpp"
#include <gtest/gtest.h>
#include <iostream>

TEST(ImageTest, PositiveNos) {
    //
    // Index type must be the same
    //
    ASSERT_EQ(typeid(Indtype),  typeid(MIndtype::Index));

    Image img1, img2;
    const char fname[] = "/home/damon/QtWorkSapce/carveCpp/Test/img_test.jpg";
    const char fname_out[] = "/home/damon/QtWorkSapce/carveCpp/Test/img_out.jpg";
    // r -> w -> r(img1) -> w(img2)
    img1 = imread(fname);
    imsave(fname_out, img1);
    img1 = imread(fname_out);
    imsave(fname_out, img1);
    img2 = imread(fname_out);
    imsave(fname_out, img2);
    Image img3 = imread(fname_out);

    ASSERT_EQ(img1.h,   480);
    ASSERT_EQ(img2.w,   720);
    ASSERT_EQ(img1.c,   3);
    
    ASSERT_EQ(img1[0](0,0),     img2[0](0,0));
    for (int i = 0; i != img1.c; ++i) {
        ASSERT_EQ(img1[i].data(),   img1.arr_[i].data());

        ASSERT_EQ(img1[i].size(),   img2[i].size());

        ASSERT_EQ(img1[i].mean() - img2[i].mean() < 1, true );
        ASSERT_EQ(img2[i].sum() - img2[i].sum() < 0.005e7, true);
        ASSERT_EQ(img1[i](222, 333), img2[i](222, 333));
    }
    ASSERT_EQ(img1.h,           img2.h);
    ASSERT_EQ(img1.d_size,      img2.d_size);

}
