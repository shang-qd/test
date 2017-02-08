using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace audio2nist
{
    /// <summary>
    /// 字符串加密函数
    /// </summary>
    public class NistAttr
    {
        [DllImport(@"thide.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern int thide_decrypt(ref byte t, int src_len, ref byte dst, int dst_len);


        [DllImport(@"thide.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern int thide_encrypt(ref byte t, int src_len, ref byte dst, int dst_len);

        /// <summary>
        /// 解密
        /// </summary>
        /// <param name="strIn"></param>
        /// <returns></returns>
        public static string ZPDecoding(string strIn)
        {
            if (string.IsNullOrEmpty(strIn))
            {
                return "";
            }
            int n = 0;
            byte t = 1;
            byte[] btIn = Encoding.Default.GetBytes(strIn);
            //获取解密所需的实际长度
            int realLength = thide_decrypt(ref btIn[0], btIn.Length, ref t, n);

            //解密
            byte[] btRes = new byte[realLength];
            realLength = thide_decrypt(ref btIn[0], btIn.Length, ref btRes[0], realLength);
            string strOut = Encoding.Default.GetString(btRes);
            return strOut;
        }

        /// <summary>
        /// 加密
        /// </summary>
        /// <param name="strIn"></param>
        /// <returns></returns>
        public static string ZPEncoding(string strIn)
        {
            if (string.IsNullOrEmpty(strIn))
            {
                return "";
            }
            int n = 0;
            byte btTest = 1;
            byte[] btIn = Encoding.Default.GetBytes(strIn);
            //获取加密所需实际长度
            int realLength = thide_encrypt(ref btIn[0], btIn.Length, ref btTest, n);

            //加密
            byte[] btRes = new byte[realLength];
            realLength = thide_encrypt(ref btIn[0], btIn.Length, ref btRes[0], realLength);

            string strOut = Encoding.Default.GetString(btRes);
            return strOut;
        }
    }
}
