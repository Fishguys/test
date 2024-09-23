#include "PreComplier.h"

class Outer {
private:
    Outer() {} // 私有构造函数
    ~Outer() {}

    Outer(const Outer&) = delete;
    Outer& operator=(const Outer&) = delete;

    friend Outer& getInstance(); // 声明getInstance为友元函数
    friend class Factory;
public:
    void doSomething() {
        std::cout << "Outer is doing something..." << std::endl;
    }
};

// 全局函数，通过友元权限访问Outer的私有构造函数
Outer& getInstance() {
    static Outer instance; // 局部静态变量
    return instance;
}

class Factory {
private:
    Factory() {}

public:
    static Outer& createInstance() {
        static Outer instance;
        return instance;
    }
};

class HeapOnly {
public:
    // 公有的构造函数
    HeapOnly() {}

    // 提供一个自定义销毁函数
    void destroy() { delete this; }

private:
    // 私有的析构函数，阻止栈上销毁
    ~HeapOnly() {}

    // 禁用拷贝构造函数和赋值运算符
    HeapOnly(const HeapOnly&) = delete;
    HeapOnly& operator=(const HeapOnly&) = delete;
};