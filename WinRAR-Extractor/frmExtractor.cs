using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WinRAR_Extractor
{
    public partial class frmExtractor : Form
    {
        public frmExtractor()
        {
            InitializeComponent();
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
            Match white14link = Regex.Match(html,
                "(?<=<a href=\"download-).+?(?=(32|64)scp.html\" target=\"_blank\" class=\"white14link\">(32|64)位下载</a>)",
                RegexOptions.Multiline | RegexOptions.Singleline);
            while (white14link.Success)
            {
                int result = 0;
                if (int.TryParse(white14link.Value, out result))
                {
                    if (version < result)
                    {
                        version = result;
                        //Console.WriteLine(white14link.Value);
                    }
                }
                white14link = white14link.NextMatch();
            }
            
            string freeUrl_x86 = $"https://www.winrar.com.cn/download/wrar{version}scp.exe";
            string lastModified_x86 = HttpWebHelper.GetHttpWebData(freeUrl_x86, 3000, null, true);
            //Console.WriteLine(lastModified_x86);
            
            string freeUrl_x64 = $"https://www.winrar.com.cn/download/winrar-x64-{version}scp.exe";
            string lastModified_x64 = HttpWebHelper.GetHttpWebData(freeUrl_x64, 3000, null, true);
            //Console.WriteLine(lastModified_x64);

            this.labVersion.Text = $"最新版本：{version}，更新时间：{lastModified_x86} - [简体中文]";

            string rrlb_x86 = $"https://www.win-rar.com/fileadmin/winrar-versions/sc/sc{lastModified_x86}/rrlb/wrar{version}sc.exe";
            string rrlb_x64 = $"https://www.win-rar.com/fileadmin/winrar-versions/sc/sc{lastModified_x64}/rrlb/winrar-x64-{version}sc.exe";
            string wrr_x86 = $"https://www.win-rar.com/fileadmin/winrar-versions/sc/sc{lastModified_x86}/wrr/wrar{version}sc.exe";
            string wrr_x64 = $"https://www.win-rar.com/fileadmin/winrar-versions/sc/sc{lastModified_x64}/wrr/winrar-x64-{version}sc.exe";

            this.tbrrlbx86.Text = rrlb_x86;
            this.tbrrlbx64.Text = rrlb_x64;
            this.tbwrrx86.Text = wrr_x86;
            this.tbwrrx64.Text = wrr_x64;

            // GMT格式时间
            //string gmt = "Wed, 14 Apr 2021 11:30:20 GMT";
            //Console.WriteLine(this.GMT2Local(gmt));
        }
        
        [DllImport("WinRAR-Generate.dll", CharSet = CharSet.Ansi, EntryPoint = "KeyGenerate", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr KeyGenerate(string UserName, string LicenseType);
        private void btnGenerate_Click(object sender, EventArgs e)
        {
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
                string rarreg = Marshal.PtrToStringAnsi(KeyGenerate(tbName.Text, tbLicense.Text));
                StreamWriter sw = new StreamWriter(saveFileDialog.FileName, false, Encoding.ASCII);
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
    }
}
