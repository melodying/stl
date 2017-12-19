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



#ifndef _DEQUE_H_
#define _DEQUE_H_

#include "config.h"
#include "alloc.h"
#include "uninitialized.h"

NAMESPACE_BEGIN

// NOTE: deque���÷ֶ�����ʽ�ռ����洢����
// �ֶοռ���ָ ͨ��һ������ָ�����������������ռ���׵�ַ
// �����ռ�Ĵ�С��ͨ��_Deque_buf_size��������
// ���һ�������ռ��������������һ���µ������ռ䣬ͨ��һ������ָ�����ܿ������е������ռ�
// Ҳ����˵���������һ�������ռ��ĩβ������Ҫͨ���������ָ����������һ�������ռ�



// �������������ռ�Ĵ�С, ���԰�512����������иı�, �÷�������int����Ϊ128���ռ�
// ��һ���汾�����512��Ϊ��������
inline size_t _Deque_buf_size(size_t _size)
{
	return _size < 512 ? size_t(512 / _size) : size_t(1);
}

template<typename _Tp, typename _Ref, typename _Ptr>
class _Deque_iterater
{
public:
	typedef _Deque_iterater<_Tp, _Tp, _Tp> iterater;
	typedef _Deque_iterater<_Tp, const _Tp&, const _Tp*> const_iterator;
	
	typedef _Tp value_type;
	typedef _Ref reference;
	typedef _Ptr pointer;
	typedef size_t size_type;
	typedef int difference_type;
	typedef _Tp** _Map_pointer;

	typedef _Deque_iterater _Self;
	static size_type _S_buffer_size()
	{
		return _Deque_buf_size(sizeof(_Tp));
	}

	_Tp *_m_cur;			// ָ��ǰnode��  ��ǰλ��		
	_Tp *_m_first;			// ָ��ǰnode��  ��ʼλ��
	_Tp *_m_last;			// ָ��ǰnode��  ���ռ����һ��λ��
	_Map_pointer _m_node;	// ָ��ǰnode����map�е�λ��


	_Deque_iterater(_Tp *_x, _Map_pointer _y):_m_cur(_x), _m_first(*_y),
		_m_last(*_y + _S_buffer_size()),_m_node(_y)
	{
	}
	_Deque_iterater():_m_cur(nullptr), _m_first(nullptr), _m_last(nullptr), _m_node(nullptr){}
	_Deque_iterater(const iterater&_x):_m_cur(_x._m_cur), _m_first(_x._m_first),
		_m_last(_x._m_last), _m_node(_x._m_node){}

	reference operator*()
	{
		return *_m_cur;
	}
	pointer operator->()
	{
		return &(operator*());
	}

	// ��������������֮��ľ���---ͨ��node֮��ļ����cur֮��ļ��
	difference_type operator-(const _Self& _x)const
	{
		return difference_type(_S_buffer_size()) * (_m_node - _x._m_node - 1) + 
			(_m_cur - _m_first) + (_x._m_last - _x._m_cur);
	}

	_Self& operator++()
	{
		++_m_cur;
		if(_m_cur == _m_last)
		{
			_M_set_node(_m_node + 1);
			_m_cur = _m_first;	
		}
		return *this;
	}

	_Self operator++(int)
	{
		_Self _tmp = *this;
		++(*this);
		return _tmp;
	}
	_Self& operator--()
	{
		if (_m_cur == _m_first)
		{
			_M_set_node(_m_node - 1);
			_m_cur = _m_last;
		}
		--_m_cur;
		return *this;
	}

	_Self operator--(int)
	{
		_Self _tmp = *this;
		--(*this);
		return _tmp;
	}

	_Self& operator+=(difference_type _n)
	{
		difference_type _offset = _n + (_m_cur - _m_first);
		if (_offset >= 0 && _offset < difference_type(_S_buffer_size()))
			_m_cur += _n;
		else//��Ҫ������һ��node��
		{
			// �������ϻ����µ�nodeƫ����
			difference_type _node_offset = _offset > 0 ? _offset / difference_type(_S_buffer_size()) :
				-difference_type((-_offset - 1) / _S_buffer_size()) - 1;

			_M_set_node(_m_node + _node_offset);
			_m_cur = _m_first + (_offset - _node_offset * difference_type(_S_buffer_size()));
		}
		return *this;
	}

	_Self operator+(difference_type _n)const
	{
		_Self _tmp = *this;
		return _tmp += _n;
	}

	_Self& operator-=(difference_type _n)
	{
		return *this += -_n;
	}
	_Self operator-(difference_type _n)
	{
		_Self _tmp = *this;
		return _tmp -= _n;
	}

	reference operator[](difference_type _index)
	{
		return *(*this + _index);
	}
	bool operator==(const _Self& _x)
	{
		return _m_cur == _x._m_cur;
	}
	bool operator!=(const _Self&_x)
	{
		return !(*this == _x);
	}

	bool operator<(const _Self& _x)
	{
		return (_m_node == _x._m_node) ? (_m_cur < _x._m_cur) : (_m_node < _x._m_node);
	}
	bool operator>(const _Self&_x)
	{
		return _x < *this;
	}
	bool operator<=(const _Self &_x)
	{
		return !(_x < *this);
	}
	bool operator>=(const _Self &_x)
	{
		return !(*this < _x);
	}

	// ͨ���ú�����������һ��������
	void _M_set_node(_Map_pointer _new_node)
	{
		_m_node = _new_node;
		_m_first = *_new_node;
		_m_last = _m_first + difference_type(_S_buffer_size());
	}
};

template<typename _Tp, typename _Alloc = alloc>
class deque
{
public:
	typedef _Tp value_type;
	typedef _Tp* pointer;
	typedef _Tp& reference;
	typedef const _Tp* const_pointer;
	typedef const _Tp& const_reference;
	typedef size_t size_type;
	typedef int difference_type;

	typedef _Deque_iterater<_Tp, _Tp&, _Tp*> iterator;
	typedef _Deque_iterater<_Tp, const _Tp&, const _Tp*> const_iterator;

protected:
	typedef pointer* _map_pointer; 
	enum
	{
		_S_initial_map_size = 8
	};


	typedef simple_alloc<_Tp, _Alloc> _Node_alloc_type;
	typedef simple_alloc<_Tp*, _Alloc > _Map_alloc_type;

	_Tp *_M_allocate_node()
	{
		return _Node_alloc_type::allocate(_Deque_buf_size(sizeof(_Tp)));
	}
	void _M_deallocate_node(_Tp *_p)
	{
		_Node_alloc_type::deallocate(_p);
	}
	_Tp **_M_allocate_map(size_type _n)
	{
		return _Map_alloc_type::allocate(_n);
	}
	void _M_deallocate_map(_Tp **_p)
	{
		_Map_alloc_type::deallocate(_p);
	}

	static size_type _S_buffer_size()
	{
		return _Deque_buf_size(sizeof(_Tp));
	}

	// ��ʼ��map�Ĵ�С�͵�������ָ��
	void _M_initialize_map(size_type _num_elements);
	void _M_create_nodes(_map_pointer _start, _map_pointer _finish);
	void _M_destory_nodes(_map_pointer _start, _map_pointer _finish);
	// ���������Чnode�ռ��ֵ
	void _M_fill_initialize(const_reference _value);
	
protected:
	iterator _m_start;			// ��һ����Чnode��λ��
	iterator _m_finish;			// ���һ����Чnode����һ��λ��
	_map_pointer _m_map;
	size_type _m_map_size;
public:
	iterator begin() { return _m_start; }
	iterator end() { return _m_finish; }
	const_iterator begin() const { return _m_start; }
	const_iterator end() const { return _m_finish; }

	reference operator[](size_type _n)
	{
		return _m_start[difference_type(_n)];// ���õ������е�op[];
	}

	const_reference operator[](size_type _n)const
	{
		return _m_start[difference_type(_n)];
	}

	reference at(size_type _n)
	{
		if(_n < size())
			return (*this)[_n];
		return (*this)[0];
	}
	const_reference at(size_type _n) const
	{
		if (_n < size())
			return (*this)[_n];; 
		return (*this)[0];
	}

	size_type size()const
	{
		return _m_finish - _m_start;// �������е�op-
	}
	size_type max_size()const
	{
		return size_type(-1);
	}
	reference front()
	{
		return *_m_start;
	}
	const_reference front()const
	{
		return *_m_start;
	}
	reference back()
	{
		iterator _tmp = _m_finish;
		--_tmp;
		return *_tmp;		// ����op*()
	}
	const_reference back()const
	{
		const_iterator _tmp = _m_finish;
		--_tmp;
		return *_tmp;
	}
	bool empty()
	{
		return _m_start == _m_finish;
	}


public:
	deque(): _m_start(), _m_finish(), _m_map(nullptr),_m_map_size(0)
	{
		_M_initialize_map(0);
	}
	deque(const deque& _x):_m_start(), _m_finish(), _m_map(nullptr), _m_map_size(0)
	{
		_M_initialize_map(_x.size());
		uninitialized_copy(_x.begin(), _x.end(), _m_start);
	}
	deque(size_type _n, const_reference _value) : _m_start(), _m_finish(), _m_map(nullptr), _m_map_size(0)
	{
		_M_initialize_map(_n);
		_M_fill_initialize(_value);
	}
	deque(const_pointer _first, const_pointer _last) :_m_start(), _m_finish(), _m_map(nullptr), _m_map_size(0)
	{
		_M_initialize_map(_last - _first);
		uninitialized_copy(_first, _last, _m_start);
	}
	deque(const_iterator _first, const_iterator _last) :_m_start(), _m_finish(), _m_map(nullptr), _m_map_size(0)
	{
		_M_initialize_map(_last - _first);
		uninitialized_copy(_first, _last, _m_start);
	}
	~deque()
	{
		destroy(_m_start, _m_finish);
		if (_m_map)
		{
			_M_destory_nodes(_m_start._m_node, _m_finish._m_node + 1);
			_M_deallocate_map(_m_map);
		}
	}

	deque& operator=(const deque& _x)
	{
		const size_type _len = size();
		if (&_x != this)
		{
			if (_len >= _x.size())
				erase(copy(_x.begin(), _x.end(), _m_start), _m_finish);
			else
			{
				const_iterator _mid = _x.begin() + difference_type(_len);
				copy(_x.begin(), _mid, _m_start);
				insert(_m_finish, _mid, _x.end());
			}
		}

		return *this;
	}

public:
	void push_back(const value_type& _val)
	{
		if (_m_finish._m_cur != _m_finish._m_last - 1)
		{
			construct(_m_finish._m_cur, _val);
			++_m_finish._m_cur;
		}
		else
		{// ��ǰnode�ռ䲻��, ��������һ��node�ռ�����·���map
			_M_push_back_aux(_val);
		}
	}

	void push_back()
	{
		push_back(_Tp());
	}

	void push_front(const value_type &_val)
	{
		if(_m_start._m_cur != _m_start._m_first)
		{
			construct(_m_start._m_cur - 1, _val);
			--_m_start._m_cur;
		}
		else
		{
			_M_push_front_aux(_val);
		}
	}
	void push_front()
	{
		push_front(_Tp());
	}

	void pop_back()
	{
		if (_m_finish._m_cur != _m_finish._m_first)
		{
			--_m_finish._m_cur;
			destroy(_m_finish._m_cur);
		}
		else
		{
			_M_pop_back_aux();
		}
	}

	void pop_front()
	{
		if (_m_start._m_cur != _m_start._m_last - 1)
		{
			destroy(_m_start._m_cur);
			++_m_start._m_cur;
		}
		else
		{
			_M_pop_front_aux();
		}
	}

protected:
	void _M_push_back_aux(const value_type&_val);
	void _M_push_front_aux(const value_type &_val);
	void _M_pop_back_aux();
	void _M_pop_front_aux();

protected://��map��node�ռ�����·���
	void _M_reserve_map_at_back(size_type _node_to_add = 1)
	{
		if (_node_to_add + 1 > _m_map_size - (_m_finish._m_node - _m_map))
			_M_reallocate_map(_node_to_add, false);
	}
	void _M_reserve_map_at_front(size_type _node_to_add = 1)
	{
		if (_node_to_add > size_type(_m_start._m_node - _m_map))
			_M_reallocate_map(_node_to_add, true);
	}

	void _M_reallocate_map(size_type _node_to_add, bool _add_at_front);

public:
	iterator insert(iterator _pos, const value_type &_val)
	{
		iterator _ret = _m_start;
		if (_pos._m_cur == _m_start._m_cur)
			push_front(_val);
		else if (_pos._m_cur == _m_finish._m_cur)
		{
			push_back(_val);
			iterator _tmp = _m_finish;
			_ret = --_tmp;
		}
		else
			_ret = _M_insert_aux(_pos, _val);

		return _ret;
	}

	iterator insert(iterator _pos)
	{
		return insert(_pos, _Tp());
	}

	void insert(iterator _pos, size_type _n, const value_type &_val)
	{
		_M_fill_insert(_pos, _n, _val);
	}

	void insert(iterator _pos, const_iterator _first, const_iterator _last);

protected:
	iterator _M_insert_aux(iterator _pos, const value_type& _val);
	void _M_insert_aux(iterator _pos, size_type _n, const value_type &_val);
	void _M_insert_aux(iterator _pos, const_iterator _first, const_iterator _last, size_type _n);
	void _M_fill_insert(iterator _pos, size_type _n, const value_type &_val);

	iterator _M_reserve_elements_at_front(size_type _n)
	{
		size_type _vacancies = _m_start._m_cur - _m_start._m_first;
		if (_n > _vacancies)
			_M_new_elements_at_front(_n - _vacancies);

		return _m_start - difference_type(_n);
	}
	iterator _M_reserve_elements_at_back(size_type _n)
	{
		size_type _vacancies = _m_finish._m_last - _m_finish._m_cur - 1;
		if (_n > _vacancies)
			_M_new_elements_at_back(_n - _vacancies);

		return _m_finish + difference_type(_n);
	}

	void _M_new_elements_at_front(size_type _new_elements);
	void _M_new_elements_at_back(size_type _new_elements);

public:
	iterator erase(iterator _pos)
	{
		iterator _next = _pos;
		++_next;
		difference_type _index = _pos - _m_start;
		if(size_type(_index) < size() / 2)
		{
			copy_backward(_m_start, _pos, _next);
			pop_front();
		}
		else
		{
			copy(_next, _m_finish, _pos);
			pop_back();
		}
		return _m_start + _index;
	}
	iterator erase(iterator _first, iterator _last);
	void resize(size_type _new_size, const value_type &_val)
	{
		const size_type _len = size();
		if (_new_size < _len)
		{
			erase(_m_start + _new_size, _m_finish);
		}
		else
		{
			insert(_m_finish, _new_size - _len, _val);
		}
	}
	void resize(size_type _new_size)
	{
		resize(_new_size, _Tp());
	}
	void clear();
};

// map�Ĵ�СĬ��Ϊ8, ����8ʱΪnodes+2 -- ʹ��map���ᱻռ��
template <typename _Tp, typename _Alloc>
void deque<_Tp, _Alloc>::_M_initialize_map(size_type _num_elements)
{
	size_type _num_nodes = _num_elements / _Deque_buf_size(sizeof(_Tp)) + 1;
	_m_map_size = _S_initial_map_size > _num_nodes + 2 ? _S_initial_map_size : _num_nodes + 2;
	_m_map = _M_allocate_map(_m_map_size);

	// ʹ��start��finish���еĿռ価���������ռ�����룬ʹ����������������������
	_map_pointer _start = _m_map + (_m_map_size - _num_nodes) / 2;
	_map_pointer _finish = _start + _num_nodes;

	_M_create_nodes(_start, _finish);
	_m_start._M_set_node(_start);
	_m_finish._M_set_node(_finish - 1);
	_m_start._m_cur = _m_start._m_first;

	// finish.cur ָ�������ЧԪ�صĺ�һ��λ��
	_m_finish._m_cur = _m_finish._m_first + _num_elements % _Deque_buf_size(sizeof(_Tp));
}

template <typename _Tp, typename _Alloc>
void deque<_Tp, _Alloc>::_M_create_nodes(_map_pointer _start, _map_pointer _finish)
{
	_map_pointer _cur;
	for (_cur = _start; _cur != _finish; ++_cur)
	{
		*_cur = _M_allocate_node();
	}
}

template <typename _Tp, typename _Alloc>
void deque<_Tp, _Alloc>::_M_destory_nodes(_map_pointer _start, _map_pointer _finish)
{
	for (_map_pointer _p = _start; _p != _finish; ++_p)
		_M_deallocate_node(*_p);
}

template <typename _Tp, typename _Alloc>
void deque<_Tp, _Alloc>::_M_fill_initialize(const_reference _value)
{
	_map_pointer _cur;
	for (_cur = _m_start._m_node; _cur < _m_finish._m_node; ++_cur)
		uninitialized_fill(*_cur, *_cur + _S_buffer_size(), _value);
	
	uninitialized_fill(_m_finish._m_first, _m_finish._m_cur, _value);
}

template <typename _Tp, typename _Alloc>
void deque<_Tp, _Alloc>::_M_push_back_aux(const value_type& _val)
{
	// ��map�����е�node�ռ�ȫ������ʱ, ��map�������·���
	_M_reserve_map_at_back();

	*(_m_finish._m_node + 1) = _M_allocate_node();

	construct(_m_finish._m_cur, _val);
	_m_finish._M_set_node(_m_finish._m_node + 1);
	_m_finish._m_cur = _m_finish._m_first;
}

template <typename _Tp, typename _Alloc>
void deque<_Tp, _Alloc>::_M_push_front_aux(const value_type& _val)
{
	_M_reserve_map_at_front();
	*(_m_start._m_node - 1) = _M_allocate_node();
	_m_start._M_set_node(_m_start._m_node - 1);
	_m_start._m_cur = _m_start._m_last - 1;
	construct(_m_start._m_cur, _val);
}

// ��_m_finish.cur == _m_finish.firstʱ
// TODO:finish.curָ�������ЧԪ�صĺ�һ��λ��
template <typename _Tp, typename _Alloc>
void deque<_Tp, _Alloc>::_M_pop_back_aux()
{
	_M_deallocate_node(_m_finish._m_first);
	_m_finish._M_set_node(_m_finish._m_node - 1);
	_m_finish._m_cur = _m_finish._m_last - 1;
	destroy(_m_finish._m_cur);
}

template <typename _Tp, typename _Alloc>
void deque<_Tp, _Alloc>::_M_pop_front_aux()
{
	destroy(_m_start._m_cur);
	_M_deallocate_node(_m_start._m_first);
	_m_start._M_set_node(_m_start._m_node + 1);
	_m_start._m_cur = _m_start._m_first;
}

template <typename _Tp, typename _Alloc>
void deque<_Tp, _Alloc>::_M_reallocate_map(size_type _node_to_add, bool _add_at_front)
{
	size_type _old_num_nodes = _m_finish._m_node - _m_start._m_node + 1;
	size_type _new_num_nodes = _old_num_nodes + _node_to_add;

	_map_pointer _new_start;

	// ������������ô? TODO: ����ĳһ�����Ԫ�ع���ʱ���ᵼ��һ�ߵĿռ䱻ռ��������һ�߻��кܶ�ռ䣬���ʱ��ֻ��Ҫ����
	// TODO�� start �� finish��ָ��ʹ�価���������ռ���м䣬������������������Ҫ���·���ռ�
	if (_m_map_size > 2 * _new_num_nodes)
	{
		_new_start = _m_map + (_m_map_size - _new_num_nodes) / 2 + (_add_at_front ? _node_to_add : 0);

		if (_new_start < _m_start._m_node)
			copy(_m_start._m_node, _m_finish._m_node + 1, _new_start);
		else
			copy_backward(_m_start._m_node, _m_finish._m_node + 1, _new_start + _old_num_nodes);
	}
	else
	{
		size_type _new_map_size = _m_map_size + 
			(_m_map_size > _node_to_add ? _m_map_size : _node_to_add) + 2;

		_map_pointer _new_map = _M_allocate_map(_new_map_size);
		_new_start = _new_map + (_new_map_size - _new_num_nodes) / 2 +
			(_add_at_front ? _node_to_add : 0);

		copy(_m_start._m_node, _m_finish._m_node + 1, _new_start);
		_M_deallocate_map(_m_map);

		_m_map = _new_map;
		_m_map_size = _new_map_size;
	}
	_m_start._M_set_node(_new_start);
	_m_finish._M_set_node(_new_start + _old_num_nodes - 1);
}

template <typename _Tp, typename _Alloc>
void deque<_Tp, _Alloc>::insert(iterator _pos, const_iterator _first, const_iterator _last)
{
	size_type _n = _last - _first;
	if (_pos._m_cur == _m_start._m_cur)
	{
		iterator _new_start = _M_reserve_elements_at_front(_n);
		uninitialized_copy(_first, _last, _new_start);
		_m_start = _new_start;
	}
	else if(_pos._m_cur == _m_finish._m_cur)
	{
		iterator _new_finish = _M_reserve_elements_at_back(_n);
		uninitialized_copy(_first, _last, _m_finish);
		_m_finish = _new_finish;
	}
	else
	{
		_M_insert_aux(_pos, _first, _last, _n);
	}
}

template <typename _Tp, typename _Alloc>
typename deque<_Tp, _Alloc>::iterator deque<_Tp, _Alloc>::_M_insert_aux(iterator _pos, const value_type& _val)
{
	difference_type _index = _pos - _m_start;

	// ����Ԫ���е�ǰ�벿��,�ͽ�ǰ���Ԫ��ǰ��
	if (size_type(_index) < size() / 2)
	{// ���ƶ���һ��Ԫ��, �ٽ��ڶ���Ԫ�ص�pos����ǰ�ƶ�
		push_front(front());		
		iterator _front1 = _m_start;
		++_front1;
		iterator _front2 = _front1;
		++_front2;
		_pos = _m_start + _index;
		iterator _pos1 = _pos;
		++_pos1;
		copy(_front2, _pos1, _front1);
	}
	else
	{
		push_back(back());
		iterator _back1 = _m_finish;
		--_back1;
		iterator _back2 = _back1;
		_pos = _m_start + _index;
		copy_backward(_pos, _back2, _back1);
	}
	*_pos = _val;
	return _pos;
}

template <typename _Tp, typename _Alloc>
void deque<_Tp, _Alloc>::_M_insert_aux(iterator _pos, size_type _n, const value_type& _val)
{
	const difference_type _elems_before = _pos - _m_start;
	size_type _length = size();
	
	if (_elems_before < difference_type(_length / 2))
	{
		iterator _new_start = _M_reserve_elements_at_front(_n);
		iterator _old_start = _m_start;
		_pos = _m_start + _elems_before;
		if (_elems_before >= difference_type(_n))
		{
			iterator _start_n = _m_start + difference_type(_n);
			uninitialized_copy(_m_start, _start_n, _new_start);
			_m_start = _new_start;
			copy(_start_n, _pos, _old_start);
			fill(_pos - difference_type(_n), _pos, _val);
		}
		else
		{
			iterator _tmp = uninitialized_copy(_m_start, _pos, _new_start);
			uninitialized_fill(_tmp, _m_start, _val);
			_m_start = _new_start;
			fill(_old_start, _pos, _val);
		}
	}
}

template <typename _Tp, typename _Alloc>
void deque<_Tp, _Alloc>::_M_insert_aux(iterator _pos, const_iterator _first, const_iterator _last, size_type _n)
{
	const difference_type _elems_before = _pos - _m_start;
	size_type _length = size();
	if (_elems_before < _length / 2)
	{
		iterator _new_start = _M_reserve_elements_at_front(_n);
		iterator _old_start = _m_start;	
		_pos = _m_start + _elems_before;
		if(_elems_before >= _n)
		{
			iterator _start_n = _m_start + _n;
			uninitialized_copy(_m_start, _start_n, _new_start);
			_m_start = _new_start;
			copy(_start_n, _pos, _old_start);
			copy(_first, _last, _pos - difference_type(_n));
		}
		else
		{
			const_iterator _mid = _first + (_n - _elems_before);
			iterator _tmp = uninitialized_copy(_m_start, _pos, _new_start);
			uninitialized_copy(_first, _mid, _tmp);

			_m_start = _new_start;
			copy(_mid, _last, _old_start);
		}
	}
	else
	{
		iterator _new_finish = _M_reserve_elements_at_back(_n);
		iterator _old_finish = _m_finish;
		const difference_type _elems_after = _length - _elems_before;
		_pos = _m_finish - _elems_after;

		if (_elems_after > _n)
		{
			iterator _finish_n = _m_finish - difference_type(_n);
			uninitialized_copy(_finish_n, _m_finish, _m_finish);
			_m_finish = _new_finish;
			copy_backward(_pos, _finish_n, _old_finish);
			copy(_first, _last, _pos);	
		}
		else
		{
			const_iterator _mid = _first + _elems_after;
			iterator _tmp = uninitialized_copy(_mid, _last, _m_finish);
			uninitialized_copy(_pos, _m_finish, _tmp);
			_m_finish = _new_finish;
			copy(_first, _mid, _pos);
		}
	}
}

template <typename _Tp, typename _Alloc>
void deque<_Tp, _Alloc>::_M_fill_insert(iterator _pos, size_type _n, const value_type& _val)
{
	if(_pos._m_cur == _m_start._m_cur)
	{
		iterator _new_start = _M_reserve_elements_at_front(_n);
		uninitialized_fill(_new_start, _m_start, _val);
		_m_start = _new_start;
	}
	else if(_pos._m_cur == _m_finish._m_cur)
	{
		iterator _new_finish = _M_reserve_elements_at_back(_n);
		uninitialized_fill(_m_finish, _new_finish, _val);
		_m_finish = _new_finish;
	}
	else
	{
		_M_insert_aux(_pos, _n, _val);
	}

}

template <typename _Tp, typename _Alloc>
void deque<_Tp, _Alloc>::_M_new_elements_at_front(size_type _new_elements)
{
	size_type _new_nodes = (_new_elements + _S_buffer_size() - 1) / _S_buffer_size();

	_M_reserve_map_at_front(_new_nodes);
	for (size_type _i = 1; _i <= _new_nodes; ++_i)
		*(_m_start._m_node - _i) = _M_allocate_node();
}

template <typename _Tp, typename _Alloc>
void deque<_Tp, _Alloc>::_M_new_elements_at_back(size_type _new_elements)
{
	size_type _new_nodes = (_new_elements + _S_buffer_size() - 1) / _S_buffer_size();

	_M_reserve_map_at_back(_new_nodes);
	for (size_type _i = 1; _i <= _new_nodes; ++_i)
	{
		*(_m_finish._m_node + _i) = _M_allocate_node();
	}
}

template <typename _Tp, typename _Alloc>
typename deque<_Tp, _Alloc>::iterator deque<_Tp, _Alloc>::erase(iterator _first, iterator _last)
{
	iterator ret;
	if (_first == _m_start && _last == _m_finish)
	{
		clear();
		ret = _m_finish;
	}
	else
	{
		difference_type _n = _last - _first;
		difference_type _elems_before = _first - _m_start;
		if (_elems_before < difference_type((size() - _n) / 2))
		{
			copy_backward(_m_start, _first, _last);
			iterator _new_start = _m_start + _n;
			destroy(_m_start, _new_start);
			_M_destory_nodes(_new_start._m_node, _m_start._m_node);
			_m_start = _new_start;
		}
		else
		{
			copy(_last, _m_finish, _first);
			iterator _new_finish = _m_finish - _n;
			destroy(_new_finish, _m_finish);
			_M_destory_nodes(_new_finish._m_node + 1, _m_finish._m_node + 1);
			_m_finish = _new_finish;
		}
		ret = _m_start + _elems_before;
	}

	return ret;
}

template <typename _Tp, typename _Alloc>
void deque<_Tp, _Alloc>::clear()
{
	for (_map_pointer _node = _m_start._m_node + 1; _node < _m_finish._m_node; ++_node)
	{
		destroy(*_node, *_node + _S_buffer_size());
		_M_deallocate_node(*_node);
	}

	if (_m_start._m_node != _m_finish._m_node)
	{
		destroy(_m_start._m_cur, _m_start._m_last);
		destroy(_m_finish._m_first, _m_finish._m_cur);
		_M_deallocate_node(_m_finish._m_first);
	}
	else
	{
		destroy(_m_start._m_cur, _m_finish._m_cur);
	}
	_m_finish = _m_start;
}

NAMESPACE_END

#endif//!_DEQUE_H_