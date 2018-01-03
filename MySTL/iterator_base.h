#ifndef _ITERATOR_BASE_H_
#define _ITERATOR_BASE_H_

#include "config.h"


NAMESPACE_BEGIN

template<typename _InputIterator, typename _Disance>
void distance(_InputIterator _first, _InputIterator _last, _Disance& _n)
{
	for (; _first != _last; ++_first)
		++_n;
}




NAMESPACE_END

#endif//!_ITERATOR_BASE_H_