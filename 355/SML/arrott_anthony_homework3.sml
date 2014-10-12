(* Anthony Arrott 10853267 HW3 *) 

(*sees if value(cmp) is in list*)
fun in_list(cmp, lst) =
	(*check if list exists*)
	if null lst then false
	else 
		(* if first value is == cmp then true *)
		if cmp = (hd lst) then true 
		(* otherwise recursively check tail of list *)
		else in_list(cmp, (tl lst));

(* helper for intersection function, removes duplicates in list *)
fun rmdups ( lst ) = 
	(* if null list then return null list*)
	if null lst then []
	else
		(* check if value is in list again by calling in_list
		   with front of list and checking rest of list       *)
		if in_list(hd lst, tl lst) then
		(* if it is then add it to an empty list, essentially
		   removing it from the list 			      *)
		hd(lst)::[]
		else
		(* otherwise return list *)
			lst;

(* get the intersection between 2 lists without duplicates *)
fun intersection ( list1, list2 ) =
	let
		(* set lst1 and lst1 to the same lists as before w/o dups *)
		val lst1 = rmdups (list1); 
		val lst2 = rmdups (list2);
	in
		(* check if either list is null *)
		if null lst1 orelse null lst2 then [] 
		else
			(* check if head of lst1 is in lst2 *)
			if in_list(hd lst1, lst2) then 
			(* if it iss, append list to beg of return list 
			   and recursively call intersection      	*)
			hd(list1)::intersection(tl lst1, lst2)
			else
				(* otherwise just recursively call *)
				intersection(tl lst1, lst2)
	end;

(* filter function applies a lambda function to a list, and returns the list *)
fun filter f lst =
	(* check if list is null *)
	if null lst then []
	else
		(* if the filter applies to first item in the list
		   append it to return list then recusively call filter *)
		if f (hd lst) then (hd lst) :: filter f (tl lst)
		(* otherwise just recursively call *)
		else filter f (tl lst);

(* partition is a  helper function for quicksort *)
(* if partitioning and pivot is the only number, return null *)
fun partition f (pivot, []) = ([],[])
|   partition f (pivot, first::rest) =
	let 
		(* value front, back are 2 lists, one in front of the pivot
   		   the rest behind the pivot, recursively calls partiton to
		   check values in rest to figure out which list they
		   should all be appended to                                *)
		val (front, back) = partition f (pivot, rest)
	in
		(* check if passed filter applies, if it does apply first
		   value to front list					    *)
		if f(first, pivot) then ( first::front, back )
		(* otherwise append first value to back of the list *)
		else	( front, first::back )
	end;

(* takes a lambda function to sort list by lambda defn *)
fun quicksort f lst = let
	(* if null return null *)
	fun qs [] = []
	(* if single item, return single item *)
	|   qs [x] = [x]
	(* otherwise pivot is at the front of the passed list *)
	|   qs (pivot::lst) = 
		let
			(* get front abd back lists from partition *)
			val (front, back) = partition f (pivot, lst) 
		in
			(* append pivot at the sorted front then append
			   the sorted back                              *) 
			qs front @ pivot :: qs back
		end
	in
		qs lst
	end;

(* declaration of datatypes, either can be an int or string *)
datatype either = INT of int | STRING of string;
(* eitherTree has LEAF of either, or a NODE of an eitherTree twice
   aka a binary tree only 2 nodes can be attached to a parent node *)
datatype eitherTree = LEAF of either | NODE of eitherTree * eitherTree;

(* eitherSearch searches for a value in the tree, not a string *)
(* if the value is an int, and the value we are looking for return true *)
fun eitherSearch(LEAF (INT i), int) = (i = int) 
(* if the leaf has a string then return false *)
|   eitherSearch(LEAF (STRING s), int) = false 
(* otherwise traverse the tree on each side *)
|   eitherSearch(NODE(n1, n2), int) =
    	eitherSearch(n1, int) orelse eitherSearch(n2, int);

(* test function for eitherSearch *)
fun treeTester () =
	let
		(* setup nodes for tree *)
		val n0 = NODE(LEAF(INT 0), LEAF(INT 1))
		val n1 = NODE(LEAF(INT 1), LEAF(INT 2))
		val n2 = NODE(LEAF(INT 2), LEAF(INT 3))
		val n3 = NODE(LEAF(INT 3), LEAF(INT 4))
		val n4 = NODE(LEAF(INT 4), LEAF(INT 5))
		val n5 = NODE(LEAF(STRING "ABC"), LEAF(STRING "DEF"))
		val n6 = NODE(LEAF(STRING "GHI"), LEAF(STRING "JKL"))
		val n7 = NODE(LEAF(STRING "MNO"), LEAF(STRING "PQR"))
		val n8 = NODE(LEAF(STRING "STU"), LEAF(STRING "VWX"))
		val n9 = NODE(LEAF(STRING "YZ "), LEAF(STRING "AAA"))
		(* link nodes in tree *)
		val t1 = NODE(n0, n5)
		val t2 = NODE(n1, n6)
		val t3 = NODE(n2, n7)
		val t4 = NODE(n3, n8)
		val t5 = NODE(n4, n9)
		val t6 = NODE(t1, t2)
		val t7 = NODE(t2, t3)
		val t8 = NODE(t3, t4)
		val t9 = NODE(t4, t5)
		val f1 = NODE(t6, t7)
		val f2 = NODE(t8, t9)
		val root = NODE (f1, f2);
	in
		(* run eithersearch *)
	(*	eitherSearch(root, 0) (* true *)*)
		eitherSearch(root, 9) (* false *)
	end;

(* declare Tree datatype *)
datatype 'a Tree = LEAF of 'a | NODE of ('a Tree) list

(* writes string as a tree with parenthesis for nodes and uses 
   a function to make the value into a string or integer       *)
(* if its just a leaf, just apply the function to the value *)
fun treeToString f (LEAF s) = (f s)
(* otherwise if its a node put parenthesis around the node and map
   treeToString to every value in the tree, this concatenates everything
   together as well                                                      *)
|   treeToString f (NODE n) = "(" ^ (concat (map (treeToString f) n)) ^ ")";

(* test function from hw file written out *)
fun TTSTest () =
	let
		val L1a = LEAF "a"
		val L1b = LEAF "b"
		val L1c = LEAF "c"
		val L2a = NODE [L1a, L1b, L1c]
		val L2b = NODE [L1b, L1c, L1a]
		val L3 = NODE [L2a, L2b, L1a, L1b]
		val L4 = NODE [L1c, L1b, L3]
		val L5 = NODE [L4]
	in
		treeToString String.toString L5
	end;

(* another test function from the hw file written out *)
fun TTSTest2 () =
	let
		val iL1a = LEAF 1
		val iL1b = LEAF 2
		val iL1c = LEAF 3
		val iL2a = NODE [iL1a, iL1b, iL1c]
		val iL2b = NODE [iL1b, iL1c, iL1a]
		val iL3 = NODE [iL2a, iL2b, iL1a, iL1b]
		val iL4 = NODE [iL1c, iL1b, iL3]
		val iL5 = NODE [iL4]
	in
		treeToString Int.toString iL5
	end;

(* helper function for perms*)
(* if list is empty return value from f as a list *)
fun rotate f [] = [[f]]
(* otherwise concatenate the lists with all possible rotations of the list *)
|   rotate f (front::back) = (f::front::back)::(map (fn x => front::x) (rotate f back));

(* returns a list of lists of all possible purmuations*)
(* if empty list return list with empty set *)
fun perms [] = [[]]
(* recursively call rest of the values, also call function 
   that rotates values in the list                         *)
|   perms (front::back) = List.concat (map (fn x => rotate front x) (perms back));
