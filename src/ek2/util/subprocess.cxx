/* eclean-kernel2
 * (c) 2017 Michał Górny
 * 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "ek2/util/subprocess.h"

#include "ek2/util/error.h"

extern "C"
{
#	include <unistd.h>
#	include <sys/wait.h>
};

#include <cerrno>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

void run_subprocess(const std::vector<std::string>& argv)
{
	std::vector<std::vector<char>> argv_rw;
	std::vector<char*> cargv;

	for (const std::string& s : argv)
	{
		// std::string -> std::vector<char>
		argv_rw.emplace_back(s.begin(), s.end());
		argv_rw.back().push_back('\0');

		// std::vector<char> -> char*
		cargv.push_back(argv_rw.back().data());
	}
	// sentinel
	cargv.push_back(nullptr);

	pid_t child_pid = fork();
	switch (child_pid)
	{
		case -1:
			throw IOError("fork() failed", errno);
			break;
		case 0:
			std::cerr << cargv[0] << "\n";
			execvp(cargv[0], cargv.data());
			std::cerr << "Failed to execute " << cargv[0]
				<< ": " << strerror(errno) << std::endl;
			_exit(1);
			break;
		default:
			// TODO: handle EINTR
			waitpid(child_pid, nullptr, 0);
	}
}
