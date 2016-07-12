/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "ek2/file.h"

#include <cassert>

File::File(RelativePath&& path)
	: path_(std::move(path))
{
}

std::string File::filename() const
{
	return path_.filename();
}
