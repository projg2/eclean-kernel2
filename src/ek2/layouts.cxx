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
#include <unordered_map>

typedef std::function<std::unique_ptr<Layout>()>
layout_construct_type;
typedef std::unordered_map<std::string, layout_construct_type>
layout_getters_type;

layout_getters_type layout_getters = {
	{ "std", StdLayout::construct },
};

std::unique_ptr<Layout> get_layout(const std::string& name)
{
	layout_getters_type::const_iterator i = layout_getters.find(name);
	if (i != layout_getters.end())
		return (i->second)();

	return nullptr;
}
