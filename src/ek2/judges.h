/* eclean-kernel2
 * (c) 2017 Michał Górny
 * 2-clause BSD license
 */

#pragma once

#ifndef EK2_JUDGES_H
#define EK2_JUDGES_H 1

#include "ek2/judge.h"
#include "ek2/options.h"

#include <memory>
#include <vector>

// Get enabled judges
std::vector<std::unique_ptr<Judge>> get_judges(const Options& opts);

#endif /*EK2_JUDGES_H*/
