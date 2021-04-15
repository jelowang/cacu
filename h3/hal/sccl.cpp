
/*

+	Semo Compiler Common Library 

+	'Semo Compiler' is a multi-objective compiler which is developing under the terms of the 
+	GNU general public license as published by the Free Software Foundation.
+	The project lunched by QDL since 2.Feb.2008 from 'Techniques of Knowledge' community. 

+	You can redistribute it and/or modify it under the terms of the gnu general public version 3 of 
+	the license as published by the free software foundation.this program is distributed in the hope 
+	that it will be useful,but without any warranty.without even the implied warranty of merchantability 
+	or fitness for a particular purpose.																					
																												
+	(C)	突壳开源Techniques of Knowledge
+		an open source community since 2008
+		Community : http://www.tok.cc
+		Contact Us : jelo.wang@gmail.com

+		-Thanks to Our Committers and Friends
+		-Best Wish to all who Contributed and Inspired
+		-Techniques of Knowledge 
+		-致伙伴们最美好祝愿
+		-突壳开源社区

*/ 

# include <assert.h>
# include "H3Memory.h"
# include "string.h"
# include "schal.h"
# include "sccl.h"

SCClList* SCClListNew () {

	//	author : QDL
	//	since : 20102806
	//	(C)TOK

	SCClList* list = (SCClList* ) MMC_MALLOC ( sizeof(SCClList) ) ;

	if ( !list ) return 0 ;
	
	list->element = 0 ;
	list->totall = 0 ;
	list->next = 0 ;

	return list ;

}

void SCClListInit ( SCClList* list ) {

	//	author : QDL
	//	since : 20091123
	//	(C)TOK

	list->element = 0 ;
	list->totall = 0 ;
	list->next = 0 ;
	list->last = 0 ;

}

void SCClListInsert ( SCClList* list , void* el ) {

	//	author : QDL
	//	since : 20091123
	//	(C)TOK

	SCClList* listnew = 0 ;

	if ( !list ) return ;

	listnew = (SCClList* ) MMC_MALLOC ( sizeof(SCClList) ) ;
	assert(0!=listnew) ;
	
    listnew->key = 0 ;
	listnew->index = list->totall ;
	listnew->element = el ;
	listnew->last = 0 ;
	listnew->next = 0 ;
	listnew->totall = 0 ;

	if ( 0 == list->next ) {
		list->next = listnew ;
		list->last = listnew ;
	} else {
		list->last->next = listnew ;
		list->last = listnew ;
	}

	list->totall ++ ;
	
}

void SCClListInsertEx ( SCClList* list , void* el , int key ) {
    
    //	author : QDL
    //	since : 20091123
    //	(C)TOK
    
    SCClList* listnew = 0 ;
    
    if ( !list ) return ;
    
    listnew = (SCClList* ) MMC_MALLOC ( sizeof(SCClList) ) ;
    assert(0!=listnew) ;
    
    listnew->key = key ;
	listnew->index = list->totall ;
    listnew->element = el ;
    listnew->last = 0 ;
    listnew->next = 0 ;
    listnew->totall = 0 ;
    
    if ( 0 == list->next ) {
        list->next = listnew ;
        list->last = listnew ;
    } else {
        list->last->next = listnew ;
        list->last = listnew ;
    }
    
    list->totall ++ ;
    
}

void SCClListDelete ( SCClList* list , void* el ) 
{
	SCClList* node = 0 ;
	SCClList* pre = 0 ;

	if ( 0 == list ) {
		return ;
	}

	node = list->next ;
	pre = list ;

	for ( ; node ; ) {
	
		if ( el == node->element ) {
			
			if ( node == list->next ) {
				list->next = node->next ;
			} else if ( list->last == node ) {
				list->last = pre ;	
			}
			pre->next = node->next ;

			MMC_FREE ( node ) ;

			list->totall -- ;

			if ( 0 == list->totall ) {
				list->next = 0 ;
				list->last = 0 ;
			}

			return ;
			
		}

		pre = node ;
		node = node->next ;

	}

	return ;

}

int SCClListEmpty ( SCClList* list ) {

	//	author : QDL
	//	since : 20100811
	//	(C)TOK

	if ( !list ) return 1 ;

	if ( 0 == list->totall ) {
		return 1 ;
	}

	return 0 ;
	
}

void* SCClListSearch ( SCClList* lt , void* element ) {

	//	author : QDL
	//	since : 20121026
	//	(C)TOK

	SCClList* node = 0 ;

	if ( 0 == lt ) {
		return 0 ;
	}

	node = lt->next ;

	for ( ; node ; ) {

		if ( element == node->element ) {
			return node->element ;
		}

		node = node->next ;

	}

	return 0 ;

}

void* SCClListSearchEx ( SCClList* lt , int key ) {
    
    //	author : QDL
    //	since : 20121026
    //	(C)TOK
    
    SCClList* node = 0 ;
    
    if ( 0 == lt ) {
        return 0 ;
    }
    
    node = lt->next ;
    
    for ( ; node ; ) {
        
        if ( key == node->key ) {
            return node->element ;
        }
        
        node = node->next ;
        
    }
    
    return 0 ;
    
}


void SCClListClear ( SCClList* list ) {

	//	author : QDL
	//	since : 20100430
	//	(C)TOK

	SCClList* node = 0 ;

	if ( 0 == list ) {
		return ;
	}

	for ( node = list->next ; node ; ) {

		list->next = node->next ;
		MMC_FREE ( node ) ;
		node = list->next ;

	}
	
	list->totall = 0 ;

}

void SCClListDestroy ( SCClList** list ) {

	//	author : QDL
	//	since : 20110705
	//	(C)TOK

	if ( 0 == list ) {
		return ;
	}

	SCClListClear ( *list ) ;

	MMC_FREE ( *list ) ;
	*list = 0 ;

}

void SCClStackInit ( SCClStack* istack ) {

	//	author : QDL
	//	since : 20090819

	istack->element = 0 ;
	istack->next = 0 ;
	istack->front = 0 ;
	istack->deep = 0 ;
 
}

void* SCClStackGet ( SCClStack* istack  ) {

	//	author : QDL
	//	since : 20090819
	
	if ( 0 != istack->next )
		return (void* )istack->next->element ;
		return 0 ;
	
}


int SCClStackEmpty ( SCClStack* istack ) {

	//	author : QDL
	//	since : 20091123
	
	return (0 != istack->next) ?  0  : 1 ;
	
}

void SCClStackPush ( SCClStack* istack , void* eelement ) {

	//	author	: QDL
	//	since	: 20090819
	
	SCClStack* nnode = 0 ;

	nnode = (SCClStack* ) MMC_MALLOC ( sizeof(SCClStack)  ) ;
	nnode->element = eelement ;
	nnode->front = istack->next ;
	nnode->next = 0  ;

	if ( 0 != istack->next ) {
		istack->next->next = nnode ;
	}

	istack->next = nnode ;
	istack->deep ++ ;

	if ( 0 == istack->front )
		istack->front = nnode ;	
	
}

void* SCClStackPop ( SCClStack* istack ) {

	//	author	: QDL
	//	since	: 20090819
	
	void* el = 0 ;
	SCClStack* deln = 0 ;
	
	if ( !istack->next )
		return 0 ;

	el = (void* )istack->next->element ;

	deln = istack->next ;

	istack->next = deln->front ;

	MMC_FREE ( deln ) ;

	istack->deep -- ;

	return el ;
	
	
}

int SCClStackGetDeep ( SCClStack* istack ) {

	//	author	: QDL
	//	since	: 20100425

	return istack->deep ;
	
}

void SCClStackDestroy ( SCClStack* istack ) {

	//	author	: QDL
	//	since	: 20100425

	while ( !SCClStackEmpty (istack) )
		SCClStackPop ( istack ) ;

	istack->front = 0 ;
	istack->next = 0 ;
	istack->deep = 0 ;

}

void SCClQueueInit ( SCClQueue* queue ) 
{
	queue->data = 0 ;
	queue->front = 0 ;
	queue->rear = 0 ;
	queue->totall = 0 ;
}

int SCClQueueEmpty ( SCClQueue* queue ) 
{
	if ( NULL == queue->rear ) {
		return 1 ;
	}

	return 0 ;

}
void SCClQueueEnter ( SCClQueue* queue , void* element ) 
{

	SCClQueue* node = (SCClQueue* )MMC_MALLOC ( sizeof(SCClQueue) ) ;
	
	assert(node) ;

	node->data = element ;
	node->front = queue->rear ;
	node->rear = 0 ;

	if ( 0 == queue->front ) {
		queue->front = node ;
	}

	if ( 0 != queue->rear ) {
		queue->rear->rear = node ;
	}

	queue->totall ++ ;
	queue->rear = node ;

}

void* SCClQueueOut ( SCClQueue* queue ) 
{

	void* el = 0 ;
	SCClQueue* node = 0 ;

	if ( 0 == queue->front ) {
		return 0 ;
	}

	node = queue->front ;
	queue->front = queue->front->rear ;

	if ( 0 == queue->front ) {
		queue->rear = 0 ;
	}

	el = node->data ;
	MMC_FREE ( node ) ;

	queue->totall -- ;

	return el ;

}

void SCClQueueDestroy ( SCClQueue* queue ) 
{

	SCClQueue* node = 0 ;

	if ( 0 == queue ) {
		return ;
	}

	for ( node = queue->front ; node ; ) {

		queue->front = queue->front->rear ;
		MMC_FREE ( node ) ;
		node = queue->front ;

	}

}