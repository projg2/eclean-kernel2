/* eclean-kernel2
 * (c) 2017 Michał Górny
 * 2-clause BSD license
 */

#pragma once

#ifndef EK2_BOOTLOADER_H
#define EK2_BOOTLOADER_H 1

#include "ek2/options.h"

// an abstract interface to a bootloader
class BootLoader
{
protected:
	const Options& opts_;
	BootLoader(const Options& opts);

public:
	// optional hook run after removing some kernels
	virtual void postrm();
};

#endif /*EK2_BOOTLOADER_H*/
