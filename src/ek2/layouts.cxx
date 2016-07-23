/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "ek2/layouts.h"

#include "ek2/layouts/std.h"

#include <functional>
#include <string>
#include <unordered_map>

struct layout_info
{
	std::function<std::unique_ptr<Layout>(const Options& opts)> construct;
	std::string desc;
};

typedef std::unordered_map<std::string, layout_info> layout_list_type;

static layout_list_type layout_list = {
	{ "std", { StdLayout::construct, "Standard /boot layout used by kernel 'make install'" } },
};

std::unique_ptr<Layout> get_layout(const std::string& name, const Options& opts)
{
	layout_list_type::const_iterator i = layout_list.find(name);
	if (i != layout_list.end())
		return (i->second.construct)(opts);

	return nullptr;
}

std::vector<std::pair<std::string, std::string>> get_layout_list()
{
	std::vector<std::pair<std::string, std::string>> ret;

	for (const layout_list_type::value_type& l : layout_list)
		ret.push_back(std::make_pair(l.first, l.second.desc));

	return ret;
}
