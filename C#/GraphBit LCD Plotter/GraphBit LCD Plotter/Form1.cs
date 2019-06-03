﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.IO;
using System.IO.Ports;
using System.Windows.Forms;
using System.Diagnostics;


namespace GraphBit_LCD_Plotter
{
    public partial class Form1 : Form
    {
        Image target_image;
        Image target_gif;
        imageFile _Imagefile;
        Serial serial;
        Int16 StatusUpdate = 0;
        byte[] rawImage = null;  // to store the image as byte
        SaveFileDialog saveFile = new SaveFileDialog();

        private byte[] RawImg;
        public byte[] rawImg
        {
            get => RawImg;
            set => RawImg = value;
        }
        

        public Form1()
        {
            InitializeComponent();
            _Imagefile = new imageFile(this);
            serial = new Serial(this);
            
        }

        private void darkThemeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.BackColor = Color.FromArgb(45, 46, 48);
            lblHeight.ForeColor = Color.LightGray;
            lblWidth.ForeColor = Color.LightGray;
            rtbOutput.BackColor = Color.Black;
            rtbOutput.ForeColor = Color.DarkOrange;
        }

        private void peasantThemeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.BackColor = Color.FromArgb(239, 239, 239);
            lblHeight.ForeColor = Color.Black;
            lblWidth.ForeColor = Color.Black;
            rtbOutput.BackColor = Color.White;
            rtbOutput.ForeColor = Color.Black;
        }

        private void importToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //string filecontent = "";

            OpenFileDialog OpenFile1 = new OpenFileDialog(); // creating an openfile dialog object
            OpenFile1.Filter = "All Files(*.*)|*.*";    // We take every format of image
            OpenFile1.RestoreDirectory = true;
           
            if(OpenFile1.ShowDialog() == DialogResult.OK && OpenFile1.FileName.Length > 0)
            {
                _Imagefile.Filepath = OpenFile1.FileName;   /* Getting the path of the specified file */
                target_image = Image.FromFile(_Imagefile.Filepath); 
            }
        }

     

        private void btnClear_Click(object sender, EventArgs e)
        {
            rtbOutput.Text = "";
        }

        private void btnConvert_Click(object sender, EventArgs e)
        {
            if ((tbHeight.Text != "") && (tbWidth.Text != ""))
            {
                pictureBox1.Width = Convert.ToInt32(tbWidth.Text);
                pictureBox1.Height = Convert.ToInt32(tbHeight.Text);
                _Imagefile.Width = Convert.ToInt32(tbWidth.Text);
                _Imagefile.Height = Convert.ToInt32(tbHeight.Text);
                pictureBox1.Image = target_image;   // resized 
                _Imagefile.ConvertBMP();
               
            }
            else
            {
                DialogResult _dr = MessageBox.Show(" Please fill in the height and width fields with a numeric value","Empty Fields",MessageBoxButtons.OKCancel, MessageBoxIcon.Error);
                
            }
        }

        private void helpToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ProcessStartInfo sInfo = new ProcessStartInfo("https://charlesmatte03.wixsite.com/website");
            Process.Start(sInfo);
        }

        private void serialPortToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // Open serial form here
            
            serial.Show();
       
        }

        private void btnSend_Click(object sender, EventArgs e)
        {
            serialPort1.PortName = serial.cbbCOM.Text;
            serialPort1.BaudRate = Convert.ToInt32(serial.cbbBaudRate.Text);
            serialPort1.StopBits = StopBits.One;
            serialPort1.DataBits = Convert.ToInt32(serial.cbbData.Text);
            serialPort1.Handshake = Handshake.None;
            serialPort1.Parity = Parity.None;
            if (serialPort1.IsOpen == true)
            {
                MessageBox.Show("The port is already opened!");
            }
            else
            {
                serialPort1.Open();
            }
            serialPort1.Write(_Imagefile.SerialArray, 0, _Imagefile.SerialArray.Length); 
            serialPort1.Close();
            serial.Dispose(); 
        }

        private void importGifToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog OpenFile1 = new OpenFileDialog(); // creating an openfile dialog object
            OpenFile1.Filter = "All Files(*.*)|*.*";    // We take every format of image
            OpenFile1.RestoreDirectory = true;

            if (OpenFile1.ShowDialog() == DialogResult.OK && OpenFile1.FileName.Length > 0)
            {
                _Imagefile.Filepath = OpenFile1.FileName;   /* Getting the path of the specified file */
                target_gif = Image.FromFile(_Imagefile.Filepath);
            }
        }
    }
}
    
   
    

