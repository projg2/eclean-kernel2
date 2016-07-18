/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#pragma once

#ifndef EK2_UTIL_DIRECTORY_H
#define EK2_UTIL_DIRECTORY_H 1

#include <memory>
#include <string>

extern "C"
{
#	include <sys/types.h>
#	include <dirent.h>
};

// forward declaration
class RelativePath;

// A wrapper for open directory stream. Provides RAII- and OO-friendly
// stream interface and a nice portability layer.
class DirectoryStream
{
	DIR* dir_;
	const struct dirent* ent_;
	std::string path_;

	friend class RelativePath;

public:
	// create a (closed) directory stream
	DirectoryStream();
	// create a directory stream and call open()
	DirectoryStream(const std::string& path);
	// create a directory stream using relative path
	// i.e. open a subdirectory
	DirectoryStream(const RelativePath& rpath);
	// destroy the directory stream
	// closes if it necessary; however, prefer calling close() explicitly
	// since this does not provide a way to check for errors
	~DirectoryStream();

	// == basic stream control ==

	// open a directory stream
	// the stream must not be open yet
	// throws IOError on error
	void open(const std::string& path);
	// close the directory stream
	// throws IOError on error
	void close();
	// read next file entry
	// returns true if entry was read, false on eof
	// throws IOError on error
	bool read();

	// == current stream entry getters ==
	// (valid only if !eof() and !bad())

	// filename of the current file
	std::string filename() const;
	// absolute path to the current file
	std::string path() const;
	// check if the current file is a regular file (not a dir, symlink...)
	bool is_regular_file() const;
	// check if the current file is a regular dir (not a file, symlink...)
	bool is_regular_directory() const;
};

#endif /*EK2_UTIL_DIRECTORY_H*/
