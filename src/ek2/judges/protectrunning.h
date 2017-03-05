/* eclean-kernel2
 * (c) 2017 Michał Górny
 * 2-clause BSD license
 */

#pragma once

#ifndef EK2_JUDGES_PROTECTRUNNING_H
#define EK2_JUDGES_PROTECTRUNNING_H 1

#include "ek2/judge.h"

// Protect the currently running kernel from being removed
// (most importantly, having its modules removed)
class ProtectRunning : public Judge
{
public:
	ProtectRunning(const Options& opts);
	static std::unique_ptr<Judge> construct(const Options& opts);

	virtual void judge(fileset_vote_map& fileset_map,
			file_vote_map&) const;
};

#endif /*EK2_JUDGES_PROTECTRUNNING_H*/
