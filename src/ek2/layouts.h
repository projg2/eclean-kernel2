/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#pragma once

#ifndef EK2_LAYOUTS_H
#define EK2_LAYOUTS_H 1

#include "ek2/layout.h"

#include <memory>

// Get layout by name
std::unique_ptr<Layout> get_layout(const std::string& name);

#endif /*EK2_LAYOUTS_H*/
