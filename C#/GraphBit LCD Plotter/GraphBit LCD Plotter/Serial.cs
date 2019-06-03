using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;

namespace GraphBit_LCD_Plotter
{
    public partial class Serial : Form
    {
        string[] portList = SerialPort.GetPortNames();
        string portSelected = "";
        int baudSelected = 0, dataSelected = 0,StopSelected = 0,i = 0;
        SerialPort serial = new SerialPort();
        
        Form1 parent;

        public Serial(Form1 Parent)
        {
            parent = Parent;
            InitializeComponent();
        }

        public Serial()
        {
            InitializeComponent();
        }

        private void cbbCOM_SelectedIndexChanged(object sender, EventArgs e)
        {
            portSelected = cbbCOM.Text;
        }

        private void Serial_FormClosing(object sender, FormClosingEventArgs e)
        {
            e.Cancel = true;
            this.Hide();
        }

        private void cbbCOM_Click(object sender, EventArgs e)
        {
            get_ports();
        }

        private void btnOk_Click(object sender, EventArgs e)
        {
            this.Hide();
        }

     

     private void get_ports()
        {
            portList = SerialPort.GetPortNames();
            try
            {
                foreach (string s in portList)
                {
                    cbbCOM.Items.Add(portList[i]);
                    i++;
                }
            }
            catch { }
        }
    }
}
