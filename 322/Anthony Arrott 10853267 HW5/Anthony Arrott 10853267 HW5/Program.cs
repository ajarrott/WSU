// this is a program that creates an expression tree based off of the
// user's input 

/***************************
 * Anthony Arrott 10853267 *
 *  CptS 322 w/ Evan Olds  * 
 *        Homework 5       *
 ***************************/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Anthony_Arrott_10853267_HW5
{
    class Program
    {
        // pretty much does nothing but instantiate and run the menu
        static void Main(string[] args)
        {
            Expression myExp = new Expression();

            myExp.menu();
        }
    }
}
