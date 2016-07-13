/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "ek2/actions.h"

#include <iostream>

void list_kernels(const Layout& l)
{
	for (const FileSet& fs : l.kernels())
	{
		if (!fs.apparent_version().empty())
			std::cerr << fs.apparent_version() << ":\n";
		else
			std::cerr << "[" << fs.internal_version() << "]:\n";

		for (const std::shared_ptr<File>& f : fs.files())
			std::cerr << "- " << f->filename() << "\n";
	}
}
