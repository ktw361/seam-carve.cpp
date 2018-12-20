#pragma once 

#include <utility>

#include "image.h"
#include "inc/Eigen/Dense"

//typedef Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> Mtype;
//typedef float Dtype;

Image calc_energy(Image const &);
Image process_seam(Image const & original, MIndtype removed);
std::pair<Image, MIndtype> minimal_seam(Image const &);
std::pair<Image, std::vector<Indtype> > carve_one_column(Image const &);

//Mtype convolve(Mtype const &, Mtype const &);
Image convolve(Image const &, Image const &);

std::pair<Image, Image> horizontal_carving(Image const &, double const scale);
std::pair<Image, Image> vertical_carving(Image const &, double const scale);
