/* eclean-kernel2
 * (c) 2017 Michał Górny
 * 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "ek2/judges/protectrunning.h"

#include <iostream>

extern "C"
{
#	include <sys/utsname.h>
};

ProtectRunning::ProtectRunning(const Options& opts)
	: Judge(opts)
{
}

std::unique_ptr<Judge> ProtectRunning::construct(const Options& opts)
{
	return std::unique_ptr<Judge>{
		static_cast<Judge*>(new ProtectRunning(opts))};
}

void ProtectRunning::judge(fileset_vote_map& fileset_map,
			file_vote_map&) const
{
	struct utsname un;

	if (uname(&un) == -1)
	{
		std::cerr << "uname() failed, running kernel will not be protected!"
			<< std::endl;
		return;
	}

	for (FileSetVoteSet& fsv : fileset_map)
	{
		// match on internal version -- usually modules directory
		if (fsv.key->internal_version() == un.release)
		{
			fsv.votes.emplace_back(false, "currently running kernel");
			break;
		}
	}
}
