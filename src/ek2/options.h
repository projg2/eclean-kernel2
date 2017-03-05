/* eclean-kernel2
 * (c) 2017 Michał Górny
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

	bool pretend;
	// newest kernels to keep, 0 to disable removing old
	int keep_newest;
};

#endif /*EK2_OPTIONS_H*/
