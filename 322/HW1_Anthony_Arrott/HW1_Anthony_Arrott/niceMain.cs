/*********Homework 1**********
 *  Anthony Arrott 10853267  *
 *  CptS 322 with Evan Olds  *
 *****************************/
// Automatically added libraries
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HW1_Anthony_Arrott
{
    class niceMain
    {
        //run main function;
        public void runMain()
        {
            // instantiate myBST class
            myBST t = new myBST();

            // set delimiter to a space
            char[] delim = { ' ' };
            // initialize a string
            string tree;
            // menu
            Console.Write("Please enter numbers (0 - 100) delimited with space to be added to a tree:\n");
            // read lines from user
            tree = Console.ReadLine();

            // as long as input is given from user split items
            if (tree != "")
            {
                // we assume the user knows to delimit the characters with spaces so no other functionality added
                // split user information into a string array
                string[] numbers = tree.Split(delim);
                
                // create an integer array with up to 128 spaces in case of a lot of duplicate numbers
                int[] toAdd = new int[128];

                // useful variables
                int i = 0;
                int end = 0;
                int temp;

                foreach (string s in numbers)
                {
                    // Convert string into integers
                    temp = Convert.ToInt32(s);
                    // make sure user stayed within the specified range
                    if (temp < 0 || temp > 100)
                    {
                        // For Debug
                        // Console.Write("Failed to add " + temp + " not in range of 0-100.\n");

                        // Do nothing since we do not want to add numbers to tree
                    }
                    else
                    {
                        // populate array with numbers to add to tree
                        toAdd[i] = temp;
                        i++;
                        end++;  // get end of array length
                    }

                }

                // reset i to 0
                i = 0;

                // add all of the numbers to the BST
                while (i < end)
                {
                    t.add(toAdd[i]);
                    i++;
                }

                // print statistics of the tree
                t.stats();

                // readline so the user can see the output
                Console.Write("\nPress enter to exit the program.\n");
                Console.ReadLine();
            }
            else
            {
                // print statistics of the empty tree
                t.stats();

                // readline so the user can see the output
                Console.Write("Press enter to exit the program.\n");
                Console.ReadLine();
            }
        }
    }
}
