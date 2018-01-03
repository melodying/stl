#ifndef _FOUNTION_H_
#define _FOUNTION_H_

#include "config.h"

NAMESPACE_BEGIN


template <typename _Tp>
struct less
{
	bool operator()(const _Tp& _x, const _Tp& _y) const
	{ 
		return _x < _y; 
	}
};

template<typename _Tp>
struct _Identity
{
	const _Tp& operator()(const _Tp& _x)const
	{
		return _x;
	}
};

template <typename _Pair>
struct _Select1st
{
	const typename _Pair::first_type& operator()(const _Pair &_x)const
	{
		return _x.first;
	}
};



NAMESPACE_END

#endif//!_FOUNTION_H_