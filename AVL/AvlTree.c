#include<stdlib.h>
#include "AvlTree.h"
#define Max( a, b ) ( ( a ) > ( b )?( a ):( b ))

struct AvlNode
{
	ElementType Element;
	AvlTree Left;
	AvlTree Right;
	int Height;
};

/**初始化一棵空树**/
AvlTree MakeEmpty( AvlTree T )
{
	if( T != NULL )
	{
		MakeEmpty( T->Left );
		MakeEmpty( T->Right );
	    free( T );	
	}
	return NULL;
}

/**查找**/
Position Find( ElementType X, AvlTree T )
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

Position FindMin( AvlTree T )
{
	if( T == NULL )
		return NULL;
	else if( T->Left == NULL )
		return T;
	else
		return FindMin( T->Left );
}

Position FindMax( AvlTree T )
{
	if( T != NULL )
		while( T->Right != NULL )
			T = T->Right;
	return T;
}

static int Height(Position P )
{
	if( P == NULL )
		return -1;
	else
		return P->Height;
}

/***********************************************************
 * 
 *	 		  k2					             k1
 *	 	     /  \							    /  \
 *		    /    \						       /    \
 *		   K1    k3		左左情况		      X	    K2
 *		  /  \         --------->			 /	   /  \	
 *	     /    \			 单旋转		        /	  /	   \
 *      X      Y						   Z     Y	   K3
 *	   /
 *	  /
 *   Z
 * 
 * 此函数对如图所示情况进行 左单旋转 接收需要旋转的节点 K2 ，
 * 完成旋转后更新对应的高度值 Height ,返回新的根 (此示例中是 K1)
 *
 ***********************************************************/
static Position SingleRotateWithLeft( Position K2 )
{
	Position K1;
	
	/**旋转**/
	K1 = K2->Left;
	K2->Left = K1->Right;
	K1->Right = K2;

	/**调整高度**/
	K2->Height = Max( Height( K2->Left ),
								Height( K2->Right ) ) + 1;
	K1->Height = Max( Height( K1->Left ),K2->Height ) + 1;

	return K1;
}

/**右单旋转和左单旋转相似的处理**/
/**********************************************************
 *				
 *			 K2									K3
 *	    	/  \							   /  \
 *		   /    \							  /    \
 *		  K1    K3							 K2	    X
 *		       /  \			右情况			/  \     \	
 *		      /    \	  --------->	   /    \     \
 *	         Y	    X		单旋转		  K1	 Y     Z
 *					 \
 *				      \
 *					   Z
 *
 * *******************************************************/
static Position SingleRotateWithRight( Position K2 )
{
	Position K3;

	/**旋转**/
	K3 = K2->Right;
	K2->Right = K3->Left;
	K3->Left = K2;

	/**调整高度**/
	K2->Height = Max( Height( K2->Right ),
								Height( K2->Left ) ) + 1;
	K3->Height = Max( Height(K3->Right ),K2->Height) + 1;

	return K3;

}

/****************************************************************
 *
 *					K3										K2
 *				   /  \									  /    \
 *				  /    \								 /      \
 *				 K1     D								K1      K3
 *				/  \			 			  		   / \		/ \	  
 *			   /    \           --------->            /   \	   /   \
 *			  A     K2			 		             A    C   B     D
 *				   /  \
 *				  /    \
 *			     C	    B
 *                                             ___    
 *						\                        /|
 *						 \                      / |
 *						  \					   /
 *						   \ |				  /
 *						 ___\|				 /
 *
 *								   K3   
 *								  /  \
 *								 /    \
 *								K2     D
 *							   /  \
 *							  /    \
 *							 K1     B
 *							/  \
 *						   /    \
 *						  A      C
 *
 * 对于上图所示的情况: K3 的左子树( K1 )的右子树( K2 )因为插入了新的
 * 节点 B或C ( C 和 B 两棵子树其中有一棵为空，另一棵不为空的便是新插
 * 入的，画两棵为了体现一般性 )而导致不平衡，属于双旋转的情况，需要
 * 调整的是 K3( 最深的需要调整的节点)，该函数接收 K3 ,进行图示的双旋转
 * ，然后调整对应的树高，最后返回新的根节点
 * 
 * ***************************************************************/
static Position DoubleRotateWithLeft( Position K3 )
{
	/**在 K1 和 K2 之间旋转**/
	K3->Left = SingleRotateWithRight( K3->Left );

	/**在 K3 和 K2 之间旋转**/
	return SingleRotateWithLeft( K3 );
}

static Position DoubleRotateWithRight( Position K3 )
{
	K3->Right = SingleRotateWithLeft( K3->Right );

	return SingleRotateWithRight( K3 );
}


/**插入节点**/
AvlTree Insert( ElementType X, AvlTree T )
{
	if( T == NULL )
	{
		/*创建新的节点返回单节点树**/
		T = malloc( sizeof( struct AvlNode ) );
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
		if( X < T->Element )
		{
			T->Left = Insert( X, T->Left );
			if( Height( T->Left ) - Height( T->Right ) == 2 )
			{
				/**此次插入导致了不平衡，需要进行旋转**/
				if( X < T->Left->Element )
					T = SingleRotateWithLeft( T );
				else
					T = DoubleRotateWithLeft( T );
			}
		}
		else if( X > T->Element )
		{
			T->Right = Insert( X, T->Right );
			if( Height( T->Right ) - Height( T->Left ) == 2)
			{
				/**此次插入导致了不平衡，需要进行旋转**/
				if( X > T->Right->Element )
					T = SingleRotateWithRight( T );
				else
					T = DoubleRotateWithRight( T );
			}
		}
		else
			/**要插入的节点已经存在，什么都不做**/
			;

	}
	/**更新树高**/
	T->Height = Max( Height( T->Left ), Height( T->Right ) ) + 1;

	return T;
}

/**删除节点值为 X 的节点**/
AvlTree Delete( ElementType X, AvlTree T )
{
	Position TmpCell;

	if( T == NULL )
	{
		printf("Element not found!!!\n");
		return NULL;
	}
	else if( X < T->Element )
		/**要删除的节点在左树中，在左树中递归删除**/
		T->Left = Delete( X, T->Left );
	else if( X > T->Element )
		/**要删除的节点在右树中，在右树中递归删除**/
		T->Right = Delete( X, T->Right );
	else
	{
		/**要删除的节点有两个孩子**/
		if( T->Left && T->Right )
		{
			TmpCell = FindMin( T->Right );
			T->Element = TmpCell->Element;
			T->Right = Delete( T->Element, T->Right );
		}
		else if( ( T->Left == NULL ) || ( T->Right == NULL ) )
		{
			TmpCell = T->Left ? T->Left : T->Right;

			/**要删除的节点是叶子节点**/
			if( TmpCell == NULL )
			{
				TmpCell = T;
				T = NULL;
			}	

			/**要删除的节点有一个或0个孩子**/
			else
			{
				/**儿子取代父亲的位置**/
				T = TmpCell;
			}

			free( TmpCell );
		}
	}

	if( T == NULL )
		return NULL;

	T->Height = Max( Height( T->Left ),Height( T->Right ) ) + 1;

	/**重新调整，使树平衡**/

	int HeiDiff = Height( T->Left ) - Height( T->Right );

	if( HeiDiff == 2 )
	{
		if( Height( T->Left->Left ) >= Height( T->Left->Right ) )
			/**左左情况**/
			return SingleRotateWithLeft( T );
		else
			/**左右情况**/
			return DoubleRotateWithLeft( T );
	}
	else if( HeiDiff == -2 )
	{
		if( Height( T->Right->Right ) >= Height( T->Right->Left ) )
			/**右右情况**/
			return SingleRotateWithRight( T );
		else
			/**右左情况**/
			return DoubleRotateWithRight( T );
	}

	return T;
}

ElementType Retrieve( Position P )
{
	if( P == NULL )
	{
		printf("Node is empty!!!\n");
		return 0;
	}
	return P->Element;
}

void PrintIn( AvlTree T )
{
	if( T == NULL )
		return;
	PrintIn( T->Left );
	printf("%d    ",T->Element );
	PrintIn( T->Right );
}

void PrintPre( AvlTree T )
{
	if( T == NULL )
		return;
	printf("%d    ",T->Element );
	PrintPre( T->Left );
	PrintPre( T->Right );	
}

void PrintPos( AvlTree T )
{
	if( T == NULL )
		return;
	PrintPos( T->Left );
	PrintPos( T->Right );
	printf("%d    ",T->Element );
}

/**
 * 0---->中序打印
 * 1---->先序打印
 * -1--->后序打印
 **/
void PrintTree( AvlTree T ,int FLAG )
{
	switch( FLAG )
	{
		case 0:
			PrintIn( T );
			break;
		case 1:
			PrintPre( T );
			break;
		case -1:
			PrintPos( T );
			break;
	}
	printf("\n");
}
