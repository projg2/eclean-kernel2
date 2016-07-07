/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#pragma once

#ifndef EK2_FILE_H
#define EK2_FILE_H 1

#include "ek2/util/relativepath.h"

// A generic interface used to refer to a file on disk.
class File
{
	RelativePath path_;

protected:
	File(RelativePath&& path);
};

#endif /*EK2_FILE_H*/
