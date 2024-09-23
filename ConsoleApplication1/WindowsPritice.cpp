#include "PreComplier.h"
#include "WindowsBasic.h"
#include <windows.h>
#include <TlHelp32.h>
#include <commctrl.h>
#include <iostream>
#include <string>
#include <vector>

namespace PraticeWin
{
#pragma comment(lib, "comctl32.lib")
    // 自定义消息，用于通知树视图项被选中
#define WM_TREEVIEW_SELCHANGE (WM_USER + 1)
#define BUTTON_ID 1001
// 函数声明
    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    void PopulateTreeView(const std::wstring& path, HTREEITEM hItem);
    void PopulateListView(const std::wstring& path);

    std::vector<wchar_t*> tvString;
    // 全局变量
    HWND g_Wnd;
    HWND g_edit;
    HWND g_button;
    HWND g_treeview;
    HWND g_listview;

    static void PrintError()
    {
        DWORD error = GetLastError();

        // 将错误码转换为消息字符串
        LPWSTR message = nullptr;
        size_t size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&message), 0, nullptr);

        if (size > 0)
        {
            std::wcout << L"错误信息: " << message << std::endl;
            LocalFree(message);
        }
        else
        {
            std::wcout << L"错误码: " << error << std::endl;
        }
    }

    // 窗口过程函数
    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
        case WM_COMMAND:
            if ((HWND)lParam == g_button)
            {
                // 获取编辑框中的路径
                wchar_t path[MAX_PATH];
                GetWindowText(g_edit, path, MAX_PATH);
                
                if (lstrlen(path) == 0)
                    break;

                // 填充树视图和列表视图
                PopulateTreeView(path,NULL);
                PopulateListView(path);
            }
            break;
        case WM_NOTIFY:
        {
            LPNMHDR nmhdr = (LPNMHDR)lParam;
            if (nmhdr->hwndFrom == g_treeview && nmhdr->code == TVN_SELCHANGED)
            {

                // 树视图项被选中，发送自定义消息通知主窗口
                NMTREEVIEW* pNMTreeView = (NMTREEVIEW*)lParam;
                HTREEITEM hSelectedItem = pNMTreeView->itemNew.hItem;

                TVITEM item;
                TCHAR szText[MAX_PATH+1];
                memset(szText, '\0', MAX_PATH + 1);
                memset(&item, 0, sizeof(item));
                item.mask = TVIF_TEXT;
                item.hItem = hSelectedItem;
                item.cchTextMax = MAX_PATH;
                item.pszText = szText;
                TreeView_GetItem(g_treeview, &item);

                // 填充列表视图
                std::wstring selectedPath(szText);
                std::wcout << "选中控件：" << selectedPath << std::endl;
                PopulateListView(selectedPath);
            }
        }
        break;
        case WM_TREEVIEW_SELCHANGE:
        {

        }
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        return 0;
    }

    /* *********************************************
    *功能：为treeview增加节点
    *参数：hwndTV,TreeView控件句柄
                lpsz，节点的名字
                hParent，父节点的句柄
    *返回值：创建的节点句柄
    **********************************************/
    HTREEITEM AddItemToTree(
        HWND hwndTV,
        LPWSTR lpszItem,
        HTREEITEM hParent
    )
    {
        TVITEM titem;
        TVINSERTSTRUCT tvins;
        HTREEITEM hme;
        //设置ITEM的参数
        titem.mask = TVIF_TEXT;
        titem.pszText = lpszItem;
        //填充STRUCT结构
        tvins.item = titem;
        tvins.hInsertAfter = TVI_ROOT;
        if (hParent == NULL)
        {
            tvins.hParent = TVI_ROOT;
        }
        else
        {
            tvins.hParent = hParent;
        }
        //调用关键的TreeView_InsertItem函数
        hme = TreeView_InsertItem(hwndTV, &tvins);
        return hme;
    }

    void PopulateTreeView(const std::wstring& path, HTREEITEM hRoot)
    {
        if (hRoot == NULL)
        {
            for (const auto& iter : tvString)
            {
                delete[] iter;
            }
            tvString.clear();
            // 清空树视图
            TreeView_DeleteAllItems(g_treeview);
            // 创建根项
            hRoot = AddItemToTree(g_treeview, (LPWSTR)path.c_str(), NULL);
        }


        // 遍历路径下的文件夹，递归填充树视图
        WIN32_FIND_DATAW findData;
        HANDLE hFind = FindFirstFileW((path + L"\\*").c_str(), &findData);
        if (hFind != INVALID_HANDLE_VALUE)
        {
            do
            {
                if (wcscmp(findData.cFileName, L".") != 0 && wcscmp(findData.cFileName, L"..") != 0)
                {
                    std::wstring subPath = path + L"\\" + findData.cFileName;
                    HTREEITEM hItem = AddItemToTree(g_treeview, (LPWSTR)subPath.c_str(), hRoot);
                    std::wcout << "-" << subPath << std::endl;

                    if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                    {
                        PopulateTreeView(subPath, hItem);
                    }
                }
            } while (FindNextFileW(hFind, &findData));
            FindClose(hFind);
        }
    }

    void initListColumn()
    {
        // 设置列表视图列
        LVCOLUMNW column;
        column.mask = LVCF_TEXT | LVCF_WIDTH;
        column.pszText = (LPWSTR)L"文件名";
        column.cx = 200;
        ListView_InsertColumn(g_listview, 0, &column);

        column.pszText = (LPWSTR)L"文件大小";
        column.cx = 100;
        ListView_InsertColumn(g_listview, 1, &column);

        column.pszText = (LPWSTR)L"文件类型";
        column.cx = 200;
        ListView_InsertColumn(g_listview, 2, &column);

        column.pszText = (LPWSTR)L"修改时间";
        column.cx = 150;
        ListView_InsertColumn(g_listview, 3, &column);
    }

    void addListViewItem(const WIN32_FIND_DATAW& findData)
    {
        if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            return;
        }

        LVITEMW item;
        item.mask = LVIF_TEXT;
        item.iItem = ListView_GetItemCount(g_listview);
        item.iSubItem = 0;
        item.pszText = (LPWSTR)findData.cFileName;
        ListView_InsertItem(g_listview, &item);

        std::wstring sizeStr = std::to_wstring(findData.nFileSizeLow);
        // 使用临时的 LPWSTR
        LPWSTR tempSizeStr = const_cast<LPWSTR>(sizeStr.c_str());
        ListView_SetItemText(g_listview, item.iItem, 1, tempSizeStr);

        std::wstring typeStr;
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
            typeStr = L"存档文件";
        else if (findData.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED)
            typeStr = L"压缩文件";
        else if (findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
            typeStr = L"隐藏文件";
        else if (findData.dwFileAttributes & FILE_ATTRIBUTE_NORMAL)
            typeStr = L"普通文件";
        else if (findData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
            typeStr = L"只读文件";
        else if (findData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)
            typeStr = L"系统文件";
        ListView_SetItemText(g_listview, item.iItem, 2, (LPWSTR)typeStr.c_str());

        // 将文件的修改时间转换为字符串
        FILETIME localFileTime;
        SYSTEMTIME sysTime;
        FileTimeToLocalFileTime(&findData.ftLastWriteTime, &localFileTime);
        FileTimeToSystemTime(&localFileTime, &sysTime);
        wchar_t timeStr[100];
        // 使用 swprintf_s 或 _snwprintf
        swprintf_s(timeStr, 100, L"%04d-%02d-%02d %02d:%02d:%02d", sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
        item.pszText = timeStr;
        ListView_SetItemText(g_listview, item.iItem, 3, timeStr);
    }

    void PopulateListView(const std::wstring& path)
    {
        // 清空列表视图
        ListView_DeleteAllItems(g_listview);

        // 遍历路径下的文件，填充列表视图
        WIN32_FIND_DATAW findData;
        HANDLE hFind = FindFirstFileW((path + L"\\*").c_str(), &findData);
        if (hFind != INVALID_HANDLE_VALUE)
        {
            do
            {
                if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                {
                    addListViewItem(findData);
                }
            } while (FindNextFileW(hFind, &findData));
            FindClose(hFind);
        }

    }

    int createWindow()
    {
        // 初始化通用控件库
        INITCOMMONCONTROLSEX icce;
        icce.dwSize = sizeof(INITCOMMONCONTROLSEX);
        icce.dwICC = ICC_TREEVIEW_CLASSES | ICC_LISTVIEW_CLASSES;
        InitCommonControlsEx(&icce);

        HMODULE hInstance = GetModuleHandle(NULL);
        // 1. 定义窗口类
            // 注册窗口类
        WNDCLASSEX wc;
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = WndProc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = hInstance;
        wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.lpszMenuName = NULL;
        wc.lpszClassName = L"FileBrowserClass";
        wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

        // 2. 注册窗口类
        if (!RegisterClassEx(&wc))
        {
            MessageBox(NULL, TEXT("窗口类注册失败"), TEXT("错误"), MB_OK | MB_ICONERROR);
            return 0;
        }

        // 3. 创建窗口
        g_Wnd = CreateWindowEx(
            0,
            L"FileBrowserClass",
            L"文件浏览器",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            800, 600,
            NULL,
            NULL,
            hInstance,
            NULL
        );

        if (!g_Wnd)
        {
            PrintError();
            MessageBox(NULL, TEXT("窗口创建失败"), TEXT("错误"), MB_OK | MB_ICONERROR);
            return 0;
        }

        // 创建编辑框（LineEdit）
        g_edit = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            L"EDIT",
            L"",
            WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
            10, 10, 300, 25,
            g_Wnd, NULL, hInstance, NULL);

        SetWindowText(g_edit, L"C:\\Users\\15380\\source\\repos\\ConsoleApplication1\\ConsoleApplication1");

        // 创建按钮
        g_button = CreateWindow(
            L"BUTTON",
            L"查找",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            320, 10, 80, 25,
            g_Wnd, NULL, hInstance, NULL);

        // 创建树视图
        g_treeview = CreateWindow(
            WC_TREEVIEW,
            L"",
            WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS,
            10, 50, 800, 400,
            g_Wnd, NULL, hInstance, NULL);

        // 创建列表视图
        g_listview = CreateWindow(
            WC_LISTVIEW,
            L"",
            WS_CHILD | WS_VISIBLE | LVS_REPORT,
            10, 400, 800, 400,
            g_Wnd, NULL, hInstance, NULL);

        initListColumn();
        // 4. 显示窗口
        ShowWindow(g_Wnd, 1);
        UpdateWindow(g_Wnd);

        // 5. 消息循环
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        return (int)msg.wParam;
    }
}