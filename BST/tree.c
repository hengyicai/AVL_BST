#include "tree.h"
#include<stdlib.h>
#define Max(a,b) ( ( a ) > ( b ) ?( a ):( b ) )

/**节点定义**/
struct TreeNode
{
	ElementType Element;
	SearchTree Left;
	SearchTree Right;
	int Height;
};

/**初始化一棵空树**/
SearchTree MakeEmpty( SearchTree T )
{
	if( T != NULL )
	{
		MakeEmpty( T->Left );
		MakeEmpty( T->Right );
		free( T );
	}
	return NULL;
}

static int Height( SearchTree T )
{
	if( T == NULL )
		return -1;
	return T->Height;
}

/**
int Max(int a,int b)
{
	return a>b?a:b;
}*/


/**查找成功的话返回指向树T中具有关键字X的节点的指针，否则返回NULL**/
Position Find( ElementType X,SearchTree T )
{
	if( T == NULL )
		return NULL;
	if( X < T->Element )
		return Find( X, T->Left );
	else if( X > T->Element )
		return Find( X, T->Right );
	else
		return T;
}

/**找到最小元素的位置**/
Position FindMin( SearchTree T )
{
	if( T == NULL )
		return NULL;
	else if( T->Left == NULL )
		return T;
	else
		return FindMin( T->Left );
}

/**找到最大元素的位置**/
Position FindMax( SearchTree T )
{
	if( T != NULL )
		while( T->Right != NULL )
			T = T->Right;
	return T;
}

/**向树中插入元素**/
SearchTree Insert( ElementType X, SearchTree T )
{
	if( T == NULL )
	{
		//此时 T 是一棵空树，分配空间并创建一棵树
		T = malloc( sizeof( struct TreeNode ) );
		if( T == NULL )
			printf("Out of space!!!\n");
		else
		{
			T->Element = X;
			T->Height = 0;
			T->Left = T->Right = NULL;
		}
	}
	else
	{
		//递归插入 X 到合适的子树中
		if( X < T->Element )
			T->Left = Insert( X, T->Left );
		else if( X > T->Element )
			T->Right = Insert( X, T->Right );
		else
			//X已经在树中，什么也不做
			;
	}

	T->Height = Max( Height( T->Left ),Height( T->Right )) + 1;	

	return T;
}

/**删除树中节点值为 X 的节点**/
SearchTree Delete( ElementType X, SearchTree T )
{
	Position TmpCell;

	if( T == NULL )
		printf("Element not found!!!\n");
	else if( X < T->Element )
		T->Left = Delete( X, T->Left );
	else if( X > T->Element )
		T->Right = Delete( X, T->Right );
	else /**要删除的节点找到了**/
	{
		//要删除的这个节点有两个孩子
		if( T->Left && T->Right )
		{
			//用该节点右子树中最小的节点代替该节点
			TmpCell = FindMin( T->Right );
			T->Element = TmpCell->Element;
			T->Right = Delete( T->Element, T->Right );
		}
		else /**要删除的这个节点有一个或0个孩子**/
		{
			TmpCell = T;
			if( T->Left == NULL )
				T = T->Right;
			else if( T->Right == NULL )
				T = T->Left;
		//	T->Height = TmpCell->Height;
			free( TmpCell );
		//	T->Height--;
		}
	}

	if( T == NULL )
		return NULL;

	T->Height = Max( Height( T->Left ),Height( T->Right ) ) + 1;

	return T;
}

ElementType Retrieve( Position P )
{
	if( P == NULL )
	{
		printf("Node is Empty!!!\n");
		return 0;
	}
	return P->Element;
}

void Print( SearchTree T )
{
	if( T == NULL )
		return;
	Print( T-> Left );
	printf("%d    ",T->Element );
	Print( T->Right );
}

void PrintTree( SearchTree T )
{
	Print( T );
	printf("\n");
}
