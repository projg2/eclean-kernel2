/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "ek2/files/builddir.h"

BuildDir::BuildDir(std::shared_ptr<RelativePath> path)
	: Directory(path)
{
}

const std::string& BuildDir::type() const
{
	static const std::string type_{"build"};
	return type_;
}

std::shared_ptr<File> BuildDir::try_construct(
		std::shared_ptr<RelativePath> path)
{
	return std::unique_ptr<File>{static_cast<File*>(
			new BuildDir(path))};
}
