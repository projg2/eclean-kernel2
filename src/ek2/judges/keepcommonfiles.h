/* eclean-kernel2
 * (c) 2017 Michał Górny
 * 2-clause BSD license
 */

#pragma once

#ifndef EK2_JUDGES_KEEPCOMMONFILES_H
#define EK2_JUDGES_KEEPCOMMONFILES_H 1

#include "ek2/judge.h"

class KeepCommonFiles : public Judge
{
public:
	KeepCommonFiles(const Options& opts);
	static std::unique_ptr<Judge> construct(const Options& opts);

	virtual void judge(fileset_vote_map& fileset_map,
			file_vote_map& file_map) const;
};

#endif /*EK2_JUDGES_KEEPCOMMONFILES_H*/
