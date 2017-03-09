/* eclean-kernel2
 * (c) 2017 Michał Górny
 * 2-clause BSD license
 */

#pragma once

#ifndef EK2_ACTIONS_H
#define EK2_ACTIONS_H 1

#include "ek2/layout.h"
#include "ek2/bootloader.h"

#include <memory>
#include <vector>

void list_kernels(const Layout& l);
void remove(Layout& l, const Options& opts,
		std::vector<std::unique_ptr<BootLoader>>& bootloaders);

#endif /*EK2_ACTIONS_H*/
