#include <iostream>
#include <cstdlib>
#include <string>

#include "image.h"
#include "processing.h"

int
main(int argc, char **argv)
{
//    if (argc != 5) {
//        std::cerr << "usage: carve.py <h/w> <scale> <image_in> <image_out>" << std::endl;
//        return -1;
//    }
    argv[1] = "w";
    argv[2] = "0.5";
    argv[3] = "/home/damon/QtWorkSapce/carveCpp/imgs/scene.jpg";
    argv[4] = "/home/damon/QtWorkSapce/carveCpp/build/out.jpg";
    const char which_axis = *argv[1];
    double scale = atof(argv[2]);
    const char *in_filename = argv[3];
    const char *out_filename = argv[4];

    Image img = imread(in_filename);
    std::cout << "Load " << in_filename << ' ' << img.height() << ' ' << img.width() << ' ' << img.channels << std::endl;

//    if (which_axis == 'h')
//        Image out = vertical_carving(img, scale);
//    else if (which_axis == 'w')
//        Image out = horizontal_carving(img, scale);
//    else {
//        std::cerr << "usage: carve.py <h/w> <scale> <image_in> <image_out> " << std::endl;
//        return -1;
//    }

    Image out = img;
    imsave(out_filename, out);
    return 0;
}
