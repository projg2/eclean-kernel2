/* eclean-kernel2
 * (c) 2017 Michał Górny
 * 2-clause BSD license
 */

#pragma once

#ifndef EK2_UTIL_SUBPROCESS_H
#define EK2_UTIL_SUBPROCESS_H 1

#include <string>
#include <vector>

// run subprocess and wait for its termination (alike system())
// argv[0] is assumed to contain the program name, argv[1..n] params
// (execvp() is used)
void run_subprocess(const std::vector<std::string>& argv);

#endif /*EK2_UTIL_SUBPROCESS_H*/
