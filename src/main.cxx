/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include <iostream>
#include <ostream>

extern "C"
{
#	include <getopt.h>
};

static const char* short_options = "hV";
static const struct option long_options[] = {
	{ "help", no_argument, nullptr, 'h' },
	{ "version", no_argument, nullptr, 'V' },
	{ nullptr, no_argument, nullptr, 0 },
};

static void print_help(std::ostream& out, const char* argv0)
{
	out << "Usage: " << argv0 << " [options]\n"
		"Options:\n"
		"  --help                  print this help message\n"
		"  --version               print program version\n";
}

int main(int argc, char* argv[])
{
	while (true)
	{
		int opt = getopt_long(argc, argv, short_options, long_options,
				nullptr);
		if (opt == -1)
			break;

		switch (opt)
		{
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

	if (optind != argc)
	{
		std::cerr << argv[0] << ": unexpected positional parameter: "
			<< argv[optind] << "\n";
		print_help(std::cerr, argv[0]);
		return 1;
	}

	return 0;
}
