/* eclean-kernel2
 * (c) 2016 Michał Górny
 * 2-clause BSD license
 */

#pragma once

#ifndef EK2_LAYOUT_H
#define EK2_LAYOUT_H 1

// System layout, defines where to look for kernels and how to handle
// them
class Layout
{
public:
	// find kernels and store them internally
	// may output to stderr
	// returns true on success, false if fatal error occured
	virtual bool find_kernels() = 0;
};

#endif /*EK2_LAYOUT_H*/
