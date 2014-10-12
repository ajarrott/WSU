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
    // Binary Search Tree class
    public class BST
    {
        // needs data, left, right, and parent nodes;
        public int m_data;
        public BST left;
        public BST right;
        public BST parent;

        // constructor, pass prnt=null for root
        public BST(int value, BST prnt)
        {
            m_data = value;
            left = null;
            right = null;
            parent = prnt;
        }
    }

    // myBST creates the Binary Search Tree and includes all the necessary functionality
    public class myBST
    {
        private BST m_root;

        // initialize the tree to null;
        public myBST()
        {
            m_root = null;
        }

        // function to add values to the tree, tree will always be at top after addition
        public void add(int value)
        {
            // root case
            if (m_root == null)
            {
                m_root = new BST(value, null);
            }

            // every other case
            else
            {
                // breaks are within the while statment
                while ( true )
                {
                    // value less than parent
                    if (value < m_root.m_data)
                    {
                        if (m_root.left == null)
                        {
                            // set new value and return to the top of the tree
                            m_root.left = new BST(value, m_root);
                            m_root = getTop(m_root);
                            break;
                        }
                        else
                        {
                            // otherwise keep traversing
                            m_root = m_root.left;
                        }
                    }

                    // value is greather than the parent
                    else if (value > m_root.m_data)
                    {
                        if (m_root.right == null)
                        {
                            // set new value and return to the top of the tree
                            m_root.right = new BST(value, m_root);
                            m_root = getTop(m_root);
                            break;
                        }
                        else
                        {
                            // otherwise keep traversing
                            m_root = m_root.right;
                        }

                    }

                    // value already in the tree
                    else 
                    {
                        // For Debug
                        // Console.Write("Cannot add duplicate number " + value + " to tree.\n");
                        // return to top of the tree
                        m_root = getTop(m_root);
                        break;
                    }
                }
            }
        }

        // get statistics of the tree
        public void stats()
        {
            int nodes = 0;
           
            // start writing contents of tree
            Console.Write("Ordered tree contents: ");

            // count levels  before deleting the tree
            int levels = countLevels();
            // use delete to print tree out and number of nodes
            if ( m_root != null )
            {
                //m_root = getTop(m_root);
                nodes = deleteAndPrintTree(m_root);
                Console.Write("\n");
            }
            // root does not exist
            else
            {
                Console.Write("(null)\n");
            }

            // cannot get top if tree does not exists
            if (m_root != null)
            {
                m_root = getTop(m_root);
                
            }

            // print tree statistics
            Console.Write("Tree Statistics:\n");
            Console.Write("\tTree has " + nodes + " nodes.\n");
            Console.Write("\tTree has " + levels + " levels.\n");
            Console.Write("\tMinimum number of levels a tree with " + nodes + " nodes could have: " + minLevels(nodes) + "\n" );

            return;
        }

        // prints numbers in order by finding minimum, reseting the "pointers", then setting the node to null (deletion).
        // this function also counts the number of nodes
        public int deleteAndPrintTree(BST tree)
        {
            // initialize variables
            int min = 0;
            int nodes = 0;

            // while tree has values
            while ( tree != null )
            {
                // find min and delete it
                min = findMin(tree);
                tree = getTop(tree);
                tree = deleteNode(tree, min);
                

                if (min == -1)
                {
                    return nodes;
                }

                else
                {
                    // since min exists in the tree, print it out with a space
                    Console.Write(min + " ");
                    nodes++;    // increase node count
                }
                
            }

            // return node count
            return nodes;
 
        }

        // delete node function
        public BST deleteNode( BST tree, int value )
        {
            // find value to delete
            while ( tree.m_data != value )
            {
                if (value < tree.m_data)
                {
                    tree = tree.left;
                }

                else //if (value > tree.m_data)
                {
                    tree = tree.right;
                }
            }

            // root case
            // already at top do not return to top
            if ( tree.parent == null )
            {
                if ( tree.left != null && tree.right == null)
                {
                    tree = tree.left;
                    tree.parent = null;
                    return tree;
                }

                else if ( tree.left == null && tree.right != null )
                {
                    tree = tree.right;
                    tree.parent = null;
                    return tree;
                }

                else // since we go through and delete from min, I can assume that the last case will be no children
                {
                    tree = null;
                    return tree;
                }              
            }
            
            // at leaf, move up one, delete correct child, return to top, return tree
            else if (tree.left == null && tree.right == null)
            {
                tree = tree.parent;
                if ( tree.left.m_data == value )
                {
                    tree.left = null;
                }
                else if ( tree.right.m_data == value )
                {
                    tree.right = null;
                }
                else
                {
                    Console.Write("DEBUG: cannot remove value" + value);
                }

                tree = getTop(tree);
                return tree;
            }

            // for the next 2 cases the current node is going to be deleted
            // so we need to assign the current parent to the left or right child's parent
            // then find the current node's parent data and assign it to the correct child node

            // non-null left child and null right child
            else if (tree.left != null && tree.right == null)
            {
                // left child of parent
                if (tree.parent.m_data > tree.m_data)
                {
                    tree.left.parent = tree.parent;     
                    tree.parent.left = tree.left;
                    tree = tree.left;
                }
                // right child of parent
                else
                {
                    tree.left.parent = tree.parent;
                    tree.parent.right = tree.left;
                    tree = tree.left;
                }
                
                // return to top
                tree = getTop(tree);
                return tree;
            }

            // null left and non-null right child
            else //if (tree.left == null && tree.right != null)
            {
                // left child of parent
                if ( tree.parent.m_data > tree.m_data )
                {
                    tree.right.parent = tree.parent;
                    tree.parent.left = tree.right;
                    tree = tree.right;
                }
                // right child of parent
                else
                {
                    tree.right.parent = tree.parent;
                    tree.parent.right = tree.right;
                    tree = tree.right;
                }

                // return to the top
                tree = getTop(tree);
                return tree;
            }
            
            // other deletion functionallity is unnecessary because we will only delete the minimum value each time.
        }

        // returns the tree to the top (or root) value
        public BST getTop( BST tree )
        {
            while (tree.parent != null)
            {
                tree = tree.parent;
            }

            return tree;
        }

        // gets the lowest number of the tree for deletion, returns tree to top
        public int findMin(BST tree)
        {
            if ( tree != null )
            {
                while ( tree.left != null )
                {
                    tree = tree.left;
                }
                return tree.m_data;
            }
            
            // should not get to this case
            else
            {
                return -1;
            }
        }

        // counts the levels of the tree (root level is 0)
        public int countLevels()
        {
            if ( m_root != null )
            {
                BST tree = m_root;
                int l = 0;
                int r = 0;

                // recursively check left
                if (tree.left != null)
                {
                    l += countLevels(tree.left);
                }

                // recursively check right
                if (tree.right != null)
                {
                    r += countLevels(tree.right);
                }

                // return whichever value is greater, add 1
                return 1 + ((l > r) ? l : r);
            }
            else
            {
                return 0;  // no root, 0 levels
            }

        }

        // overloaded countLevels helper function
        public int countLevels ( BST lr )
        {
            int left = 0, right = 0;

            // recursively check left
            if ( lr.left != null )
            {
                left += countLevels(lr.left);
            }

            // recursively check right
            if ( lr.right != null )
            {
                right += countLevels(lr.right);
            }

            // return whichever is greater + 1
            return 1 + (( left > right ) ? left : right);
        }

        // minLevels finds minimun levels of a balanced BST, assuming the top level is 0
        public double minLevels(int nodes)
        {
            // cannot log(0) return 0
            if (nodes == 0) { return 0; }
            // to get correct levels need to get the ceiling of Log base 2 ( n + 1 )
            // since root is considered at level 1 we do not subtract 1
            int levels = Convert.ToInt32(Math.Ceiling(Math.Log(nodes + 1, 2))); 
            //levels = levels / nodes;
            return levels;
        }
    }
}
