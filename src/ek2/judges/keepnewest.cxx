/* eclean-kernel2
 * (c) 2017 Michał Górny
 * 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "ek2/judges/keepnewest.h"

#include <cassert>

KeepNewest::KeepNewest(const Options& opts)
	: Judge(opts)
{
}

std::unique_ptr<Judge> KeepNewest::construct(const Options& opts)
{
	return std::unique_ptr<Judge>{
		static_cast<Judge*>(new KeepNewest(opts))};
}

void KeepNewest::judge(fileset_vote_map& fileset_map) const
{
	assert(opts_.keep_newest >= 0);
	int to_remove = fileset_map.size() - opts_.keep_newest;

	for (FileSetVoteSet& fsv : fileset_map)
	{
		// skip kernels already listed for removal
		if (!fsv.votes.empty() && fsv.votes.back().remove)
			continue;

		if (to_remove > 0)
		{
			fsv.votes.emplace_back(true, "old");
			--to_remove;
		}
		else
			fsv.votes.emplace_back(false, "one of the newest kernels");
	}
}
