/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "ek2/sorts.h"

#include <functional>
#include <string>
#include <unordered_map>

typedef std::unordered_map<std::string, fileset_sorting_function>
sort_getters_type;

static bool mtime_less(const FileSet& a, const FileSet& b)
{
	return a.mtime() < b.mtime();
}

static sort_getters_type sort_getters = {
	{ "mtime", mtime_less },
};

fileset_sorting_function get_sorting_function(const std::string& name)
{
	sort_getters_type::const_iterator i = sort_getters.find(name);
	if (i != sort_getters.end())
		return i->second;

	return {};
}
