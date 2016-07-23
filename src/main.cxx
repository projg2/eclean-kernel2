/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "ek2/actions.h"
#include "ek2/layout.h"
#include "ek2/layouts.h"
#include "ek2/options.h"
#include "ek2/sorts.h"
#include "ek2/util/error.h"

#include <algorithm> // XXX TMP
#include <cassert>
#include <iostream>
#include <memory>
#include <ostream>

extern "C"
{
#	include <getopt.h>
};

static const char* short_options = "ln:o:s:B:M:phV";
static const struct option long_options[] = {
	{ "list-kernels", no_argument, nullptr, 'l' },
	{ "keep-newest", required_argument, nullptr, 'n' },

	{ "layout", required_argument, nullptr, 'o' },
	{ "sort-order", required_argument, nullptr, 's' },

	{ "boot-path", required_argument, nullptr, 'B' },
	{ "module-path", required_argument, nullptr, 'M' },

	{ "pretend", no_argument, nullptr, 'p' },

	{ "help", no_argument, nullptr, 'h' },
	{ "version", no_argument, nullptr, 'V' },

	{ nullptr, no_argument, nullptr, 0 },
};

static void print_help(std::ostream& out, const char* argv0)
{
	out << "Usage: " << argv0 << " [options] <action>\n"
		"Actions:\n"
		"  -l, --list-kernels      list installed kernels\n"
		"  -n, --keep-newest N     keep only N newest kernels\n"
		"\n"
		"Configuration:\n"
		"  -o, --layout <layout>   use specific layout (by name)\n"
		"  -s, --sort-order <ord>  use specific sort order (mtime, version)\n"
		"*** 'list' can be used instead of the value to print choices\n"
		"\n"
		"Path overrides:\n"
		"  -B, --boot-path <path>  alternate path for kernels (def: /boot)\n"
		"  -M, --module-path <p>   alternate path for modules (def: /lib/modules)\n"
		"\n"
		"Options:\n"
		"  -p, --pretend           print the plan but do not do anything\n"
		"\n"
		"  -h, --help              print this help message\n"
		"  -V, --version           print program version\n";
}

enum class Action
{
	none,
	list_kernels,
	keep_newest,
};

// print short list of possible values
static void print_list_short(std::vector<std::pair<std::string, std::string>> l)
{
	std::cerr << "Possible values: ";
	for (const std::pair<std::string, std::string>& kv : l)
		std::cerr << kv.first << ", ";
	std::cerr << "list (for detailed info)\n";
}

// print list of possible values with descriptions
static void print_list_long(const std::string& option,
		std::vector<std::pair<std::string, std::string>> l)
{
	std::cout << "Possible values for --" << option << ":\n";
	for (const std::pair<std::string, std::string>& kv : l)
		std::cout << kv.first << " - " << kv.second << "\n";
}

int sub_main(int argc, char* argv[])
{
	Action act = Action::none;
	int keep_num;
	std::string layout = "std";
	std::string sort_order = "version";
	bool pretend = false;

	Options opts = {
		"/boot", // boot_path
		"/lib/modules", // module_path
	};

	while (true)
	{
		int opt = getopt_long(argc, argv, short_options, long_options,
				nullptr);
		if (opt == -1)
			break;

		switch (opt)
		{
			case 'l':
				if (act != Action::none)
				{
					std::cerr << argv[0] << ": multiple actions specified\n";
					return 1;
				}
				act = Action::list_kernels;
				break;
			case 'n':
				if (act != Action::none)
				{
					std::cerr << argv[0] << ": multiple actions specified\n";
					return 1;
				}
				act = Action::keep_newest;
				try
				{
					keep_num = std::stoul(optarg);
				}
				catch (std::invalid_argument& e)
				{
					std::cerr << argv[0] << ": invalid number: " << optarg << "\n";
					return 1;
				}
				catch (std::out_of_range& e)
				{
					std::cerr << argv[0] << ": number out of range: " << optarg << "\n";
					return 1;
				}
				if (keep_num <= 0)
				{
					std::cerr << argv[0] << ": number must be positive: " << optarg << "\n";
					return 1;
				}
				break;

			case 'o':
				layout = optarg;
				break;
			case 's':
				sort_order = optarg;
				break;

			case 'B':
				opts.boot_path = optarg;
				break;
			case 'M':
				opts.module_path = optarg;
				break;

			case 'p':
				pretend = true;
				break;

			case 'h':
				print_help(std::cout, argv[0]);
				return 0;
			case 'V':
				std::cout << PACKAGE_STRING << std::endl;
				return 0;
			case '?':
				print_help(std::cerr, argv[0]);
				return 1;
		}
	}

	if (act == Action::none)
	{
		std::cerr << argv[0] << ": no action specified\n";
		return 1;
	}

	if (optind != argc)
	{
		std::cerr << argv[0] << ": unexpected positional parameter: "
			<< argv[optind] << "\n";
		print_help(std::cerr, argv[0]);
		return 1;
	}

	if (layout == "list")
	{
		print_list_long("layout", get_layout_list());
		return 0;
	}
	if (sort_order == "list")
	{
		print_list_long("sort-order", get_sort_list());
		return 0;
	}

	std::unique_ptr<Layout> l = get_layout(layout, opts);
	if (!l)
	{
		std::cerr << argv[0] << ": unknown layout " << layout << "\n";
		print_list_short(get_layout_list());
		return 1;
	}

	fileset_sorting_function f = get_sorting_function(sort_order);
	if (!f)
	{
		std::cerr << argv[0] << ": unknown sort order " << sort_order << "\n";
		print_list_short(get_sort_list());
		return 1;
	}

	l->find_kernels();
	std::sort(l->kernels().begin(), l->kernels().end(), f);

	switch (act)
	{
		case Action::none:
			assert(0 && "Action::none reached past first check");
			break;
		case Action::list_kernels:
			list_kernels(*l);
			break;
		case Action::keep_newest:
			keep_newest(*l, keep_num, pretend);
			break;
	}

	return 0;
}

int main(int argc, char* argv[])
{
	try
	{
		return sub_main(argc, argv);
	}
	catch (const Error& e)
	{
		std::cerr << argv[0] << ": terminating due to an error:\n"
			<< e.what() << std::endl;
		return 1;
	}
}
