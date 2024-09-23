// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "PreComplier.h"
#include <iostream>
#include "MyDll.h"
#include "MyDll2.h"
#include "StaticLib1.h"
#include "StaticLib2.h"
#include "FriendFeature.h"
#include "TextAnalyzer.h"
#include "ClassFeature.h"
#include "Tree.h"
#include "ProducerConsumer.h"
#include "WindowsBasic.h"
#include "TemplateFeature.h"
#include "LinkNode.h"
#include "ThreadPool.h"

using namespace std;
using ListNode = LinkNode::ListNode ;

void AnalyzeStaticInLibrary() {
    SingletonDll::getInstance();
    fnMyDll(7);
    SingletonLib::getInstance();
    fnStaticLib1();
}

void TextAnalyzerFunction()
{
    std::string filename = "C:\\Users\\15380\\source\\repos\\ConsoleApplication1\\x64\\Debug\\test.txt";
    TextAnalyzer analyzer(filename);
    analyzer.analyze();
}

void AnalyzerFriendFeature() {
    Outer& instance1 = getInstance(); // 通过友元函数获取实例
    instance1.doSomething();

    Outer& instance2 = Factory::createInstance();
    instance2.doSomething();
}

class DerivedSingle :public Singleton<DerivedSingle> {
    friend class Singleton<DerivedSingle>;
public:
    DerivedSingle(const DerivedSingle&) = delete;
    DerivedSingle& operator =(const DerivedSingle&) = delete;
private:
    DerivedSingle() {
        std::cout << "DerivedSingle base constructor called!" << std::endl;
    }
    ~DerivedSingle() {
        std::cout << "DerivedSingle base destructor called!" << std::endl;
    }
};

int main()
{
    DerivedSingle::Instance();
    Thread_Pool::testThreadPool();
    TemplateFeature::printArgs(1,2,3,"444");
    //PraticeWin::createWindow();
    //PraticeWin::InjectRemoteThread();
    ListNode* node = new ListNode(1);
    node->next = new ListNode(2);
    node->next->next = new ListNode(3);
    node->next->next->next = new ListNode(4);
    node->next->next->next->next = new ListNode(5);
    //testThreadPool();
    std::vector<int> vec({ 73,74,75,71,69,72,76,73 });
    LinkNode::Solution sln;
    std::vector<int> res = sln.dailyTemperatures(vec);
    //ProducerConsumer::testABC();
    //DerivedSingle::Instance();
    //ClassFeature::AnalyzeSharedPtr();
    //ClassFeature::PrintMultiInheritRelation();
    //TextAnalyzerFunction();
    //AnalyzerFriendFeature();
    //AnalyzeStaticInLibrary();
    //ClassFeature::AnalyzeVirtualFunctionAndClassMember();
}

