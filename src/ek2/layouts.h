/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#pragma once

#ifndef EK2_LAYOUTS_H
#define EK2_LAYOUTS_H 1

#include "ek2/layout.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>

// Get layout by name
std::unique_ptr<Layout> get_layout(const std::string& name);
// Get available layouts with descriptions
std::vector<std::pair<std::string, std::string>> get_layout_list();

#endif /*EK2_LAYOUTS_H*/
