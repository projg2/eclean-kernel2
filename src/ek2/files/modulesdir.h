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
class ModulesDir : public Directory
{
public:
	ModulesDir(std::shared_ptr<RelativePath> path);

	virtual const std::string& type() const;

	// read path to build directory (if available)
	// returns nullptr if no 'build' directory is found
	std::shared_ptr<RelativePath> build_path();

	// allocate and construct a new instance
	// always succeeds
	static std::shared_ptr<File> try_construct(
			std::shared_ptr<RelativePath> path);
};

#endif /*EK2_FILES_MODULESDIR_H*/
