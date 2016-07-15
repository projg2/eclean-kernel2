/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#pragma once

#ifndef EK2_FILES_MODULESDIR_H
#define EK2_FILES_MODULESDIR_H 1

#include "ek2/file.h"
#include "ek2/util/relativepath.h"

#include <memory>

// a module directory for the kernel
class ModulesDir : public File
{
public:
	ModulesDir(RelativePath&& path);

	virtual const std::string& type() const;

	// allocate and construct a new instance
	// always succeeds
	static std::shared_ptr<File> try_construct(RelativePath& path);
};

#endif /*EK2_FILES_MODULESDIR_H*/
