// MyDll.cpp : 定义 DLL 应用程序的导出函数。
//

#include "MyDll.h"
#include "MyDll2.h"
#include <iostream>

// 这是导出变量的一个示例
MYDLL_API int nMyDll = 0;

// 这是导出函数的一个示例。
MYDLL_API int fnMyDll(int i)
{
	SingletonDll::getInstance();
	std::cout << "fnMyDll" << std::endl;

	return i;
}

MYDLL_API void commonFunc()
{
	std::cout << "fnMyDll commonFunc" << std::endl;
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 MyDll.h
CMyDll::CMyDll()
{
	return;
}
