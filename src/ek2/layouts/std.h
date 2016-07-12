/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#pragma once

#ifndef EK2_LAYOUTS_STD_H
#define EK2_LAYOUTS_STD_H 1

#include "ek2/fileset.h"
#include "ek2/layout.h"
#include "ek2/util/directorystream.h"

#include <memory>
#include <unordered_map>

// Standard flat /boot used for a single system
class StdLayout : public Layout
{
	// keep /boot open for relative actions
	DirectoryStream dir_;

	std::unordered_map<std::string, FileSet> file_map_;

public:
	// allocate and construct a new StdLayout
	static std::unique_ptr<Layout> construct();

	virtual bool find_kernels();
};

#endif /*EK2_LAYOUTS_STD_H*/
