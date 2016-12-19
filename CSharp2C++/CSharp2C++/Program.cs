using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;


namespace CSharp2C__
{
    class Program
    {
        [DllImport("C++.dll")]
        //extern static int Add(int a, int b);
        extern static uint Add();


        [DllImport("C++.dll")]
        //extern static void cprintf(string p);

        extern static void cprintf(uint p);

        static void Main(string[] args)
        {
            uint a = Add();

            cprintf(a);

            /*
            int a = 1;
            int b = 2;
            int c = Add(a, b);
            Console.WriteLine(c);
            cprintf("C++ 输出！");
            Console.ReadLine();*/
        }
    }
}
