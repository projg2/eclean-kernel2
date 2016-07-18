/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "ek2/files/modulesdir.h"

#include "ek2/util/directorystream.h"
#include "ek2/util/error.h"

#include <memory>

ModulesDir::ModulesDir(std::shared_ptr<RelativePath> path)
	: Directory(path)
{
}

const std::string& ModulesDir::type() const
{
	static const std::string type_{"modules"};
	return type_;
}

std::shared_ptr<RelativePath> ModulesDir::build_path()
{
	std::shared_ptr<DirectoryStream> ds{
		new DirectoryStream(*path_)};
	RelativePath rp{ds, "build"};
	std::shared_ptr<RelativePath> ret;

	try
	{
		ret.reset(new RelativePath(ds, rp.readlink()));
	}
	catch (IOError& e)
	{
		if (e.err() != ENOENT)
			throw;
	}

	return ret;
}

std::shared_ptr<File> ModulesDir::try_construct(
		std::shared_ptr<RelativePath> path)
{
	return std::unique_ptr<File>{static_cast<File*>(
			new ModulesDir(path))};
}
