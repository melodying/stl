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

	_Tp *_m_cur;			
	_Tp *_m_first;
	_Tp *_m_last;
	_Map_pointer _m_node;


	_Deque_iterater(_Tp *_x, _Map_pointer _y):_m_cur(_x), _m_first(*_y),
		_m_last(*_y + _S_buffer_size()),_m_node(_y)
	{
	}
	_Deque_iterater():_m_cur(nullptr), _m_first(nullptr), _m_last(nullptr), _m_node(nullptr){}
	_Deque_iterater(const iterater&_x):_m_cur(_x._m_node), _m_first(_x._m_first),
		_m_last(_x._m_last), _m_node(_x._m_node){}

	reference operator*()
	{
		return *_m_cur;
	}
	pointer operator->()
	{
		return &(operator*());
	}

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
			_m_cur = _m_last;	
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
		else//��Ҫ���пռ���Ծ
		{
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
	size_type _S_initial_map_size = 8;

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

	void _M_initialize_map(size_type _num_elements);
	void _M_create_nodes(_map_pointer _start, _map_pointer _finish);
	void _M_destory_nodes(_map_pointer _start, _map_pointer _finish);
	void _M_fill_initialize(const_reference _value);
	
protected:
	iterator _m_start;
	iterator _m_finish;
	_map_pointer _m_map;
	size_type _m_map_size;
public:
	iterator begin() { return _m_start; }
	iterator end() { return _m_finish; }
	const_iterator begin() const { return _m_start; }
	const_iterator end() const { return _m_finish; }

	reference operator[](size_type _n)
	{
		return _m_start[difference_type(_n)];
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
		return (*this)[_n];
	}

	size_type size()const
	{
		return _m_finish - _m_start;
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
		return *_tmp;
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
};

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

NAMESPACE_END

#endif//!_DEQUE_H_