/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#pragma once

#ifndef EK2_SORTS_H
#define EK2_SORTS_H 1

#include "ek2/layout.h"

#include "ek2/fileset.h"

#include <functional>
#include <string>

typedef std::function<bool(const FileSet&, const FileSet&)>
	fileset_sorting_function;

fileset_sorting_function get_sorting_function(const std::string& name);

#endif /*EK2_SORTS_H*/
