/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#pragma once

#ifndef EK2_FILESET_H
#define EK2_FILESET_H 1

#include "ek2/file.h"

#include <memory>
#include <string>
#include <vector>

#include <ctime>

// A set of associated files, i.e. corresponding to the same kernel.
class FileSet
{
public:
	typedef std::vector<std::shared_ptr<File>> file_vector_type;

private:
	file_vector_type files_;
	std::string apparent_version_;
	std::string internal_version_;

public:
	// get the mutable file vector
	file_vector_type& files();
	const file_vector_type& files() const;

	// get or set the apparent version
	const std::string& apparent_version() const;
	void apparent_version(const std::string& val);

	// get or set the internal version
	const std::string& internal_version() const;
	void internal_version(const std::string& val);

	// get mtime (for sorting)
	time_t mtime() const;
};

#endif /*EK2_FILESET_H*/
