/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "ek2/files/genericfile.h"

GenericFile::GenericFile(RelativePath&& path)
	: File(std::move(path))
{
}

std::shared_ptr<File> GenericFile::construct(RelativePath& path)
{
	return std::unique_ptr<File>{static_cast<File*>(
			new GenericFile(std::move(path)))};
}
