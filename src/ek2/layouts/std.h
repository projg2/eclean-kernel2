/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#pragma once

#ifndef EK2_LAYOUTS_STD_H
#define EK2_LAYOUTS_STD_H 1

#include "ek2/layout.h"

#include <memory>

class StdLayout : public Layout
{
public:
	// allocate and construct a new StdLayout
	static std::unique_ptr<Layout> construct();
};

#endif /*EK2_LAYOUTS_STD_H*/
