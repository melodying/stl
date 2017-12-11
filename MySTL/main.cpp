
#include "vector.h"
#include "list.h"
#include <stdio.h>
#include <vector>

class Test
{
public:
	Test()
	{
		index = 0;
		memset(data, 0, 40);
	}
	void SetData(int *arr, int len)
	{
		for (int i = 0; i < len; ++i)
		{
			data[i] = arr[i];
		}
	}
	int GetData(int index)
	{
		return data[index];
	}
private:
	int data[10];
	int index;
};



int main()
{
	/*BA::vector<Test> vec;

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
	catch(std::out_of_range e)
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
	vec4.clear();*/

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

	return 0;
}