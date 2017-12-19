#ifndef _STACK_H_
#define _STACK_H_

#include "config.h"
#include "deque.h"


NAMESPACE_BEGIN

template <typename _Tp, typename _Sequence = deque<_Tp> >
class stack;

template <typename _Tp, typename _Seq>
bool operator==(const stack<_Tp, _Seq>& _x, const stack<_Tp, _Seq>& _y);

template <typename _Tp, typename _Seq>
bool operator<(const stack<_Tp, _Seq>& __x, const stack<_Tp, _Seq>& __y);

template<typename _Tp, typename _Sequence>
class stack
{
public:
	typedef typename _Sequence::value_type value_type;
	typedef typename _Sequence::size_type size_type;

	typedef          _Sequence                  container_type;
	typedef typename _Sequence::reference       reference;
	typedef typename _Sequence::const_reference const_reference;

protected:
	_Sequence c;

public:
	stack() :c() {}
	stack(const _Sequence &_s) :c(_s) {}
	bool empty()const
	{
		return c.empty();
	}

	size_type size()const
	{
		return c.size();
	}

	reference top()
	{
		return c.back();
	}

	void push(const value_type& _val)
	{
		c.push_back(_val);
	}

	void pop()
	{
		c.pop_back();
	}

private:
	template <typename _Tp1, typename _Seq1>
	friend bool operator==(const stack<_Tp1, _Seq1>&, const stack<_Tp1, _Seq1>&);

	template <typename _Tp1, typename _Seq1>
	friend bool operator<(const stack<_Tp1, _Seq1>&, const stack<_Tp1, _Seq1>&);
};


template <typename _Tp, typename _Seq>
bool operator==(const stack<_Tp, _Seq>& _x, const stack<_Tp, _Seq>& _y)
{
	return _x.c == _y.c;
}

template <typename _Tp, typename _Seq>
bool operator<(const stack<_Tp, _Seq>& _x, const stack<_Tp, _Seq>& _y)
{
	return _x.c < _y.c;
}

template <typename _Tp, typename _Seq>
bool operator!=(const stack<_Tp, _Seq>& _x, const stack<_Tp, _Seq>& _y)
{
	return !(_x.c == _y.c);
}

template <typename _Tp, typename _Seq>
bool operator>(const stack<_Tp, _Seq>& _x, const stack<_Tp, _Seq>& _y)
{
	return _y < _x;
}

template <typename _Tp, typename _Seq>
bool operator<=(const stack<_Tp, _Seq>& _x, const stack<_Tp, _Seq>& _y)
{
	return !(_y < _x);
}

template <typename _Tp, typename _Seq>
bool operator>=(const stack<_Tp, _Seq>& _x, const stack<_Tp, _Seq>& _y)
{
	return !(_x < _y);
}

NAMESPACE_END

#endif// !_STACK_H_