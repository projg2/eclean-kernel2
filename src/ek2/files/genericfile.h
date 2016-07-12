/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#pragma once

#ifndef EK2_FILES_GENERICFILE_H
#define EK2_FILES_GENERICFILE_H 1

#include "ek2/file.h"
#include "ek2/util/relativepath.h"

#include <memory>

// a generic kernel-related file such as System.map or config
class GenericFile : public File
{
public:
	GenericFile(RelativePath&& path);

	// allocate and construct a new instance
	// always succeeds
	static std::shared_ptr<File> try_construct(RelativePath& path);
};

#endif /*EK2_FILES_GENERICFILE_H*/
