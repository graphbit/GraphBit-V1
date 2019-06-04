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
        int baudSelected = 0;
        int dataSelected = 0;
        int StopSelected = 0;
        int i = 0;
        public bool portOpened = false;
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
            parent.serialPort1.PortName = cbbCOM.Text;
            parent.serialPort1.BaudRate = Convert.ToInt32(cbbBaudRate.Text);
            parent.serialPort1.StopBits = StopBits.One;
            parent.serialPort1.DataBits = Convert.ToInt32(cbbData.Text);
            parent.serialPort1.Handshake = Handshake.None;
            parent.serialPort1.Parity = Parity.None;
            if(parent.serialPort1.IsOpen == true)
            {
                MessageBox.Show("The port is already opened!");
            }
            else
            {
                parent.serialPort1.Open();
                portOpened = true;
            }
            parent.serialPort1.Write("\r");

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
