#include<stdio.h>
#include "AvlTree.c"

int main()
{
	AvlTree T;
	T = MakeEmpty( T );

	int num = 100000;
	int i;
	
	for( i = 0; i < num; i++ )
		T = Insert( i, T );
	
	printf("%d    \n",Height( T ) );
	return 0;
}
