/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "ek2/files/genericfile.h"

GenericFile::GenericFile(std::shared_ptr<RelativePath> path)
	: File(path)
{
}

const std::string& GenericFile::type() const
{
	static const std::string type_{"aux"};
	return type_;
}

std::shared_ptr<File> GenericFile::try_construct(
		std::shared_ptr<RelativePath> path)
{
	return std::unique_ptr<File>{static_cast<File*>(
			new GenericFile(path))};
}
