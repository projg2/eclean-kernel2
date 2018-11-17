/* eclean-kernel2
 * (c) 2017 Michał Górny
 * 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "ek2/judges/keepnewest.h"

#include <cassert>
#include <algorithm>

KeepNewest::KeepNewest(const Options& opts)
	: Judge(opts)
{
}

std::unique_ptr<Judge> KeepNewest::construct(const Options& opts)
{
	return std::unique_ptr<Judge>{
		static_cast<Judge*>(new KeepNewest(opts))};
}

void KeepNewest::judge(fileset_vote_map& fileset_map,
			file_vote_map&) const
{
	assert(opts_.keep_newest >= 0);
	auto is_for_removal = [](FileSetVoteSet& fsv)
	{
		return !fsv.votes.empty() && fsv.votes.back().remove;
	};
	int to_remove = fileset_map.size() - count_if(fileset_map.begin(), fileset_map.end(), is_for_removal) - opts_.keep_newest;

	for (FileSetVoteSet& fsv : fileset_map)
	{
		// skip kernels already listed for removal
		if (is_for_removal(fsv))
			continue;

		if (to_remove > 0)
		{
			fsv.votes.emplace_back(true, "old");
			--to_remove;
		}
		else
			fsv.votes.emplace_back(false, "one of the newest kernels");
	}
	assert(to_remove == 0);
}
