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

File::File(std::shared_ptr<RelativePath> path)
	: path_(path)
{
}

std::string File::filename() const
{
	return path_->filename();
}

std::string File::path() const
{
	return path_->path();
}

const std::string& File::version() const
{
	static const std::string empty;
	return empty;
}

FileID File::id() const
{
	return path_->id();
}

time_t File::mtime() const
{
	return path_->stat().st_mtime;
}
