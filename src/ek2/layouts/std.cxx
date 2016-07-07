/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "ek2/layouts/std.h"

#include <iostream>

#include <cstring>

std::unique_ptr<Layout> StdLayout::construct()
{
	return std::unique_ptr<Layout>{static_cast<Layout*>(new StdLayout())};
}

bool StdLayout::find_kernels()
{
	const char* boot_path = "/boot";

	if (!dir_.open(boot_path))
	{
		std::cerr << "Unable to open " << boot_path << ": "
			<< strerror(dir_.error()) << std::endl;
		return false;
	}

	while (dir_.read())
	{
		// skip ., .. and all hidden files
		if (dir_.filename()[0] == '.')
			continue;
		// skip directories, symlinks etc.
		if (!dir_.is_regular_file())
			continue;

		std::cerr << dir_.path() << std::endl;
	}

	if (dir_.bad())
	{
		std::cerr << "readdir() failed on " << boot_path << ": "
			<< strerror(dir_.error()) << std::endl;
	}

	if (!dir_.close())
		std::cerr << "Warning: closedir() failed on " << boot_path << ": "
			<< strerror(dir_.error()) << std::endl;

	return true;
}
