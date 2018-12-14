#include "processing.h"

#include <vector>

#include "image.h"
#include "inc/Eigen/Dense"

// 重命名Mtype？
Mtype 
calc_energy(Image const & img)
{
    ;
    
}

void
minimal_seam(Image const & energy_map, Mtype & M, std::vector<int> track)
{
    ;
}

Image process_seam(Image const & original, std::vector<std::vector<int> > removed)
{
    ;
}

Mtype 
convole(Mtype const & img, Mtype const & kern)
{
    ;
}

Image
convole(Image const & img, Image const &, kern)
{
    ;
}

Image
horizontal_carving(Image const & image, double const scale)
{
    ;
}

Image
vertical_carving(Image const & image, double const scale)
{

}
