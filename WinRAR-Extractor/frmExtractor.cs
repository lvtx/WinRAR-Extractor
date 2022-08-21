using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WinRAR_Extractor
{
    public partial class frmExtractor : Form
    {
        private string Version = string.Empty;

        public frmExtractor()
        {
            InitializeComponent();
            this.Version = Assembly.GetExecutingAssembly().GetName().Version.ToString();
            this.Text = $"WinRAR 简体中文-商业版 提取器 v{this.Version}";
        }

        private void frmExtractor_Load(object sender, EventArgs e)
        {
            this.btnRefresh.PerformClick();
        }

        private void btnRefresh_Click(object sender, EventArgs e)
        {
            string checkUrl = "https://www.winrar.com.cn/download.htm";
            string html = HttpWebHelper.GetHttpWebData(checkUrl, 3000, null);
            //Console.WriteLine(html);

            int version = 0;
            string freeUrl_x86 = string.Empty;
            string freeUrl_x64 = string.Empty;

            Match white14Link = Regex.Match(html,
                "(?<=<a href=\")download-.+?(32|64)scp.html(?=\" target=\"_blank\" class=\"white14link\">(32|64)位下载</a>)",
                RegexOptions.Multiline | RegexOptions.Singleline);
            //Console.WriteLine(white14Link.Groups.Count);

            while (white14Link.Success)
            {
                //Console.WriteLine(white14Link.Value);
                int result = 0, bit = 0;
                Match versionLink = Regex.Match(white14Link.Value, "(?<=download-).+?(?=(32|64)scp.html)");
                //Console.WriteLine(versionLink.Value);

                if (int.TryParse(versionLink.Value, out result))
                {
                    if (version <= result)
                    {
                        version = result;
                        Match bitLink = Regex.Match(white14Link.Value, $"(?<=download-{version}).+?(?=scp.html)");
                        //Console.WriteLine(bitLink.Value);

                        string downloadUrl = $"https://www.winrar.com.cn/{white14Link.Value}";
                        //Console.WriteLine(downloadUrl);
                        string htmld = HttpWebHelper.GetHttpWebData(downloadUrl, 3000, null);
                        //Console.WriteLine(htmld);

                        Match downloadLink = Regex.Match(htmld, $"(?<=meta http-equiv=\"refresh\" content=\".+?;url=).*?(?=\")");
                        //Console.WriteLine(downloadLink.Value);

                        if (int.TryParse(bitLink.Value, out bit) && bit != 64)
                        {
                            freeUrl_x86 = downloadLink.Value;
                        }
                        else
                        {
                            freeUrl_x64 = downloadLink.Value;
                        }
                    }
                }
                white14Link = white14Link.NextMatch();
            }

            Match lastSCname_x86 = Regex.Match(freeUrl_x86, "(?<=https://www.winrar.com.cn/download/).+?(?=scp.exe)");
            string lastModified_x86 = HttpWebHelper.GetHttpWebData(freeUrl_x86, 3000, null, true);
            //Console.WriteLine(lastModified_x86);

            Match lastSCname_x64 = Regex.Match(freeUrl_x64, "(?<=https://www.winrar.com.cn/download/).+?(?=scp.exe)");
            string lastModified_x64 = HttpWebHelper.GetHttpWebData(freeUrl_x64, 3000, null, true);
            //Console.WriteLine(lastModified_x64);

            this.labVersion.Text = $"最新版本：[{version}]，更新时间：[{lastModified_x86}]-[x86] / [{lastModified_x64}]-[x64] - [简体中文]";

            string rrlb_x86 = $"https://www.win-rar.com/fileadmin/winrar-versions/sc/sc{lastModified_x86}/rrlb/{lastSCname_x86.Value}sc.exe";
            string rrlb_x64 = $"https://www.win-rar.com/fileadmin/winrar-versions/sc/sc{lastModified_x64}/rrlb/{lastSCname_x64.Value}sc.exe";
            string wrr_x86 = $"https://www.win-rar.com/fileadmin/winrar-versions/sc/sc{lastModified_x86}/wrr/{lastSCname_x86.Value}sc.exe";
            string wrr_x64 = $"https://www.win-rar.com/fileadmin/winrar-versions/sc/sc{lastModified_x64}/wrr/{lastSCname_x64.Value}sc.exe";

            this.tbrrlbx86.Text = rrlb_x86;
            this.tbrrlbx64.Text = rrlb_x64;
            this.tbwrrx86.Text = wrr_x86;
            this.tbwrrx64.Text = wrr_x64;

            // GMT格式时间
            //string gmt = "Wed, 14 Apr 2021 11:30:20 GMT";
            //Console.WriteLine(this.GMT2Local(gmt));
        }

        private void btnrrlbx86_Click(object sender, EventArgs e)
        {
            this.BrowserUrl(tbrrlbx86.Text.Trim());
        }

        private void btnrrlbx64_Click(object sender, EventArgs e)
        {
            this.BrowserUrl(tbrrlbx64.Text.Trim());
        }

        private void btnwrrx86_Click(object sender, EventArgs e)
        {
            this.BrowserUrl(tbwrrx86.Text.Trim());
        }

        private void btnwrrx64_Click(object sender, EventArgs e)
        {
            this.BrowserUrl(tbwrrx64.Text.Trim());
        }

        [DllImport("WinRAR-Generate.dll", CharSet = CharSet.Ansi, EntryPoint = "KeyGenerate", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr KeyGenerate(string UserName, string LicenseType);

        private void btnGenerate_Click(object sender, EventArgs e)
        {
            if (tbName.Text.Equals(string.Empty) || tbName.Text == "")
            {
                MessageBox.Show("请先填写授权用户！", "授权提示");
                tbName.Focus();
                return;
            }
            if (tbLicense.Text.Equals(string.Empty) || tbLicense.Text == "")
            {
                MessageBox.Show("请先填写授权信息！", "授权提示");
                tbLicense.Focus();
                return;
            }

            string UserName = tbName.Text;
            string LicenseType = tbLicense.Text;

            // 点击弹出对话框
            SaveFileDialog saveFileDialog = new SaveFileDialog();
            // 设置对话框的标题
            saveFileDialog.Title = "保持 WinRAR 授权文件";
            // 设置对话框的初始目录
            saveFileDialog.InitialDirectory = "";
            // 设置对话框的文件类型
            saveFileDialog.Filter = "WinRAR 授权文件|rarreg.key";
            // 设置对话框的文件名
            saveFileDialog.FileName = "rarreg.key";

            //展示对话框
            if (saveFileDialog.ShowDialog() == DialogResult.OK)
            {
                string rarreg = Marshal.PtrToStringAnsi(KeyGenerate(UserName, LicenseType));
                // [2022/08/21] 修复中文字符变问号问题 [Encoding.ASCII => Encoding.Default]
                StreamWriter sw = new StreamWriter(saveFileDialog.FileName, false, Encoding.Default);
                sw.Write(rarreg);
                sw.Flush();
                sw.Close();
                MessageBox.Show(rarreg, "授权文件保存成功！");
            }
        }

        /// <summary>
        /// GMT时间转成本地时间
        /// </summary>
        /// <param name="gmt">字符串形式的GMT时间</param>
        /// <returns></returns>
        public DateTime GMT2Local(string gmt)
        {
            DateTime dt = DateTime.MinValue;
            try
            {
                string pattern = "";
                if (gmt.IndexOf("+0") != -1)
                {
                    gmt = gmt.Replace("GMT", "");
                    pattern = "ddd, dd MMM yyyy HH':'mm':'ss zzz";
                }
                if (gmt.ToUpper().IndexOf("GMT") != -1)
                {
                    pattern = "ddd, dd MMM yyyy HH':'mm':'ss 'GMT'";
                }
                if (pattern != "")
                {
                    dt = DateTime.ParseExact(gmt, pattern, System.Globalization.CultureInfo.InvariantCulture, System.Globalization.DateTimeStyles.AdjustToUniversal);
                    dt = dt.ToLocalTime();
                }
                else
                {
                    dt = Convert.ToDateTime(gmt);
                }
            }
            catch
            {
            }
            return dt;
        }

        public void BrowserUrl(string target)
        {
            try
            {
                Process.Start(target);
            }
            catch (Win32Exception noBrowser)
            {
                if (noBrowser.ErrorCode == -2147467259)
                    MessageBox.Show(noBrowser.Message);
            }
            catch (Exception other)
            {
                MessageBox.Show(other.Message);
            }
        }
    }
}
