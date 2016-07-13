/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "ek2/fileset.h"

FileSet::file_vector_type& FileSet::files()
{
	return files_;
}

const FileSet::file_vector_type& FileSet::files() const
{
	return files_;
}

const std::string& FileSet::apparent_version() const
{
	return apparent_version_;
}

void FileSet::apparent_version(const std::string& val)
{
	apparent_version_ = val;
}

const std::string& FileSet::internal_version() const
{
	return internal_version_;
}

void FileSet::internal_version(const std::string& val)
{
	internal_version_ = val;
}
