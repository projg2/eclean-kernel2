/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "ek2/files.h"

#include "ek2/files/kernelfile.h"

#include <array>
#include <functional>
#include <memory>

#include <cassert>

typedef std::function<std::shared_ptr<File>(RelativePath&)>
	file_construct_type;

static const std::array<file_construct_type, 1> file_ctors{
	KernelFile::construct,
};

std::shared_ptr<File> get_file_by_magic(RelativePath&& path)
{
	std::shared_ptr<File> ret;

	for (const file_construct_type& f : file_ctors)
	{
		ret = f(path);
		if (ret)
			return ret;
	}

	return nullptr;
}
