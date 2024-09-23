// StaticLib2.cpp : 定义静态库的函数。
//

#include "StaticLib2.h"
#include <iostream>
using namespace std;

SingletonLib* SingletonLib::instance = nullptr;
std::mutex SingletonLib::mutex_;

SingletonLib* SingletonLib::getInstance()
{
    cout << "SingletonLib::getInstance " << instance << ", &instance: " << &instance << endl;
    std::lock_guard<std::mutex> lock(mutex_);
    if (instance == nullptr) {
        cout << "SingletonLib::getInstance new" << instance << endl;
        instance = new SingletonLib();
    }
    return instance;
}

