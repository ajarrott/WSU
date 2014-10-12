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
using System.Text.RegularExpressions; // for variable checking

namespace Anthony_Arrott_10853267_HW5
{
    class Expression
    {
        private string m_exp; // to be turned into a tree
        private Node m_root;  // the tree
        private static Dictionary<string, double> m_dict;  // dictionary of variable values

        // default constructor
        /*public Expression()
        {
            m_exp = null;
            m_root = null;
            m_dict = new Dictionary<string, double>();
        }*/

        // base class
        private class Node
        {
            
        }

        // always a child node
        private class cNode : Node
        {
            private double m_val;

            public cNode( double d )
            {
                m_val = d;
            }
            public double getVal()
            {
                return m_val;
            }
        }

        // always a child node
        private class varNode : Node
        {
            private string m_val;

            public varNode( string s )
            {
                m_val = s;
            }
            public string getVal()
            {
                return m_val;
            }
        }

        // can have children nodes
        private class opNode : Node
        {
            private char m_val;
            private Node m_left;
            private Node m_right;

            // main value constructor
            public opNode(char op)
            {
                m_val = op;
            }

            // Left node constructor
            public Node Left
            {
                get
                {
                    return m_left;
                }
                set
                {
                    m_left = value;
                }
            }

            // Right node constructor
            public Node Right
            {
                get
                {
                    return m_right;
                }
                set
                {
                    m_right = value;
                }
            }

            // evaluates the expression
            public double Eval(double a, double b)
            {
                switch(m_val)
                {
                    case '+':
                        return a + b;
                    case '-':
                        return a - b;
                    case '*':
                        return a * b;
                    case '/':
                        if (b == 0)
                        {
                            Console.WriteLine("cannot divide by 0, returning 0");
                            return 0;
                        }
                        return a / b;
                    case '^':
                        return Math.Pow(a, b);
                    default:
                        return 0;
                }
            }
        }

        // set the expression string
        private void setExp()
        {
            Console.WriteLine("Enter the new expression: ");
            string s = Console.ReadLine();
            m_exp = s;
            m_root = MakeTree(s); // make the tree based off the of the users expression
        }

        // print menu for user in the console
        public void menu()
        {
            int c; // for switch case
            string s; // check users string

            while (true)
            {
                Console.WriteLine("Current Expression is: " + this.m_exp);
                Console.WriteLine("1 = Enter a new expression");
                Console.WriteLine("2 = Set a variable value");
                Console.WriteLine("3 = Evaluate tree");
                Console.WriteLine("0 = Quit");

                s = Console.ReadLine(); // read input into s

                // if greater than 1 invalid input
                if (s.Length > 1 )
                {
                    Console.WriteLine("Please input a valid option");
                    continue;
                }

                c = Convert.ToInt32(s); // convert users input to an integer
                switch (c)
                {
                    case 0:
                        return; // quit case
                    case 1:
                        setExp(); // set expression case
                        break;
                    case 2:
                        SetVar(); // set variable case
                        break;
                    case 3:
                        Console.WriteLine("Evaluates to: {0}", Evaluate()); // evaluate and prints the value of the tree
                        break;
                    default: // otherwise not valid
                        Console.WriteLine(c + " is not a valid command, please enter a new one");
                        break;
                }
            }
            
        }

        // recursively makes the tree based on the current string and operator
        // nothing but valid operators should be sent to this function
        private static Node MakeTree(string s, char op)
        {
            int pcounter = 0; // parenthesis counter

            for (int i = s.Length - 1; i >= 0; i--) // 0 based indexing, start from right of the string
            {
                // keep count of total parentesis open and closed
                if (')' == s[i]) // check if closing parenthesis
                    pcounter++;
                else if ('(' == s[i]) // check for open parenthesis
                    pcounter--;

                // no mismatching parenthesis set op to a valid op, do not want to set parenthesis to opnode
                if (0 == pcounter && op == s[i])
                {
                    opNode on = new opNode(s[i]); // make a new opnode
                    on.Left = MakeTree(s.Substring(0, i)); // make left subtree
                    on.Right = MakeTree(s.Substring(i + 1)); // make right subtree
                    return on;
                }
            }

            if (pcounter != 0) // no matching parenthesis
            {
                throw new Exception();
            }

            return null; // no node created
        }

        // make tree based off of the current string
        private static Node MakeTree( string s )
        {
            if (string.IsNullOrEmpty(s)) // no string, return no node
                return null;

            // get rid of parenthesis
            if( '(' == s[0] )
            {
                int counter = 0;
                for( int i = 1; i < s.Length; i++) // check for other parenthesis
                {
                    if ( '(' == s[i] )
                    {
                        counter++;
                    }
                    else if ( ')' == s[i] )
                    {
                        counter--;
                        if(0 == counter) // send items inside parenthesis for current string
                        {
                            // not last char
                            if (i != s.Length - 1)
                            {
                                break;
                            }
                            else
                            {
                                //make tree again ( based off the stringwithout outside parenthesis)
                                return MakeTree(s.Substring(1, s.Length - 2));
                            }
                        }
                    }
                    // make tree again (based off the string without outside parenthesis)
                    if (')' == s[s.Length - 1])
                    {
                        s = s.Substring(1, s.Length - 2);
                    }
                }
            }
            // valid op characters in reverse PEMDAS order
            char[] ops = { '+', '-', '*', '/', '^' }; 

            //send the string to search for each valid operator allowed
            foreach(char op in ops)
            {
                Node n = MakeTree(s, op); // make the tree based off the current operator
                if (n != null) return n; // if a node was returned, return that node to the calling function
            }

            double num; // try to make the string as a value
            if (double.TryParse(s, out num)) // if it works
            {
                return new cNode(num); // return a cNode (constant node)
            }
            else
            {
                return MakeVarNode(s); // otherwise it has letters so make a variable node
            }
        }

        // public evaluate function
        public double Evaluate()
        {
            // evaluates the tree based on the root node
            return EvalTree(m_root);
        }

        // evaluate the tree recursively
        private double EvalTree(Node n)
        {
            if ( n == null)
            {
                Console.WriteLine("No expression entered.");
                return 0;
            }
            else
            {
                if ( n is cNode ) // tree only has one const node
                {
                    cNode n1 = (cNode) n;
                    return n1.getVal();
                    
                }
                else if ( n is varNode ) // tree has one variable node
                {
                    varNode n2 = (varNode) n;
                    return m_dict[n2.getVal()]; // return value assigned by user in the dictionary
                }
                else // m_root is a opNode
                {
                    // need to evaluate the tree
                    double l;
                    double r;
                    opNode op = (opNode) n;

                    l = EvalTree(op.Left);
                    r = EvalTree(op.Right);
                    return op.Eval(l,r);
                }
            }
        }

        // set variable
        private void SetVar()
        {
            // find variable in tree, if variable is in tree, set variable name to value set by user
            // need a dictionary for this
            string key;
            Console.WriteLine("Enter variable name: "); 
            key = Console.ReadLine();

            // make sure the user selects a variable that exists
            if ( m_dict.ContainsKey(key) == true )
            {
                Console.WriteLine("Enter a value: "); // get user defined value

                m_dict[key] = Convert.ToDouble(Console.ReadLine()); // get number from user
                Console.WriteLine("Variable {0} = {1}", key, m_dict[key]); // print out what the new variable and value to the user
            }
            else
            {
                // otherwise doesn't exist
                Console.WriteLine("Variable does not exist within the current expression.");
                return;
            }
        }

        // make var node with value of 0
        private static Node MakeVarNode(string s)
        {
            // make sure varnode is of type (starts with a character) then has letters and numbers)
            if (Regex.IsMatch(s, "^[a-zA-Z]+[a-zA-Z0-9]*") == true)
            {
                if (m_dict.ContainsKey(s))
                {
                    return new varNode(s);
                }
                else
                {
                    // if user doesn't update the value it will be 0
                    m_dict.Add(s, 0);
                    return new varNode(s);
                }

            }
            return null;
        }
    }
}
