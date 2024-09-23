// StaticLib1.cpp : 定义静态库的函数。
//

#include "StaticLib1.h"
#include "StaticLib2.h"

void fnStaticLib1()
{
	SingletonLib::getInstance();
}
