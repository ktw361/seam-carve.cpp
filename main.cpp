#include <iostream>
#include <cstdlib>
#include <string>

#include "image.h"
#include "processing.h"

int
main(int argc, char *argv[])
{
    if (argc != 5) {
        std::cerr << "usage: carvecpp <h/w> <scale> <image_in> <out_dir>" << std::endl;
        return -1;
    }
    /* argv[1] = "w"; */
    /* argv[2] = "0.99"; */
    /* argv[3] = "/home/damon/QtWorkSapce/carveCpp/imgs/scene.jpg"; */
    /* argv[4] = "/home/damon/QtWorkSapce/carveCpp/build/out.jpg"; */
    std::string out_dir(argv[4]);
    std::string seam_f = out_dir + "/seam.jpg";
    std::string out_filename = out_dir + "/out.jpg";
    const char which_axis = *argv[1];
    double scale = atof(argv[2]);
    const char *in_filename = argv[3];

    Image img = imread(in_filename);
    std::cout << "Load " << in_filename << ' ' << img.height() << ' ' << img.width() << ' ' << img.channels << std::endl;

    if (which_axis != 'h' && which_axis != 'w') {
        std::cerr << "usage: carvecpp <h/w> <scale> <image_in> <image_out> " << std::endl;
        return -1;
    }

    Image im_out(1);
    Image seam(1);
    if (which_axis == 'h') {
        auto out = vertical_carving(img, scale); 
        im_out = out.first;
        seam = out.second;
    }
    else {
        auto out = horizontal_carving(img, scale);
        im_out = out.first;
        seam = out.second;
    }
    imsave(out_filename.c_str(), im_out);
    imsave(seam_f.c_str(), seam);

    return 0;
}
