/* eclean-kernel2
 * (c) 2017 Michał Górny
 * 2-clause BSD license
 */

#pragma once

#ifndef EK2_JUDGES_KEEPNEWEST_H
#define EK2_JUDGES_KEEPNEWEST_H 1

#include "ek2/judge.h"

class KeepNewest : public Judge
{
public:
	KeepNewest(const Options& opts);
	static std::unique_ptr<Judge> construct(const Options& opts);

	virtual void judge(fileset_vote_map& fileset_map) const;
};

#endif /*EK2_JUDGES_KEEPNEWEST_H*/
