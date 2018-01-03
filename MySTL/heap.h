#ifndef _HEAP_H_
#define _HEAP_H_

#include "config.h"

NAMESPACE_BEGIN

template<typename _RandomAccessIter, typename _Compare>
void make_heap(_RandomAccessIter _first, _RandomAccessIter _last, _Compare _comp)
{
	if(_last - _first < 2)
		return;


}


NAMESPACE_END

#endif//!_Cmpare