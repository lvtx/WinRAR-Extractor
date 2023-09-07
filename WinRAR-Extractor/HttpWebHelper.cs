using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Security;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;

namespace WinRAR_Extractor
{
    /// <summary>
    /// 有关HTTP请求的辅助类
    /// </summary>
    public static class HttpWebHelper
    {
        private static readonly string DefaultUserAgent = "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/78.0.3904.108 Safari/537.36";
        /// <summary>
        /// 线程锁对象
        /// </summary>
        private static object _lotGetLocker = new object();

        //public HttpWebHelper()
        //{
        //	_lotGetLocker = new object();
        //}

        /// <summary>
        /// 创建GET方式的HTTP请求
        /// </summary>
        /// <param name="url">请求的URL</param>
        /// <param name="timeout">请求的超时时间</param>
        /// <param name="userAgent">请求的客户端浏览器信息，可以为空</param>
        /// <param name="cookies">随同HTTP请求发送的Cookie信息，如果不需要身份验证可以为空</param>
        /// <returns></returns>
        public static string GetHttpWebData(string url, int? timeout, string userAgent = null, bool lastModified = false)
        {
            lock (_lotGetLocker)
            {
                //如果是发送HTTPS请求
                if (url.StartsWith("https", StringComparison.OrdinalIgnoreCase))
                {
                    // 请求前设置一下使用的安全协议类型
                    ServicePointManager.DefaultConnectionLimit = 512;
                    ServicePointManager.Expect100Continue = true;
                    ServicePointManager.CheckCertificateRevocationList = false;
                    ServicePointManager.SecurityProtocol = SecurityProtocolType.Ssl3
                        | (SecurityProtocolType)12288   // SecurityProtocolType.Tls13
                        | SecurityProtocolType.Tls12
                        | SecurityProtocolType.Tls11
                        | SecurityProtocolType.Tls;
                    ServicePointManager.ServerCertificateValidationCallback = new RemoteCertificateValidationCallback(CheckValidationResult);
                }

                HttpWebRequest request = WebRequest.Create(url) as HttpWebRequest;
                string WebData = string.Empty;
                try
                {
                    if (string.IsNullOrEmpty(url))
                    {
                        throw new ArgumentNullException("url");
                    }

                    request.ProtocolVersion = HttpVersion.Version10;
                    request.Method = "GET";
                    request.UserAgent = DefaultUserAgent;
                    request.Accept = "text/html";
                    request.Headers.Add("Accept-Language", "zh-CN,zh;q=0.9");
                    request.AllowAutoRedirect = true;
                    request.KeepAlive = false;
                    if (!string.IsNullOrEmpty(userAgent))
                    {
                        request.UserAgent = userAgent;
                    }
                    if (timeout.HasValue)
                    {
                        request.Timeout = timeout.Value;
                    }
                    request.CookieContainer = new CookieContainer();

                    //获取网页响应结果
                    //while (response == null || response.StatusCode != HttpStatusCode.OK)
                    //{
                    //	if (Config.IsAppExit) return string.Empty;

                    //	response = request.GetResponse() as HttpWebResponse;
                    //}
                    using (HttpWebResponse response = request.GetResponse() as HttpWebResponse)
                    {
                        // 获取HttpWebResponse数据
                        if (response != null && response.StatusCode == HttpStatusCode.OK)
                        {
                            if (lastModified)
                            {
                                WebData = response.LastModified.ToString("yyyyMMdd");
                            }
                            else
                            {
                                using (StreamReader reader = new StreamReader(response.GetResponseStream(), Encoding.GetEncoding("gb2312")))
                                {
                                    // 获取的Json字符串
                                    WebData = reader.ReadToEnd();

                                    if (reader != null) reader.Close();
                                }
                            }
                            response.Close();
                            response.Dispose();
                        }
                    }
                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex.Message);
                    //Config.logProc.WriteLine(ex.Message);
                    if (request != null) request.Abort();
                }
                finally
                {
                    if (request != null) request.Abort();
                }
                return WebData;
            }
        }

        /// <summary>
        /// 创建GET方式的HTTP请求
        /// </summary>
        /// <param name="url">请求的URL</param>
        /// <param name="timeout">请求的超时时间</param>
        /// <param name="userAgent">请求的客户端浏览器信息，可以为空</param>
        /// <param name="cookies">随同HTTP请求发送的Cookie信息，如果不需要身份验证可以为空</param>
        /// <returns></returns>
        public static bool GetHttpCheck(string url, int? timeout, string userAgent, bool lastModified = false)
        {
            lock (_lotGetLocker)
            {
                HttpWebRequest request = WebRequest.Create(url) as HttpWebRequest;
                
                try
                {
                    if (string.IsNullOrEmpty(url))
                    {
                        throw new ArgumentNullException("url");
                    }

                    //如果是发送HTTPS请求
                    if (url.StartsWith("https", StringComparison.OrdinalIgnoreCase))
                    {
                        // 请求前设置一下使用的安全协议类型
                        ServicePointManager.DefaultConnectionLimit = 512;
                        ServicePointManager.Expect100Continue = true;
                        ServicePointManager.CheckCertificateRevocationList = false;
                        ServicePointManager.SecurityProtocol = SecurityProtocolType.Ssl3 | SecurityProtocolType.Tls12 | SecurityProtocolType.Tls11 | SecurityProtocolType.Tls;
                        ServicePointManager.ServerCertificateValidationCallback = new RemoteCertificateValidationCallback(CheckValidationResult);
                    }

                    request.Method = "GET";
                    request.UserAgent = DefaultUserAgent;
                    request.Accept = "text/html";
                    request.Headers.Add("Accept-Language", "zh-CN,zh;q=0.9");
                    request.AllowAutoRedirect = true;
                    request.KeepAlive = false;
                    if (!string.IsNullOrEmpty(userAgent))
                    {
                        request.UserAgent = userAgent;
                    }
                    if (timeout.HasValue)
                    {
                        request.Timeout = timeout.Value;
                    }
                    request.CookieContainer = new CookieContainer();

                    //获取网页响应结果
                    //while (response == null || response.StatusCode != HttpStatusCode.OK)
                    //{
                    //	if (Config.IsAppExit) return string.Empty;

                    //	response = request.GetResponse() as HttpWebResponse;
                    //}
                    using (HttpWebResponse response = request.GetResponse() as HttpWebResponse)
                    {
                        // 获取HttpWebResponse数据
                        if (response != null && response.StatusCode == HttpStatusCode.OK)
                        {
                            response.Close();
                            response.Dispose();
                        }
                    }
                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex.Message);
                    //Config.logProc.WriteLine(ex.Message);
                    if (request != null) request.Abort();
                    return false;
                }
                finally
                {
                    if (request != null) request.Abort();
                }
                return true;
            }
        }

        //该方法用于验证服务器证书是否合法，当然可以直接返回true来表示验证永远通过。服务器证书具体内容在参数certificate中。可根据个人需求验证
        //该方法在request.GetResponse()时触发
        private static bool CheckValidationResult(object sender, X509Certificate certificate, X509Chain chain, SslPolicyErrors errors)
        {
            return true; //总是接受
        }
    }
}
