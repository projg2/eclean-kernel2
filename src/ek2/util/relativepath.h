/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#pragma once

#ifndef EK2_UTIL_RELATIVEPATH_H
#define EK2_UTIL_RELATIVEPATH_H 1

#include <string>

// Path relative to an open directory
// (on systems not supporting *at() functions, absolute path is used)
class RelativePath
{
	int dir_fd_;
	std::string filename_;

	int file_fd_;
	int open_mode_;

public:
	// create a new relative path
	// fd specifies parent directory, filename the filename
	// if *at() is not available, fd == -1, filename is full path
	RelativePath(int dir_fd, std::string&& filename);
	~RelativePath();

	// prevent copying the fd, allow moving
	RelativePath(const RelativePath&) = delete;
	RelativePath(RelativePath&&) = default;
	RelativePath& operator=(const RelativePath&) = delete;
	RelativePath& operator=(RelativePath&&) = default;

	// return either the filename or the full path
	const std::string& filename() const;

	// return the shared fd for file open in given mode
	// if the file is not open yet, it is opened
	// if it is open in a weaker mode, it is reopened
	int file_fd(int flags);
};

#endif /*EK2_UTIL_RELATIVEPATH_H*/
