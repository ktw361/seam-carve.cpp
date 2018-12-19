#include <iostream>
#include <fstream>
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

    if (!std::ifstream(in_filename)) {
        std::cerr << "File not exist, quit\n";
        return -1;
    }
    Image img = imread(in_filename);
    std::cout << "Load " << in_filename << ' ' << img.w << ' ' << img.h << ' ' << img.c << std::endl;

    if (which_axis == 'h') {
        auto carve_ret = vertical_carving(img, scale);
        Image out = carve_ret.first;
        Image seam = carve_ret.second;
    }
    else if (which_axis == 'w') {
        auto carve_ret = horizontal_carving(img, scale);
        Image out = carve_ret.first;
        Image seam = carve_ret.second;
    }
    else {
        std::cerr << "usage: carve.py <h/w> <scale> <image_in> <image_out> " << std::endl;
        return -1;
    }

    Image out = img;
    imsave(out_filename, out);
    return 0;
}
