using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace audio2nist
{
    /// <summary>
    /// wav 数据
    /// </summary>
    public class wav
    {
        #region RIFF标准头文件数据
        /// <summary>
        /// 资源交换档案标准（Resource Interchange File Format）
        /// </summary>
        public char[] RIFF { get; set; }
        /// <summary>
        /// 文件长度
        /// </summary>
        public int file_len { get; set; }
        /// <summary>
        /// wave 格式标准文件
        /// </summary>
        public char[] WAVE { get; set; }
        #endregion

        #region frm数据
        /// <summary>
        /// frm格式开始
        /// </summary>
        public char[] frm_title { get; set; }
        /// <summary>
        /// frm 格式长度
        /// </summary>
        public int frm_len { get; set; }
        /// <summary>
        /// 编码格式
        /// </summary>
        public short frm_code { get; set; }
        /// <summary>
        /// 声道
        /// </summary>
        public short frm_channel_count { get; set; }
        /// <summary>
        /// 采样率
        /// </summary>
        public int frm_sample_rate { get; set; }
        /// <summary>
        /// 每秒字节数
        /// </summary>
        public int frm_Bps { get; set; }
        /// <summary>
        /// 每个采样单元字节数
        /// </summary>
        public short frm_Bpu { get; set; }
        /// <summary>
        /// 采样单位位数 一般为16
        /// </summary>
        public short frm_bpu { get; set; }
        /// <summary>
        /// 附加信息
        /// </summary>
        public char[] frm_attach { get; set; }
        #endregion

        #region fact数据
        /// <summary>
        /// fact 标准标记
        /// </summary>
        public char[] fact_title { get; set; }
        /// <summary>
        /// fact 数据长度
        /// </summary>
        public int fact_len { get; set; }
        /// <summary>
        /// fact 数据
        /// </summary>
        public char[] fact { get; set; }
        #endregion

        #region 音频数据
        /// <summary>
        /// 数据标记头
        /// </summary>
        public char[] data_title { get; set; }
        /// <summary>
        /// 数据长度
        /// </summary>
        public int data_len { get; set; }
        /// <summary>
        /// 数据内容
        /// </summary>
        public byte[] data { get; set; }
        #endregion

        /// <summary>
        /// 加载数据
        /// </summary>
        /// <param name="file"></param>
        /// <returns></returns>
        public bool LoadData(string file)
        {
            BinaryReader br = new BinaryReader(File.OpenRead(file));
            RIFF = br.ReadChars(4);
            file_len = br.ReadInt32();
            WAVE = br.ReadChars(4);
            if (RIFF[0] != 'R' || RIFF[1] != 'I' || RIFF[2] != 'F' || RIFF[3] != 'F')
            {
                return false;
            }
            frm_title = br.ReadChars(4);
            frm_len = br.ReadInt32();
            frm_code = br.ReadInt16();
            frm_channel_count = br.ReadInt16();
            frm_sample_rate = br.ReadInt32();
            frm_Bps = br.ReadInt32();
            frm_Bpu = br.ReadInt16();
            frm_bpu = br.ReadInt16();
            int len = frm_len - 16;
            if (len > 0)
            {
                frm_attach = br.ReadChars(len);
            }

            string data_type = new string(br.ReadChars(4));
            if (data_type == "fact")
            {
                fact_title = new char[] { 'f', 'a', 'c', 't' };
                fact_len = br.ReadInt32();
                if (fact_len > 0)
                {
                    fact = br.ReadChars(fact_len);
                }
                data_title = br.ReadChars(4);
            }
            else
            {
                data_title = new char[] { 'd', 'a', 't', 'a' };
            }

            data_len = br.ReadInt32();
            if (data_len > 0)
            {
                data = br.ReadBytes(data_len);
            }
            return true;
        }

        /// <summary>
        /// 得到nist对象
        /// </summary>
        /// <returns></returns>
        public nist GetNist()
        {
            nist obj = new nist();
            obj.sample_coding = "pcm";
            obj.sample_rate = this.frm_sample_rate;
            obj.channel_count = this.frm_channel_count;
            obj.sample_n_bytes = this.frm_Bpu;
            obj.sample_sig_bits = this.frm_bpu;
            obj.sample_count = this.data_len / obj.sample_n_bytes;
            obj.data = this.data;
            return obj;
        }
    }
}
