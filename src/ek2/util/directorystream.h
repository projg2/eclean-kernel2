/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#pragma once

#ifndef EK2_UTIL_DIRECTORY_H
#define EK2_UTIL_DIRECTORY_H 1

#include <string>

extern "C"
{
#	include <sys/types.h>
#	include <dirent.h>
};

// A wrapper for open directory stream. Provides RAII- and OO-friendly
// stream interface and a nice portability layer.
class DirectoryStream
{
	DIR* dir_;
	const struct dirent* ent_;
	int err_;
	std::string path_;

public:
	// create a (closed) directory stream
	DirectoryStream();
	// create a directory stream and call open()
	// make sure to check is_open() to verify that the attempt succeeded
	DirectoryStream(const char* path);
	// destroy the directory stream
	// closes if it necessary; however, prefer calling close() explicitly
	// since this does not provide a way to check for errors
	~DirectoryStream();

	// == basic stream control ==

	// open a directory stream
	// the stream must not be open yet
	// returns true on success, false on error
	bool open(const char* path);
	// close the directory stream
	// returns true on success, false on error
	bool close();
	// read next file entry
	// returns true if entry was read, false on eof or error
	// (check eof() for EOF, bad() for error)
	bool read();

	// == stream state getters ==

	// check if directory stream has been opened successfully
	// (if false post open(), then an error occured)
	bool is_open() const;
	// check if last read() resulted in an EOF
	bool eof() const;
	// check if last read() resulted in an error
	bool bad() const;
	// check if last read() was good, i.e. no error and no EOF happened
	bool good() const;
	// return the stored errno of last stream I/O call
	// if is_open() is false after open(), contains opendir() errno
	// if bad() is true, contains readdir() errno
	// if close() returned false, contains closedir() errno
	int error() const;

	// == current stream entry getters ==
	// (valid only if !eof() and !bad())

	// filename of the current file
	std::string filename() const;
	// full path to the current file
	std::string path() const;
	// check if the current file is a regular file (not a dir, symlink...)
	bool is_regular_file() const;
};

#endif /*EK2_UTIL_DIRECTORY_H*/
