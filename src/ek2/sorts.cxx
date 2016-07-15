/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "ek2/sorts.h"

#include <algorithm>
#include <functional>
#include <string>
#include <unordered_map>

#include <cassert>

typedef std::unordered_map<std::string, fileset_sorting_function>
sort_getters_type;

static bool mtime_less(const FileSet& a, const FileSet& b)
{
	return a.mtime() < b.mtime();
}

struct VersionComponent
{
	std::string::const_iterator begin, end;
	bool is_numeric;
};

// get next version component out of version string at start
// end specifies the end of string
static VersionComponent next_component(std::string::const_iterator start,
		std::string::const_iterator end)
{
	// preinitiate to empty string == no further components
	VersionComponent ret = {end, end, false};
	bool has_any = false;

	for (std::string::const_iterator pos = start; pos != end; ++pos)
	{
		bool is_num = (*pos >= '0' && *pos <= '9');
		bool is_alpha = ((*pos >= 'a' && *pos <= 'z')
				|| (*pos >= 'A' && *pos <= 'Z'));
		bool is_delim = !is_num && !is_alpha;

		// if we didn't get to the component yet...
		if (!has_any)
		{
			// skip any delimiter
			if (!is_delim)
			{
				// then record beginning and type
				ret.begin = pos;
				ret.is_numeric = is_num;
				has_any = true;
			}
			// treat + and ~ specially, return them single
			else if (*pos == '+' || *pos == '~')
			{
				ret.begin = pos;
				ret.end = pos + 1;
				return ret;
			}
		}
		else
		{
			// terminate if component type changes or another delimiter
			// is reached
			if (is_num != ret.is_numeric || is_delim)
			{
				ret.end = pos;
				break;
			}
		}
	}

	return ret;
}

static const std::unordered_map<std::string, int> component_weights{
	{"old", -1},
	{"~", -2},
	{"rc", -3},
};

static int get_component_weight(const VersionComponent& vc)
{
	// short-circuit, our special suffixes are non-numeric
	if (vc.is_numeric)
		return 0;

	std::unordered_map<std::string, int>::const_iterator it
		= component_weights.find(std::string(vc.begin, vc.end));
	if (it != component_weights.end())
		return it->second;
	return 0;
}

// compare *it with given number of padded zeros
// returns true if non-zero is found in *it (i.e. *it > 0)
static bool compare_with_padded_zeros(std::string::const_iterator& it,
		std::string::size_type no_zeros)
{
	while (no_zeros > 0)
	{
		if (*it != '0')
			return true;
		++it;
		--no_zeros;
	}

	return false;
}

// check if decimal string a < b
static bool numeric_less(const VersionComponent& a, const VersionComponent& b)
{
	std::string::size_type a_len = a.end - a.begin;
	std::string::size_type b_len = b.end - b.begin;
	std::string::size_type min_len = std::min(a_len, b_len);

	// determine the necessary left-padding
	std::string::size_type a_pad = a_len - min_len;
	std::string::size_type b_pad = b_len - min_len;

	std::string::const_iterator ai = a.begin;
	std::string::const_iterator bi = b.begin;

	// if B is longer and does not start with a_pad zeros => A < B
	if (compare_with_padded_zeros(bi, a_pad))
		return true;
	// if A is longer and does not start with b_pad zeros => A > B
	if (compare_with_padded_zeros(ai, b_pad))
		return false;

	// now iterators should be in sync
	assert(a.end - ai == b.end - bi);

	for (; ai != a.end; ++ai, ++bi)
	{
		if (*ai != *bi)
			return *ai < *bi;
	}

	return false;
}

// check if version string a < b
static bool version_less(const std::string& a, const std::string& b)
{
	std::string::const_iterator
		a_pos = a.begin(),
		b_pos = b.begin(),
		a_end = a.end(),
		b_end = b.end();

	while (true)
	{
		VersionComponent a_comp = next_component(a_pos, a_end);
		VersionComponent b_comp = next_component(b_pos, b_end);

		// -rc, .old, ~ are less than anything else, so handle them first
		// e.g. 4.0-rc1 < 4.0, 4.0-rc1 < 4.0.old
		int a_weight = get_component_weight(a_comp);
		int b_weight = get_component_weight(b_comp);
		if (a_weight != b_weight)
			return a_weight < b_weight;

		// end of either string? the shorter is smaller
		// e.g. 4.0 < 4.0.1
		bool a_end = a_comp.begin == a_comp.end;
		bool b_end = b_comp.begin == b_comp.end;
		if (a_end || b_end)
			return !b_end;

		// short-circuit equal components
		if (a_comp.end - a_comp.begin != b_comp.end - b_comp.begin
				|| !std::equal(a_comp.begin, a_comp.end, b_comp.begin))
		{
			// numeric always evaluates above non-numeric
			if (a_comp.is_numeric != b_comp.is_numeric)
				return b_comp.is_numeric;

			// numeric string comparison
			if (a_comp.is_numeric)
				return numeric_less(a_comp, b_comp);
			else
				return std::lexicographical_compare(
						a_comp.begin, a_comp.end,
						b_comp.begin, b_comp.end);
		}

		a_pos = a_comp.end;
		b_pos = b_comp.end;
	}

	assert(0 && "Unreachable: version comparison loop broken");
	return false;
}

// check if version of fileset A < B
// prefers apparent version, falls back to internal
static bool versions_less(const FileSet& a, const FileSet& b)
{
	if (a.apparent_version() != b.apparent_version())
		return version_less(a.apparent_version(), b.apparent_version());
	else if (a.internal_version() != b.internal_version())
		return version_less(a.internal_version(), b.internal_version());
	else
		return false;
}

static sort_getters_type sort_getters = {
	{ "mtime", mtime_less },
	{ "version", versions_less },
};

fileset_sorting_function get_sorting_function(const std::string& name)
{
	sort_getters_type::const_iterator i = sort_getters.find(name);
	if (i != sort_getters.end())
		return i->second;

	return {};
}
