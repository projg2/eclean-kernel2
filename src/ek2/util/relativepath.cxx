/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "ek2/util/relativepath.h"

#include <stdexcept>

extern "C"
{
#	include <fcntl.h>
#	include <unistd.h>
};

RelativePath::RelativePath(int dir_fd, std::string&& filename)
	: dir_fd_(dir_fd), filename_(filename), file_fd_(-1)
{
#if !defined(HAVE_OPENAT)
	assert(dir_fd_ == -1);
#endif
}

RelativePath::~RelativePath()
{
	if (file_fd_ != -1)
		close(file_fd_);
}

int RelativePath::file_fd(int flags)
{
	if (file_fd_ != -1 && flags != open_mode_)
		throw std::runtime_error("Reopening not implemented yet");

	if (dir_fd_ == -1)
		return ::open(filename_.c_str(), flags);
#if defined(HAVE_OPENAT)
	return openat(dir_fd_, filename_.c_str(), flags);
#endif
}
