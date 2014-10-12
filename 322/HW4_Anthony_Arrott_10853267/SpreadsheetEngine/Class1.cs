/*******************************
 * Anthony Arrott 10853267 HW4 *
 *      SpreadsheetEngine      *
 *******************************/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;


//Main functionality for speadsheet, 
namespace SpreadsheetEngine
{
    //When Cell class text is changed then the PropertyChanged event is fired
    public abstract class Cell : INotifyPropertyChanged 
    {
        // properties
        protected int RowIndex;
        protected int ColumnIndex;
        protected string Text;
        protected string Value;

        // event handler
        public event PropertyChangedEventHandler PropertyChanged = delegate { };

        // constructor
        public Cell(int row, int column)
        {
            RowIndex = row;
            ColumnIndex = column;
        }

        // Text setter/getter
        public string CellString
        {
            get { return this.Text; }
            set
            {
                if ( this.Text != value ) 
                {
                    if (value[0] == '=') // equation
                    {
                        this.Value = value;
                        PropertyChanged(this, new PropertyChangedEventArgs("Text")); // change value equation, fire event "Value"
                    }
                    else // string
                    {
                        this.Text = value;
                        PropertyChanged(this, new PropertyChangedEventArgs("Text")); // fire propertychanged event "Text"
                    }
                }
            }
        }

        public string GetValue
        {
            get { return this.Value; }
        }
        // row index getter
        public int Row
        {
            get { return RowIndex; }
        }

        // column index getter
        public int Column
        {
            get { return ColumnIndex; }
        }
    }
    
    // spreadsheet class, creates cells and updates their text randomly
    public class Spreadsheet
    {
        // private SpeadsheetCell inherits from Cell so we can instatiate the abstract Cell class
        private class SpreadsheetCell : Cell
        {
            // constructor inherits the base constructor
            public SpreadsheetCell(int row, int col) : base(row, col) { }
        }

        // Properties
        private SpreadsheetCell[,] ss;  // spreadsheet as a 2d array
        private int RowCount;           // total rows
        private int ColumnCount;        // total columns
        public event PropertyChangedEventHandler CellPropertyChanged = delegate { };

        // spreadsheet constructor
        public Spreadsheet(int rows, int columns)
        {
            RowCount = rows;
            ColumnCount = columns;

            ss = new SpreadsheetCell[rows, columns]; // create new spreadsheet with size of rows by columns

            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < columns; j++)
                {
                    ss[i, j] = new SpreadsheetCell(i,j);               // create cell
                    ss[i, j].PropertyChanged += CellPropertyChanged;   // subscribe cell event handler to CellPropertyChanged
                                                                       // so proper eventfiring can happen
                }
            }
        }

        // this function finds if the designated cell if it has an = operator in the front
        // e.g. "=B2" would give you B2's CellString in the designated cell
        internal protected void setEq(int row, int column)
        {
            int r, c; // row, column
            string cellvalue = ss[row, column].GetValue;

            // if CellValue is not null (CellValue always starts with =)
            if (cellvalue != null)
            {
                if (cellvalue.Length > 1) // Letters are columns, needs to be > 1 (0 is =)
                {
                    c = (int)(cellvalue[1] - 'A'); // A - A would be 0, B - A would be 1, etc
                }
                else return;

                // get row
                if (cellvalue.Length > 2) // numbers are rows, needs to have length of 3 to have a number
                {
                    r = Convert.ToInt32(cellvalue.Substring(2)) - 1; // -1 for 0 based indexing
                }
                else return;

                if (ss[r, c] != null) // make sure cell exists
                {
                    ss[row, column].CellString = ss[r, c].CellString; // change designated CellString to found CellString
                }
            }
        }

        // set CellString
        public void SetCell(int row, int column, string s)
        {
            // make sure cell exists
            if (ss[row, column] != null)
            {
                ss[row, column].CellString = s; // set cell to string

                setEq(row, column); // check if the CellValue exists return false means no event fired
                CellPropertyChanged(row, new PropertyChangedEventArgs("Text")); // fire event, send row number to event handler
            }
        }

        // returns designated cell's CellString
        public string GetCell(int row, int column)
        {
            if (ss[row, column] != null) { return ss[row, column].CellString; }
            else return null;
        }

        // returns RowCount
        public int GetRows
        {
            get { return this.RowCount; }
        }

        // returns ColumnCount
        public int GetColumns
        {
            get { return this.ColumnCount; }
        }
    }
}
