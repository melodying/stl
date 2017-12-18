/*
*
* Copyright (c) 1994
* Hewlett-Packard Company
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies and
* that both that copyright notice and this permission notice appear
* in supporting documentation.  Hewlett-Packard Company makes no
* representations about the suitability of this software for any
* purpose.  It is provided "as is" without express or implied warranty.
*
*
* Copyright (c) 1997
* Silicon Graphics Computer Systems, Inc.
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies and
* that both that copyright notice and this permission notice appear
* in supporting documentation.  Silicon Graphics makes no
* representations about the suitability of this software for any
* purpose.  It is provided "as is" without express or implied warranty.
*/



#ifndef  _CONSTRUCT_H_
#define _CONSTRUCT_H_

#include <new.h>
#include "config.h"

NAMESPACE_BEGIN

template <typename _T1, typename _T2>
void construct(_T1 *_p, const _T2 _val)
{
	new ((void*)_p) _T1(_val);
}

template <typename _T1>
void construct(_T1 *_p)
{
	new ((void*)_p) _T1();
}

template <class _Tp>
void destroy(_Tp* _pointer)
{
	_pointer->~_Tp();
}

template <class _ForwardIterator>
void destroy(_ForwardIterator _first, _ForwardIterator _last)
{
	for (; _first != _last; ++_first)
		destroy(&*_first); //*_first调用这个迭代器的op*()
}

NAMESPACE_END
#endif//!_CONSTRUCT_H_