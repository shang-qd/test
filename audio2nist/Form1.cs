using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace audio2nist
{
    public partial class Form1 : Form
    {

        string m_src;
        string m_dst;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            this.txt_src.Text = string.Empty;
            this.txt_dst.Text = string.Empty;
            /*
            string wav_file = @"D:\音频工具\不同格式语音数据\wav\0b9e3fa546224dff3d20e4aa7c5f115c.2.wav";
            string nist_file1 = @"D:\wav2nist.nist";
            wav2nisit(wav_file, nist_file1);
            
            string pcm_file = @"D:\音频工具\不同格式语音数据\pcm\part-00f473f0-6089-47ee-9145-9acda39c4fd4.pcm";
            string nist_file2 = @"D:\pcm2nist.nist";
            pcm2nist(pcm_file, nist_file2);

            string src_nist = @"D:\音频工具\不同格式语音数据\nist\10840_b3e1e290_121345.nist";
            string dst_nist = @"D:\nist2nist.nist";
            nist2nist(src_nist, dst_nist);
             */
        }

        void wav2nist(string wav_file,string nist_file)
        {
            wav obj = new wav();
            obj.LoadData(wav_file);
            nist obj_nist = obj.GetNist();
            obj_nist.SaveData(nist_file);
        }

        void pcm2nist(string pcm_file, string nist_file)
        {
            pcm obj = new pcm();
            obj.LoadData(pcm_file);
            nist obj_nist = obj.GetNist();
            obj_nist.SaveData(nist_file);
        }

        void nist2nist(string src_file, string dst_file)
        {
            nist obj = new nist();
            obj.LoadData(src_file);
            obj.SaveData(dst_file);
        }

        private void btn_run_Click(object sender, EventArgs e)
        {
            m_src = this.txt_src.Text;
            m_dst = this.txt_dst.Text;
            if (string.IsNullOrEmpty(m_src))
            {
                Shell.WriteLine("{0}：{1}", "警告", "请填写源路径");
                return;
            }
            if (string.IsNullOrEmpty(m_dst))
            {
                Shell.WriteLine("{0}：{1}", "警告", "请填写目标路径");
                return;
            }
            if (Directory.Exists(m_src) == false)
            {
                Shell.WriteLine("{0}：{1}", "警告", "源路径无效");
                return;
            }
            if (Directory.Exists(m_dst) == false)
            {
                Shell.WriteLine("{0}：{1}", "警告", "目标路径无效");
                return;
            }
            PathRecursion(m_src, m_dst);
        }

        void PathRecursion(string src_path,string dst_path)
        {
            FileTo(src_path, dst_path);
            string[] dirs = Directory.GetDirectories(src_path);
            foreach (string dir in dirs)
            {
                string temp_path = dst_path + dir.Replace(src_path,string.Empty);
                Directory.CreateDirectory(temp_path);
                PathRecursion(dir, temp_path);
            }
        }

        void FileTo(string src_path, string dst_path)
        {
            string[] fs = Directory.GetFiles(src_path);
            for (int i = 0; i < fs.Length; i++)
            {
                try
                {
                    string file = Path.GetFileName(fs[i]);
                    string ext = Path.GetExtension(file);
                    if (ext == ".pcm")
                    {
                        string dst_file = dst_path + "\\" + file.Replace(".pcm", ".nist");
                        pcm2nist(fs[i], dst_file);
                    }
                    if (ext == ".wav")
                    {
                        string dst_file = dst_path + "\\" + file.Replace(".wav", ".nist");
                        wav2nist(fs[i], dst_file);
                    }
                    if (ext == ".nist")
                    {
                        string dst_file = dst_path + "\\" + file;
                        nist2nist(fs[i], dst_file);
                    }
                    Shell.WriteLine("{0}：{1}", "注意", string.Format("{0}/{1}:{2}", i + 1, fs.Length, file));
                }
                catch (Exception ex)
                {
                    Shell.WriteLine("{0}：{1}", "错误", string.Format("{0}/{1}:{2}", i + 1, fs.Length, fs[i] + ex.ToString()));
                }
            }
        }

    }
}
