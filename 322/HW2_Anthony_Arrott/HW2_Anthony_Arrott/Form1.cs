/*Anthony Arrott 10853267 HW2*/

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace HW2_Anthony_Arrott
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent(); // autopopulated
        }

        // I assumed unique integers meant any one value of a number is counted,
        // if there were duplicates, those duplicates were uncounted.

        // when the object loads build list, run functions, and print text
        private void Form1_Load(object sender, EventArgs e)
        {
            // once the form has been created make a list with ints
            List<int> l = new List<int>();
            StringBuilder s = new StringBuilder(); // initiialize a string
            mkList(l); //populate list

            // call functions
            hashSet(l, s);
            noSpace(l, s);
            sort(l, s);
            textBox1.Text = s.ToString(); // print information to textbox
        }

        // Populates list with random numbers
        private void mkList (List<int> L)
        {
            Random rnd = new Random(); // create random class
            int i = 0;
            while (i < 10000) // only populate 0 - 9999 in the list
            {
                L.Add(rnd.Next(20000)); // add a random number from 0 - 20000
                i++;
            }      
        }

        // find how many unique items are in a list using HashSet
        private void hashSet ( List<int> L, StringBuilder S )
        {
            HashSet<int> h = new HashSet<int>(); // initialize hashset h ( O(1) )
            int i = 0; // O(1)
            while (i <= 20000) // from 0 - 20000  ( O(n) )
            {
                if ( L.Contains(i) ) // if the list has the number i ( O(1) )
                {
                    h.Add(i); // and the hashset does not contain i, add i ( O(m)
                }

                i++; // O ( 1 )
            }

            // print information
            S.Append("1. HashSet method: ");
            S.Append(h.Count() + " "); // h.Count() gives us the number of items in h
            S.Append("unique numbers");
            S.AppendLine(); // newline
            S.Append("Time Complexity: O(n)");
            S.AppendLine();
            // used serveral lines for ease of reading/editing in visual studio
            S.Append("This function goes through list once and adds only unique numbers. ");
            S.Append("L.Contains(T) is of O(n) time complexity according to MSDN.  The time complexity ");
            S.Append("depends on which is larger, the amount of items in the list, or the range of ");
            S.Append("of random numbers.  If they were both equal then it would be O(n^2).  Since they are ");
            S.Append("not we take the larger of the two and select that for the time complexity.  In this case ");
            S.Append("this is the range of the random numbers.");
            S.AppendLine(); // adds a newline
        }

        // does not allocate extra space to find amount of unique items
        private void noSpace ( List<int> L, StringBuilder S )
        {
            int i = 0,  unq = 0;     // unq is the unique number counter

            //Time Complexity: O(n)
            while (i <= 20000) // O(n)
            {
                if (L.Contains(i)) { unq++; } // O(m), described above, same theory
                i++;
            }

            S.Append("2. O(1) Storage Method: " + unq + " unique numbers.");
            S.AppendLine(); // adds a newline
        }

        // sorts the list then compares index i to index i + 1, if it changes then it increments unq
        private void sort ( List<int> L, StringBuilder S )
        {
            L.Sort();
            int i = 0, check = 0, unq = 1; // unq is the unique number counter, starts at 1 because the first number will be unique

            while ( i < 9999 ) // end at 9998 because my check will check at 9999
            {
                check = L.ElementAt(i);     // set check to current index value
                if (check != L.ElementAt(i + 1)) { unq++; }  // when the next number changes increment unq
                
                i++;
            }

            S.Append("3. Sorted Method: " + unq + " unique numbers."); // print useful information
            S.AppendLine(); // appends a newline
        }
    }

  
}
