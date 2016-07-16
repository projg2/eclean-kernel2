/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "ek2/actions.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

static std::string pretty_version(const FileSet& fs)
{
	if (!fs.apparent_version().empty())
		return fs.apparent_version();
	else
		return '[' + fs.internal_version() + ']';
}

void list_kernels(const Layout& l)
{
	for (const FileSet& fs : l.kernels())
	{
		std::cerr << pretty_version(fs) << ':';
		for (const std::shared_ptr<File>& f : fs.files())
			std::cerr << "- " << f->type() << ": " << f->path() << "\n";
	}
}

void keep_newest(Layout& l, int keep_num, bool pretend)
{
	int to_remove = l.kernels().size() - keep_num;

	if (to_remove > 0)
	{
		if (pretend)
			std::cerr << "The following kernels would be removed:\n";
		else
			std::cerr << "Removing...\n";

		// map of files mapped to 'old' kernels that use them
		typedef std::unordered_map<std::string, std::vector<std::string>>
			file_use_map_type;
		file_use_map_type file_use_map;

		std::vector<FileSet>::iterator first_kept_kernel
			= l.kernels().begin() + to_remove;
		for (std::vector<FileSet>::iterator it = first_kept_kernel;
				it != l.kernels().end(); ++it)
		{
			std::string v{pretty_version(*it)};
			// store list of files to keep w/ reasons
			for (const std::shared_ptr<File>& f : it->files())
				file_use_map[f->path()].push_back(v);
		}

		// remove files that we're not keeping
		for (std::vector<FileSet>::iterator it = l.kernels().begin();
				it != first_kept_kernel; ++it)
		{
			std::cerr << pretty_version(*it) << ':' << std::endl;
			for (std::shared_ptr<File>& f : it->files())
			{
				std::string path{f->path()};

				file_use_map_type::iterator
					file_used = file_use_map.find(path);
				// file used by keptkernels
				if (file_used != file_use_map.end())
				{
					std::cerr << "+ " << path << " (keeping, used also by: "
						<< file_used->second.back();
					if (file_used->second.size() > 1)
						std::cerr << " and " << file_used->second.size() - 1
							<< " other kernel(s)";
					std::cerr << ')' << std::endl;
				}
				else
				{
					std::cerr << "- " << path << std::endl;
				}
			}
		}
	}
	else
		std::cerr << "No kernels to remove\n";
}
