#include <iostream>
using namespace std;
// this is a little program i wrote to teach myself about pointers and addresses
// when i first returned to WSU 

int main ( void )
{
	int a = 10;
	int *b = &a;
	int **c = &b;

	cout << a << " " << *b << " " << **c << endl;
	cout << &a << " " << &b << " " << &c << endl;

	b = new int(40);

	cout << a << " " << *b << " " << **c << endl;
	cout << &a << " " << &b << " " << &c << endl;
	**c = 20;



	cout << a << " " << *b << " " << **c << endl;
	cout << &a << " " << &b << " " << &c << endl;
	*c = new int(30);

	cout << a << " " << *b << " " << **c << endl;
	cout << &a << " " << &b << " " << &c << endl;
	a = 50;

	cout << a << " " << *b << " " << **c << endl;
	cout << &a << " " << &b << " " << &c << endl;
	b = &a;

	cout << a << " " << *b << " " << **c << endl;
	cout << &a << " " << &b << " " << &c << endl;

	getchar();

	return 1;
}