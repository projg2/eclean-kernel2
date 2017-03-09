/* eclean-kernel2
 * (c) 2017 Michał Górny
 * 2-clause BSD license
 */

#pragma once

#ifndef EK2_BOOTLOADERS_H
#define EK2_BOOTLOADERS_H 1

#include "ek2/bootloader.h"
#include "ek2/options.h"

#include <memory>
#include <vector>

// Get supported bootloaders
std::vector<std::unique_ptr<BootLoader>> get_bootloaders(const Options& opts);

#endif /*EK2_BOOTLOADERS_H*/
