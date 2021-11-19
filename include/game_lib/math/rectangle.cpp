#include "rectangle.h"

std::ostream& operator<<(std::ostream& os, const Rectangle &r) {
    os << "(" << r.x << ", " << r.y << ", " << r.w << ", " << r.h << ")";
    return os;
}
