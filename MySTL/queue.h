#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "config.h"
#include "deque.h"

NAMESPACE_BEGIN

template<typename _Tp, typename _Sequence = deque<_Tp> >
class queue
{
public:
	typedef typename _Sequence::value_type      value_type;
	typedef typename _Sequence::size_type       size_type;
	typedef          _Sequence                  container_type;

	typedef typename _Sequence::reference       reference;
	typedef typename _Sequence::const_reference const_reference;
protected:
	_Sequence c;
public:
	queue() :c() {}
	queue(const _Sequence &_c) :c(_c) {}
	bool empty()const
	{
		return c.empty();
	}

	size_type size() const
	{
		return c.size();
	}

	reference front()
	{
		return c.front();
	}

	const_reference front()const
	{
		return c.front();
	}

	reference back()
	{
		return c.back();
	}

	const_reference back() const
	{
		return c.back();
	}

	void push(const value_type& _val)
	{
		c.push_back(_val);
	}
	void pop()
	{
		return c.pop_front();
	}

private:
	template<typename _Tp1, typename _Seq1>
	friend bool operator==(const queue<_Tp1, _Seq1>&, const queue<_Tp1, _Seq1>&);

	template<typename _Tp1, typename _Seq1>
	friend bool operator<(const queue<_Tp1, _Seq1>&, const queue<_Tp1, _Seq1>&);
};

template<typename _Tp, typename _Sequence>
bool operator==(const queue<_Tp, _Sequence>& _x, const queue<_Tp, _Sequence>& _y)
{
	return _x.c == _y.c;
}

template<typename _Tp, typename _Sequence>
bool operator<(const queue<_Tp, _Sequence>& _x, const queue<_Tp, _Sequence>& _y)
{
	return _x.c < _y.c;
}

template <typename _Tp, typename _Sequence>
bool operator!=(const queue<_Tp, _Sequence>& _x, const queue<_Tp, _Sequence>& _y)
{
	return !(_x == _y);
}

template <typename _Tp, typename _Sequence>
bool operator>(const queue<_Tp, _Sequence>& _x, const queue<_Tp, _Sequence>& _y)
{
	return _y < _x;
}

template <typename _Tp, typename _Sequence>
bool operator<=(const queue<_Tp, _Sequence>& _x, const queue<_Tp, _Sequence>& _y)
{
	return !(_y < _x);
}

template <typename _Tp, typename _Sequence>
bool operator>=(const queue<_Tp, _Sequence>& _x, const queue<_Tp, _Sequence>& _y)
{
	return !(_x < _y);
}


NAMESPACE_END

#endif//!_QUEUE_H_


