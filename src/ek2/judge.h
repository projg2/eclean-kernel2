/* eclean-kernel2
 * (c) 2017 Michał Górny
 * 2-clause BSD license
 */

#pragma once

#ifndef EK2_JUDGE_H
#define EK2_JUDGE_H 1

#include "ek2/file.h"
#include "ek2/fileset.h"
#include "ek2/layout.h"
#include "ek2/options.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

// a single vote upon a kernel or a file
struct Vote
{
	// true to remove, false to keep
	bool remove;
	std::string reason;

	Vote(bool new_remove, const std::string& new_reason)
		: remove(new_remove), reason(new_reason)
	{
	}
};

// a complete set of votes on a kernel
struct FileSetVoteSet
{
	FileSet* key;
	std::vector<Vote> votes;

	FileSetVoteSet(FileSet* new_key)
		: key(new_key)
	{
	}
};

typedef std::vector<FileSetVoteSet> fileset_vote_map;
typedef std::unordered_map<FileID, std::vector<Vote>> file_vote_map;

class Judge
{
protected:
	const Options& opts_;
	Judge(const Options& opts);

public:
	virtual void judge(fileset_vote_map& fileset_map,
			file_vote_map& file_map) const = 0;
};

#endif /*EK2_JUDGE_H*/
