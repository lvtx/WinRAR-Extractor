namespace WinRAR_Extractor
{
    partial class frmExtractor
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(frmExtractor));
            this.gbTop = new System.Windows.Forms.GroupBox();
            this.btnRefresh = new System.Windows.Forms.Button();
            this.labVersion = new System.Windows.Forms.Label();
            this.gbBottom = new System.Windows.Forms.GroupBox();
            this.btnGenerate = new System.Windows.Forms.Button();
            this.tbLicense = new System.Windows.Forms.TextBox();
            this.tbName = new System.Windows.Forms.TextBox();
            this.labLicense = new System.Windows.Forms.Label();
            this.labName = new System.Windows.Forms.Label();
            this.gbFill = new System.Windows.Forms.GroupBox();
            this.gbWRR = new System.Windows.Forms.GroupBox();
            this.btnwrrx64 = new System.Windows.Forms.Button();
            this.btnwrrx86 = new System.Windows.Forms.Button();
            this.tbwrrx64 = new System.Windows.Forms.TextBox();
            this.tbwrrx86 = new System.Windows.Forms.TextBox();
            this.labwrrx64 = new System.Windows.Forms.Label();
            this.labwrrx86 = new System.Windows.Forms.Label();
            this.gbRRLB = new System.Windows.Forms.GroupBox();
            this.btnrrlbx64 = new System.Windows.Forms.Button();
            this.btnrrlbx86 = new System.Windows.Forms.Button();
            this.tbrrlbx64 = new System.Windows.Forms.TextBox();
            this.tbrrlbx86 = new System.Windows.Forms.TextBox();
            this.labrrlbx64 = new System.Windows.Forms.Label();
            this.labrrlbx86 = new System.Windows.Forms.Label();
            this.gbTop.SuspendLayout();
            this.gbBottom.SuspendLayout();
            this.gbFill.SuspendLayout();
            this.gbWRR.SuspendLayout();
            this.gbRRLB.SuspendLayout();
            this.SuspendLayout();
            // 
            // gbTop
            // 
            this.gbTop.Controls.Add(this.btnRefresh);
            this.gbTop.Controls.Add(this.labVersion);
            this.gbTop.Dock = System.Windows.Forms.DockStyle.Top;
            this.gbTop.Location = new System.Drawing.Point(0, 0);
            this.gbTop.Name = "gbTop";
            this.gbTop.Size = new System.Drawing.Size(684, 50);
            this.gbTop.TabIndex = 0;
            this.gbTop.TabStop = false;
            this.gbTop.Text = "版本信息";
            // 
            // btnRefresh
            // 
            this.btnRefresh.Location = new System.Drawing.Point(596, 18);
            this.btnRefresh.Name = "btnRefresh";
            this.btnRefresh.Size = new System.Drawing.Size(75, 25);
            this.btnRefresh.TabIndex = 1;
            this.btnRefresh.Text = "刷新(&R)";
            this.btnRefresh.UseVisualStyleBackColor = true;
            this.btnRefresh.Click += new System.EventHandler(this.btnRefresh_Click);
            // 
            // labVersion
            // 
            this.labVersion.AutoSize = true;
            this.labVersion.Location = new System.Drawing.Point(8, 22);
            this.labVersion.Name = "labVersion";
            this.labVersion.Size = new System.Drawing.Size(452, 17);
            this.labVersion.TabIndex = 0;
            this.labVersion.Text = "最新版本：[0000]，更新时间：[00000000]-[x86] / [00000000]-[x64] - [简体中文]";
            // 
            // gbBottom
            // 
            this.gbBottom.Controls.Add(this.btnGenerate);
            this.gbBottom.Controls.Add(this.tbLicense);
            this.gbBottom.Controls.Add(this.tbName);
            this.gbBottom.Controls.Add(this.labLicense);
            this.gbBottom.Controls.Add(this.labName);
            this.gbBottom.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.gbBottom.Location = new System.Drawing.Point(0, 274);
            this.gbBottom.Name = "gbBottom";
            this.gbBottom.Size = new System.Drawing.Size(684, 97);
            this.gbBottom.TabIndex = 4;
            this.gbBottom.TabStop = false;
            this.gbBottom.Text = "授权信息";
            // 
            // btnGenerate
            // 
            this.btnGenerate.Location = new System.Drawing.Point(142, 61);
            this.btnGenerate.Name = "btnGenerate";
            this.btnGenerate.Size = new System.Drawing.Size(400, 27);
            this.btnGenerate.TabIndex = 4;
            this.btnGenerate.Text = "生成授权文件";
            this.btnGenerate.UseVisualStyleBackColor = true;
            this.btnGenerate.Click += new System.EventHandler(this.btnGenerate_Click);
            // 
            // tbLicense
            // 
            this.tbLicense.ForeColor = System.Drawing.Color.Maroon;
            this.tbLicense.Location = new System.Drawing.Point(348, 28);
            this.tbLicense.Name = "tbLicense";
            this.tbLicense.Size = new System.Drawing.Size(322, 23);
            this.tbLicense.TabIndex = 3;
            this.tbLicense.Text = "Unlimited Company License";
            this.tbLicense.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // tbName
            // 
            this.tbName.ForeColor = System.Drawing.Color.Navy;
            this.tbName.Location = new System.Drawing.Point(71, 28);
            this.tbName.Name = "tbName";
            this.tbName.Size = new System.Drawing.Size(200, 23);
            this.tbName.TabIndex = 2;
            this.tbName.Text = "Federal Agency for Education";
            this.tbName.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // labLicense
            // 
            this.labLicense.AutoSize = true;
            this.labLicense.Location = new System.Drawing.Point(285, 31);
            this.labLicense.Name = "labLicense";
            this.labLicense.Size = new System.Drawing.Size(68, 17);
            this.labLicense.TabIndex = 1;
            this.labLicense.Text = "授权信息：";
            // 
            // labName
            // 
            this.labName.AutoSize = true;
            this.labName.Location = new System.Drawing.Point(8, 31);
            this.labName.Name = "labName";
            this.labName.Size = new System.Drawing.Size(68, 17);
            this.labName.TabIndex = 0;
            this.labName.Text = "授权用户：";
            // 
            // gbFill
            // 
            this.gbFill.Controls.Add(this.gbWRR);
            this.gbFill.Controls.Add(this.gbRRLB);
            this.gbFill.Dock = System.Windows.Forms.DockStyle.Top;
            this.gbFill.Location = new System.Drawing.Point(0, 50);
            this.gbFill.Name = "gbFill";
            this.gbFill.Size = new System.Drawing.Size(684, 220);
            this.gbFill.TabIndex = 3;
            this.gbFill.TabStop = false;
            this.gbFill.Text = "提取信息";
            // 
            // gbWRR
            // 
            this.gbWRR.Controls.Add(this.btnwrrx64);
            this.gbWRR.Controls.Add(this.btnwrrx86);
            this.gbWRR.Controls.Add(this.tbwrrx64);
            this.gbWRR.Controls.Add(this.tbwrrx86);
            this.gbWRR.Controls.Add(this.labwrrx64);
            this.gbWRR.Controls.Add(this.labwrrx86);
            this.gbWRR.Dock = System.Windows.Forms.DockStyle.Top;
            this.gbWRR.Location = new System.Drawing.Point(3, 119);
            this.gbWRR.Name = "gbWRR";
            this.gbWRR.Size = new System.Drawing.Size(678, 100);
            this.gbWRR.TabIndex = 1;
            this.gbWRR.TabStop = false;
            this.gbWRR.Text = "商业发行版";
            // 
            // btnwrrx64
            // 
            this.btnwrrx64.Location = new System.Drawing.Point(618, 57);
            this.btnwrrx64.Name = "btnwrrx64";
            this.btnwrrx64.Size = new System.Drawing.Size(50, 23);
            this.btnwrrx64.TabIndex = 5;
            this.btnwrrx64.Text = "下载";
            this.btnwrrx64.UseVisualStyleBackColor = true;
            this.btnwrrx64.Click += new System.EventHandler(this.btnwrrx64_Click);
            // 
            // btnwrrx86
            // 
            this.btnwrrx86.Location = new System.Drawing.Point(618, 27);
            this.btnwrrx86.Name = "btnwrrx86";
            this.btnwrrx86.Size = new System.Drawing.Size(50, 23);
            this.btnwrrx86.TabIndex = 4;
            this.btnwrrx86.Text = "下载";
            this.btnwrrx86.UseVisualStyleBackColor = true;
            this.btnwrrx86.Click += new System.EventHandler(this.btnwrrx86_Click);
            // 
            // tbwrrx64
            // 
            this.tbwrrx64.Location = new System.Drawing.Point(68, 58);
            this.tbwrrx64.Name = "tbwrrx64";
            this.tbwrrx64.Size = new System.Drawing.Size(540, 23);
            this.tbwrrx64.TabIndex = 3;
            // 
            // tbwrrx86
            // 
            this.tbwrrx86.Location = new System.Drawing.Point(68, 28);
            this.tbwrrx86.Name = "tbwrrx86";
            this.tbwrrx86.Size = new System.Drawing.Size(540, 23);
            this.tbwrrx86.TabIndex = 2;
            // 
            // labwrrx64
            // 
            this.labwrrx64.AutoSize = true;
            this.labwrrx64.Location = new System.Drawing.Point(8, 61);
            this.labwrrx64.Name = "labwrrx64";
            this.labwrrx64.Size = new System.Drawing.Size(52, 17);
            this.labwrrx64.TabIndex = 1;
            this.labwrrx64.Text = "wrr-x64";
            // 
            // labwrrx86
            // 
            this.labwrrx86.AutoSize = true;
            this.labwrrx86.Location = new System.Drawing.Point(8, 31);
            this.labwrrx86.Name = "labwrrx86";
            this.labwrrx86.Size = new System.Drawing.Size(52, 17);
            this.labwrrx86.TabIndex = 0;
            this.labwrrx86.Text = "wrr-x86";
            // 
            // gbRRLB
            // 
            this.gbRRLB.Controls.Add(this.btnrrlbx64);
            this.gbRRLB.Controls.Add(this.btnrrlbx86);
            this.gbRRLB.Controls.Add(this.tbrrlbx64);
            this.gbRRLB.Controls.Add(this.tbrrlbx86);
            this.gbRRLB.Controls.Add(this.labrrlbx64);
            this.gbRRLB.Controls.Add(this.labrrlbx86);
            this.gbRRLB.Dock = System.Windows.Forms.DockStyle.Top;
            this.gbRRLB.Location = new System.Drawing.Point(3, 19);
            this.gbRRLB.Name = "gbRRLB";
            this.gbRRLB.Size = new System.Drawing.Size(678, 100);
            this.gbRRLB.TabIndex = 0;
            this.gbRRLB.TabStop = false;
            this.gbRRLB.Text = "实验室开发版";
            // 
            // btnrrlbx64
            // 
            this.btnrrlbx64.Location = new System.Drawing.Point(618, 57);
            this.btnrrlbx64.Name = "btnrrlbx64";
            this.btnrrlbx64.Size = new System.Drawing.Size(50, 23);
            this.btnrrlbx64.TabIndex = 5;
            this.btnrrlbx64.Text = "下载";
            this.btnrrlbx64.UseVisualStyleBackColor = true;
            this.btnrrlbx64.Click += new System.EventHandler(this.btnrrlbx64_Click);
            // 
            // btnrrlbx86
            // 
            this.btnrrlbx86.Location = new System.Drawing.Point(618, 27);
            this.btnrrlbx86.Name = "btnrrlbx86";
            this.btnrrlbx86.Size = new System.Drawing.Size(50, 23);
            this.btnrrlbx86.TabIndex = 4;
            this.btnrrlbx86.Text = "下载";
            this.btnrrlbx86.UseVisualStyleBackColor = true;
            this.btnrrlbx86.Click += new System.EventHandler(this.btnrrlbx86_Click);
            // 
            // tbrrlbx64
            // 
            this.tbrrlbx64.Location = new System.Drawing.Point(68, 58);
            this.tbrrlbx64.Name = "tbrrlbx64";
            this.tbrrlbx64.Size = new System.Drawing.Size(540, 23);
            this.tbrrlbx64.TabIndex = 3;
            // 
            // tbrrlbx86
            // 
            this.tbrrlbx86.Location = new System.Drawing.Point(68, 28);
            this.tbrrlbx86.Name = "tbrrlbx86";
            this.tbrrlbx86.Size = new System.Drawing.Size(540, 23);
            this.tbrrlbx86.TabIndex = 2;
            // 
            // labrrlbx64
            // 
            this.labrrlbx64.AutoSize = true;
            this.labrrlbx64.Location = new System.Drawing.Point(8, 61);
            this.labrrlbx64.Name = "labrrlbx64";
            this.labrrlbx64.Size = new System.Drawing.Size(54, 17);
            this.labrrlbx64.TabIndex = 1;
            this.labrrlbx64.Text = "rrlb-x64";
            // 
            // labrrlbx86
            // 
            this.labrrlbx86.AutoSize = true;
            this.labrrlbx86.Location = new System.Drawing.Point(8, 31);
            this.labrrlbx86.Name = "labrrlbx86";
            this.labrrlbx86.Size = new System.Drawing.Size(54, 17);
            this.labrrlbx86.TabIndex = 0;
            this.labrrlbx86.Text = "rrlb-x86";
            // 
            // frmExtractor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.ClientSize = new System.Drawing.Size(684, 371);
            this.Controls.Add(this.gbBottom);
            this.Controls.Add(this.gbFill);
            this.Controls.Add(this.gbTop);
            this.Font = new System.Drawing.Font("微软雅黑", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.MaximizeBox = false;
            this.Name = "frmExtractor";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "WinRAR 简体中文-商业版 提取器";
            this.Load += new System.EventHandler(this.frmExtractor_Load);
            this.gbTop.ResumeLayout(false);
            this.gbTop.PerformLayout();
            this.gbBottom.ResumeLayout(false);
            this.gbBottom.PerformLayout();
            this.gbFill.ResumeLayout(false);
            this.gbWRR.ResumeLayout(false);
            this.gbWRR.PerformLayout();
            this.gbRRLB.ResumeLayout(false);
            this.gbRRLB.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox gbTop;
        private System.Windows.Forms.Label labVersion;
        private System.Windows.Forms.Button btnRefresh;
        private System.Windows.Forms.GroupBox gbBottom;
        private System.Windows.Forms.GroupBox gbFill;
        private System.Windows.Forms.GroupBox gbWRR;
        private System.Windows.Forms.TextBox tbwrrx64;
        private System.Windows.Forms.TextBox tbwrrx86;
        private System.Windows.Forms.Label labwrrx64;
        private System.Windows.Forms.Label labwrrx86;
        private System.Windows.Forms.GroupBox gbRRLB;
        private System.Windows.Forms.TextBox tbrrlbx64;
        private System.Windows.Forms.TextBox tbrrlbx86;
        private System.Windows.Forms.Label labrrlbx64;
        private System.Windows.Forms.Label labrrlbx86;
        private System.Windows.Forms.Label labName;
        private System.Windows.Forms.Label labLicense;
        private System.Windows.Forms.TextBox tbLicense;
        private System.Windows.Forms.TextBox tbName;
        private System.Windows.Forms.Button btnGenerate;
        private System.Windows.Forms.Button btnrrlbx86;
        private System.Windows.Forms.Button btnwrrx64;
        private System.Windows.Forms.Button btnwrrx86;
        private System.Windows.Forms.Button btnrrlbx64;
    }
}

