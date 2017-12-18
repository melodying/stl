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



#ifndef _UNINITIALIZED_H_
#define _UNINITIALIZED_H_


// TODO: 在这里不考虑traits, 需要进行POD路由的全部使用需要构造的版本

#include "construct.h"
#include "config.h"

NAMESPACE_BEGIN

template<typename _ForwardIter, typename  _Size, typename _Tp>
_ForwardIter uninitialized_fill_n(_ForwardIter _first, _Size _n, const _Tp& _val)
{
	_ForwardIter _cur = _first;
	for (; _n > 0; --_n, ++_cur)
		construct(_cur, _val);

	return _cur;
}

template<typename _InputIter, typename _ForwardIter>
_ForwardIter uninitialized_copy(_InputIter _first, _InputIter _last, _ForwardIter _result)
{
	_ForwardIter _cur = _result;

	for(; _first != _last; ++_first, ++_cur)
	{
		construct(_cur, *_first);
	}
	return _cur;
}

template<typename _ForwardIter, typename _Tp>
void uninitialized_fill(_ForwardIter _first, _ForwardIter _last, const _Tp& _val)
{
	_ForwardIter _cur = _first;
	for (; _cur != _last; ++_cur)
		construct(_cur, _val);
}


NAMESPACE_END

#endif//!_UNINITIALIZED_H_