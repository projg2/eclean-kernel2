/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#pragma once

#ifndef EK2_FILESET_H
#define EK2_FILESET_H 1

#include "ek2/file.h"

#include <memory>
#include <vector>

// A set of associated files, i.e. corresponding to the same kernel.
class FileSet
{
public:
	typedef std::vector<std::shared_ptr<File>> file_vector_type;

private:
	file_vector_type files_;

public:
	// get the mutable file vector
	file_vector_type& files();
	const file_vector_type& files() const;
};

#endif /*EK2_FILESET_H*/
