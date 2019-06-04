namespace GraphBit_LCD_Plotter
{
    partial class Serial
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Serial));
            this.cbbCOM = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.cbbBaudRate = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.btnOk = new System.Windows.Forms.Button();
            this.cbbData = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.cbbStop = new System.Windows.Forms.ComboBox();
            this.label4 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // cbbCOM
            // 
            this.cbbCOM.FormattingEnabled = true;
            this.cbbCOM.Location = new System.Drawing.Point(85, 35);
            this.cbbCOM.Name = "cbbCOM";
            this.cbbCOM.Size = new System.Drawing.Size(76, 21);
            this.cbbCOM.TabIndex = 0;
            this.cbbCOM.SelectedIndexChanged += new System.EventHandler(this.cbbCOM_SelectedIndexChanged);
            this.cbbCOM.Click += new System.EventHandler(this.cbbCOM_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(86, 19);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(31, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "COM";
            // 
            // cbbBaudRate
            // 
            this.cbbBaudRate.AccessibleRole = System.Windows.Forms.AccessibleRole.None;
            this.cbbBaudRate.AutoCompleteCustomSource.AddRange(new string[] {
            "9600",
            "19200",
            "38400",
            "57600",
            "115200",
            "230400",
            "460800",
            "921600"});
            this.cbbBaudRate.FormattingEnabled = true;
            this.cbbBaudRate.Items.AddRange(new object[] {
            "9600",
            "19200",
            "38400",
            "57600",
            "115200",
            "230400",
            "460800",
            "921600"});
            this.cbbBaudRate.Location = new System.Drawing.Point(85, 78);
            this.cbbBaudRate.Name = "cbbBaudRate";
            this.cbbBaudRate.Size = new System.Drawing.Size(76, 21);
            this.cbbBaudRate.TabIndex = 2;
            this.cbbBaudRate.Text = "921600";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(85, 63);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(61, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "Baud Rate:";
            // 
            // btnOk
            // 
            this.btnOk.Location = new System.Drawing.Point(201, 93);
            this.btnOk.Name = "btnOk";
            this.btnOk.Size = new System.Drawing.Size(75, 23);
            this.btnOk.TabIndex = 4;
            this.btnOk.Text = "OK";
            this.btnOk.UseVisualStyleBackColor = true;
            this.btnOk.Click += new System.EventHandler(this.btnOk_Click);
            // 
            // cbbData
            // 
            this.cbbData.FormattingEnabled = true;
            this.cbbData.Items.AddRange(new object[] {
            "8"});
            this.cbbData.Location = new System.Drawing.Point(85, 118);
            this.cbbData.Name = "cbbData";
            this.cbbData.Size = new System.Drawing.Size(76, 21);
            this.cbbData.TabIndex = 6;
            this.cbbData.Text = "8";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(85, 103);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(30, 13);
            this.label3.TabIndex = 7;
            this.label3.Text = "Data";
            // 
            // cbbStop
            // 
            this.cbbStop.FormattingEnabled = true;
            this.cbbStop.Items.AddRange(new object[] {
            "0",
            "1",
            "2"});
            this.cbbStop.Location = new System.Drawing.Point(88, 158);
            this.cbbStop.Name = "cbbStop";
            this.cbbStop.Size = new System.Drawing.Size(73, 21);
            this.cbbStop.TabIndex = 8;
            this.cbbStop.Text = "1";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(88, 143);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(29, 13);
            this.label4.TabIndex = 9;
            this.label4.Text = "Stop";
            // 
            // Serial
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(349, 250);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.cbbStop);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.cbbData);
            this.Controls.Add(this.btnOk);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.cbbBaudRate);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.cbbCOM);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Serial";
            this.RightToLeftLayout = true;
            this.Text = "Serial";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Serial_FormClosing);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        public System.Windows.Forms.ComboBox cbbCOM;
        public System.Windows.Forms.Label label1;
        public System.Windows.Forms.ComboBox cbbBaudRate;
        public System.Windows.Forms.Label label2;
        public System.Windows.Forms.Button btnOk;
        public System.Windows.Forms.ComboBox cbbData;
        public System.Windows.Forms.Label label3;
        public System.Windows.Forms.ComboBox cbbStop;
        public System.Windows.Forms.Label label4;
    }
}