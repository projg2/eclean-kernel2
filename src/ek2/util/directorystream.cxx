/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "ek2/util/directorystream.h"

#include <cassert>
#include <cerrno>

extern "C"
{
#	include <sys/types.h>
#	include <sys/stat.h>
#	include <dirent.h>
#	include <fcntl.h> // AT_*
#	include <unistd.h>
};

DirectoryStream::DirectoryStream()
	: dir_(nullptr)
{
}

DirectoryStream::DirectoryStream(const char* path)
	: dir_(opendir(path)), err_(errno), path_(path)
{
}

DirectoryStream::~DirectoryStream()
{
	if (is_open())
		close();
}

bool DirectoryStream::open(const char* path)
{
	assert(!is_open());
	path_ = path;
	dir_ = opendir(path);
	if (!dir_)
		err_ = errno;
	return is_open();
}

bool DirectoryStream::close()
{
	DIR* dir = dir_;
	assert(is_open());
	// always set to null, otherwise we'd be calling close() again
	// in destructor if error occurred
	dir_ = nullptr;
	path_.clear();

	if (closedir(dir) != 0)
	{
		err_ = errno;
		return false;
	}
	else
		return true;
}

bool DirectoryStream::read()
{
	assert(is_open());
	// needed to distinguish EOF & error
	errno = 0;
	ent_ = readdir(dir_);
	err_ = errno;
	return good();
}

bool DirectoryStream::is_open() const
{
	return dir_ != nullptr;
}

bool DirectoryStream::eof() const
{
	return !good() && err_ == 0;
}

bool DirectoryStream::bad() const
{
	return !good() && err_ != 0;
}

bool DirectoryStream::good() const
{
	assert(is_open());
	return ent_ != nullptr;
}

int DirectoryStream::error() const
{
	return err_;
}

std::string DirectoryStream::filename() const
{
	assert(good());
	return ent_->d_name;
}

std::string DirectoryStream::path() const
{
	return path_ + "/" + filename();
}

bool DirectoryStream::is_regular_file() const
{
	assert(good());
#if defined(HAVE_STRUCT_DIRENT_D_TYPE)
	switch (ent_->d_type)
	{
		case DT_REG:
			return true;
		default:
			return false;
		case DT_UNKNOWN:
			;
	}
#endif

	struct stat st;
	int ret;

#if defined(HAVE_FSTATAT)
	ret = fstatat(dirfd(dir_), ent_->d_name, &st, AT_SYMLINK_NOFOLLOW);
#elif defined(HAVE_LSTAT)
	ret = lstat(path(), &st);
#else
	ret = stat(path());
#endif
	if (ret != 0)
	{
		// TODO: better error handling
		// (assume not a regular file -- rather a broken thingie)
		return false;
	}

	return S_ISREG(st.st_mode);
}
