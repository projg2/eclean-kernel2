/* eclean-kernel2
 * (c) 2017 Michał Górny
 * 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "ek2/judges/keepcommonfiles.h"

#include "ek2/util/error.h"

#include <sstream>

KeepCommonFiles::KeepCommonFiles(const Options& opts)
	: Judge(opts)
{
}

std::unique_ptr<Judge> KeepCommonFiles::construct(const Options& opts)
{
	return std::unique_ptr<Judge>{
		static_cast<Judge*>(new KeepCommonFiles(opts))};
}

void KeepCommonFiles::judge(fileset_vote_map& fileset_map,
			file_vote_map& file_map) const
{
	typedef std::unordered_map<FileID, std::vector<std::string>>
		file_use_map_type;
	file_use_map_type file_use_map;

	// first, collect all files to keep with all kernels using them
	for (const FileSetVoteSet& fsv : fileset_map)
	{
		if (fsv.votes.empty() || !fsv.votes.back().remove)
		{
			std::string v{fsv.key->pretty_version()};
			// store list of files to keep w/ reasons
			for (const std::shared_ptr<File>& f : fsv.key->files())
			{
				try
				{
					file_use_map[f->id()].push_back(v);
				}
				catch (const IOError& e)
				{
					// skip missing files
					if (e.err() != ENOENT)
						throw;
				}
			}
		}
	}

	// now, convert that into a nice single vote
	for (const file_use_map_type::value_type& file_used : file_use_map)
	{
		std::stringstream uses;
		uses << "used also by: " << file_used.second.back();
		if (file_used.second.size() > 1)
			uses << " and " << file_used.second.size() - 1
				<< " more kernel(s)";
		file_map[file_used.first].emplace_back(false, uses.str());
	}
}
