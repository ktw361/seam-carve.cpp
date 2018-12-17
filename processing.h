#pragma once 

#include <utility>

#include "image.h"
#include "inc/Eigen/Dense"

//typedef Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> Mtype;
//typedef float Dtype;

std::pair<Image, Image> horizontal_carving(Image const &, double const scale);
std::pair<Image, Image> vertical_carving(Image const &, double const scale);

Mtype calc_energy(Image const &);
std::pair<Mtype, MIndtype> minimal_seam(Mtype const &);
Image process_seam(Image const &, std::vector<std::vector<int> > );
std::pair<Image, std::vector<Indtype> > carve_one_column(Image const &);

enum ConvMode { ZeroPad, Reflect };
Mtype convolve(Mtype const &, Mtype const &, ConvMode); // convole with reflect mode
Image convolve(Image const &, Image const &, ConvMode);

