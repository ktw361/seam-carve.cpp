#include "processing.h"

#include <utility>
#include <vector>
#include <iterator>
#include <cmath>
//#debue
#include <iostream>

#include "image.h"
#include "operations.h"
#include "inc/Eigen/Dense"

//using namespace Eigen;
// 重命名Mtype？
Image 
calc_energy(Image const & img)
{
    Mtype kern_du(3,3);
    kern_du << 1., 2., 1.,
               0., 0., 0.,
               -1, -2, -1;
    Mtype kern_dv = kern_du.transpose();
    auto flipdg = [&](Mtype m) {
        return m.colwise().reverse().rowwise().reverse();
    };
    Image filter_du = Mrepeat(flipdg(kern_du));
    Image filter_dv = Mrepeat(flipdg(kern_dv));

    Image convolved = absolute(convolve(img, filter_du, Reflect)) + 
                      absolute(convolve(img, filter_dv, Reflect));
    Image energy_map = channel_sum(convolved);
    return energy_map;
}

std::pair<Image, MIndtype>
minimal_seam(Image const & energy_map)
{
    int h = energy_map.height(), w = energy_map.width();
    Mtype Mp = energy_map[0];
    MIndtype track =  MIndtype::Zero(h, w);

    int row_start = 1;
    Indtype ind_aux;
    for (int i = row_start; i != h; ++i) {
        int j = 0;
        Mp.block<1, 2>(i - 1, j).minCoeff(&ind_aux, &track(i ,0));
        Mp(i, 0) += Mp(i - 1, track(i, 0));

        for (j = 1; j != w - 1; ++j) {
            Mp.block<1, 3>(i - 1, j - 1).minCoeff(&ind_aux, &track(i, j));
            track(i, j) += j - 1;
            Mp(i, j) += Mp(i - 1, track(i, j));
        }

        Mp.block<1, 2>(i - 1, j - 1).minCoeff(&ind_aux, &track(i, j));
        track(i, j) += j - 1;
        Mp(i, j) += Mp(i - 1, track(i ,j));
        
    }

    return {Mp, track};
}

Image 
process_seam(Image const & original, MIndtype removed)
{
    int h = original.height();
    int num_seam = removed.cols();
    Image image_seam = original;

//    int ind_start = 1;
//    for (int i = ind_start; i != h; ++i) {
//        for (int j = 0; j != i - 1; ++j) {
//            removed.row(i) = (removed.row(i) > removed.row(j)).select(
//                    removed.row(i) + 1, 
//                    removed.row(i)
//                    );
//        }
//    }
    int ind_start = 1;
    for (int j = ind_start; j != num_seam; ++j) {
        for (int i = 0; i != j - 1; ++i) {
            removed.col(i) = (removed.col(i) >= removed.col(j)).select(
                    removed.col(i) + 1, 
                    removed.col(i)
                    );
        }
    }
    for (int i = 0; i != h; ++i) {
        for (int j = 0; j != num_seam; ++j) {
            image_seam[0](i, removed(i, j)) = 255.0;
            image_seam[1](i, removed(i, j)) = 0.0;
            image_seam[2](i, removed(i, j)) = 0.0;
        }
    }
    return image_seam;
}

std::pair<Image, std::vector<Indtype> >
carve_one_column(Image const & img)
{
    int h = img.height(), w = img.width(), c = img.channels;

    auto energy_map = calc_energy(img);
    auto seam_info = minimal_seam(energy_map);
    auto Mp = seam_info.first;
    auto track = seam_info.second;

    Image img_next(h, w - 1, c);
    Indtype pick;
    Mp[0].row(h - 1).minCoeff(&pick);
    std::vector<Indtype> pick_list = {pick};

    // 
    // A simple method
    // Compare speed???
    //
    for (int j = 0; j != w - 1; ++j) {
        img_next[0](h - 1, j) = img[0](h - 1, j >= pick ? j + 1 : j );
        img_next[1](h - 1, j) = img[1](h - 1, j >= pick ? j + 1 : j );
        img_next[2](h - 1, j) = img[2](h - 1, j >= pick ? j + 1 : j );
    }
    for (int i = h - 2; i >= 0; --i) {
        pick = track(i, pick);
        for (int j = 0; j!= w - 1; ++j) {
            img_next[0](i, j) = img[0](i, j >= pick ? j + 1 : j ); 
            img_next[1](i, j) = img[1](i, j >= pick ? j + 1 : j ); 
            img_next[2](i, j) = img[2](i, j >= pick ? j + 1 : j ); 
        }
        pick_list.push_back(pick);
    }

    return {img_next, pick_list};
}

std::pair<Image, Image>
horizontal_carving(Image const & image, double const scale)
{
    int h = image.height(), w = image.width();
    int w_finish = static_cast<Indtype>(std::round(w * scale));

    Image img_ret = image;
    std::vector<Indtype> pick_list_all;

    for (int i = 0; i != w - w_finish; ++i) {
        std::cout << " In " << i << " of " << w - w_finish<< std::endl;
        auto carve_ret = carve_one_column(img_ret);
        img_ret = carve_ret.first;
        std::vector<Indtype> pick_list = carve_ret.second;
        std::reverse(pick_list.begin(), pick_list.end());
        pick_list_all.insert(
                pick_list_all.end(),
                std::make_move_iterator(pick_list.begin()),
                std::make_move_iterator(pick_list.end())
                );
    }
    // Eigen Happens to be ColMajor !!!
    MIndtype removed = asMtype(pick_list_all, h, w - w_finish);
    Image image_seam = process_seam(image, removed);

    return {img_ret, image_seam};
}

std::pair<Image, Image>
vertical_carving(Image const & image, double const scale)
{
    Image img = rotate90aclk(image);
    // ??? 构造一个新的？
    auto hcarve_ret = horizontal_carving(img, scale);
    img = rotate90clk(hcarve_ret.first);
    Image image_seam = rotate90clk(hcarve_ret.second);

    return {img, image_seam};
}
