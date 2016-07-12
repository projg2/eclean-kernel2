/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#pragma once

#ifndef EK2_FILES_KERNELFILE_H
#define EK2_FILES_KERNELFILE_H 1

#include "ek2/file.h"
#include "ek2/util/relativepath.h"

#include <memory>
#include <string>

// a kernel file
class KernelFile : public File
{
	std::string version_;

public:
	KernelFile(RelativePath&& path, std::string&& version);

	// get the internal kernel version
	virtual const std::string& version() const;

	// allocate and construct a new instance if the file format
	// is supported
	// returns nullptr if it is not
	static std::shared_ptr<File> try_construct(RelativePath& path);
};

#endif /*EK2_FILES_KERNELFILE_H*/
