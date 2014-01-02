#include "Point.h"

namespace Nearest{

double give_key(Point const &a)
{
    int abs_x = (a.x >= 0 ? a.x : -a.x);
    int abs_y = (a.y >= 0 ? a.y : -a.y);

    int fst = (abs_x >= abs_y ? abs_x : abs_y);
    int snd = (abs_x >= abs_y ? abs_y : abs_x);

    double key = fst * fst + snd;
    double sign = 4 * (a.x > 0 ? 3 : (a.x < 0 ? 2 : 1));
    sign += (a.y > 0 ? 3 : (a.y < 0 ? 2 : 1));
    sign -= 1;

    return (key + (sign / 16));
}

}
