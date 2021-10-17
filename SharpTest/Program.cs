using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SharpTest
{
    static class Program {
        public static int TestInteger() {
            int ret = 0;
            int max = 1000000000;
            for (int i = 0; i < max; i++) {
                ret += i;
                //    Console.WriteLine("i = " + i + ", ret = " + ret);
            }
            return ret;
        }

        public static bool TestNull() {
            bool ret = true;
            Object o = null;
            ret &= o == null;
            //   Console.WriteLine(o == null); // true
            Object o2 = new Object();
            //   Console.WriteLine(o == o2); // false
            ret &= o == o2;
            o = o2;
            ret &= o == o2;
            //   Console.WriteLine(o == o2); // true
            o = o2 = null;
            ret &= o == o2;
            //   Console.WriteLine(o == o2); // false
            return ret;
        }

        public static void TestPerformance() {
            System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
            sw.Start();
            long cnt = 0;
            for (int i = 0; i < 100000000; i++) {
                cnt += i;
            }
            sw.Stop();
            Console.WriteLine(cnt);
            Console.WriteLine((long)sw.ElapsedMilliseconds);
        }

        public static void TestPerformanceList() {
            System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
            sw.Start();
            long cnt = 0;
            for (int i = 0; i < 1000000; i++) {
                List<int> lst = new List<int>();
                for (int j = 0; j < 1000; j++) {
                    lst.Add(i + j);
                    cnt += i + j;
                }
            }
            sw.Stop();
            Console.WriteLine(cnt);
            Console.WriteLine((long)sw.ElapsedMilliseconds);
        }

        public static void TestPerformanceIterator() {
            List<int> lst = new List<int>();
            for (int j = 0; j < 1000; j++) {
                lst.Add(j);
            }
            System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
            sw.Start();
            long cnt = 0;
            for (int i = 0; i < 1000000; i++) {
                foreach (int j in lst) {
                    cnt += j;
                }
                //for (int j = 0; j < 1000; j++) {
                //    cnt += lst[j];
                //}
            }
            sw.Stop();
            Console.WriteLine(cnt);
            Console.WriteLine("TestPerformanceIterator " + (long)sw.ElapsedMilliseconds);
        }

        public static void TestPerformanceLambda() {
            //List<int> lst = new List<int>();
            //for (int j = 0; j < 1000; j++) {
            //    lst.Add(j);
            //}
            System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
            sw.Start();
            long cnt = 0;

            Action<long> lambda = (y) => { cnt += y; };

            for (long i = 0; i < 1000000000; i++) {
                lambda(i);
                //for (int j = 0; j < 1000; j++) {
                //    cnt += lst[j];
                //}
            }
            sw.Stop();
            Console.WriteLine(cnt);
            Console.WriteLine("TestPerformanceLambda " + (long)sw.ElapsedMilliseconds);
        }

        public static void TestPerformanceLinqWhere() {
            List<int> lst = new List<int>();
            for (int j = 0; j < 1000; j++) {
                lst.Add(j);
            }
            System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
            sw.Start();
            long cnt = 0;

            for (long i = 0; i < 100000; i++) {
                List<int> filtered = lst.Where((x) => { return x > 500; }).ToList();
                cnt += filtered.Count;
            }
            sw.Stop();
            Console.WriteLine(cnt);
            Console.WriteLine("TestPerformanceLinqWhere " + (long)sw.ElapsedMilliseconds);
        }

        public static void TestPerformanceLinqOrderBy() {
            List<int> lst = new List<int>();
            for (int j = 0; j < 1000; j++) {
                lst.Add(j);
            }
            System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
            sw.Start();
            long cnt = 0;

            for (long i = 0; i < 1000; i++) {
                List<int> filtered = lst.OrderBy((x) => { return x; }).ToList();
                cnt += filtered.Count;
            }
            sw.Stop();
            Console.WriteLine(cnt);
            Console.WriteLine("TestPerformanceLinqOrderBy " + (long)sw.ElapsedMilliseconds);
        }

        public static void MethodWithRef(ref int result) {
            result = result + 37;
        }

        public static void TestPerformanceRef() {
            System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
            sw.Start();

            long cnt = 0;
            for (int i = 0; i < 1000000000; i++) {
                int x = i;
                MethodWithRef(ref x);
                cnt += x;
            }

            sw.Stop();
            Console.WriteLine(cnt);
            Console.WriteLine((long)sw.ElapsedMilliseconds);
        }

        public static void TestPerformanceThread() {
            System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
            sw.Start();

           // System.Threading.ManualResetEvent mre = new System.Threading.ManualResetEvent(false);
            long cnt = 0;
            for (int i = 0; i < 100000; i++) {
                System.Threading.Thread t = new System.Threading.Thread(() => {
                    cnt++;
              //      mre.Set();
                });
                t.Start();
             //   mre.WaitOne(600000);
                t.Join();
           //     mre.Reset();
            }

            sw.Stop();
            Console.WriteLine(cnt);
            Console.WriteLine((long)sw.ElapsedMilliseconds);

        }

        public static void TestPerformanceManualResetEvent() {
            System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
            sw.Start();

            System.Threading.ManualResetEvent mre = new System.Threading.ManualResetEvent(false);
            long cnt = 0;
            for (int i = 0; i < 100000; i++) {
                System.Threading.Thread t = new System.Threading.Thread(() => {
                    cnt++;
                    mre.Set();
                });
                t.Start();
                mre.WaitOne(600000);
                t.Join();
                mre.Reset();
            }

            sw.Stop();
            Console.WriteLine(cnt);
            Console.WriteLine((long)sw.ElapsedMilliseconds);

        }

        public static void TestPerformanceStack() {
            System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
            sw.Start();
            //int numThreads = 1;
            System.Collections.Generic.Stack<int> stack = new System.Collections.Generic.Stack<int>();
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
                    int res;
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
            Console.WriteLine((long)sw.ElapsedMilliseconds);
        }

        public static void TestPerformanceConcurrentStack() {
           // System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
           // sw.Start();
        //    int numThreads = 1;
            ConcurrentStack<int> stack = new ConcurrentStack<int>();
        //    System.Collections.Generic.List<System.Threading.ManualResetEvent> lstthreads = new System.Collections.Generic.List<System.Threading.ManualResetEvent>();


        //    for (int t = 0; t < numThreads; t++) {
        //        System.Threading.ManualResetEvent mre = new System.Threading.ManualResetEvent(false);
        //        System.Threading.ThreadPool.QueueUserWorkItem((a) => {
                    for (int i = 0; i < 1000000; i++) {
                        for (int q = 0; q < 100; q++) {
                            stack.Push(q);
                        }
                        int cnt = 0;
                        for (int j = 0; j < 100; j++) {
                            cnt += j;
                            int res;
                            stack.TryPop(out (res));
                        }
                    }
          //          mre.Set();
          //      });
          //      lstthreads.Add(mre);
          //  }
          //  for (int t = 0; t < numThreads; t++) {
          //      lstthreads[t].WaitOne();
          //  }

           // sw.Stop();
           // Console.WriteLine((long)sw.ElapsedMilliseconds);
        }



public static void TestPerformanceConcurrentQueue() {
            System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
            sw.Start();

            int numt = 100;
            List<Thread> lst = new List<Thread>();

            System.Collections.Concurrent.ConcurrentQueue<int> q = new System.Collections.Concurrent.ConcurrentQueue<int>();
            Action act = () => {
                for (int j = 0; j < 1000; j++) {
                    for (int i = 0; i < 100; i++) {
                        q.Enqueue(i);
                    }
                    for (int i = 0; i < 100; i++) {
                        int res;
                        q.TryDequeue(out res);
                    }
                }
            };

            for (int i = 0; i < numt; i++) {
                System.Threading.Thread t = new Thread(() => { act(); });
                lst.Add(t);
                t.Start();
            }
            for (int i = 0; i < numt; i++) {
                lst[i].Join();
            }


            sw.Stop();
            Console.WriteLine((long)sw.ElapsedMilliseconds);

        }


        public static void TestPerformanceThreadPool() {

            System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
            sw.Start();
            int cnt = 1000000;

            ManualResetEvent mre = new ManualResetEvent(false);
            int x = 0;
            WaitCallback act2 = (a) => {
                //Console::WriteLine("starting 2");
                Interlocked.Increment(ref x);
                if (x == cnt) {
                    mre.Set();
                }
            };
            for (int i = 0; i < cnt; i++)
                System.Threading.ThreadPool.QueueUserWorkItem(act2);

            mre.WaitOne(100000);


            sw.Stop();
            Console.WriteLine(x);
            Console.WriteLine((long)sw.ElapsedMilliseconds);
        }


        public static void TestSocket() {
            List<int> lst = new List<int>();
            for (int j = 0; j < 1000; j++) {
                lst.Add(j);
            }
            System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
            sw.Start();
            long cnt = 0;

            System.Net.Sockets.Socket s = new System.Net.Sockets.Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            IPAddress adr;
            bool success = IPAddress.TryParse("127.0.0.1", out (adr));
            s.Connect(adr, 443);



            sw.Stop();
            Console.WriteLine(cnt);
            Console.WriteLine("TestPerformanceLinqOrderBy " + (long)sw.ElapsedMilliseconds);
        }

        public static void TestPerformanceDictionary() {
            System.Collections.Generic.Dictionary<int, int> dic = new System.Collections.Generic.Dictionary<int, int>();
            for (int i = 0; i < 10000; i++) {
                dic[i] = i;
            }


            System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
            sw.Start();
            long sum = 0;
            for (int i = 0; i < 100000; i++) {
                foreach(var kvp in dic) {
                    sum += kvp.Value;
                }
            }


            sw.Stop();
            Console.WriteLine(sum);
            Console.WriteLine(sw.ElapsedMilliseconds);

        }

        public static void TestPerformanceUDPClient() {
            System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
            sw.Start();

            System.Net.Sockets.Socket sock = new Socket(System.Net.Sockets.AddressFamily.InterNetwork, System.Net.Sockets.SocketType.Dgram, System.Net.Sockets.ProtocolType.Udp);
            byte[] bts = { 0, 1, 2 };


            System.Net.IPAddress ipa;
            System.Net.IPAddress.TryParse("127.0.0.1", out (ipa));

            System.Net.IPEndPoint ipe = new System.Net.IPEndPoint(ipa, 9060);

            for (int i = 0; i < 1000000; i++) {
                sock.BeginSendTo(bts, 0, 3, System.Net.Sockets.SocketFlags.None, ipe, null, null);
            }


            sw.Stop();
            Console.WriteLine((long)sw.ElapsedMilliseconds);
        }

        public static void TestPerformanceStringBuilder()
        {
            System.Text.StringBuilder sb = new System.Text.StringBuilder();

            System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
            sw.Start();

            for (int i = 0; i < 10000000; i++) {
                for (int j = 0; j < 20; j++)
                {
                    sb.Append(i);
                    //sb.Append("bla");
                }
                sb.Clear();
            }


            sw.Stop();
            Console.WriteLine(sw.ElapsedMilliseconds);
        }


        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            System.Text.StringBuilder sb = new System.Text.StringBuilder();
            sb.Append("bla");
            sb.Append(37.38);
            sb.Append("blo");
            Console.WriteLine(sb.ToString());
            TestPerformanceStringBuilder();
            return;

            //Tuple<string, int, int> tpl;
            //tpl.

            TestPerformanceDictionary();
            return;
            Random rnd = new Random();
            byte[] bts = Encoding.UTF8.GetBytes("a€b");
            DateTime dt = DateTime.Now;

            Console.WriteLine(dt.ToString());


            string stry = Encoding.UTF8.GetString(new byte[] { 0xBF });

            Console.WriteLine(stry);
            int ix = 37;

            Console.WriteLine(((Int32) 37).ToString("X2"));
            //System.Threading.Thread t = new Thread(() => {
            //    int cnt = 0;
            //    while (true) {
            //        Console.WriteLine("cnt = " + cnt++);
            //        Thread.Sleep(100);
            //    }
            //});
            //t.IsBackground = true;
            //t.Start();



            TestPerformanceUDPClient();
            return;

            TestPerformanceStack();
            TestPerformanceConcurrentStack();
            TestPerformanceThreadPool();


            return;




            TestPerformanceConcurrentQueue();
            
            return;
            TestPerformanceThread();
           // return;

            TestPerformanceManualResetEvent();
            return;

            TestPerformanceRef();

            return;

            TestPerformanceThreadPool();
            return;

            int maxt, maxct;
            ThreadPool.GetMaxThreads(out maxt, out maxct);
            //double xx = (15600.0 / 15400) * (110 / Math.Sqrt(3));

            //double first = xx * 0.7;
            //double second = xx * 0.25;
            //double third = xx * 1.1;
            //double fourth = Math.Sqrt(3) * xx * 0.2 / 3;

            TestSocket();
            TestPerformanceLinqOrderBy();

            TestPerformanceLinqWhere();
            TestPerformanceLambda();

            TestPerformanceIterator();
            return;

            System.Diagnostics.Process[] procs = System.Diagnostics.Process.GetProcesses();
            for (int i = 0; i < procs.Length; i++) {
                try {
                    Console.WriteLine(procs[i].ProcessName + " " + procs[i].TotalProcessorTime.Ticks);
                } catch (Exception) { }
            }



         //   long ul = Int64.MinValue - 1;

            object obj = 2u;
            object obj2 = obj;
            bool bl = obj is int;
            bool bl2 = obj is uint;
            obj2 = 3;
            obj = null;
            bool bl3 = (int)obj == 2;


            Process proc = new Process();
            // System::String fname = "C:\\projects\\CPP\\SharpTest\\bin\\Debug\\SharpTest.exe";
            //proc.StartInfo.CreateNoWindow = true;
            proc.StartInfo.UseShellExecute = false;

            proc.StartInfo.FileName = "C:\\projects\\CPP\\SharpTest\\bin\\Release\\SharpTest.exe";
            proc.Start();
            bool stop = true;
            if (stop)
                return;

            Console.WriteLine("SharpTest");
            Thread.Sleep(5000);
            Console.WriteLine("After sleep");
            if (stop)
                return;
            string[] comports = System.IO.Ports.SerialPort.GetPortNames();



            System.IO.Ports.SerialPort sp = new System.IO.Ports.SerialPort("COM5", 19200, Parity.None, 8, StopBits.One);
            sp.Open();
            while (!stop) {

                sp.Write(new byte[] { 1, 2, 3 }, 0, 3);
            }

            ////  var x = Microsoft.Dynamics.Nav.Runtime.NavInteger.Create(37);
            //Microsoft.Dynamics.Nav.WindowsServices.DynamicsNavServer.Main(new string[] { });
            //GCCBuilder.bla.Test bla = new GCCBuilder.bla.Test();
            ////   byte[] bts = System.IO.File.ReadAllBytes("c:\\tmp\\test.txt");

            //// string strbts = Encoding.UTF8.GetString(bts);
            //Console.WriteLine("OK");
            
            //return;


            object o = "a";
            int hc = o.GetHashCode();


            string str = "BloBĦloblabla";

            object ostr = str;
            str = "q";
            str = ostr.ToString();

            str = str.ToLowerInvariant();
            //blobħloblabla

          //  int billion = 1000000000;
          //  int million = 1000000;
            int tmillion = 10000000;
            Stopwatch sw = new Stopwatch();
            bool ok = true;

            sw.Start();
            ok = TestInteger() != 0;
            sw.Stop();
            Console.WriteLine(ok);
            Console.WriteLine(sw.ElapsedMilliseconds);

            sw.Start();
            for (int i = 0; i < tmillion; i++)
            {
                ok &= TestNull();
            }
            sw.Stop();
            Console.WriteLine(ok);
            Console.WriteLine(sw.ElapsedMilliseconds);
            return;




    //        List<int> lst = new List<int>() { 0, 1, 2 };
    //      //  lst.Join()
    ////      IE
    //        List<int> lst2 = (from x in lst select x).ToList();
    //        List<int> lst3 = lst.Where((x) => { return x > 0; }).Select((x) => { return x; }).ToList();
    //        lst = null;
          //  lst.Add(new object());

            //Application.EnableVisualStyles();
            //Application.SetCompatibleTextRenderingDefault(false);
            //Application.Run(new Form1());
        }
    }
}
