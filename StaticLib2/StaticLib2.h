// StaticLib2.cpp : 定义静态库的函数。
//
#include <mutex>
// TODO: 这是一个库函数示例
class SingletonLib {
private:
    static SingletonLib* instance;
    static std::mutex mutex_;
    SingletonLib() {}
public:
    static SingletonLib* getInstance();
};
