/* eclean-kernel2
 * (c) 2017 Michał Górny
 * 2-clause BSD license
 */

#pragma once

#ifndef EK2_JUDGES_CLEANSTALE_H
#define EK2_JUDGES_CLEANSTALE_H 1

#include "ek2/judge.h"

// Clean filesets consisting purely of stale files, e.g. modules without
// a matching kernel
class CleanStale : public Judge
{
public:
	CleanStale(const Options& opts);
	static std::unique_ptr<Judge> construct(const Options& opts);

	virtual void judge(fileset_vote_map& fileset_map,
			file_vote_map&) const;
};

#endif /*EK2_JUDGES_CLEANSTALE_H*/
