#include "pch.h"
#include "../System.h"
#include "../Lorentzoft.h"

//
//Let me answer this specifically for ubuntu users.First start by installing the gtest development package.
//
//sudo apt-get install libgtest-dev
//Note that this package only install source files.You have to compile the code yourself to create the necessary library files.These source files should be located at / usr / src / gtest.Browse to this folder and use cmake to compile the library :
//
//sudo apt-get install cmake # install cmake
//cd /usr/src/gtest
//sudo mkdir build
//cd build
//sudo cmake ..
//sudo make
//sudo cp *.a /usr/lib
//sudo mkdir /usr/local/lib/gtest
//sudo ln -s /usr/lib/libgtest.a /usr/local/lib/gtest/libgtest.a
//sudo ln -s /usr/lib/libgtest_main.a /usr/local/lib/gtest/libgtest_main.a
//Now to compile your programs that uses gtest, you have to link it (AFTER YOUR OBJECT .o DECLARATION) with :
//
//-lgtest -lgtest_main -pthread

// a build script:
//mono --debug GCCBuilder.exe -p /mnt/CPP/SystemTest/SystemTest.vcxproj -c g++ -lo -lgtest -lo -lgtest_main -lo -pthread -co -g
//sudo chmod 777 /mnt/CPP/SystemTest/bin/Release/g++/arm64/SystemTest.exe
//echo y | gdb -ex run -ex q /mnt/CPP/SystemTest/bin/Release/g++/arm64/SystemTest.exe

namespace System {


    TEST(System_String, String) {
        std::stringstream buffer;
        std::cout.rdbuf(buffer.rdbuf());
        // auto str = buffer.str();

        ulong activeobjects = System::GC::Collect();
        {
            string nstr;
            ASSERT_TRUE(nstr == null);
            string str = u"blabla";
            ASSERT_EQ(6, (int)str.Length);
            string str2 = str + "bloblo";
            ASSERT_EQ(12, (int)str2.Length);
            str2 = "bloblo" + str;
            ASSERT_TRUE(str2 == "blobloblabla");
            str2 = u"BloBĦlo" + str;
            ASSERT_EQ(13, (int)str2.Length);
            ASSERT_TRUE(str2 == u"BloBĦloblabla");

            bool eq = str == "blabla";
            ASSERT_TRUE(eq); 
            bool eq2 = str == null;
            ASSERT_TRUE(!eq2);
            str = null;
            bool eq3 = str != null;
            ASSERT_TRUE(!eq3);

            int pos = str2.IndexOf("oB");

            ASSERT_EQ(2, pos);
            pos = str2.IndexOf(u"BĦl");
            ASSERT_EQ(3, pos);
            pos = str2.IndexOf(u"BĦlll");
            ASSERT_EQ(-1, pos);

            string str3 = str2.Substring(2, 4);
            ASSERT_TRUE(str3 == u"oBĦl");

            string str4 = str2.ToLower();
            ASSERT_TRUE(str4 == u"blobĦloblabla");

            string str5 = str2.Replace("b", "qu");
            ASSERT_EQ(15, (int)str5.Length);
            ASSERT_TRUE(str5 == u"BloBĦloqulaqula");
        }
        ulong remaining = System::GC::Collect() - activeobjects;
        ASSERT_EQ(0, remaining);

    }
    TEST(System_Object, GetHashCode) {
        Object onull;
        bool gotexception = false;
        try {
            onull.GetHashCode();
        }
        catch (NullReferenceException nrex) {
            gotexception = true;
        }
        ASSERT_TRUE(gotexception);

        onull = new Object();
        int hash = onull.GetHashCode();
        int hash2 = onull.GetHashCode();
        onull = new Object();
        int hash3 = onull.GetHashCode();
        ASSERT_TRUE(hash == hash2);
        ASSERT_TRUE(hash != hash3);
    }

    TEST(System_Object, Boxing) {
        ulong activeobjects = System::GC::Collect();
        {
            Object oint = 2ll;
            Object oint2 = oint;
            ASSERT_TRUE(oint.is<long>());
            ASSERT_TRUE(oint2.is<long>());
            ASSERT_EQ(2ll, (long)oint);
            ASSERT_EQ(2ll, (long)oint2);
            oint2 = 3ll;
            ASSERT_EQ(2ll, (long)oint);
            ASSERT_EQ(3ll, (long)oint2);

            Object onull;
            ASSERT_TRUE(onull == null);
            //   ASSERT_NE(0ll, onull);

            ASSERT_FALSE(onull.is<long>());

            onull = 2ll;
            ASSERT_TRUE(onull != null);
            ASSERT_EQ(2ll, (long)onull);

            ASSERT_TRUE(onull.is<long>());

            //Object oint = 37;
            //ASSERT_TRUE(oint == 37);
            //int ointval = (int)oint;
            //ASSERT_TRUE(ointval == 37);

            //Object odouble = 37.2;
            //ASSERT_TRUE(odouble == 37.2);
            //double odoubleval = (double)odouble;
            //ASSERT_TRUE(odoubleval == 37.2);
        }
        ulong remaining = System::GC::Collect() - activeobjects;
        ASSERT_EQ(0, remaining);

    }

    TEST(System_TimeSpan, TimeSpan) {
        System::TimeSpan ts = new TimeSpan(1);
        long ticks = ts.Ticks;
        System::TimeSpan ts2 = ts;
        ts2.Set_Ticks(2);
        ASSERT_EQ(1, ts.Ticks);


    }


    TEST(System_Action, Action) {
    	int x = 1;
	    Action<> act = new Action<>([&x]() { 
		    //Console::WriteLine(x + 7);
		    x += 3;
	    });
	    act();
        ASSERT_EQ(4 , x);

	  //  Action<int> act2 = new Action<int>([x](int y) { Console::WriteLine(x + y); });
	  //  act2(37);
    }

    TEST(System_Tuple, Tuple) {
        Tuple<int, string> tpl(1, "bla");
        ASSERT_EQ(1, tpl.Item1);


    }

    namespace Threading {
        TEST(System_Threading_Thread, Thread) {

            ulong activeobjects = System::GC::Collect();
            {

                int start = 37;

                Action<> act2 = []() {
                    //Console::WriteLine("starting 2");
                };

                act2 = []() {
                    //Console::WriteLine("starting 2b");
                };

                Action<>* act3 = new Action<>([]() {
                    //Console::WriteLine("starting 3");
                    });
                delete act3;

                Action<> act4 = act2;

                Object o;

                if (!act3)
                    throw Exception();

                Thread t = Thread([start, o]() {

                    });
                t.Start();

                long cnt = 0;
                for (int i = 0; i < 10; i++) {
                    System::Threading::Thread t = new System::Threading::Thread([&cnt]() {
                        cnt++;
                        });
                    t.Start();
                    t.Join();
                }
                ASSERT_EQ(cnt, 10);
            }
            ulong remaining = System::GC::Collect() - activeobjects;
            ASSERT_EQ(0, remaining);

        }

        TEST(System_Threading_ManualResetEvent, ManualResetEvent) {
            ulong activeobjects = System::GC::Collect();
            {
                System::Threading::ManualResetEvent mre = new System::Threading::ManualResetEvent(false);
                long cnt = 0;
                for (int i = 0; i < 10; i++) {
                    System::Threading::Thread t([&mre]() {
                        mre.Set();
                        });
                    t.Start();
                  //  t.Join();
                    mre.WaitOne(600000);
                    t.Join();
                    mre.Reset();
                }
                Thread::Sleep(100);
            }
            ulong remaining = System::GC::Collect() - activeobjects;
            ASSERT_EQ(0, remaining);

        }


        TEST(System_Threading_ThreadPool, ThreadPool) {
            //ulong activeobjects = System::GC::Collect();



            ManualResetEvent mre = new ManualResetEvent();
            int x = 0;
            Action<> act2 = [&x]() {
                //Console::WriteLine("starting 2");
                x++;
            };

          //  ManualResetEvent mre = new ManualResetEvent();
          //  mre.Set();
              System::Threading::ThreadPool::QueueUserWorkItem(act2);

            Thread::Sleep(1000);
            ASSERT_EQ(x, 1);
           //   ThreadPool::


            //ulong remaining = System::GC::Collect() - activeobjects;
            //ASSERT_EQ(0, remaining);
        }

    }





    namespace Diagnostics {
        TEST(System_Diagnostics_Process, Process) {
            //ProcessStartInfo psi = new ProcessStartInfo();
            //psi.UseShellExecute = false;
            //psi.CreateNoWindow = true;
            //psi.FileName = "c:\\tmp\\adwc.exe";

            System::Collections::Generic::Array<Process> arr = Process::GetProcesses();




            ulong activeobjects = System::GC::Collect();
            {
                ProcessStartInfo psi = new ProcessStartInfo();
                //System::String str = "blabla";
                psi.FileName = "C:\\projects\\CPP\\SharpTest\\bin\\Release\\SharpTest.exe";


                Process proc = new Process();
               // System::String fname = "C:\\projects\\CPP\\SharpTest\\bin\\Debug\\SharpTest.exe";
                // proc.StartInfo->CreateNoWindow = true;
                proc.StartInfo->UseShellExecute = true;
                proc.StartInfo->FileName = "C:\\projects\\CPP\\SharpTest\\bin\\Release\\SharpTest.exe";
            //    proc.StartInfo->Verb = "open";
                if (proc.Start())
                    proc.WaitForExit(-1);

                //Process proc = new Process();
                //proc.StartInfo.FileName = "C:\\projects\\CPP\\SharpTest\\bin\\Debug\\SharpTest.exe";
                //proc.StartInfo.CreateNoWindow = true;

                //proc.Start();
            }
            System::Threading::Thread::Sleep(500);
            ulong remaining = System::GC::Collect() - activeobjects;
            ASSERT_EQ(0, remaining);

            //           ASSERT_TRUE(Directory::Exists(str));
        }
    
    }

    namespace Collections {
        namespace Generic {

            TEST(System_Collections_Generic_List, List) {
                ulong activeobjects = System::GC::Collect();
                {
                    List<int> lst({ 5, 3, 6, 0, 2, 1, 8, 7, 9, 4 });
                    //for (int i = 0; i < 10000; i++)
                    //    lst->Add(i);

                    ASSERT_EQ(5, lst[0]);


                    int cnt = 0;
                    for (int val : lst) {
                        ASSERT_EQ(lst[cnt++], val);
                    }

                    IEnumerable<int> enu = lst;
                    IEnumerator<int> it = enu.GetEnumerator();

                    cnt = 0;
                    while (it.MoveNext()) {
                        ASSERT_EQ(lst[cnt++], it.Current);
                    }



                    ASSERT_EQ(cnt, lst->Count);

                }
                ulong remaining = System::GC::Collect() - activeobjects;
                ASSERT_EQ(0, remaining);
            }

            TEST(System_Collections_Generic_Comparer, Compare) {
                ulong activeobjects = System::GC::Collect();
                {
                    //new Comparer<int>();
                 //   ASSERT_EQ(37, Comparer<int>::number);

                    IComparer<int> comp = Comparer<int>(new Comparer<int>()); // Comparer<int>::Default;
                    //ASSERT_TRUE(comp.is<Comparer<int>>());

                    int result = comp.Compare(1, 2);
                    ASSERT_EQ(-1, result);
                    result = comp.Compare(2, 1);
                    ASSERT_EQ(1, result);
                    result = comp.Compare(1, 1);
                    ASSERT_EQ(0, result);
                }
                ulong remaining = System::GC::Collect() - activeobjects;
                ASSERT_EQ(0, remaining);
            }

            TEST(System_Collections_Generic_IEnumerable, Where) {
                ulong activeobjects = System::GC::Collect();
                {
                    List<string> lst({ u"bli", u"bla", u"blo" });
                    List<string> filtered =
                        lst.Where([](string str) { return str == u"bla"; }).ToList();
                    ASSERT_EQ(1, (int)filtered.Count);
                    ASSERT_TRUE(filtered[0] == u"bla");
                }
                ulong remaining = System::GC::Collect() - activeobjects;
                ASSERT_EQ(0, remaining);
            }

            TEST(System_Collections_Generic_IEnumerable, Select) {
                ulong activeobjects = System::GC::Collect();
                {
                    List<string> lst({ u"bli", u"bla", u"blo" });

                    List<char16_t> lstfirstchars = lst.Select<char16_t>([](string str) {
                        return str[0];
                        }).ToList();

                        ASSERT_EQ(3, lstfirstchars.Count);
                        for (int i = 0; i < lstfirstchars.Count; i++) {
                            ASSERT_TRUE(lstfirstchars[i] == u'b');
                        }
                }
                ulong remaining = System::GC::Collect() - activeobjects;
                ASSERT_EQ(0, remaining);
            }

            TEST(System_Collections_Generic_IEnumerable, OrderBy) {

                ulong activeobjects = System::GC::Collect();
                {

                    List<int> lsti({ 5, 3, 6, 0, 2, 1, 8, 7, 9, 4 });
                    //List<int> lsti({4,3,2,1,0});

                    lsti = lsti.OrderBy<int>([](int i) { return i; }).ToList();
                    //ASSERT_EQ(10, lsti.Count);
                    for (int i = 0; i < lsti.Count; i++) {
                        int el = lsti[i];
                        ASSERT_EQ(i, lsti[i]);
                    }
                    ASSERT_TRUE(true);
                    //List<string> lst({ u"bli", u"bla", u"blo" });
                    //List<string> ordered = lst.OrderBy<string>([](string str) { return str; }).ToList();

                    //ASSERT_TRUE(ordered.Count == 3);
                    //ASSERT_TRUE(ordered[0] == u"bla");
                    //ASSERT_TRUE(ordered[1] == u"bli");
                    //ASSERT_TRUE(ordered[2] == u"blo");
                }
                ulong remaining = System::GC::Collect() - activeobjects;
                ASSERT_EQ(0, remaining);
            }

            TEST(System_Collections_Generic_Array, Array) {
                ulong activeobjects = System::GC::Collect();
                {
                    Array<int> narr = null;
                    ASSERT_TRUE(narr == null);

                    Array<int> arr = { 1, 2, 3 };
                    ASSERT_TRUE(arr.Length == 3);
                    ASSERT_TRUE(arr[0] == 1);
                    ASSERT_TRUE(arr[1] == 2);
                    ASSERT_TRUE(arr[2] == 3);
                    arr[1] = 4;
                    ASSERT_TRUE(arr.Length == 3);
                    ASSERT_TRUE(arr[0] == 1);
                    ASSERT_TRUE(arr[1] == 4);
                    ASSERT_TRUE(arr[2] == 3);
                    bool exok = false;
                    try {
                        arr[3] = 0;
                    }
                    catch (IndexOutOfRangeException&) {
                        exok = true;
                    }
                    ASSERT_TRUE(exok);
                }
                ulong remaining = System::GC::Collect() - activeobjects;
                ASSERT_EQ(0, remaining);

            }


            TEST(System_Collections_Generic_Dictionary, Dictionary) {
                //   GTEST_FLAG(Fla  catch_exceptions);

                ulong activeobjects = System::GC::Collect();
                {
                    Dictionary<int, int> ndic = null;
                    ASSERT_TRUE(ndic == null);

                    Dictionary<int, int> dic = new Dictionary<int, int>();
                    dic.Add(0, 1);
                    dic.Add(1, 0);
                    dic.Add(37, 37);

                    int cnt = 0;
                    int sum = 0;
                    for (KeyValuePair<int, int>& kvp : dic) {
                        cnt++;
                        sum += kvp.Value;
                    }
                    ASSERT_EQ(3, cnt);
                    ASSERT_EQ(38, sum);
                    //  throw NullReferenceException();
                    dic.Remove(37);

                    int x;
                    ASSERT_TRUE(dic.TryGetValue(0, x));
                    ASSERT_TRUE(x == 1);
                    ASSERT_TRUE(dic.Contains(1));
                    ASSERT_TRUE(!dic.Contains(2));
                    ASSERT_TRUE(dic.Remove(1));
                    ASSERT_TRUE(!dic.Remove(1));
                    ASSERT_TRUE(!dic.Contains(1));
                    dic[1] = 37;
                    ASSERT_TRUE(dic[1] == 37);
                    dic[2] = 38;
                    ASSERT_TRUE(dic[2] == 38);

                    Dictionary<System::String, Object> odic = new Dictionary<System::String, Object>();
                    //odic.Add("a", 0);
                    //odic.Add("b", 0);
                }
                ulong remaining = System::GC::Collect() - activeobjects;
                ASSERT_EQ(0, remaining);
            }

            //class Node128 {
            //public:
            //    int l1;
            //};


            TEST(System_Collections_Generic_Stack, Stack) {
                std::stringstream buffer;
                std::cout.rdbuf(buffer.rdbuf());
                // auto str = buffer.str();

                ulong activeobjects = System::GC::Collect();
                {
                    //std::atomic<int> ix;
                    //ASSERT_TRUE(ix.is_lock_free());
                    //std::atomic<Node128> nx;
                    //bool dcas = nx.is_lock_free();
                    //ASSERT_TRUE(dcas);

                   // ASSERT_TRUE(std::atomic<Node128>::is_lock_free());

                    Stack<int> s = new Stack<int>();
                    for (int i = 0; i < 100; i++) {
                        s.Push(i);
                    }
                    for (int i = 0; i < 100; i++) {
                        int qval = s.Pop();
                        ASSERT_TRUE(qval == 100 - i - 1);
                    }
                    bool gotioex = false;
                    try {
                        s.Pop();
                    }
                    catch (InvalidOperationException&) {
                        gotioex = true;
                    }
                    ASSERT_TRUE(gotioex);
                }
                ulong remaining = System::GC::Collect() - activeobjects;
                ASSERT_EQ(0, remaining);
            }


            TEST(System_Collections_Generic_Queue, Queue) {
                std::stringstream buffer;
                std::cout.rdbuf(buffer.rdbuf());
                // auto str = buffer.str();

                ulong activeobjects = System::GC::Collect();
                {
                    Queue<int> q = new Queue<int>();
                    for (int i = 0; i < 100; i++) {
                        q.Enqueue(i);
                    }
                    for (int i = 0; i < 100; i++) {
                        int qval = q.Dequeue();
                        ASSERT_TRUE(qval == i);
                    }
                    bool gotioex = false;
                    try {
                        q.Dequeue();
                    }
                    catch (InvalidOperationException&) {
                        gotioex = true;
                    }
                    ASSERT_TRUE(gotioex);
                }
                ulong remaining = System::GC::Collect() - activeobjects;
                ASSERT_EQ(0, remaining);
            }


        }

        namespace Concurrent {
            TEST(System_Collections_Concurrent_ConcurrentStack, ConcurrentStack) {

                for (int i = 0; i < 3000; i++)
                    System::Threading::ThreadPool::QueueUserWorkItem([]() {});
                System::Threading::Thread::Sleep(2000);

                ulong activeobjects = System::GC::Collect();
                {
                    int numThreads = 4;
                    ConcurrentStack<int> stack = new ConcurrentStack<int>();
                    System::Collections::Generic::List<System::Threading::ManualResetEvent> lstthreads = new System::Collections::Generic::List<System::Threading::ManualResetEvent>();
                    
                    
                    for (int t = 0; t < numThreads; t++) {
                        System::Threading::ManualResetEvent mre = new System::Threading::ManualResetEvent(false);
                        System::Threading::ThreadPool::QueueUserWorkItem([&stack, mre, numThreads]() mutable {
                            //for (int i = 0; i < 10; i++) {
                            //    for (int q = 0; q < 100; q++) {
                            //        stack.Push(q);
                            //    }
                            //    int cnt = 0;
                            //    int res = 0;
                            //    for (int j = 0; j < 100; j++) {
                            //        cnt += j;
                            //        int res;
                            //        bool okay = stack.TryPop(out(res));
                            //        ASSERT_TRUE(okay);
                            //        if (numThreads == 1)
                            //            ASSERT_EQ(res, 99 - j);
                            //    }
                            //    if (numThreads == 1) {
                            //        bool nokay = stack.TryPop(out(res));
                            //        ASSERT_FALSE(nokay);
                            //    }
                            //}
                            mre.Set();
                        });
                        lstthreads.Add(mre);
                    }
                    for (int t = numThreads; t-->0;) {
                        lstthreads[t].WaitOne();
                        Console::WriteLine("okay");
                    }
                    Console::WriteLine("done");
                }
                ulong remaining = System::GC::Collect() - activeobjects;
                ASSERT_EQ(0, remaining);
            }


            TEST(System_Collections_Concurrent_ConcurrentQueue, ConcurrentQueue) {
                ulong activeobjects = System::GC::Collect();
                {
                    ConcurrentQueue<int> q = new ConcurrentQueue<int>();
                    for (int i = 0; i < 97; i++) {
                        q.Enqueue(i);
                    }

                    //System::Threading::Thread t = new System::Threading::Thread([&q]() {
                    //    for (int i = 0; i < 10000000; i++) {
                    //        q.Enqueue(i);
                    //    }
                    //    });
                    //t.Start();
                    //t.Join();
                    Console::WriteLine("done");
                }
                ulong remaining = System::GC::Collect() - activeobjects;
                ASSERT_EQ(0, remaining);
            }


            TEST(System_Collections_Concurrent_ConcurrentDictionary, ConcurrentDictionary) {
                // System::Collections::Generic::Dictionary<string, Object> odic = new System::Collections::Generic::Dictionary<string, Object>();

                //testing::internal::CaptureStdout();

                // This can be an ofstream as well or any other ostream
                std::stringstream buffer;

                // Save cout's buffer here
                //std::streambuf* sbuf = std::cout.rdbuf();

                // Redirect cout to our stringstream buffer or any other ostream
                std::cout.rdbuf(buffer.rdbuf());


                std::cout << "COUT" << std::endl;

                /*auto str = testing::internal::GetCapturedStdout();
                testing::internal::CaptureStdout();
                std::cout << str;
                */
                std::cout << "COUT2" << std::endl;
             //   str = testing::internal::GetCapturedStdout();

                auto str = buffer.str();
                ulong activeobjects = System::GC::Collect();
                int numinsert = 10000;
                {
                    ConcurrentDictionary<int, int> dic = new ConcurrentDictionary<int, int>();
                    for (int i = 0; i < numinsert; i++) {
                        //if (i == 112)
                        //    std::cout << "bla" << std::endl;
                        dic.Add(i, i + 1);

                    }


                    for (int i = 0; i < numinsert; i++) {
                        int val;
                        bool there = dic.TryGetValue(i, val);

                        //if (i & 1) {
                        //    ASSERT_TRUE(!there);
                        //}
                        //else {
                            ASSERT_TRUE(there);
                            ASSERT_TRUE(val == i + 1);
                        //}
                    }
                }
                ulong remaining = System::GC::Collect() - activeobjects;
                ASSERT_EQ(0, remaining);
            }
        }
    }

    namespace IO {

        TEST(System_IO_Path, Path) {
            System::String str = Path::GetTempPath();
 //           ASSERT_TRUE(Directory::Exists(str));
        }

        TEST(System_IO_File, File) {
            std::stringstream buffer;
            std::cout.rdbuf(buffer.rdbuf());
            // auto str = buffer.str();

            ulong activeobjects = System::GC::Collect();
            {
                System::String pth = "c:\\tmp\\test.txt";
                if (File::Exists(pth)) {
                    String strx = File::ReadAllText(pth);
                }
            }
            ulong remaining = System::GC::Collect() - activeobjects;
            ASSERT_EQ(0, remaining);




            //auto bts = File::ReadAllBytes("c:\\tmp\\test.txt");
            //int len = bts.Length;
            //for (int i = 0; i < len; i++)
            //    std::cout << (char) bts[i];
            //
            //auto str = buffer.str();


        }

        TEST(System_IO_DirectoryInfo, DirectoryInfo) {
            DirectoryInfo di = new DirectoryInfo("/mnt");
            ASSERT_TRUE((bool)di.Exists);
            ASSERT_TRUE((bool)di.Exists);
            System::Collections::Generic::Array<DirectoryInfo> arr = di.GetDirectories();
            ASSERT_EQ(116, (int)arr.Length);
        }


        namespace Ports {

#ifdef _MSC_VER
            TEST(System_IO_SerialStream, SerialStream) {
                std::stringstream buffer;
                std::cout.rdbuf(buffer.rdbuf());
                // auto str = buffer.str();

                ulong activeobjects = System::GC::Collect();
                {
                    //String comport = "COM3";

                    System::Collections::Generic::Array<byte> arr = { 1, 2, 3 };

                    SerialStream ssw = new SerialStream("COM3", 19200, Parity::None, 8, StopBits::One, 1000, 1000, Handshake::None, false, false, false, (byte) '?');
                    SerialStream ssr = new SerialStream("COM5", 19200, Parity::None, 8, StopBits::One, 1000, 1000, Handshake::None, false, false, false, (byte)'?');
                    System::Collections::Generic::Array<byte> arrr(100);


                    ssw.Write(arr, 0, 3);
                    int cnt = ssr.Read(arrr, 0, arrr.Length);

                    ASSERT_TRUE(cnt == 3);
                    ASSERT_TRUE(arrr[0] == 1);
                    ASSERT_TRUE(arrr[1] == 2);
                    ASSERT_TRUE(arrr[2] == 3);

                    bool gottex = false;
                    try {
                        cnt = ssr.Read(arrr, 0, arrr.Length);
                    }
                    catch (TimeoutException&) {
                        gottex = true;
                    }
                    ASSERT_TRUE(gottex);

                }
                ulong remaining = System::GC::Collect() - activeobjects;
                ASSERT_EQ(0, remaining);
            }

            TEST(System_IO_SerialPort, GetPortNames) {
                std::stringstream buffer;
                std::cout.rdbuf(buffer.rdbuf());
                // auto str = buffer.str();

                ulong activeobjects = System::GC::Collect();
                {
                    System::Collections::Generic::Array<String> portNames = SerialPort::GetPortNames();

                    ASSERT_TRUE(portNames[0] == "COM1");
                    ASSERT_TRUE(portNames[1] == "COM2");



                }
                ulong remaining = System::GC::Collect() - activeobjects;
                ASSERT_EQ(0, remaining);
            
            }

#endif

        }
    
    }

    namespace Net {
        TEST(System_Net_IPAddress, IPAddress) {
            ulong activeobjects = System::GC::Collect();
            {
                IPAddress adr;
                bool success = IPAddress::TryParse("127.0.0.1", out(adr));
                ASSERT_TRUE(success);
            }
            ulong remaining = System::GC::Collect() - activeobjects;
            ASSERT_EQ(0, remaining);
        }

        namespace Sockets {
            TEST(System_Net_Sockets_Socket, Socket) {
                Socket s = new Socket(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::Tcp);
                IPAddress adr;
                bool success = IPAddress::TryParse("127.0.0.1", out(adr));
                ASSERT_TRUE(success);
                s.Connect(adr, 443);
            	
                System::Collections::Generic::Array<byte> sbuf(2);
                sbuf[0] = 37;
                sbuf[1] = 38;
                s.Send(sbuf);
            
            }
        }
    }
}

namespace Lorentzoft {

    TEST(Lorentzoft_JsonConvert, JsonConvert) {
        System::String str = "{}";
        System::Object o = JsonConvert::DeserializeObject(str);
    }


}

