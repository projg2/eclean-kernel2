/* eclean-kernel2
 * (c) 2017 Michał Górny
 * 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "ek2/judges/cleanstale.h"

CleanStale::CleanStale(const Options& opts)
	: Judge(opts)
{
}

std::unique_ptr<Judge> CleanStale::construct(const Options& opts)
{
	return std::unique_ptr<Judge>{
		static_cast<Judge*>(new CleanStale(opts))};
}

void CleanStale::judge(fileset_vote_map& fileset_map,
			file_vote_map&) const
{
	for (FileSetVoteSet& fsv : fileset_map)
	{
		bool had_kernel = false;

		for (const std::shared_ptr<File>& f : fsv.key->files())
		{
			// TODO: provide enum API instead of string?
			if (f->type() == "kernel")
			{
				had_kernel = true;
				break;
			}
		}

		if (!had_kernel)
			fsv.votes.emplace_back(true, "stale files (no matching kernel)");
	}
}
