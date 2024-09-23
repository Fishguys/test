#include "PreComplier.h"

namespace ClassFeature
{
    using namespace std;

    class A {
    public:
        virtual void func() {
            std::cout << "funcA: " << a << " b: " << b << " c: " << c << std::endl;
        }

        virtual ~A() {
            std::cout << "~A()" << std::endl;
        }

        void testFunc() {
            std::cout << "testFunc" << std::endl;
        }
    private:
        int a = 1;
        int b = 2;
        int c = 3;
    };

    class DerivedA:public A {
    public:
        ~DerivedA() {
            std::cout << "~DerivedA()" << std::endl;
        }

        void testFunc() {
            std::cout << "DerivedA::testFunc" << std::endl;
        }
    };

    class B {
    public:
        virtual void func() {
            std::cout << "funcB: " << a << " b: " << b << " c: " << c << std::endl;
        }
        virtual void func1() {
            std::cout << "funcB1: " << a << " b: " << b << " c: " << c << std::endl;
        }
    private:
        int a = 4;
        int b = 5;
        int c = 6;
    };

    class C {
    public:
        void func() {
            std::cout << "funcC: " << a << " b: " << b << " c: " << c << std::endl;
        }
    private:
        int a = 7;
        int b = 8;
        int c = 9;
    };

    void AnalyzeVirtualFunctionAndClassMember()
    {
        A* pa = new A;
        B* pb = new B;
        C* pc = new C;
        B* ppb = (B*)pa;
        ppb->func();
        ppb->func1();
        C* ppc = (C*)pa;
        ppc->func();
        A* ppa = (A*)pc;
        ppa->func();
    }

    void AnalyzeSharedPtr()
    {
        DerivedA* pa = new DerivedA;
        A* parent = pa;
        parent->testFunc();
        delete parent;
        cout << "---------------------------------" << endl;

        DerivedA* paa = new DerivedA;
        shared_ptr<A> parent1(paa);
        parent1->testFunc();


    }

    ////////////////////////////////////////////////////////////////////////

    class Base {
    public:
        int ibase;
        virtual void f() { cout << "Base::f()" << endl; }
        virtual void Base_g() { cout << "Base::Base_g()" << endl; }
    };

    class Base1 : public Base {
    public:
        int ibase{ 101 };
        virtual void f() { cout << "Base1::f()" << endl; }
        virtual void Base1_g() { cout << "Base1::Base1_g()" << endl; }
        virtual void Base1_h() { cout << "Base1::Base1_h()" << endl; }
        Base1()
        {
            Base::ibase = 11;
        }
    };

    class Base2 : public Base {
    public:
        int ibase{ 102 };
        virtual void f() { cout << "Base2::f()" << endl; }
        virtual void Base2_g() { cout << "Base2::Base2_g()" << endl; }
        virtual void Base2_h() { cout << "Base2::Base2_h()" << endl; }
        Base2()
        {
            Base::ibase = 12;
        }
    };

    class Derive : public Base1, public Base2 {
    public:
        int ibase{ 1000 };
        virtual void f() { cout << "Derive::f()" << endl; }
        virtual void Base1_g() { cout << "Derive::Base1_g()" << endl; }
        virtual void Base2_g() { cout << "Derive::Base2_g()" << endl; }
        virtual void Derive_h() { cout << "Derive::Derive_h()" << endl; }
    };


    void PrintMultiInheritRelation()
    {
        typedef void(*Fun)(void);
        Derive d;
        d.ibase = 11;
        int** pVtab = (int**)&d;
        Fun pFun = nullptr;

        /*----------------------------Base1--------------------------------*/
        cout << "[0] Base1::_vptr" << "(" << pVtab << ")" << "->" << endl;
        for (int i = 0; nullptr != (Fun)pVtab[0][i]; i++) {
            pFun = (Fun)pVtab[0][i];
            cout << "    [0][" << i << "] ";
            pFun();
        }
        cout << "[1] Base::ibase Addr: " << &(d.Base::ibase) << " value: " << (int)pVtab[1] << endl;
        cout << "[2] Base1::ibase Addr: " << &(d.Base1::ibase) << " value: " << (int)pVtab[2] << endl;

        /*----------------------------Base2--------------------------------*/
        cout << "[3]Base2::_vptr" << "(" << &pVtab[3] << ")" << "->" << endl;
        for (int i = 0; i < 4; i++) {
            pFun = (Fun)pVtab[3][i];
            cout << "    [3][" << i << "] ";
            pFun();
        }
        cout << "[4] Base::ibase Addr: " << &(d.Base::ibase) << " value: " << (int)pVtab[4] << endl;
        cout << "[5] Base2::ibase Addr: " << &(d.Base2::ibase) << " value: " << (int)pVtab[5] << endl;
        cout << "[6] Derive::ibase Addr: " << &(d.Derive::ibase) << " value: " << (int)pVtab[6] << endl;
    }

}
