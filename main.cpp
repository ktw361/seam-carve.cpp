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
    argv[2] = "0.99";
    argv[3] = "/home/damon/QtWorkSapce/carveCpp/imgs/scene.jpg";
    argv[4] = "/home/damon/QtWorkSapce/carveCpp/build/out.jpg";
    const char which_axis = *argv[1];
    double scale = atof(argv[2]);
    const char *in_filename = argv[3];
    const char *out_filename = argv[4];

    Image img = imread(in_filename);
    std::cout << "Load " << in_filename << ' ' << img.height() << ' ' << img.width() << ' ' << img.channels << std::endl;

    if (which_axis == 'h') {
        auto out = vertical_carving(img, scale); 
        Image im_out = out.first;
        Image seam = out.second;
        imsave(out_filename, im_out);
    }
    else if (which_axis == 'w') {
        auto out = horizontal_carving(img, scale);
        Image im_out = out.first;
        Image seam = out.second;
    }
    else {
        std::cerr << "usage: carve.py <h/w> <scale> <image_in> <image_out> " << std::endl;
        return -1;
    }

    return 0;
}
