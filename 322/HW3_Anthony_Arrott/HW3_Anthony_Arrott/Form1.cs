/***************************
 * Anthony Arrott 10853267 *
 * CPTS 322 w/ Evan Olds   *
 ***************************/
using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Numerics;      // added for BigInteger
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace HW3_Anthony_Arrott
{
    // main notepad class
    public partial class Notepad : Form
    {
        public Notepad()
        {
            InitializeComponent();
        }

        // Loads from a file, either all files or .txt
        private void loadFromFileToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var l = new OpenFileDialog();

            l.InitialDirectory = "C:\\"; // start at C:\
            l.Filter = "txt files (*.txt)|*.txt|All files(*.*)|*.*"; // file types
            l.DefaultExt = "*.txt"; // default to txt

            if (l.ShowDialog() == DialogResult.OK) // as long as it can show the dialog
            {
                TextReader tr = new StreamReader(l.FileName); // instantiate textreader
                load(tr); // load text in file
            }
        }

        // load puts all the text from the file into the textbox
        private void load (TextReader tr)
        {
            textBox1.Text = tr.ReadToEnd();
        }

        // loads 50 fibonacci numbers into text
        private void loadFibonacciNumbersfirst50ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var f = new FibonacciTextReader(50);
            load(f);
        }

        // loads 100 fibonacci numbers into text
        private void loadFibonacciNumbersfirst100ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var f = new FibonacciTextReader(100);
            load(f);
        }

        // save to a new file
        private void saveToFileToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var save = new SaveFileDialog(); // instanciate save

            save.InitialDirectory = "C:\\"; // set default dir to C:\
            save.Filter = "txt files (*.txt) |*.txt|All files(*.*)|*.*"; // file types
            save.DefaultExt = ".txt"; // default to .txt

            if ( save.ShowDialog() == DialogResult.OK ) // if the dialog box opens
            {
                System.IO.File.WriteAllText(save.FileName, textBox1.Text); // write all text in textbox to FileName given by user to new file
            }
            
        }
    }

    // FibonacciTextReader Class, inherits from textreader
    public class FibonacciTextReader : TextReader
    {
        /* properties */
        private int maxLines; // max num to get fib
        private int curNum; // curnum of fib
        
        public FibonacciTextReader(int val)
        {
            maxLines = val;     // when created set it to either 50/100
            curNum = 0;         // initialize curNum to 0 for fib sequence
        }

        // override the ReadLine function to create fibonnaci sequence
        public override string ReadLine()
        {
            var fibPrev = new BigInteger(); // prev fib number to be added to fib for next number 
            var fib = new BigInteger(); // current fib number (to be printed
            var temp = new BigInteger();
            int i = 2; // since both are at 1 start i at 2

            fibPrev = 1; // start both at 1
            fib = 1;

            if (curNum <= 1) { return curNum.ToString(); } // print 0, 1, 1 to start
            else
            {
                while (i < curNum) // add to current fib number case (curNum)
                {
                    temp = fib; // save current fibonacci number
                    fib += fibPrev; // add previous number to next number
                    fibPrev = temp; // set previous number to last number
                    i++; // increment i
                }
            }

            return fib.ToString(); // return number as a string
        }

        // override ReadToEnd() to print up to MaxLines of fib sequences
        public override string ReadToEnd()
        {
            var s = new StringBuilder(); // need to print into textbox

            while (curNum < maxLines) // save each case as a string with a newline
            {
                s.AppendLine((curNum + 1) + ": " + ReadLine()); // curNum + 1 to increase human readability
                curNum++;
            }

            return s.ToString(); // set value of FibonacciTextReader to all the concatenated text
        }
    }
}
