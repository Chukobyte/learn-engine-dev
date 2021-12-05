#ifndef COMPONENT_H
#define COMPONENT_H

#include <bitset>

const std::uint32_t MAX_COMPONENT_TYPES = 32;

using ComponentType = std::uint32_t;
using ComponentSignature = std::bitset<MAX_COMPONENT_TYPES>;

#endif //COMPONENT_H
