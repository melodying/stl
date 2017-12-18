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



#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <stdexcept>

#include "config.h"
#include "alloc.h"
#include "uninitialized.h"
#include "algobase.h"


NAMESPACE_BEGIN

template <typename _Tp, typename _Alloc = alloc >
class vector
{
public:
	typedef vector<_Tp, _Alloc> _Base;
	typedef simple_alloc<_Tp, alloc> allocate_type;
	//typedef _Alloc				allcoator_type;
	typedef size_t				size_type;
	typedef _Tp					value_type;
	typedef value_type*			iterator;
	typedef value_type*			pointer;
	typedef const value_type*	const_pointer;
	typedef const value_type*	const_iterator;
	typedef value_type&			reference;
	typedef const value_type&	const_reference;
	typedef int					difference_type;

protected:
	iterator _m_start;
	iterator _m_finish;
	iterator _m_end;

public:
	explicit vector():_m_start(nullptr), _m_finish(nullptr), _m_end(nullptr)
	{
	}

	vector(size_type _n, const value_type &_val = value_type())
	{
		_m_start = allocate_type::allocate(_n);
		_m_finish = uninitialized_fill_n(_m_start, _n, _val);
		_m_end = _m_finish;
	}

	vector(const _Base & _other)
	{
		_m_start = allocate_type::allocate(_other.size());
		_m_finish = uninitialized_copy(_other.begin(), _other.end(), _m_start);
		_m_end = _m_finish;
	}

	vector(const_iterator _first, const_iterator _last)
	{
		_m_start = allocate_type::allocate(_last - _first);
		_m_finish = uninitialized_copy(_first, _last, _m_start);
		_m_end = _m_finish;
	}

	~vector()
	{
		destroy(_m_start, _m_finish);
		allocate_type::deallocate(_m_start);
		_m_start = _m_finish = _m_end = nullptr;
	}

	iterator begin()
	{
		return _m_start;
	}
	const_iterator begin() const
	{
		return _m_start;
	}

	iterator end()
	{
		return _m_finish;
	}
	const_iterator end()const
	{
		return _m_finish;
	}

	iterator rbegin()
	{
		return end();
	}
	const_iterator rbegin()const
	{
		return end();
	}
	const_iterator rend()const
	{
		return begin();
	}

	iterator rend()
	{
		return begin();
	}

	size_type size()const
	{
		return _m_finish - _m_start;
	}

	size_type max_size()const
	{
		return (size_type(-1) / sizeof(value_type));
	}

	bool empty()const
	{
		return _m_start == _m_finish;
	}

	reference at(size_type _n)
	{
		return const_cast<reference> (static_cast<const _Base&>(*this).at(_n));
	}

	const_reference at(size_type _n)const
	{
		if (_n < size())
			return *(begin() + _n);
		else
		{
			throw std::out_of_range("out of array range");
		}
	}

	void resize(size_type _new_size, value_type &val = value_type())
	{
		if (_new_size < size())
			erase(begin() + _new_size, end());
		else
		{
			insert(end(), _new_size - size(), val);
		}
	}

	void reserve(size_type _n)
	{
		if(capacity() < _n)
		{
			const size_type _old_size = size();
			iterator _tmp = _M_allocate_and_copy(_m_start, _m_finish, _n);
			destroy(_m_start, _m_finish);
			allocate_type::deallocate(_m_start);

			_m_start = _tmp;
			_m_finish = _m_start + _old_size;
			_m_end = _m_finish + _n;
		}
	}

	void swap(_Base &other)
	{
		swap(_m_start, other._m_start);
		swap(_m_finish, other._m_finish);
		swap(_m_end, other._m_end);
	}

	reference front()
	{
		return *(begin());
	}

	const_reference front()const
	{
		return *(begin());
	}

	reference back()
	{
		return *(end() - 1);
	}

	const_reference back()const
	{
		return *(end() - 1);
	}

	const_reference operator[](size_type _index)const
	{
		return *(begin() + _index);
	}
	reference operator[](size_type _index)
	{
		return *(begin() + _index);
	}

	iterator insert(iterator _pos, const value_type &_val)
	{
		size_type n = _pos - begin();
		insert(_pos, 1, _val);
		return begin() + n;
	}

	void insert(iterator _pos, size_type _n, const value_type &_val)
	{
		if ((size_type)(_m_end - _m_finish) < _n) // 空间不够
		{
			size_type _old_size = size();
			size_type _len = _old_size + (_old_size > _n ? _old_size : _n);
			iterator _new_start = allocate_type::allocate(_len);
			iterator _new_finish = uninitialized_copy(begin(), _pos, _new_start);
			_new_finish = uninitialized_fill_n(_new_finish, _n, _val);
			_new_finish = uninitialized_copy(_pos, end(), _new_finish);
			
			destroy(begin(), end());
			allocate_type::deallocate(_m_start);
			_m_start = _new_start;
			_m_finish = _new_finish; 
			_m_end = _m_start + _len;
		}
		else if ((size_type)(_m_finish - _pos) < _n)// 填充的值部分需要构造的情况
		{
			size_type _elems_after = _m_finish - _pos;
			uninitialized_fill_n(_m_finish, _n - _elems_after, _val);

			iterator _old_finish = _m_finish;
			_m_finish += (_n - _elems_after);

			uninitialized_copy(_pos, _old_finish, _m_finish);
			_m_finish += _elems_after;
			fill(_pos, _old_finish, _val);
		}
		else if (0 < _n) // 空间够, 填充的值不需要构造的情况
		{
		 	uninitialized_copy(_m_finish - _n, _m_finish, _m_finish);
			iterator _old_finish = _m_finish;
			_m_finish += _n;
			copy_backward(_pos, _old_finish - _n, _old_finish);
			fill_n(_pos, _n, _val);
		}

	}

	void insert(iterator _pos, iterator _first, iterator _last)
	{
		if(_first != _last)
		{
			size_type _n = (size_type)(_last - _first);

			if(_m_end - _m_finish >= _n)//空间足够
			{
				const size_type _elem_after = _m_finish - _pos;
				iterator _old_finish = _m_finish;

				if (_elem_after > _n) // 无元素需要构造
				{
					uninitialized_copy(_m_finish - _n, _m_finish, _m_finish);
					_m_finish += _n;
					copy_backward(_pos, _old_finish - _n, _old_finish);
					copy(_first, _last, _pos);
				}
				else //部分元素需要构造
				{
					uninitialized_copy(_first + _elem_after, _last, _m_finish);
					_m_finish += _n - _elem_after;
					uninitialized_copy(_pos, _old_finish, _m_finish);
					_m_finish += _elem_after;
					copy(_first, _first + _elem_after, _pos);
				}
			}
			else
			{
				const size_type _old_size = size();
				const size_type _len = _old_size + (_old_size > _n ? _old_size : _n);
				iterator _new_start = allocate_type::allocate(_len);
				iterator _new_finish = uninitialized_copy(begin(), _pos, _new_start);
				_new_finish = uninitialized_copy(_first, _last, _new_finish);
				_new_finish = uninitialized_copy(_pos, _m_finish, _new_finish);

				destroy(_m_start, _m_finish);
				allocate_type::deallocate(_m_start);
				_m_start = _new_start;
				_m_finish = _new_finish;
				_m_end = _new_start + _len;
			}
		}
	}

	void push_back(value_type &_val)
	{
		insert(end(), _val);
	}

	size_type capacity()
	{
		return _m_start == nullptr ? 0 : _m_end - _m_start;
	}

	iterator erase(iterator _pos)
	{
		if (_pos + 1 != end())
			copy(_pos + 1, _m_finish, _pos);

		--_m_finish;
		destroy(_m_finish);
		return _pos;
	}

	iterator erase(iterator _first, iterator _last)
	{
		iterator tmp = copy(_last, _m_finish, _first);
		destroy(tmp, _m_finish);
		_m_finish -= (_last - _first);
		return _first;
	}

	void assign(iterator first, iterator last)
	{
		erase(begin(), end());
		insert(begin(), first, last);
	}

	void pop_back()
	{
		erase(end() - 1);
	}

	void clear()
	{
		erase(begin(), end());
	}

	_Base& operator=(_Base& _other)
	{
		if(this != &_other)
		{
			const size_type _otherLen = _other.size();
			if(_otherLen > capacity())
			{
				iterator _tmp = _M_allocate_and_copy(_other.begin(), _other.end(), _otherLen);
				destroy(_m_start, _m_finish);
				allocate_type::deallocate(_m_start);
				_m_start = _tmp;
				_m_end = _m_start + _otherLen;
			}
			else if (size() >= _otherLen)
			{
				iterator tmp = copy(_other.begin(), _other.end(), begin());
				destroy(tmp, end());

			}
			else
			{
				copy(_other.begin(), _other.begin() + size(), begin());
				uninitialized_copy(_other.begin() + size(), _other.end(), _m_finish);
			}
			_m_finish = _m_start + _otherLen;
		}
		return *this;
	}

protected:
	iterator _M_allocate_and_copy(const_iterator _first, const_iterator _last, size_type _n)
	{
		iterator _result = allocate_type::allocate(_n);
		uninitialized_copy(_first, _last, _result);
		return _result;
	}

	void swap(iterator _lhs, iterator _rhs)
	{
		iterator _tmp = _lhs;
		_lhs = _rhs;
		_rhs = _tmp;
	}
};

template<typename _Tp, typename _Alloc>
bool operator==(const vector<_Tp, _Alloc>&_lhs, const vector<_Tp, _Alloc> &_rhs)
{
	if(_lhs.size() == _rhs.size())
	{
		typename vector<_Tp, _Alloc>::iterator it1 = _lhs.begin();
		typename vector<_Tp, _Alloc>::iterator it2 = _rhs.begin();
		for (; it1 != _lhs.end(); ++it1, ++it2)
		{
			if(*it1 != *it2)
				return false;
		}
		return true;
	}
	return false;;
}

template<typename _Tp, typename _Alloc>
bool operator!=(const vector<_Tp, _Alloc>&_lhs, const vector<_Tp, _Alloc> &_rhs)
{
	return !(_lhs == _rhs);
}

NAMESPACE_END


#endif//!_VECTOR_H_
