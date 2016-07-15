/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "ek2/util/directorystream.h"
#include "ek2/util/error.h"

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

DirectoryStream::DirectoryStream(const std::string& path)
	: dir_(opendir(path.c_str())), path_(path)
{
	if (!dir_)
		throw IOError("Unable to open directory " + path, errno);
}

DirectoryStream::~DirectoryStream()
{
	if (dir_)
		closedir(dir_);
}

void DirectoryStream::open(const std::string& path)
{
	assert(!dir_);
	path_ = path;
	dir_ = opendir(path.c_str());
	if (!dir_)
		throw IOError("Unable to open directory " + path, errno);
}

void DirectoryStream::close()
{
	DIR* dir = dir_;
	assert(dir);
	// always set to null, otherwise we'd be calling close() again
	// in destructor if error occurred
	dir_ = nullptr;

	if (closedir(dir) != 0)
		throw IOError("Unable to close directory " + path_, errno);

	path_.clear();
}

bool DirectoryStream::read()
{
	assert(dir_);
	// needed to distinguish EOF & error
	errno = 0;
	ent_ = readdir(dir_);
	if (!ent_ && errno != 0)
		throw IOError("Reading directory " + path_ + "failed", errno);
	return ent_ != nullptr;
}

std::string DirectoryStream::filename() const
{
	assert(dir_ && ent_);
	return ent_->d_name;
}

std::string DirectoryStream::path() const
{
	return path_ + "/" + filename();
}
	
// TODO: combine the two into something generic
bool DirectoryStream::is_regular_file() const
{
	assert(dir_ && ent_);
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
		throw IOError("Unable to stat file " + path(), errno);

	return S_ISREG(st.st_mode);
}

bool DirectoryStream::is_regular_directory() const
{
	assert(dir_ && ent_);
#if defined(HAVE_STRUCT_DIRENT_D_TYPE)
	switch (ent_->d_type)
	{
		case DT_DIR:
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
		throw IOError("Unable to stat file " + path(), errno);

	return S_ISDIR(st.st_mode);
}
