using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

// 
namespace audio2nist
{
    /// <summary>
    /// nist 数据文件处理
    /// </summary>
    public class nist
    {
        #region 自定义属性
        /// <summary>
        /// 数据长度
        /// </summary>
        public int head_len { get; set; }
        /// <summary>
        /// 数据内容
        /// </summary>
        public byte[] data { get; set; }
        #endregion

        #region 音频属性
        /// <summary>
        /// 音频编码格式
        /// </summary>
        [IndexAttribute(1)]
        public string sample_coding { get; set; }
        /// <summary>
        /// 采样率
        /// </summary>
        [IndexAttribute(2)]
        public int sample_rate { get; set; }
        /// <summary>
        /// 声道数
        /// </summary>
        [IndexAttribute(3)]
        public int channel_count { get; set; }
        /// <summary>
        /// 样本宽度 几个字节
        /// </summary>
        [IndexAttribute(4)]
        public int sample_n_bytes { get; set; }
        /// <summary>
        /// 样本精度 多少位
        /// </summary>
        [IndexAttribute(5)]
        public int sample_sig_bits { get; set; }
        /// <summary>
        /// 音频数据部分的单元数
        /// </summary>
        [IndexAttribute(6)]
        public int sample_count { get; set; }
        #endregion

        #region 识别信息
        /// <summary>
        /// 识别日期
        /// </summary>
        [IndexAttribute(11)]
        public string date { get; set; }
        /// <summary>
        /// 识别时间
        /// </summary>
        [IndexAttribute(12)]
        public string create_time { get; set; }
        /// <summary>
        /// 端点检测获取的语音开始时间
        /// </summary>
        [IndexAttribute(13)]
        public string speech_begin { get; set; }
        /// <summary>
        /// 端点检测获取的语音结束时间
        /// </summary>
        [IndexAttribute(14)]
        public string speech_end { get; set; }
        /// <summary>
        /// 识别结果
        /// </summary>
        [IndexAttribute(15)]
        public string result { get; set; }
        /// <summary>
        /// 识别中间结果
        /// </summary>
        [IndexAttribute(16)]
        public string result_raw { get; set; }
        /// <summary>
        /// 识别结果可信度
        /// </summary>
        [IndexAttribute(17)]
        public int score { get; set; }
        /// <summary>
        /// 语法得分
        /// </summary>
        [IndexAttribute(18)]
        public int score_raw { get; set; }
        /// <summary>
        /// 语法名
        /// </summary>
        [IndexAttribute(19)]
        public string g_name { get; set; }
        /// <summary>
        /// 语法类型
        /// </summary>
        [IndexAttribute(20)]
        public string g_type { get; set; }
        /// <summary>
        /// 语法语言
        /// </summary>
        [IndexAttribute(21)]
        public string g_lang { get; set; }
        /// <summary>
        /// 语法文件名
        /// </summary>
        [IndexAttribute(22)]
        public string g_file { get; set; }
        #endregion

        #region 标注信息
        /// <summary>
        /// 标注员账号
        /// </summary>
        [IndexAttribute(31)]
        public string transcriber { get; set; }
        /// <summary>
        /// 标注时间
        /// </summary>
        [IndexAttribute(32)]
        public string t_time { get; set; }
        /// <summary>
        /// 业务分类
        /// </summary>
        [IndexAttribute(33)]
        public string business_type { get; set; }
        /// <summary>
        /// 存储项选择
        /// </summary>
        [IndexAttribute(34)]
        public string store_type { get; set; }
        /// <summary>
        /// 音频内容
        /// </summary>
        [IndexAttribute(35)]
        public string orthography { get; set; }
        /// <summary>
        /// 音频内容的拼音
        /// </summary>
        [IndexAttribute(36)]
        public string phonetic_trans { get; set; }
        /// <summary>
        /// 男女声
        /// </summary>
        [IndexAttribute(37)]
        public string gender { get; set; }
        /// <summary>
        /// 项目名
        /// </summary>
        [IndexAttribute(38)]
        public string prj_name { get; set; }
        #endregion

        #region 质检信息
        /// <summary>
        /// 质检员账号
        /// </summary>
        [IndexAttribute(41)]
        public string inspector { get; set; }
        /// <summary>
        /// 质检时间
        /// </summary>
        [IndexAttribute(42)]
        public string i_time { get; set; }
        #endregion

        #region 质检结果
        /// <summary>
        /// 质检结果
        /// </summary>
        [IndexAttribute(51)]
        public string quality { get; set; }
        /// <summary>
        /// 错误类型
        /// </summary>
        [IndexAttribute(52)]
        public string remark { get; set; }

        #endregion

        public nist()
        {
            sample_rate = -1;
            channel_count = -1;
            sample_n_bytes = -1;
            sample_sig_bits = -1;
            sample_count = -1;
            score  = -1;
            score_raw = -1;
        }

        public void LoadData(string file)
        {
            StreamReader sr = new StreamReader(file, Encoding.GetEncoding("GB2312"));
            Type type = this.GetType();
            String line;
            while ((line = sr.ReadLine()) != null)
            {
                string[] strs = line.Split(' ');
                if (strs[0] == "")
                {
                    int len = 0;
                    if (int.TryParse(strs[strs.Length - 1], out len))
                    {
                        this.head_len = len;
                    }
                    continue;
                }
                if (strs.Length >= 3)
                {
                    string res = strs[2];
                    for (int i = 3; i < strs.Length; i++)
                    {
                        res += " " + strs[i];
                    }
                    PropertyInfo pi = type.GetProperty(strs[0]);
                    if (pi.PropertyType == typeof(int))
                    {
                        int i_res;
                        if (int.TryParse(strs[2], out i_res))
                        {
                            pi.SetValue(this, i_res);
                        }
                    }
                    else
                    {
                        pi.SetValue(this, res);
                    }
                }
                if (strs.Length > 0)
                {
                    if (strs[0] == "end_head")
                    {
                        break;
                    }
                }
            }
            long data_len = sr.BaseStream.Length - this.head_len;
            data = new byte[data_len];
            sr.BaseStream.Seek(this.head_len, SeekOrigin.Begin);
            sr.BaseStream.Read(data, 0, (int)data_len);
            sr.Close();
            // 转换无规则
        }

        void Write(StreamWriter sw)
        {
            Dictionary<int, NistInfo> dic = new Dictionary<int, NistInfo>();
            var properties = GetType().GetProperties();
            foreach (var property in properties)
            {
                if (!property.IsDefined(typeof(IndexAttribute), false))
                {
                    continue;
                }
                var attributes = property.GetCustomAttributes();
                foreach (var attribute in attributes)
                {
                    NistInfo obj = new NistInfo();
                    obj.Index = (int)attribute.GetType().GetProperty("Index").GetValue(attribute);
                    obj.Val = property.GetValue(this);
                    obj.Key = property.Name;
                    obj.t = property.PropertyType;
                    dic.Add(obj.Index, obj);
                }
            }
            var dicSort = from objDic in dic orderby objDic.Key ascending select objDic;

            Encoding e = Encoding.GetEncoding("GB2312");
            foreach (KeyValuePair<int, NistInfo> kvp in dicSort)
            {
                byte[] bs;
                if (kvp.Value.Val == null)
                {
                    kvp.Value.Val = string.Empty;
                }
                if (kvp.Value.t == typeof(string))
                {
                    string str = string.Format("{0} -s{1} {2}\n", kvp.Value.Key, kvp.Value.Val.ToString().Length, kvp.Value.Val.ToString());
                    bs = e.GetBytes(str);
                }
                else
                {
                    int res = (int)kvp.Value.Val;
                    string str = string.Format("{0} -i {1}\n", kvp.Value.Key, res);
                    if (res == -1)
                    {
                        str = string.Format("{0} -i {1}\n", kvp.Value.Key, string.Empty);
                    }
                    bs = e.GetBytes(str);
                }
                sw.BaseStream.Write(bs, 0, bs.Length);
            }
            Debug.WriteLine(dic.Count);
        }

        public void SaveData(string file)
        {
            if (channel_count == 2)
            {
                Shell.WriteLine("{0}：{1}", "错误", "不能处理双通道数据");
                return;
            }
            StreamWriter sw = new StreamWriter(file);
            Encoding e = Encoding.GetEncoding("GB2312");
            byte[] bs = e.GetBytes("NIST_1A\n");
            sw.BaseStream.Write(bs, 0, bs.Length);

            bs = e.GetBytes("       \n");
            sw.BaseStream.Write(bs, 0, bs.Length);
            Write(sw);
            bs = e.GetBytes("end_head\n");
            sw.BaseStream.Write(bs, 0, bs.Length);

            string len_str = sw.BaseStream.Length.ToString();
            sw.BaseStream.Seek(15 - len_str.Length, SeekOrigin.Begin);
            bs = e.GetBytes(len_str);
            sw.BaseStream.Write(bs, 0, bs.Length);

            //sw.BaseStream.Seek(0, SeekOrigin.End);
            //sw.BaseStream.Write(data, 0, data.Length);
            sw.Close();

            //.nist
            string pcm_file = file.Replace(".nist",".pcm");
            StreamWriter sw_pcm = new StreamWriter(pcm_file);
            sw_pcm.BaseStream.Write(data, 0, data.Length);
            sw_pcm.Close();

            StreamReader nist_read = new StreamReader(file);
            string res = nist_read.ReadToEnd();
            res = NistAttr.ZPDecoding(res);
            nist_read.Close();

            StreamWriter sw_nist = new StreamWriter(file);
            sw_nist.Write(res);
            sw_nist.Close();
        }
    }
}