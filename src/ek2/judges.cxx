/* eclean-kernel2
 * (c) 2017 Michał Górny
 * 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "ek2/judges.h"

#include "ek2/judges/keepnewest.h"

#include <functional>
#include <memory>
#include <vector>

struct judge_info
{
	std::string name;
	std::function<std::unique_ptr<Judge>(const Options& opts)> construct;
};

typedef std::vector<judge_info> judge_list_type;

static const judge_list_type judge_list = {
	{ "keep-newest", KeepNewest::construct },
};

std::vector<std::unique_ptr<Judge>> get_judges(const Options& opts)
{
	std::vector<std::unique_ptr<Judge>> ret;

	for (const judge_info& ji : judge_list)
	{
		std::unique_ptr<Judge> j = ji.construct(opts);
		if (j)
			ret.push_back(std::move(j));
	}

	return ret;
}
