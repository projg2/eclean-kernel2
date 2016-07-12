/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "ek2/files/modulesdir.h"

ModulesDir::ModulesDir(RelativePath&& path)
	: File(std::move(path))
{
}

std::shared_ptr<File> ModulesDir::try_construct(RelativePath& path)
{
	return std::unique_ptr<File>{static_cast<File*>(
			new ModulesDir(std::move(path)))};
}
