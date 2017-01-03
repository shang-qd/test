using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Obd;

using System.Net.Security;
using System.Security;
using System.Security.Cryptography;
using System.Security.Cryptography.X509Certificates;
using System.Net;
using System.IO;
 

namespace Tools
{
    class Test
    {
        private bool CheckValidationResult(object sender,
        X509Certificate certificate, X509Chain chain, SslPolicyErrors errors)
        {
            System.Console.WriteLine(certificate.Issuer);
            return true;// Always accept
        }

        public HttpWebResponse getUrlResponse(string url)
        {
            HttpWebResponse resp = null;
            HttpWebRequest req = (HttpWebRequest)WebRequest.Create(url);

            if (url.StartsWith("https", StringComparison.OrdinalIgnoreCase))
            {
                ServicePointManager.ServerCertificateValidationCallback =
                        new RemoteCertificateValidationCallback(CheckValidationResult);
            }
            resp = (HttpWebResponse)req.GetResponse();
            return resp;
        }
    }

    class Program
    {
        /// <summary>
        /// 
　　    /// </summary>
        /// <param name="args"></param>
        static void Main(string[] args)
        {
            Test t = new Test();
            HttpWebResponse response = t.getUrlResponse("https://localhost/");
            Stream responseStream = response.GetResponseStream();
            StreamReader sr = new StreamReader(responseStream, Encoding.Default);
            string msg = sr.ReadToEnd();
            System.Console.WriteLine(msg);
            sr.Close();
            response.Close();
        }
    }
}
