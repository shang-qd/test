using System;
using System.Collections.Generic;
using System.Configuration;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace VD_SSL
{
    public partial class Index : System.Web.UI.Page
    {
        Dictionary<string, string> sdk2vd = new Dictionary<string, string>();
        Dictionary<string, string> vd2sdk = new Dictionary<string, string>();

        void GetSdk2vd(HttpRequest Request)
        {
            string val = Request.Headers["Cookie"];
            if (!string.IsNullOrEmpty(val))
            {
                sdk2vd.Add("Cookie", val);
            }
        }

        void SetSdk2vd(HttpWebRequest webReq)
        {
            foreach (string key in sdk2vd.Keys)
            {
                webReq.Headers.Add(key, sdk2vd[key]);
            }
            sdk2vd.Clear();
        }

        void GetVd2sdk(HttpWebResponse response)
        {
            string val = response.GetResponseHeader("Set-Cookie");
            if (!string.IsNullOrEmpty(val))
            {
                vd2sdk.Add("Set-Cookie", val);
            }
        }

        void SetVd2sdk(HttpResponse Response)
        {
            foreach (string key in vd2sdk.Keys)
            {
                Response.Headers.Add(key, vd2sdk[key]);
            }
            vd2sdk.Clear();
        }

        string Post(byte[] bs)
        {
            string vd_url = ConfigurationManager.AppSettings["vd_url"];
            HttpWebRequest webReq = (HttpWebRequest)WebRequest.Create(new Uri(vd_url));
            webReq.Method = "POST";
            webReq.ContentType = "application/json; charset=utf-8";
            webReq.ContentLength = bs.Length;
            SetSdk2vd(webReq);
            Stream newStream = webReq.GetRequestStream();
            newStream.Write(bs, 0, bs.Length);
            newStream.Close();
            HttpWebResponse response = (HttpWebResponse)webReq.GetResponse();

            GetVd2sdk(response);
            Stream respStream = response.GetResponseStream();
            StreamReader reader = new StreamReader(respStream, Encoding.UTF8);
            string res = reader.ReadToEnd();
            reader.Close();
            response.Close();
            newStream.Close();
            return res;
        }

        protected void Page_Load(object sender, EventArgs e)
        {
            GetSdk2vd(this.Request);
            byte[] bs = this.Request.BinaryRead(this.Request.TotalBytes);
            string res = Post(bs);

            this.Response.Expires = 0;
            this.Response.Clear();
            SetVd2sdk(this.Response);
            this.Response.Write(res);
            this.Response.End();
        }
    }
}