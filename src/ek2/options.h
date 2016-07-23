/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#pragma once

#ifndef EK2_OPTIONS_H
#define EK2_OPTIONS_H 1

#include <string>

// Common options used by various modules
struct Options
{
	std::string boot_path;
	std::string module_path;
};

#endif /*EK2_OPTIONS_H*/
