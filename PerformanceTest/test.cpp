#include <iostream>
#include <cstdint>
#include <cstring>
#include <assert.h>
//#include <intrin.h> 
//#include <thread>
//#include "MemoryPool.h" 
//#include "boost/lockfree/stack.hpp"
//#include "boost/asio/thread_pool.hpp"
//#include "boost/asio/post.hpp"
#include "System.h" 



//
//#define var SP
//#define lock System::Threading::Thread::Lock
////#define Random System_Random
//
//
using namespace System;
using namespace System::Diagnostics;
using namespace System::Threading;
using namespace System::Collections::Generic;
//
//class Test
//{
//public:
//	int x;
//	~Test() {
//		//std::cout << "Test" << std::endl;
//	}
//};
//
//class TestD
//{
//public:
//	~TestD() {
//		std::cout << "Test" << std::endl;
//	}
//
//};
//
//int x;
//
//void task0()
//{
//	for (int j = 0; j < 100; j++) {
//		for (int i = 0; i < 3000000000; ++i) {
//		}
//	}
//}
//
//ulong GetThreadID()
//{
//	//static_assert((sizeof(std::thread::id) == sizeof(std::thread::id)), "this function only works if size of thead::id is equal to the size of uint_64");
//
//	//int* y = (int*)&(std::this_thread::get_id());
//
//	return (ulong)GetCurrentThreadId();
//
//}
//
////int accy1[5];
//int task1()
//{
////	accy1 = new int[5];
//
//
//	int acc = 0;
//
//	for (int i = 0; i < 1000000000; i++)
//	{
//		Test t;
//		t.x = i;
//		acc += t.x;
//		//GetThreadID();
//		{
//		//	int tmp = x + 1;
//		//	Thread::Sleep(100);
//		//	Console::WriteLine(x);
//		////	Console::WriteLine(Thread::CurrentThread()->ManagedThreadId);
//		//	x = tmp;
//		}
//	}
//	//Random* o5b = new Random();
//	//Console::WriteLine((int64_t)o5b);
//	//Thread::Sleep(1000);
//	//std::cout << acc << std::endl;
//	return acc;
//}
//
//ManualResetEvent mre;
//
//std::string* strptr;
//
//FixedSizeMemoryPool fsmp;
//VariableSizeMemoryPool vsmp;
//
//
//
//
//int task2()
//{
//
//
//
//	vsmp.pools[7].Create((1ull << 7) - sizeof(byte*)-sizeof(byte*)-1);
//	Purgatory::RegisterMemoryPool(&vsmp.pools[7]);
//
//
//	//vsmp.pools[7].Create((1ull << 7) - sizeof(byte*)-sizeof(byte*)-1, 0);
//
//	//fsmp.Create(64, 0);
//	byte* ptr;
//	//byte* ptr2;
//	//ptr = fsmp.Get(64);
//	//ptr2 = fsmp.Get(64);
//	//fsmp.Put(ptr);
//	//fsmp.Put(ptr2);
//
//	size_t acc = 0;
//	//while (++i < 1000000000) 
//
//	//ptr = vsmp.Get(64);
//	//size_t* ptrx = (size_t*)ptr;
//
//	for (int i = 0; i < 10000000; ++i)
//		//	std::cout << "CALLED" << std::endl;
//	{
//		{
//		//	int* ptr = new int[64];
//		//	delete ptr;
//		//	*ptrx = i;
//		//	acc += *ptrx;
//			ptr = vsmp.Get(64);
//			*ptr = 37;
//
//			vsmp.Put(ptr);
//		//	acc += GetThreadID();
//			//	Object o;
//			//	o.dta = 37;
//			//std::string* str = new std::string();
//			//delete str;
//
//			//std::string str;
//			//str += i;
//			//strptr = &str;
//			//	o.dta = i;
//			//	o.test();
//		}
//		//	ManualResetEvent mre;
//		//Test tst;
//		//	mre.WaitOne(1000);
//		//	mre.Reset();
//	}
//	return acc;
//	//	mre.WaitOne(1000);
//	//	mre.Reset();
//}
//
//__forceinline double get_value()
//
//{
//	
//	return 0;
//
//}
//
//
//
//void task3()
//{
//
//	for (int i = 0; i < 1; ++i) {
//		//	std::cout << "CALLED" << std::endl;
//		{
//			
//				Object o;
//				
//			//	o.dta = 37;
//				mre.WaitOne(1000);
//				mre.Reset();
//		}
//		mre.WaitOne(1000);
//		mre.Reset();
//	}
//
//
//}
//
//Test tg;
//Test* tgp;
//
////#pragma optimize( "", off )
//void task4()
//{
//	Test t;
//	size_t acc;
//	tgp = new Test();
//
//	for (int i = 0; i < 1000000000; ++i)
//	{
//			t.x = i;
//			acc += t.x;
//
//		
//	}
//	std::cout << acc << std::endl;
//	delete tgp;
//}
////#pragma optimize( "", on ) 
//
//
//std::atomic<size_t> sdata;
//
//#pragma optimize( "", off )
//void task5()
//{
//	size_t acc;
//	for (int i = 0; i < 1000000000; ++i)
//	{
//		//sdata.store(i, std::memory_order_relaxed);
//		acc += i; // sdata.load(std::memory_order_relaxed);
//	}
//	std::cout << acc << std::endl;
//	delete tgp;
//}
//#pragma optimize( "", on ) 
//
//__forceinline int TestInteger() {
//	int ret = 0;
//	int max = 1000000000;
//	for (int i = 0; i < max; i++)
//	{
//		ret += i;
//	}
//	return ret;
//}
//
//__forceinline bool TestNull() {
//	bool ret = true;
//	Object o = null;
//	ret &= o == null;
//	Object o2;// = new Object();
//	ret &= o == o2;
//	o = o2;
//	ret &= o == o2;
//	o = o2 = null;
//	ret &= o == o2;
//
//	return ret;
//}
//
//__forceinline bool TestBoxing() {
//	bool ret = true;
//
//	int i = 1;
//	Object o = i;
//	Object o2;
//	o2 = i;
//	int j = (int)o;
//	ret &= o == 1;
//	ret &= o2 == 1;
//	ret &= o2 == o;
//	ret &= j == 1;
//
//	return ret;
//}
//
//
//void test()
//{	
//	
//	StopWatch sw;
//
//	//sw.Start();
//
//	//std::thread trd1 = std::thread(task5);
//	//std::thread trd2 = std::thread(task5);
//	//trd1.join();
//	//trd2.join();
//
//	//sw.Stop();
//
//	//std::cout << "DONE " << sw.Get_ElapsedMilliseconds() << std::endl;
//
//	for (int j = 0; j < 10; j++)
//	{
//		sw.Start();
//		int acc = task1();
//		sw.Stop();
//		std::cout << "T1 " << sw.Get_ElapsedMilliseconds() << " " << acc << std::endl;
//		sw.Start();
//		task2();
//		sw.Stop();
//		std::cout << "T2 " << sw.Get_ElapsedMilliseconds() << " " << acc << std::endl;
//		//std::cout << "DONE " << sw.Get_ElapsedMilliseconds() << std::endl;
//
//	//	Thread::Sleep(1000);
//	}
////	Thread::Sleep(10000);
//	GC::Collect();
////	Thread::Sleep(10000);
//std::cout << "EXIT " << std::endl;
//
//
////Purgatory::Active = false;
//	//Purgatory::reaper.join();
//
//	return;
//
//	while (std::cin.get())
//	{
//		std::cout << "cinget" << std::endl;
//		mre.Set();
//
//	}
//
//
//	Array<int> arr1 = Array<int>(5);
//	Array<Random> arr2 = Array<Random>(6);
//
//	Random rnd;
//	for (int i = 0; i < arr1.Length; i++)
//	{
//		
////		arr1[i] = rnd.Next(10);
//	//	Random bla = arr1[1];
//	}
//
////	Object arr3 = null;
//
////	arr1 = arr2;
//
//	Console::WriteLine(arr1.Length);
//}
//
//#define var auto
//
//
//void testa()
//{
//	//Object o = new Object(37);
////	Console::WriteLine(o.dta);
//} 
//
//void testb() {
//
////	Console::WriteLine(o.dta);
//
////	Object arr[] = {new Object(1), new Object(2)};
////	Object arr2[2]; // = new Object[2];// { new Object(1), new Object(2) };
////	Object arr[2] = new Object[] { new Object(1), new Object(2) };
//
//	Object* arr3 = new Object[2];
//	Console::WriteLine("");
//	for (int i = 0; i < 2; i++)
//	{
//		//Object* nptr = new Object(i + 50);
//		Console::WriteLine("");
//		//arr3[i] = nptr;
//		Console::WriteLine("");
//	}
//
////	Object o2;
////	delete o2;
////	DateTime dt;
////	Array<int> arr(2);
////	arr[0] = 37;
////	Console::WriteLine(arr3[0].dta);
//	delete[] arr3;
//}
//
////#define object Object;
//
//void testc()
//{
//	Array<int> arr2 = new Array<int>(2);
//	arr2[1] = 37;
//	Console::WriteLine(arr2[1]);
////	return;
//	
//	Object* o = new Array<int>(1);
//	delete o;
//
////	Base* tst = new Derived<int>();
////	Console::WriteLine( tst->Derive());
//
//	Array<Object> arr = new Object[1];
//	GC::Collect();
//	arr[0] = new Object();
//	//Object obj = new Object(37);
//	GC::Collect();
//	//arr[0] = obj;
//	//GC::Collect();
////	Console::WriteLine(arr.Length);	
////	Console::WriteLine(arr[1].dta);
//	
//	arr = new Array<Object>(1);
//	//arr[0] = new Object(37);
//	//arr[0].dta = 50;
//	//Console::WriteLine(arr[0].dta);
//	
//}
//
//void testd() {
//	List<int> lsti = new List<int>();
//	for (int i = 37; i < 137; i += 2) {
//		lsti.Add(i);
//	}
//
//	//Func<int, bool> f = new Func<int, bool>([](int x) { return x < 50; });
//	//IEnumerator<int> enu = lsti.Where([](int x) -> bool { return x < 50; }).GetEnumerator();
//	//while (enu.MoveNext())
//	//{
//	//	Console::WriteLine(enu.Current);
//	//}
//
//}
//
//	//template < typename Ret, typename... Args >
//	//using Func = std::function< Ret(Args...) >;
//
//
//// A class template that just a list of types:
////template <typename T, typename... R> struct TLast { 
////	typedef typename TLast<R...>::result result;
////};
////
////template <typename T> struct TLast<T> {
////	typedef typename T result;
////};
//
//
//
//template <typename T, typename R> class Fun : public Object {
//public:
//	// typedef R(*FTP1)(T);
//
//};
//
//
//double testee(int x) { return x; }
//double testeee(int x, int y) { return x + y; }
//
//double testeeimplicit(Func<int, double> f) { return f(0);  }
//

void ASSERT_TRUE(bool result) {

}

void testAction() {
	int x = 1;
	Action<> act = new Action<>([&x]() { 
		Console::WriteLine(x + 7);
		x += 3;
	});
	act();
	Action<int> act2 = new Action<int>([x](int y) { Console::WriteLine(x + y); });
	act2(37);
}

void testFunc() {
//	typelist<int, double> x;
	//Fun<int, bool>::result bla;
//	Thread::Sleep(1000);
	//bla = 0.1;

	Func<double> tst([]() { return 37; });
	int y = 37;
	Func<int, int> f = [y](int in) {return in + 1 + y; };// = new Func<int, int>([](int x) { return 37; });
	int res = f(25);
	std::cout << "bla";
    Console::WriteLine(res);
	//int(*foo)(int);
	/* the ampersand is actually optional */
//	ctst c();
	
	//Func<Object, int, double> memberfunc = [](Object ct, int in) {return in + 1; };
	//memberfunc = &Object::test;

	//memberfunc(Object(null), 5);

	//foo = f;
	
//	std::function<int(int)> stdf = f;
//	Console::WriteLine(stdf(27));

	//double x = testeeimplicit(f);

//	Func<double, int> f = foo;
}

void TestTuple() {
	Tuple<int, string> tpl(1, "bla");
	tpl.Item2 = "blo";
	Console::WriteLine(tpl.Item2);

}

void TestThread() {
	int start = 37;

	Action<> act2 = []() {
		Console::WriteLine("starting 2");
	};
	
	act2 = []() {
		Console::WriteLine("starting 2b");
	};
	
	Action<>* act3 = new Action<>([]() {
		Console::WriteLine("starting 3");
	});

	if (!act3)
		throw Exception();

//	return;
	//act2();
	Thread t = Thread([start, act2, act3]() {
		//Action<> act4 = []() {
		//	Console::WriteLine("starting 4");
		//};
		//act2();
		//(*act3)();
		//act4();
		//delete act3;
		//int x = 0;
		//while (x < 5) {
		//	Console::WriteLine(start + x++);
		//	Thread::Sleep(1000);
		//}
	});
	//t.IsBackground = true;
	t.Start();
//	t.Join();

}

void TestStringFunc(string str) {
	Console::WriteLine(str);
}

void TestString() {
	string nstr;
	if (nstr == null) {
		Console::WriteLine("okay");
	}
	string str = "blabla";
	//return;
	Console::WriteLine(str);
	TestStringFunc(str);
	Console::WriteLine((int)str.Length);
	string str2 = str + "bloblo";
	Console::WriteLine(str2);
	str2 = "bloblo" + str;
	Console::WriteLine(str2);
	str2 = u"BloBĦlo" + str;
	Console::WriteLine(str2);
	Console::WriteLine((int)str2.Length);
	
	bool eq = str == "blabla";
	ASSERT_TRUE(eq);
	bool eq2 = str == null;
	ASSERT_TRUE(!eq2);
	str = null;
	bool eq3 = str != null;
	ASSERT_TRUE(!eq3);

	int pos = str2.IndexOf("oB");
	Console::WriteLine(pos);
	pos = str2.IndexOf(u"BĦl");
	Console::WriteLine(pos);
	pos = str2.IndexOf(u"BĦlll");
	Console::WriteLine(pos);

	string str3 = str2.Substring(2, 4);
	Console::WriteLine(str3);

	string str4 = str2.ToLower();
	Console::WriteLine(str4);

	string str5 = str2.Replace("b", "qu");
	Console::WriteLine(str5);
	//TestStringFunc("bliblo");

}

void TestArray() {
	Array<int> narr;
	if (narr == null) {
		Console::WriteLine("okay");
	}

	Array<int> arr(2);
	arr[1] = 37;
	Console::WriteLine(arr[1]);
	Array<int> arr2 = arr;

	Console::WriteLine(arr2[1]);

	Console::WriteLine((int)arr2.Length); // only one user defined implicit conversion allowed :(
	Console::WriteLine(String(arr2.Length));

	Array<string> strarr(3);
	strarr[0] = "blabla";
	Console::WriteLine(strarr[0]);
	if (strarr[1] == null) {
		Console::WriteLine("yep");
	}

	Array<string> strarr2({ "bla", "bli", "blo" });
	for (int i = 0; i < 3; i++) {
		Console::WriteLine(strarr2[i].ToString());
		string strx = strarr2[i];
		Console::WriteLine(strx.ToString());
	}
}

void TestArray2() {
	Array<string> arr(2);
	arr[0] = "bla";
}


void TestList() {
	{
		List<string> strs = new List<string>();
		strs.Add("bla");
	}
	size_t sz = GC::Collect();
	ASSERT_TRUE(sz == 0);
}

void TestDictionary() {
	//Dictionary<int, int> ndic = null;
	//ASSERT_TRUE(ndic == null);

	//Dictionary<int, int> dic = new Dictionary<int, int>();
	//dic.Add(0, 1);
	//dic.Add(1, 0);

	//int x;
	//ASSERT_TRUE(dic.TryGetValue(0, x));
	//ASSERT_TRUE(x == 1);
	//ASSERT_TRUE(dic.Contains(1));
	//ASSERT_TRUE(!dic.Contains(2));
	//ASSERT_TRUE(dic.Remove(1));
	//ASSERT_TRUE(!dic.Remove(1));
	//ASSERT_TRUE(!dic.Contains(1));
	//dic[1] = 37;
	//ASSERT_TRUE(dic[1] == 37);
	//dic[2] = 38;
	//ASSERT_TRUE(dic[2] == 38);
	{
		Dictionary<string, Object> odic = new Dictionary<string, Object>();
		odic.Add("a", 0);
	}
	size_t sz = GC::Collect();
	ASSERT_TRUE(sz == 0);


//	odic.Add("b", 0);
}

void TestProcess() {
	int processorcount = Environment::ProcessorCount;

	System::Collections::Generic::Array<Process> arr = Process::GetProcesses();
	int cnt = 0;
	for (int i = 0; i < arr.Length; i++) {
		Process p = arr[i];
		cnt += (int) p.TotalProcessorTime->Ticks;
		Console::WriteLine(arr[i]->ProcessName + " " + (String)p.TotalProcessorTime->Ticks);
	}
}

void TestPerformance() {
	System::Diagnostics::Stopwatch sw = new System::Diagnostics::Stopwatch();
	sw.Start();
	long cnt = 0;
	for (int i = 0; i < 100000000; i++) {
		cnt += i;
	}
	sw.Stop();
	Console::WriteLine(cnt);
	Console::WriteLine((long)sw.ElapsedMilliseconds);
}

void TestPerformanceList() {

	System::Diagnostics::Stopwatch sw = new System::Diagnostics::Stopwatch();
	sw.Start();
	long cnt = 0;
	for (int i = 0; i < 1000000; i++) {
		System::Collections::Generic::List<int> lst = new System::Collections::Generic::List<int>();
		for (int j = 0; j < 1000; j++) {
			lst.Add(i + j);
			cnt += i + j;
		}
	}
	sw.Stop();
	Console::WriteLine(cnt);
	Console::WriteLine((long)sw.ElapsedMilliseconds);
}

void TestPerformanceIterator() {
	System::Collections::Generic::List<int> lst = new System::Collections::Generic::List<int>();
	for (int j = 0; j < 1000; j++) {
		lst.Add(j);
	}

	System::Diagnostics::Stopwatch sw = new System::Diagnostics::Stopwatch();
	sw.Start();
	long cnt = 0;
	System::Collections::Generic::IEnumerator<int> enu = lst.GetEnumerator();
	/**/
	//int* cur;
	System::Collections::Generic::IEnumerator<int>::MoveNextGetCurrentFN* mngc = enu.GetFP_MoveNextGetCurrent();
	//System::Collections::Generic::IEnumerator<int>::ResetFN* rst = enu.GetFN_Reset();
	for (int i = 0; i < 1000000; i++) {
		//while (cur = mngc(enu.od)) {
		//	cnt += *cur;
		//}
		//enu.Reset();
		for (int j : lst) {
			cnt += j;
		}
	}
	/*/
	for (int i = 0; i < 1000000; i++) {
		//for (int j : lst) {
		//	cnt += j;
		//}
		//for (int j = 0; j < 1000; j++)
		//	cnt += lst[j];
		
		while (enu.MoveNext()) {
			cnt += enu.Current;
		}
		enu.Reset();
	}
	/**/
	sw.Stop();
	Console::WriteLine(cnt);
	Console::WriteLine((long)sw.ElapsedMilliseconds);
}





class A {
public:
	virtual int GetVirtual() = 0;
	Func<int> GetFunc;
};

class B : public A {
public:
	B() {
		GetFunc = []() {
			return 37;
		};
	}

	int GetVirtual() override {
		return 37;
	}

};


void TestPerformanceVirtualFunc() {
	B* b = new B();
	A* a = b;
	System::Diagnostics::Stopwatch sw = new System::Diagnostics::Stopwatch();
	sw.Start();
	long cnt = 0;
	for (int i = 0; i < 1000000000; i++) {
		//cnt += a->GetVirtual();
		cnt += a->GetFunc();
	}
	sw.Stop();
	Console::WriteLine(cnt);
	Console::WriteLine((long)sw.ElapsedMilliseconds);
}

void SimpleAdd(long& cnt, long y) { cnt += y; }

void TestPerformanceLambda() {
	//System::Collections::Generic::List<int> lst = new System::Collections::Generic::List<int>();
	//for (int j = 0; j < 1000; j++) {
	//	lst.Add(j);
	//}

	System::Diagnostics::Stopwatch sw = new System::Diagnostics::Stopwatch();
	sw.Start();
	long cnt = 0;
	
	auto lambda = [&cnt](long y) { cnt += y; };
	std::function<void(long)> f = lambda;
	System::Action<long> act = lambda;
	System::VAction<long> vact = lambda;

	//System::VA

	for (long i = 0; i < 1000000000; i++) {
		//lambda(i);
		//SimpleAdd(cnt, i);
		//f(i);
		act(i);
	}



	sw.Stop();
	Console::WriteLine(cnt);
	Console::WriteLine((long)sw.ElapsedMilliseconds);
}

void TestPerformanceLinqWhere() {
	System::Collections::Generic::List<int> lst = new System::Collections::Generic::List<int>();
	for (int j = 0; j < 1000; j++) {
		lst.Add(j);
	}

	System::Diagnostics::Stopwatch sw = new System::Diagnostics::Stopwatch();
	sw.Start();
	long cnt = 0;

	for (long i = 0; i < 100000; i++) {
		List<int> filtered = lst.Where([](int x) { return x > 500; }).ToList();
		cnt += filtered.Count;
	}

	sw.Stop();
	Console::WriteLine(cnt);
	Console::WriteLine((long)sw.ElapsedMilliseconds);
}

void TestPerformanceLinqOrderBy() {
	System::Collections::Generic::List<int> lst = new System::Collections::Generic::List<int>();
	for (int j = 0; j < 1000; j++) {
		lst.Add(j);
	}

	System::Diagnostics::Stopwatch sw = new System::Diagnostics::Stopwatch();
	sw.Start();
	long cnt = 0;

	for (long i = 0; i < 1000; i++) {
		
		List<int> filtered = 
			lst.OrderBy<int>([](int x) { return x;  }).ToList();
		cnt += filtered.Count;
	}

	sw.Stop();
	Console::WriteLine(cnt);
	Console::WriteLine((long)sw.ElapsedMilliseconds);
}

void MethodWithRef(Ref<int> result) {
	result = result + 37;
}

void TestPerformanceRef() {
	System::Diagnostics::Stopwatch sw = new System::Diagnostics::Stopwatch();
	sw.Start();

	long cnt = 0;
	for (int i = 0; i < 1000000000; i++) {
		int x = i;
		MethodWithRef(ref(x));
		cnt += x;
	}

	sw.Stop();
	Console::WriteLine(cnt);
	Console::WriteLine((long)sw.ElapsedMilliseconds);
}

void TestPerformanceThread() {
	System::Diagnostics::Stopwatch sw = new System::Diagnostics::Stopwatch();
	sw.Start();

	//System::Threading::ManualResetEvent mre = new System::Threading::ManualResetEvent(false);
	long cnt = 0;
	for (int i = 0; i < 100000; i++) {
		System::Threading::Thread t([&cnt]() {
			cnt++;
			//mre.Set();
			});
		t.Start();
		//mre.WaitOne(600000);
		t.Join();
	//	mre.Reset();
	}

	sw.Stop();
	Console::WriteLine(cnt);
	Console::WriteLine((long)sw.ElapsedMilliseconds);
}


void TestPerformanceManualResetEvent() {
	System::Diagnostics::Stopwatch sw = new System::Diagnostics::Stopwatch();
	sw.Start();

	System::Threading::ManualResetEvent mre = new System::Threading::ManualResetEvent(false);
	long cnt = 0;
	for (int i = 0; i < 100000; i++) {
		System::Threading::Thread t([&mre, &cnt]() {
			cnt++;
			mre.Set();
		});
		t.Start();
		mre.WaitOne(600000);
		t.Join();
		mre.Reset();
	}

	sw.Stop();
	Console::WriteLine(cnt);
	Console::WriteLine((long)sw.ElapsedMilliseconds);
}

void TestPerformanceStack() {
	System::Diagnostics::Stopwatch sw = new System::Diagnostics::Stopwatch();
	sw.Start();
	//int numThreads = 1;
	System::Collections::Generic::Stack<int> stack = new System::Collections::Generic::Stack<int>();
	//System::Collections::Generic::List<System::Threading::ManualResetEvent> lstthreads = new System::Collections::Generic::List<System::Threading::ManualResetEvent>();


	//for (int t = 0; t < numThreads; t++) {
		//System::Threading::ManualResetEvent mre = new System::Threading::ManualResetEvent(false);
		//System::Threading::ThreadPool::QueueUserWorkItem([&stack, mre]() mutable {

		for (int i = 0; i < 1000000; i++) {
			for (int q = 0; q < 100; q++) {
				stack.Push(q);
			}
			int cnt = 0;
			for (int j = 0; j < 100; j++) {
				cnt += j;
				//int res;
				stack.Pop();
			}
		}
		//mre.Set();
		//});
	//lstthreads.Add(mre);
	//}
	//for (int t = 0; t < numThreads; t++) {
	//	lstthreads[t].WaitOne();
	//}

	sw.Stop();
	Console::WriteLine((long)sw.ElapsedMilliseconds);
}


void TestPerformanceConcurrentStack() {
	System::Diagnostics::Stopwatch sw = new System::Diagnostics::Stopwatch();
	sw.Start();
	int numThreads = 1;
	System::Collections::Concurrent::ConcurrentStack<int> stack = new System::Collections::Concurrent::ConcurrentStack<int>();
	//System::Collections::Generic::List<System::Threading::ManualResetEvent> lstthreads = new System::Collections::Generic::List<System::Threading::ManualResetEvent>();


	for (int t = 0; t < numThreads; t++) {
		//System::Threading::ManualResetEvent mre = new System::Threading::ManualResetEvent(false);
		//System::Threading::ThreadPool::QueueUserWorkItem([&stack, mre]() mutable {

			for (int i = 0; i < 1000000; i++) {
				for (int q = 0; q < 100; q++) {
					stack.Push(q);
				}
				int cnt = 0;
				for (int j = 0; j < 100; j++) {
					cnt += j;
					int res;
					stack.TryPop(out(res));
					//stack.Pop();
				}
			}
			//mre.Set();
			//});
		//lstthreads.Add(mre);
	}
	//for (int t = 0; t < numThreads; t++) {
	//	lstthreads[t].WaitOne();
	//}

	sw.Stop();
	Console::WriteLine((long)sw.ElapsedMilliseconds);
}
/*
void TestPerformanceConcurrentStackBOOST() {
	System::Diagnostics::Stopwatch sw = new System::Diagnostics::Stopwatch();
	sw.Start();
	int numThreads = 1;
	boost::lockfree::stack<int> stack(128);
	//System::Collections::Generic::List<System::Threading::ManualResetEvent> lstthreads = new System::Collections::Generic::List<System::Threading::ManualResetEvent>();


	for (int t = 0; t < numThreads; t++) {
		//System::Threading::ManualResetEvent mre = new System::Threading::ManualResetEvent(false);
		//System::Threading::ThreadPool::QueueUserWorkItem([&stack, mre]() mutable {

		for (int i = 0; i < 1000000; i++) {
			for (int q = 0; q < 100; q++) {
				stack.push(q);
			}
			int cnt = 0;
			for (int j = 0; j < 100; j++) {
				cnt += j;
				int res;
			//	stack.TryPop(out(res));
				stack.pop(res);
			}
		}
		//mre.Set();
		//});
	//lstthreads.Add(mre);
	}
	//for (int t = 0; t < numThreads; t++) {
	//	lstthreads[t].WaitOne();
	//}

	sw.Stop();
	Console::WriteLine((long)sw.ElapsedMilliseconds);
}
*/


void TestPerformanceConcurrentQueue() {
	System::Diagnostics::Stopwatch sw = new System::Diagnostics::Stopwatch();
	sw.Start();

	int numt = 100;
	List<Thread> lst = new List<Thread>();
	

	System::Collections::Concurrent::ConcurrentQueue<int> q = new System::Collections::Concurrent::ConcurrentQueue<int>();
	Action<> act = [&q]() {
		for (int j = 0; j < 1000; j++) {
			for (int i = 0; i < 100; i++) {
				q.Enqueue(i);
			}
			for (int i = 0; i < 100; i++) {
				int res;
				q.TryDequeue(out(res));
			}
		}
	};

	for (int i = 0; i < numt; i++) {
		System::Threading::Thread t(act);
		lst.Add(t);
		t.Start();
	}
	for (int i = 0; i < numt; i++) {
		lst[i].Join();
	}

	sw.Stop();
	Console::WriteLine((long)sw.ElapsedMilliseconds);

}




void TestPerformanceThreadPool() {
	System::Diagnostics::Stopwatch sw = new System::Diagnostics::Stopwatch();
	sw.Start();
	int cnt = 1000000;

	ManualResetEvent mre = new ManualResetEvent();
	std::atomic<int> x{};
	Action<> act2 = [&x, &mre, cnt]() {
		//Console::WriteLine("starting 2");
		int val = x.fetch_add(1);
		if (val == cnt - 1) {
			mre.Set();
		}
		//x++;
		//if (x == cnt)
		//	mre.Set();
	};
	for (int i = 0; i < cnt; i++)
		System::Threading::ThreadPool::QueueUserWorkItem(act2);


	mre.WaitOne(100000);


	sw.Stop();
	Console::WriteLine(x.load());
	Console::WriteLine(sw.ElapsedMilliseconds);
}

void TestPerformanceDictionary() {
	System::Collections::Generic::Dictionary<int, int> dic = new System::Collections::Generic::Dictionary<int, int>();
	for (int i = 0; i < 10000; i++) {
		dic[i] = i;
	}


	System::Diagnostics::Stopwatch sw = new System::Diagnostics::Stopwatch();
	sw.Start();
	long sum = 0;
	for (int i = 0; i < 100000; i++) {
		for (auto& kvp : dic) {
			sum += kvp.Value;
		}
	}
	sw.Stop();
	Console::WriteLine(sum);
	Console::WriteLine(sw.ElapsedMilliseconds);

}


void TestPerformanceUDPClient() {
	System::Diagnostics::Stopwatch sw = new System::Diagnostics::Stopwatch();
	sw.Start();

	System::Net::Sockets::Socket sock(System::Net::Sockets::AddressFamily::InterNetwork, System::Net::Sockets::SocketType::Dgram, System::Net::Sockets::ProtocolType::Udp);
	byte bts[] = {0, 1, 2};

	
	System::Net::IPAddress ipa;
	System::Net::IPAddress::TryParse("127.0.0.1", out(ipa));

	System::Net::IPEndPoint ipe(ipa, 9060);

	for (int i = 0; i < 1000000; i++) {
		sock.BeginSendTo(&bts[0], 0, 3, System::Net::Sockets::SocketFlags::None, ipe, [](System::IAsyncResult& ar) { 
			Console::WriteLine("yah");
			}, null);
	}


	sw.Stop();
	Console::WriteLine((long)sw.ElapsedMilliseconds);
}


//void TestPerformanceThreadPoolBOOST() {
//	System::Diagnostics::Stopwatch sw = new System::Diagnostics::Stopwatch();
//	sw.Start();
//	int cnt = 1000000;
//
//	std::atomic<int> x{};
//	boost::asio::thread_pool pool(8);
//
//
//
//	for (int i = 0; i < cnt; i++) {
//		boost::asio::post(pool,
//			[&x]()
//			{
//				int val = x.fetch_add(1);
//			});
//	}
//
//	pool.join();
//
//	sw.Stop();
//	Console::WriteLine(x.load());
//	Console::WriteLine((long)sw.ElapsedMilliseconds);
//}


void TestPerformanceStringBuilder() {
	System::Text::StringBuilder sb = new System::Text::StringBuilder();

	System::Diagnostics::Stopwatch sw = new System::Diagnostics::Stopwatch();
	sw.Start();

	for (int i = 0; i < 10000000; i++) {
		for (int j = 0; j < 20; j++) {
			sb.Append(i);
		}
		sb.Clear();
	}


	sw.Stop();
	Console::WriteLine((long)sw.ElapsedMilliseconds);
}

class Base {
public:
	virtual int SomeMethod() { return 0; }
};

class Derived : public Base {
public:
	int SomeMethod() override { return 1; }
};


int main(int argc, char** argv)
{
	// STACK
	Derived d;
	Base b = d;
	std::cout << d.SomeMethod() << b.SomeMethod();
	
	// HEAP
	Derived* pd = new Derived();
	Base* pb = pd;
	std::cout << pd->SomeMethod() << pb->SomeMethod() << ((Base*)&b)->SomeMethod() << std::endl;
	Base* pd2 = &b;
	std::cout << pd2->SomeMethod() << std::endl;



	string strx = u"bloep";
	Console::WriteLine(strx);

	System::Text::StringBuilder sb = new System::Text::StringBuilder();
	sb.Append(u"bla");
	sb.Append((double)370.2);
	sb.Append(u"blo");
	Console::WriteLine(sb.ToString());
	TestPerformanceStringBuilder();
	return 0;

	Console::WriteLine("Starting tests");

	TestTuple();
	return 0;

	TestPerformanceDictionary();
	return 0;

	DateTime dt = DateTime::Now;
	Console::WriteLine(dt.ToString());
	Console::WriteLine(dt.Hour);
	string str = u8"BloĦBlo";
	Array<byte> arr2 = System::Text::Encoding::UTF8.GetBytes(str);
	int x2 = arr2.Length;
	Console::WriteLine(Int32(37).ToString("X4"));
	Console::WriteLine(((Int32)37).ToString("D4"));
	Console::WriteLine(((Int32)37).ToString("F15"));
	string Format = "X2";

		Console::WriteLine(((Int32)37).ToString(Format));

		System::Dynamic::ExpandoObject expo = new System::Dynamic::ExpandoObject();
		expo["bla"] = 37;
		Console::WriteLine(expo["bla"].ToString());
		Dictionary<string, System::Object> dic = new Dictionary<string, System::Object>();
		dic["bla"] = 37;
		//for (System::Collections::Generic::KeyValuePair<string, System::Object> val : d) {
		//	Console::WriteLine("ok");
		//}


	return 0;


	return 0;

	TestPerformanceUDPClient();
	return 0;

	//System::Net::Sockets::Socket s = new System::Net::Sockets::Socket(System::Net::Sockets::AddressFamily::InterNetwork, System::Net::Sockets::SocketType::Stream, System::Net::Sockets::ProtocolType::Tcp);
 //               System::Net::IPAddress adr;
 //               bool success = System::Net::IPAddress::TryParse("127.0.0.1", out(adr));
 //               ASSERT_TRUE(success);
 //               s.Connect(adr, 443);
	//for (int i = 0; i < 2000000; i++)
	TestPerformanceStack();
	TestPerformanceConcurrentStack();
	//TestPerformanceConcurrentStackBOOST();
	TestPerformanceThreadPool();
	return 0;

	


	
	TestPerformanceConcurrentQueue();
	TestPerformanceThreadPool();
	return 0;

	TestPerformanceThread();
	//return 0;

	TestPerformanceManualResetEvent();
	return 0;
	TestPerformanceRef();
	return 0;

	TestPerformanceThreadPool();
	return 0;
	TestPerformanceLinqOrderBy();
	TestPerformanceLinqWhere();

	TestPerformanceLambda();
	TestPerformanceIterator();
	return 0;
	//const char* chrs = u8"töest";
	//std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
	//std::u16string wstr = converter.from_bytes(chrs, chrs + 5);

	TestProcess();
	return 0;




	Console::WriteLine(u8"töo\nst");
	Console::WriteLine(u8"wth");
	Console::Write(u8"ö\r\n");
	Console::Write(u8"a\r\n");

	Console::WriteLine(u8"tö2est");

	System::IO::DirectoryInfo di = new System::IO::DirectoryInfo(System::IO::Path::Combine(u"c:\\tmp", u"ab"));
    System::Collections::Generic::Array<System::IO::DirectoryInfo> arr = di.GetDirectories();
	for (int i = 0; i < arr.Length; i++) {
		Console::WriteLine(arr[i].Name);
	}




	return 0;

	//size_t sz = GC::Collect();
	TestList();
	TestDictionary();
	size_t sz = GC::Collect();
	ASSERT_TRUE(sz == 0);
	//Console::WriteLine("start");

	std::cout << "start" << std::endl;
	//return 0;
//	TestASM();
	//Random rnd = Random();
	//testAction();
	//testFunc();
	//Purgatory::reaper.detach();
	TestString();
	TestArray();
	
	for (int i = 0; i < 10000000; i++) {
		TestList();
		TestDictionary();
		//TestArray2();
	}
	Thread::Sleep(10000);
	Console::WriteLine("EXIT");
	return 0;

	int billion = 1000000000;
	int cnt = 0;

//	bool ok = true;

	//sw.Start();
	//ok = TestInteger() != 0;
	for (int i = 0; i < billion; i++) {
		cnt += i;
	}

	

	//sw.Stop();
	//Console::WriteLine(ok);
	//Console::WriteLine(sw.Get_ElapsedMilliseconds());
	return cnt;


//	int million = 1000000;
//	int tmillion = 10000000;
//	StopWatch sw;// = new StopWatch();
//	sw.Start();
//	for (int i = 0; i < tmillion; i++)
//	{
//		ok &= TestNull();
//	}
//	sw.Stop();
//	Console::WriteLine(ok);
//	Console::WriteLine(sw.Get_ElapsedMilliseconds());
//	GC::Collect();
//
//	sw.Start();
//	for (int i = 0; i < tmillion; i++)
//	{
//	//	ok &= TestBoxing();
//	}
//	sw.Stop();
//	Console::WriteLine(ok);
//	Console::WriteLine(sw.Get_ElapsedMilliseconds());
//
//	GC::Collect();
//	
//	return 0;
//
//	Console::WriteLine("starting testa...");
//	//testa();
//	GC::Collect();
//
//	Console::WriteLine("starting testb...");
//	//testb();
//	GC::Collect();
//
//	Console::WriteLine("starting testc...");
//	testc();
//	GC::Collect();
//
//	Console::WriteLine("starting testd...");
//	testd();
//	GC::Collect();
//
//	Console::WriteLine("starting teste...");
//	teste();
//	GC::Collect();
//
//	test();
//
//	return 0;
//	uint xe = sizeof(std::atomic<uint>); //* ptr = new std::atomic<bool>()
//	
//	//SP<Object> ox = new Object();
////	return 0;
//	/*
//	SP<Thread> t = new Thread(task1);
//	t->Start(ox);
//
//	var<Thread> t2 = new Thread(task1);
//	t2->Start(ox);
//
//	t->Join();
//	t2->Join();
//	*/
////	Random o2;
//
//
//
////	if (o2 == null)
//	//	Console::WriteLine("it's null");
//
//
////	Random rndx;
//
////	Object oo;
////	Object oob = Object();
//	//Object oo2 = new Object();
//
//
//	Random o3 = Random();
//
////	o3.Length = 37;
////	int xxx = o3.Length;
////	Console::WriteLine(xxx);
//	Random o7 = Random(2);
//
//
//
//
//	//o3->somedata = 37;
//
//
//	//++o3;
////	++o3;
//
//	//SP<Random> o3b = o3;
//
//	//o3.Dispose();
//	return 0;
//
//
//
//
//
//
////	Console::WriteLine((long)o3);
//	//delete o3;
//	
//	Random* o4 = new Random();
//	Console::WriteLine((long)o4);
//
//	Random* o4b = new Random();
//	Console::WriteLine((long)o4b);
//
//	delete o4b;
//
////	Random rnd = new Random();
//
//	//SP<Thread> t = new Thread(task1);
//	/*t->Start(ox);
//	t->Join();*/
//
//
//	//return 0;
//	Object* o5b = new Object();
//	Console::WriteLine((long)o5b);
//
//	
//
//	//Random o5 = new Random[37];
//	
//	
//	//delete[] o5;
//	
///*	String* s = new String();
//
//	int szstr = sizeof(*s);
//
//	//Console::WriteLine((int64_t) System::SP<Object>::pool.Last);
//	//Console::WriteLine(System::SP<Object>::pool.Size);
//
//	//return 0;
//
//	Random* rnd = new Random();
//
//	for (int i = 0; i < 10; i++) {
//		Console::WriteLine(rnd->Next(10));
//	}
//
//	//rnd->ne
//	*/
//	Random* x;
//	for (int i = 0; i < 10000000; ++i)
//	{
//		x = new Random();
//		delete x;
//
//		//Object* o1 = new Object();
//		//delete o1;
//		//System::Object* o = new System::Object();
//		//o->somedata = 37;
//
//
//		//std::string* str = new std::string("blabla");
//		//delete str;
//		//String* s = new String();
//		//String* s1 = new String("bla");
//		//String* s2 = new String("bli");
//		//*s1 = "pompidom";
//
//
//		//String s3 = u"refie";
//		//String s4 = u"abc";
//	//	s4 += s3;
//		
//		//s4 += "teteke";
//		//s3 = s4;
//
//		////Console::WriteLine(s1);
//		////Console::WriteLine(s2);
//		////Console::WriteLine(s3);
//		////Console::WriteLine(s4);
//
//		//delete s;
//		//delete s1;
//		//delete s2;
//
//	}
//
//	Console::WriteLine("DONE");
//	std::cin.get();
//
//	//char s1[256];
//	//std::cin >> s1;
//
//	return 0;
}
