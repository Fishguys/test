#include <mutex>

//MyDll.h 头文件
#ifdef Dll2_EXPORTS
#define MYDLL_API2 __declspec(dllexport)
#else
#define MYDLL_API2 __declspec(dllimport)
#endif

// 此类是从 MyDll.dll 导出的
class MYDLL_API2 CMyDll2 {
public:
	CMyDll2();
	// TODO:  在此添加您的方法。
	void testFunc();
};

class MYDLL_API2 SingletonDll {
private:
    static SingletonDll* instance;
    static std::mutex mutex_;
    SingletonDll() {}
public:
    static SingletonDll* getInstance();
};

extern MYDLL_API2 int nMyDll2;
MYDLL_API2 int fnMyDll2(const int& i);
