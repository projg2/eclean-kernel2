/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#pragma once

#ifndef EK2_UTIL_RELATIVEPATH_H
#define EK2_UTIL_RELATIVEPATH_H 1

#include "ek2/util/directorystream.h"

#include <memory>
#include <string>

extern "C"
{
#	include <sys/stat.h>
};

// Open fd wrapper
class OpenFD
{
	int fd_;

public:
	OpenFD(int fd);
	~OpenFD();

	// prevent copying the fd, allow moving
	OpenFD(const OpenFD&) = delete;
	OpenFD(OpenFD&&);
	OpenFD& operator=(const OpenFD&) = delete;
	OpenFD& operator=(OpenFD&&);

	operator int() const;
};

// Path relative to an open directory
// (on systems not supporting *at() functions, absolute path is used)
class RelativePath
{
	std::shared_ptr<DirectoryStream> dir_;
	std::string filename_;

	OpenFD file_fd_;
	int open_mode_;

public:
	// create a new relative path
	// the path will be relative to the associated DirectoryStream
	// and reference the file named by filename
	RelativePath(std::shared_ptr<DirectoryStream> dir,
			std::string&& filename);

	// get the filename
	std::string filename() const;
	// get the full path
	std::string path() const;

	// return the shared fd for file open in given mode
	// if the file is not open yet, it is opened
	// if it is open in a weaker mode, it is reopened
	int file_fd(int flags);

	// stat the file
	// if the file is open, fstat() is used
	// if it is not, fstatat() is used instead
	struct stat stat() const;

	// read the symbolic link
	std::string readlink() const;
};

#endif /*EK2_UTIL_RELATIVEPATH_H*/
