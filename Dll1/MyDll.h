//MyDll.h 头文件
#ifdef DLL1_EXPORTS
#define MYDLL_API __declspec(dllexport)
#else
#define MYDLL_API __declspec(dllimport)
#endif

// 此类是从 MyDll.dll 导出的
class MYDLL_API CMyDll {
public:
	CMyDll();
	// TODO:  在此添加您的方法。
	void testFunc();
};

extern "C"
{
	extern MYDLL_API int nMyDll;
	MYDLL_API int fnMyDll(int i);
}

