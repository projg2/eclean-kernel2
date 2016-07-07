/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#pragma once

#ifndef EK2_FILES_H
#define EK2_FILES_H 1

#include "ek2/file.h"

#include <memory>

// Get File* instance matching filetype guessed by reading magic bytes
// returns nullptr on I/O error
std::shared_ptr<File> get_file_by_magic(RelativePath&& path);

#endif /*EK2_FILES_H*/
