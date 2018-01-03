#ifndef _PAIR_H_
#define _PAIR_H_

#include "config.h"

NAMESPACE_BEGIN


template<typename _T1, typename _T2>
struct pair
{
	typedef _T1 first_type;
	typedef _T2 second_type;

	_T1 first;
	_T2 second;

	pair():first(_T1()), second(_T2()){}
	pair(const _T1& _a, const _T2 &_b):first(_a), second(_b){}
};

template<typename _T1, typename _T2>
bool operator==(const pair<_T1, _T2> &_x, const pair<_T1, _T2> &_y)
{
	return _x.first == _y.first && _x.second == _y.second;
}

template<typename _T1, typename _T2>
bool operator<(const pair<_T1, _T2> &_x, const pair<_T1, _T2> &_y)
{
	return _x.first < _y.first || (!(_y.first < _x.first) && _x.second < _y.second);
}

template<typename _T1, typename _T2>
bool operator!=(const pair<_T1, _T2> &_x, const pair<_T1, _T2> &_y)
{
	return !(_x == _y);
}

template<typename _T1, typename _T2>
bool operator>(const pair<_T1, _T2> &_x, const pair<_T1, _T2> &_y)
{
	return _y < _x;
}

template<typename _T1, typename _T2>
bool operator>=(const pair<_T1, _T2> &_x, const pair<_T1, _T2> &_y)
{
	return !(_x < _y);
}

template<typename _T1, typename _T2>
bool operator<=(const pair<_T1, _T2> &_x, const pair<_T1, _T2> &_y)
{
	return !(_y < _x);
}

template<typename _T1, typename _T2>
pair<_T1, _T2> make_pair(const _T1 &_x, const _T2 &_y)
{
	return pair<_T1, _T2>(_x, _y);
}


NAMESPACE_END

#endif//!_PAIR_H_