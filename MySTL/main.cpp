
#include "vector.h"
#include "list.h"
#include "deque.h"
#include "stack.h"
#include "queue.h"
#include "tree.h"
#include "set.h"
#include "map.h"

#include <stdio.h>
#include <vector>
#include <map>

class Test
{
public:
	Test()
	{
		_len = 5;
		memset(data, 0, 40);
	}
	void SetData(int *arr, int len)
	{
		_len = len;
		for (int i = 0; i < len; ++i)
		{
			data[i] = arr[i];
		}
	}
	int GetData(int index)
	{
		return data[index];
	}

	void Print()
	{
		for (int i = 0; i < _len; ++i)
		{
			printf("%d ", data[i]);
		}
		printf("\n");
	}

	bool operator<(const Test &_t)const
	{
		return _len < _t._len;
	}
private:
	int data[10];
	int _len;
};



void ListTest()
{
	Test test[5];
	int arr[5] = { 1,2,3,4,5 };
	for (int i = 0; i < 5; ++i)
	{
		Test tmp;
		tmp.SetData(arr, 5);
		test[i] = tmp;
	}

	BA::list<Test> list;
	BA::list<Test> list1(test, test + 5);
	BA::list<Test> list2(list1.begin(), list1.end());
	BA::list<Test> list3(5);
	BA::list<Test> list4(list2);

	BA::list<int> li(arr, arr + 5);
	BA::list<int> li2;
	for (int i = 6; i < 11; ++i)
		li2.push_back(i);

	BA::list<int>::iterator it = li2.begin();
	++it;
	++it;
	//li2.splice(it, li);
	li2.merge(li);

	for (auto it1 = li2.begin(); it1 != li2.end(); ++it1)
	{
		printf("%d ", *it1);
	}

	printf("\n");
	for (auto it1 = li.begin(); it1 != li.end(); ++it1)
	{
		printf("%d ", *it1);
	}
	li.clear();
	li.push_back(1);
	li.push_back(3);
	li.push_back(2);
	li.push_back(5);
	li.push_back(4);
	li.sort();
	li.begin();
	li.end();
	li.empty();
	li.size();
	li.front();
	li.back();
	li.pop_back();
	li.push_front(0);
	li.pop_front();
	li.insert(li.begin(), 4);
	li.erase(li.begin());
	li.resize(8);
	li.remove(3);
}

void VectorTest()
{
	BA::vector<Test> vec;

	int arr[5] = { 1,2,3,4,5 };
	std::vector<Test> vec4;
	for (int i = 0; i < 5; ++i)
	{
		Test tmp;
		tmp.SetData(arr, 5);
		vec.push_back(tmp);
		vec4.push_back(tmp);
	}

	for (auto value : vec)
	{
		for (int i = 0; i < 5; ++i)
		{
			printf("%d ", value.GetData(i));
		}
		printf("\n");
	}
	try
	{
		vec.at(1);
	}
	catch (std::out_of_range e)
	{
		printf("%s", e.what());
	}

	Test *tmp = vec.begin();
	tmp = vec.end();
	tmp = vec.rbegin();
	tmp = vec.rend();

	BA::vector<Test> vec1(vec);
	BA::vector<Test> vec2 = vec;
	BA::vector<Test> vec3;
	vec3 = vec;

	int size = vec.size();
	size_t max_size = vec.max_size();
	vec.resize(10);
	int space = vec.capacity();
	Test tmp1 = vec[1];
	tmp1 = vec.front();
	tmp1 = vec.back();
	vec.erase(vec.begin() + 2);
	vec.swap(vec2);

	vec3.clear();
	vec4.clear();
}

void DequeTest()
{
	Test test;
	Test t1[10];
	int arr[5] = { 1,2,3,4,5 };
	test.SetData(arr, 5);
	BA::deque<Test> deque(5, test);

	for (int i = 0; i < deque.size(); ++i)
		deque[i].Print();

	
	deque.push_front();
	for (int i = 0; i < deque.size(); ++i)
		deque[i].Print();

	const BA::deque<Test> de;
	BA::deque<Test>::const_iterator it(de.begin());

	deque.insert(deque.begin() + 2, test);
	deque.insert(deque.begin() + 1, 10, test);
	deque.insert(deque.begin(), it,it + 1);
	deque.insert(deque.begin(), deque.begin(), deque.end());
}


void StackTest()
{
	BA::stack<int> s;
	BA::stack<int> s1;
	s.push(1);
	s.pop();
	s.top();
	s.empty();
	s.size();
	if (s == s1)
	{

	}
	if (s < s1)
	{

	}
}

void QueueTest()
{
	BA::queue<int> s;
	BA::queue<int> s1;
	s.push(1);
	s.pop();
	
	s.empty();
	s.size();

	if(s == s1)
	{
		
	}
	if(s < s1)
	{
		
	}
	
}

void TreeTest()
{

	BA::_Rb_tree<int, int, int , int> tree;
	//tree.find(1);
}


void setTest()
{
	Test test[5];
	int arr[5] = { 1,2,3,4,5 };
	for (int i = 0; i < 5; ++i)
	{
		Test tmp;
		tmp.SetData(arr, 5);
		test[i] = tmp;
	}
	
	BA::set<Test> s;
	s.begin();
	s.end();
	s.clear();
	for (int i = 0; i < 5; ++i)
	{
		s.insert(test[i]);
	}

	printf("%d\n", s.count(test[2]));
	
}

void mapTest()
{
	Test test[5];
	int arr[5] = { 1,2,3,4,5 };
	for (int i = 0; i < 5; ++i)
	{
		Test tmp;
		tmp.SetData(arr, 5);
		test[i] = tmp;
	}

	BA::map<char, Test> m;

	for (int i = 0; i < 5; ++i)
	{
		const char c = i + '0';
		// TODO: map的插入bug。又一个由const引发的错误
		// 在map中的value_type的key应为const。因为构造的pair中的key并不是const。造成无法插入
		// 如果将构造的pair中的first_type设置为const。make_pair()无法工作
		// 如果使用pair的拷贝构造则没有问题。问题应该是出在const的成员变量只能初始化。无法赋值
		
		// 需要手动构造const first_type的pair。使用起来不够友好
		BA::pair<const char, Test> pair(c, test[i]);
		
		m.insert(pair);
	}

	m['0'].Print();
}

int main()
{
	//VectorTest();
	//ListTest();

	//DequeTest();
	
	//StackTest();i
	mapTest();

	
	return 0;
}