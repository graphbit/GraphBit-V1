using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace GraphBit_LCD_Plotter
{
    public partial class ProgressBar : Form
    {
        Form1 parent;
        public ProgressBar(Form1 Parent)
        {
            parent = Parent;
            InitializeComponent();
        }
    

    }
}
