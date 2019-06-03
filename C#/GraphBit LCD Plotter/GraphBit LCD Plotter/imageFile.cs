/* @File : imageFile class this class ihnerite the attributes of the form1
 *
 * @ Created : May 2019 
 * 
 * @ Engineers: Charles Lévesque-Matte & Pascal-Emmanuel Lachance
 * 
 * @ Company : GraphBit inc. 
 * 
 */
using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.IO;
using System.Windows.Forms;
using System.Threading;
using System.Text;


namespace GraphBit_LCD_Plotter
{
    internal class imageFile
    {
        public
        Bitmap Resized;
        byte[] RawImg;
        string ImgName = "";
        
        SaveFileDialog sfd;
        OpenFileDialog _ofd = new OpenFileDialog();
        Form1 parent;
        

        public imageFile()
        {

        }
        public imageFile(Form1 anything)
        {
            parent = anything;
        }

        private Int16 progress;
        public Int16 Progress
        {
            get => progress;
            set => progress = value;
        }

        /* Access modifier for the path of the image*/
        private string filepath;
        public string Filepath
        {
            get => filepath;
            set => filepath = value;
        }

        private byte[] serialArray;
        public byte[] SerialArray
        {
            get => serialArray;
            set => serialArray = value;
        }

        /* Access modifier for the width  of the lcd*/
        private Int32 width;
        public Int32 Width
        {
            get => width;
            set => width = value;
        }
        /* Access modifiers of the height of the LCD */
        private Int32 height;
        public Int32 Height
        {
            get => height;
            set => height = value;
        }
        /// <summary>
        /// Taking any image file to convert it into a bmp file 
        /// </summary>
        public void ConvertBMP()
        {
            _ofd.Filter = "All Files(*.*)|*.*";    // We take every format of image
            _ofd.RestoreDirectory = true;

            if (filepath == null)
            {
                DialogResult result = MessageBox.Show("You need to import an image!", "Empty file", MessageBoxButtons.OKCancel, MessageBoxIcon.Hand);
                if (result == DialogResult.OK)  /* If the user press ok */
                {
                    if (_ofd.ShowDialog() == DialogResult.OK && _ofd.FileName.Length > 0)
                    {
                        filepath = _ofd.FileName;
                        ImgName = Path.GetFileName(filepath);
                    }
                }
            }
            else
            {
                Bitmap ogBmp = (Bitmap)Image.FromFile(Filepath);   // Image path from form1
                Resized = new Bitmap(ogBmp, new Size(width, height));   // parameters set by the user in form1
                BlackNWhite(Resized);

            }

        }
        /// <summary>
        ///  Converting a bitmap to a real black and white image
        ///  and keeping the metadat of the image
        /// </summary>
        public void BlackNWhite(Bitmap frame)
        {

            // Variable for image brightness
            double avgBright = 0;
            for (int y = 0; y < frame.Height; y++)
            {
                for (int x = 0; x < frame.Width; x++)
                {
                    // Get the brightness of this pixel
                    avgBright += frame.GetPixel(x, y).GetBrightness();
                }
            }

            // Get the average brightness and limit it's min / max
            avgBright = avgBright / (frame.Width * frame.Height);
            avgBright = avgBright < .3 ? .3 : avgBright;
            avgBright = avgBright > .7 ? .7 : avgBright;

            // Convert image to black and white based on average brightness
            for (int y = 0; y < frame.Height; y++)
            {
                for (int x = 0; x < frame.Width; x++)
                {
                    // Set this pixel to black or white based on threshold
                    if (frame.GetPixel(x, y).GetBrightness() > avgBright) frame.SetPixel(x, y, Color.White);
                    else frame.SetPixel(x, y, Color.Black);
                }
            }
            //frame.Save(Filepath + "blacknWhite", ImageFormat.Bmp);
            BMP2Raw(frame);
        }

        /// <summary>
        /// This function is taking down a bitmap image to a raw file 
        /// which contain the data and the metadata.
        /// </summary>
        public void BMP2Raw(Bitmap frame)
        {
            byte[] RGB = new byte[Height * Width / 8];
            serialArray = new byte[Height * Width / 8];
            int size = (Height * Width / 8);
            byte[] cArray = new byte[((6 * size) + ("static const unsigned char").Length + ImgName.Length + size.ToString().Length + (" [] = {};").Length + 1)];
            string Header = ("Static const unsigned char " + ImgName + " [" + size.ToString() + "] = {");
           
            for(int i = 0; i < Header.Length; i++)
            {
                cArray[i] = Convert.ToByte(Header[i]);
            }

            for (int i = 0; i < Height; i++)
            {
                for (int j = 0; j < Width; j++)
                {
                    RGB[(Width / 8 * i) + (j / 8)] |= (byte)(((frame.GetPixel(j, i).R > 0x80) ? 0x00 : 0x01) << (7 - (j % 8)));
                    progress++;
                }

            }
            /* For the serial port we don't need the array declaration */
         
            serialArray = arrayGenerator(RGB, RGB.Length, 2);
            /* end of serial send */

            byte[] tempChar = new byte[6];
            int index = Header.Length;
            
            /* Creating a compatible c array */
            for (int i = 0; index < ((RGB.Length * 6) + Header.Length); index += 6, i++)
            {
                tempChar = Encoding.ASCII.GetBytes("0x" + RGB[i].ToString("X2") + ", ");
                for (int j = 0; j < tempChar.Length; j++)
                {
                    cArray[index + j] = tempChar[j];  
                }
            }
            tempChar = Encoding.ASCII.GetBytes("};");
            cArray[cArray.Length - 4] = tempChar[0];
            cArray[cArray.Length - 3] = tempChar[1];
//            char[] tempString = new char[cArray.Length];
// tempString = (Encoding.Convert(Encoding.ASCII, Encoding.Unicode,cArray, 0, cArray.Length));
            ASCIIEncoding ASCII = new ASCIIEncoding();
            parent.rtbOutput.Text = ASCII.GetString(cArray, 0, cArray.Length);

        }

        public Image[] getFrames(Image originalImg)
        {
            int numberOfFrames = originalImg.GetFrameCount(FrameDimension.Time);
            Image[] frames = new Image[numberOfFrames];

            for (int i = 0; i < numberOfFrames; i++)
            {
                originalImg.SelectActiveFrame(FrameDimension.Time, i);
                frames[i] = ((Image)originalImg.Clone());
            }

            return frames;
        }

        public byte[] arrayGenerator(byte[] byteArray, int size, int mode)
        {
            int sizeadd;
            if (mode == 1)
            {
                sizeadd = 6;
            }
            else if (mode == 2)
            {
                sizeadd = 4;
            }
            else
            {
                return null;
            }
            byte[] finalByte = new byte[size + sizeadd];
            finalByte[0] = (byte)(((Convert.ToInt16(size) + sizeadd) & 0xFF00) >> 8);
            finalByte[1] = (byte)((Convert.ToInt16(size) + sizeadd) & 0x000000FF);
            finalByte[2] = Convert.ToByte(mode);
            if (mode == 1)
            {
                finalByte[3] = 0x01;
                finalByte[4] = 0x01;
            }
            int i;
            byte Checksum = 0x00;
            for (i = (sizeadd - 1); i < size + (sizeadd - 1); i++)
            {
                finalByte[i] = byteArray[i - (sizeadd - 1)];
                Checksum += finalByte[i];
            }
            finalByte[i] = Checksum;
            return finalByte;
        }
    }
}

      
