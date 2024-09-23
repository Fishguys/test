// MyDll.cpp : 定义 DLL 应用程序的导出函数。
//

#include "MyDll2.h"
#include <iostream>

using namespace std;

// 这是导出变量的一个示例
MYDLL_API2 int nMyDll2 = 0;

// 这是导出函数的一个示例。
MYDLL_API2 int fnMyDll2(const int& i)
{
	std::cout << "fnMyDll" << std::endl;

	return i;
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 MyDll.h
CMyDll2::CMyDll2()
{
	return;
}

SingletonDll* SingletonDll::instance = nullptr;
std::mutex SingletonDll::mutex_;

SingletonDll* SingletonDll::getInstance()
{
    cout << "SingletonDll::getInstance " << instance << ", &instance: " << &instance << endl;
    std::lock_guard<std::mutex> lock(mutex_);
    if (instance == nullptr) {
        cout << "SingletonDll::getInstance new : " << instance << endl;
        instance = new SingletonDll();
    }
    return instance;
}
