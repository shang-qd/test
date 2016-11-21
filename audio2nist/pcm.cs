using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace audio2nist
{
    public class pcm
    {
        /// <summary>
        /// 数据长度
        /// </summary>
        public int data_len { get; set; }
        /// <summary>
        /// 数据内容
        /// </summary>
        public byte[] data { get; set; }

        public bool LoadData(string file)
        {
            BinaryReader br = new BinaryReader(File.OpenRead(file));
            data_len = (int)br.BaseStream.Length;
            data = br.ReadBytes(data_len);
            return true;
        }

        public nist GetNist()
        {
            nist obj = new nist();
            obj.data = this.data;
            //obj.sample_count = data_len / 2;
            return obj;
        }
    }
}
