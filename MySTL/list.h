#ifndef _LIST_H_
#define _LIST_H_

#include "config.h"
#include "alloc.h"
#include "construct.h"

NAMESPACE_BEGIN

struct _List_node_base
{
	_List_node_base *_m_next;
	_List_node_base *_m_prev;
};

template<typename _Tp>
struct _List_node : public _List_node_base
{
	_Tp _m_data;
};



template<typename _Tp, typename _Ref, typename _Ptr>
struct _List_iterator
{
public:
	typedef _List_iterator<_Tp, _Tp&, _Tp*>					iterator;
	typedef _List_iterator<_Tp, const _Tp&, const _Tp*>		const_iterator;
	typedef _List_iterator<_Tp, _Ref, _Ptr>					_Self;

	typedef _Tp value_type;
	typedef _Ptr pointer;
	typedef _Ref reference;
	typedef _List_node<_Tp> _Node;

	_List_node_base *_m_node;

	_List_iterator(_Node *_x):_m_node(_x){}
	_List_iterator():_m_node(nullptr){}
	_List_iterator(const iterator& _x):_m_node(_x._m_node){}

	reference operator*()const 
	{
		return ((_Node*)_m_node)->_m_data;
	}

	pointer operator->()const
	{
		return &(operator*());
	}

	_Self& operator++()
	{
		this->_M_decr();
		return *this;
	}

	_Self operator++(int)
	{
		_Self _tmp = *this;
		this->_M_incr();
		return _tmp;
	}

	_Self &operator--()
	{
		this->_M_decr();
		return *this;
	}
	_Self operator--(int)
	{
		_Self _tmp = *this;
		this->_M_decr();
		return _tmp;
	}

	bool operator==(const _List_iterator &_x)const
	{
		return _m_node == _x._m_node;
	}
	bool operator!=(const _List_iterator &_x)const
	{
		return _m_node != _x._m_node;
	}

	void _M_incr()
	{
		_m_node = _m_node->_m_next;
	}
	void _M_decr()
	{
		_m_node = _m_node->_m_prev;
	}
};

template<typename _Tp, typename _Alloc = alloc >
class list
{
public:
	typedef _Tp						value_type;
	typedef _Tp*					pointer;
	typedef const _Tp*				const_point;
	typedef _Tp&					reference;
	typedef const _Tp&				const_reference;
	typedef  _List_node<_Tp>		_Node;
	typedef size_t					size_type;
	typedef int						difference_type;

	typedef simple_alloc<_Node, _Alloc> allocator_type;

	typedef _List_iterator<_Tp, _Tp&, _Tp*>			iterator;
	typedef _List_iterator<_Tp, const _Tp&, const _Tp*> const_iterator;

protected:
	_Node *_m_head;

	_Node* _M_get_node()
	{
		return allocator_type::allocate(1);
	}
	void _M_put_node(_Node *_p)
	{
		allocator_type::deallocate(_p);
	}

	void _M_clear()
	{
		_Node *_cur = (_Node*)_m_head->_m_next;
		while (_cur != _m_head)
		{
			_Node *_tmp = _cur;
			_cur = (_Node *)_cur->_m_next;
			destroy(&_tmp->_m_data);
			_M_put_node(_tmp);
		}
		_m_head->_m_next = _m_head;
		_m_head->_m_prev = _m_head;
	}

	_Node *_M_create_node(const_reference _x)
	{
		_Node *_p = _M_get_node();
		construct(&_p->_m_data, _x);
		return _p;
	}


public:
	list()
	{
		_m_head = _M_get_node();
		_m_head->_m_next = _m_head;
		_m_head->_m_prev = _m_head;
	}

	list(size_type _n, const_reference _value)
	{
		_m_head = _M_get_node();
		_m_head->_m_next = _m_head;
		_m_head->_m_prev = _m_head;
		insert(begin(), _n, _value);
	}

	list(size_type _n)
	{
		_m_head = _M_get_node();
		_m_head->_m_next = _m_head;
		_m_head->_m_prev = _m_head;
		insert(begin(), _n, _Tp());
	}

	list(const_point _first, const_point _last)
	{
		_m_head = _M_get_node();
		_m_head->_m_next = _m_head;
		_m_head->_m_prev = _m_head;
		insert(begin(), _first, _last);
	}

	list(const_iterator _first, const_iterator _last)
	{
		_m_head = _M_get_node();
		_m_head->_m_next = _m_head;
		_m_head->_m_prev = _m_head;
		insert(begin(), _first, _last);
	}

	list(const list<_Tp, _Alloc> &_x)
	{
		_m_head = _M_get_node();
		_m_head->_m_next = _m_head;
		_m_head->_m_prev = _m_head;
		insert(begin(), _x.begin(), _x.end());
	}

	~list()
	{
		clear();
		_M_put_node(_m_head);
	}

	iterator begin()
	{
		return (_Node*)_m_head->_m_next;
	}
	const_iterator begin()const
	{
		return (_Node*)_m_head->_m_next;
	}
	iterator end()
	{
		return (_Node*)_m_head;
	}
	const_iterator end()const
	{
		return (_Node*)_m_head;
	}

	bool empty()const
	{
		return _m_head == _m_head->_m_next;
	}

	size_type size()const
	{
		iterator _it = begin();
		size_type _result = 0;
		while (_it != end())
		{
			++_it;
			++_result;
		}
		return _result;
	}
	size_type max_size()const
	{
		return (size_type)(-1);
	}

	reference front()
	{
		return *begin();
	}
	const_reference front()const
	{
		return *begin();
	}
	reference back()
	{
		return *(--end());
	}
	const_reference back()const
	{
		return *(--end());
	}

	iterator insert(iterator _pos, const_reference _x)
	{
		_Node *_tmp = _M_create_node(_x);
		_tmp->_m_next = _pos._m_node;
		_tmp->_m_prev = _pos._m_node->_m_prev;
		_pos._m_node->_m_prev->_m_next = _tmp;
		_pos._m_node->_m_prev = _tmp;
		return _tmp;
	}

	iterator insert(iterator _pos)
	{
		return insert(_pos, _Tp());
	}

	void insert(iterator _pos, const_point _first, const_point _last)
	{
		for (; _first != _last; ++_first)
			insert(_pos, *_first);
	}
	void insert(iterator _pos, const_iterator _first, const_iterator _last)
	{
		for (; _first != _last; ++_first)
			insert(_pos, *_first);
	}

	void insert(iterator _pos, size_type _n, const_reference _x)
	{
		for (; _n > 0; --_n)
			insert(_pos, _x);
	}

	void push_front(const_reference _x)
	{
		insert(begin(), _x);
	}
	void push_front()
	{
		insert(begin());
	}
	void push_back(const_reference _x)
	{
		insert(end(), _x);
	}
	void push_back()
	{
		insert(end());
	}

	iterator erase(iterator _pos)
	{
		_List_node_base *_next_node = _pos._m_node->_m_next;
		_List_node_base *_prev_node = _pos._m_node->_m_prve;
		_Node *_tmp = (_Node*)_pos._m_node;
		_prev_node->_m_next = _next_node;
		_next_node->_m_next = _prev_node;
		destroy(&_tmp->_m_data);
		_M_put_node(_tmp);
		return iterator((_Node)_next_node);
	}
	iterator erase(iterator _first, iterator _last)
	{
		while (_first != _last)
			erase(_first++);
		return _last;
	}
	void clear()
	{
		_M_clear();
	}
	void pop_front()
	{
		erase(begin());
	}
	void pop_back()
	{
		erase(--end());
	}
	void resize(size_type _new_size, const_reference _x)
	{
		iterator _i = begin();
		size_type _len = 0;
		for (; _i != end() && _len < _new_size; ++_i)
			++_len;

		if (_len == _new_size)// _i != end()
			erase(_i, end());
		else
			insert(_i, _new_size - _len, _x);
	}
	void resize(size_type _new_size)
	{
		resize(_new_size, _Tp());
	}

	list<_Tp, _Alloc>& operator=(const list<_Tp, _Alloc>& _x)
	{
		if(this != &_x)
		{
			iterator _first1 = begin();
			iterator _last1 = end();
			const_iterator _first2 = _x.begin();
			const_iterator _last2 = _x.end();
			while (_first1 != _last1 && _first2 != _last2)
				*_first1++ = *_first2++;

			if (_first2 == _last2)//表示first1中有剩余空间
				erase(_first1, _last1);
			else
				insert(_last1, _first2, _last2);
		}
		return *this;
	}

};



NAMESPACE_END
#endif//!_LIST_H_