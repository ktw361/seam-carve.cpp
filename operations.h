#pragma once

#include <vector>

#include "image.h"
#include "inc/Eigen/Dense"

Image
absolute(Image const & img);

Image 
channel_sum(Image const & img);

Image
rotate90aclk(Image const & img);

Image
rotate90clk(Image const & img);

Image 
Mrepeat(Mtype const & mat); 

Image 
Mrepeat(Image const & img); 

//template<typename T, typename retType>
//retType
//asMtype(std::vector<T> vec, int h, int w);
// 模板怎么不行？
MIndtype 
asMtype(std::vector<Indtype> vec, int h, int w);

enum ConvMode { ZeroPad, Reflect };

Mtype 
make_padding(Mtype const & mat, Mtype const & kern, ConvMode mode = Reflect);

// Classical conv: filter flipped 
Mtype 
convolve(Mtype const &, Mtype const &, ConvMode = Reflect);

// Same filter conv for 'channels time'
Image 
convolve(Image const &, Image const &, ConvMode = Reflect);
