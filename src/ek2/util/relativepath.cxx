/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "ek2/util/relativepath.h"

#include "ek2/util/error.h"

#include <cerrno>
#include <stdexcept>

extern "C"
{
#	include <fcntl.h>
#	include <unistd.h>
};

OpenFD::OpenFD(int fd)
	: fd_(fd)
{
}

OpenFD::~OpenFD()
{
	if (fd_ != -1)
		close(fd_);
}

OpenFD::OpenFD(OpenFD&& other)
	: fd_(other.fd_)
{
	other.fd_ = -1;
}

OpenFD& OpenFD::operator=(OpenFD&& other)
{
	fd_ = other.fd_;
	other.fd_ = -1;
	return *this;
}

OpenFD::operator int() const
{
	return fd_;
}

RelativePath::RelativePath(int dir_fd, std::string&& filename)
	: dir_fd_(dir_fd), filename_(filename), file_fd_(-1)
{
#if !defined(HAVE_OPENAT)
	assert(dir_fd_ == -1);
#endif
}

std::string RelativePath::filename() const
{
	std::string::size_type slash_pos = filename_.rfind('/');

	if (slash_pos != filename_.npos)
		return filename_.substr(slash_pos+1);
	return filename_;
}

int RelativePath::file_fd(int flags)
{
	if (file_fd_ != -1 && flags != open_mode_)
		throw Error("Reopening not implemented yet");

	if (file_fd_ == -1)
	{
		if (dir_fd_ == -1)
			file_fd_ = ::open(filename_.c_str(), flags);
		else
		{
#if defined(HAVE_OPENAT)
			file_fd_ = openat(dir_fd_, filename_.c_str(), flags);
#else
			assert(0 && "dir_fd_ != -1 but no openat()");
#endif
		}

		if (file_fd_ == -1)
			throw IOError("Unable to open " + filename_, errno);

		open_mode_ = flags;
	}

	return file_fd_;
}

struct stat RelativePath::stat() const
{
	int ret;
	struct stat buf;

	if (file_fd_ != -1)
		ret = fstat(file_fd_, &buf);
	else
	{
		if (dir_fd_ == -1)
		{
#if defined(HAVE_LSTAT)
			ret = lstat(filename_.c_str(), &buf);
#else
			ret = ::stat(filename_.c_str(), &buf);
#endif
		}
		else
		{
#if defined(HAVE_FSTATAT)
			ret = fstatat(dir_fd_, filename_.c_str(), &buf,
					AT_SYMLINK_NOFOLLOW);
#else
			assert(0 && "dir_fd_ != -1 but no fstatat()");
#endif
		}
	}

	if (ret == -1)
		throw IOError("Unable to stat " + filename_, errno);

	return buf;
}
