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
