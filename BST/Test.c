#include<stdio.h>
#include "tree.c"
#include<math.h>

int main()
{
	//初始化一棵树
	SearchTree my_tree;
	my_tree = MakeEmpty( my_tree );
	
	int num = 100000;
	int i;
	
	for( i = 0; i < num; i++ )
		my_tree = Insert( i, my_tree );
	
	printf("%d\n",Height(my_tree ));
	return 0;
}
