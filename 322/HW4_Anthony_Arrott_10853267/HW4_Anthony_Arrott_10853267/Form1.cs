/*******************************
 * Anthony Arrott 10853267 HW4 *
 *            Form1            *
 *******************************/
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using SpreadsheetEngine;

namespace HW4_Anthony_Arrott_10853267
{
    public partial class Form1 : Form
    {
        Spreadsheet spreadsheet; // where the info in the cells is stored

        public Form1()
        {
            InitializeComponent();

            makeCells(dataGridView1, 50, 26); // make the dataGridView 50 rows and 26 columns

            // initialize spreadsheet to dataGridView Rows and Columns count.
            spreadsheet = new Spreadsheet(dataGridView1.Rows.Count, dataGridView1.Columns.Count); 
            spreadsheet.CellPropertyChanged += spreadsheet_PropertyChanged; // subscribe to spreadsheet property changed evenet
        }

        // when the spreadsheet property is changed
        private void spreadsheet_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            // if type "Text", then the sender will return a row number, so you only update the columns in that row
            if ( e.PropertyName == "Text") 
            {
                int r = (int)sender; // cast the sender as an int just in case (r is for row)
                int c = 0;           // set c to 0 (c is for column)

                // set row[r].Cell[c] to CellString
                while (c < 26)
                {
                    dataGridView1.Rows[r].Cells[c].Value = spreadsheet.GetCell(r, c);
                    c++;
                }
            }
        }

        // populates datagridview ss (spreadsheet) with cc (columncount) and rc (rowcount) 
        private void makeCells(DataGridView ss, int rc, int cc)
        {
            int i = 0;
            ss.ColumnCount = cc;
            while (i < ss.ColumnCount) // set each value to A, B, C ... etc. need to cast as char to get correct letter
                ss.Columns[i].Name = ((char)('A' + i++)).ToString(); 

            i = 0; // set i back to zero
            while (i < rc) // populate rows
            {
                ss.Rows.Add(); // add row
                ss.Rows[i].HeaderCell.Value = (i + 1).ToString(); // add headercell value, user sees 1 based indexing
                i++;
            }
        }

        // when the button is clicked it populates the spreadsheet with random hello worlds
        // then sets column B# to "This is Column B#", then sets column A to "=B#"
        private void button1_Click(object sender, EventArgs e)
        {
            int rRow; // rRow (random row)
            int rCol; // rCol (random column)
            int i = 0;
            string s = "Hello World!";

            System.Random randomNum = new Random();

            while (i < 50)
            {
                rRow = randomNum.Next(50); // get random number up to 49
                rCol = randomNum.Next(26); // get random number up to 25

                spreadsheet.SetCell(rRow, rCol, s); // SetCell to s
                i++;
            }

            // do in this order to ensure "This is column B#" overwrites "Hello World!"
            i = 0;
            while(i < 50)
            {
                s = 'B' + (i + 1).ToString(); // ( string is B# with 1 based indexing ) e.g. B1
                spreadsheet.SetCell(i, 1, "This is cell " + s); // set column 1 (B) to "This is cell B#"
                spreadsheet.SetCell(i, 0, "=" + s); // set column 0 (A) to "=B#"
                i++;
            }
        }
    }
}
