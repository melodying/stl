#ifndef _ALGOBASE_H_
#define _ALGOBASE_H_

// TODO: 以下是对一些简单算法的封装, 这些算法皆是对不需要进行构造的空间的操作


#include "config.h"
NAMESPACE_BEGIN

template<typename _InputIter, typename _OutputIter>
_OutputIter copy(_InputIter _first, _InputIter _last, _OutputIter _result)
{
	for (; _first != _last; ++_first, ++_result)
	{
		*_result = *_first;
	}
	return _result;
}

template<typename  _ForwardIter, typename _Tp>
void fill(_ForwardIter _first, _ForwardIter _last, const _Tp& _val)
{
	for (; _first != _last; ++_first)
		*_first = _val;
}

template<typename _OutputIter, typename _Size, typename _Tp>
_OutputIter fill_n(_OutputIter _first, _Size _n, const _Tp &_val)
{
	for (; _n > 0; --_n, ++_first)
	{
		*_first = _val;
	}
	return _first;
}

template<typename _BidirectionalIter1, typename _BidirectionalIter2>
_BidirectionalIter2 copy_backward(_BidirectionalIter1 _first, _BidirectionalIter1 _last, 
									_BidirectionalIter2 _result)
{
	while (_first != _last)
	{
		*(--_result) = *(--_last);
	}
	return _result;
}

NAMESPACE_END
#endif//~_ALGOBASE_H_