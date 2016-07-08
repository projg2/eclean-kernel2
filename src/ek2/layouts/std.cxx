/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "ek2/layouts/std.h"

#include "ek2/files.h"

#include <iostream>
#include <memory>

std::unique_ptr<Layout> StdLayout::construct()
{
	return std::unique_ptr<Layout>{static_cast<Layout*>(new StdLayout())};
}

bool StdLayout::find_kernels()
{
	const std::string boot_path{"/boot"};

	dir_.open(boot_path);
	while (dir_.read())
	{
		// skip ., .. and all hidden files
		if (dir_.filename()[0] == '.')
			continue;
		// skip directories, symlinks etc.
		if (!dir_.is_regular_file())
			continue;

		std::shared_ptr<File> f;
		f = get_file_by_magic(dir_.relative_path());
		if (f)
			std::cerr << dir_.path() << std::endl;
	}
	dir_.close();

	return true;
}
