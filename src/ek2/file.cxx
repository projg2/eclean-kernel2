/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "ek2/file.h"

#include <cassert>

extern "C"
{
#	include <sys/stat.h>
};

File::File(RelativePath&& path)
	: path_(std::move(path))
{
}

std::string File::filename() const
{
	return path_.filename();
}

const std::string& File::version() const
{
	static const std::string empty;
	return empty;
}

time_t File::mtime() const
{
	struct stat st{path_.stat()};

	return st.st_mtime;
}
