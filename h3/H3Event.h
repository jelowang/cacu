
/*

	+	H3Event.h
	+	QDL

	+	hello3d
	+	realtime 3d engine																
 
	+	(C)	Quantum Dynamics Lab.
			FACEGOOD.
	+	

*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef H3Event_H
#define H3Event_H

/*
 brief : key , mouse event
*/
typedef enum
{

	H3KeyEvent_Press = 0x1 << 1 ,
	H3KeyEvent_Release = 0x1 << 2 , 
	H3KeyEvent_Key_A = 0x10 , 
	H3KeyEvent_Key_B = 0x20 ,
	H3KeyEvent_Key_C = 0x30 ,
	H3KeyEvent_Key_D = 0x40 ,
	H3KeyEvent_Key_E = 0x50 ,
	H3KeyEvent_Key_F = 0x60 ,
	H3KeyEvent_Key_G = 0x70 ,
	H3KeyEvent_Key_H = 0x80 ,
	H3KeyEvent_Key_I = 0x90 ,
	H3KeyEvent_Key_J = 0xa0 ,
	H3KeyEvent_Key_K = 0xb0 ,
	H3KeyEvent_Key_L = 0xc0 ,
	H3KeyEvent_Key_M = 0xd0 ,
	H3KeyEvent_Key_N = 0xe0 ,
	H3KeyEvent_Key_O = 0xf0 ,
	H3KeyEvent_Key_P = 0x100,
	H3KeyEvent_Key_Q = 0x110 ,
	H3KeyEvent_Key_R = 0x120 ,
	H3KeyEvent_Key_S = 0x130 ,
	H3KeyEvent_Key_T = 0x140 ,
	H3KeyEvent_Key_U = 0x150 ,
	H3KeyEvent_Key_V = 0x160 ,
	H3KeyEvent_Key_W = 0x170 ,
	H3KeyEvent_Key_X = 0x180 ,
	H3KeyEvent_Key_Y = 0x190 ,
	H3KeyEvent_Key_Z = 0x1a0 ,
	H3KeyEvent_Key_UP = 0x1b0 ,
	H3KeyEvent_Key_DOWN = 0x1c0 ,
	H3KeyEvent_Key_LEFT = 0x1d0 ,
	H3KeyEvent_Key_RIGHT = 0x1e0 ,

} H3KeyEventDef ;

typedef enum
{

	H3MouseEvent_Click = 0x1 << 1 ,
	H3MouseEvent_Release = 0x1 << 2 ,
	H3MouseEvent_Moving = 0x1 << 4 ,
	H3MouseEvent_Wheel = 0x1 << 10 ,
	H3MouseEvent_Left_Button = 0x20 ,
	H3MouseEvent_Right_Button = 0x30 ,

} H3MouseEventDef ;

class H3KeyEvent
{
public:
	H3KeyEvent(){e = 0 ;}
	~H3KeyEvent(){}
	int e ;
} ;

class H3MouseEvent
{
public:
	H3MouseEvent(){
		e = 0 ;
		memset ( &this->point , 0 , sizeof(this->point) ) ;
	}
	~H3MouseEvent(){}

	int e ;

	struct {

		int id[5] ;
		int x[5] ;
		int y[5] ;
		int totall ;

	} point ;

} ;

class H3AccelerometerEvent
{
public:
	H3AccelerometerEvent(){}
	~H3AccelerometerEvent(){}

	float x ;
	float y ;
	float z ;
} ;

#define H3KeyEvent_Event_Press(x) ( (H3KeyEvent_Press & x) == H3KeyEvent_Press )
#define H3KeyEvent_Event_Release(x) ( (H3KeyEvent_Release & x) == H3KeyEvent_Release )
#define H3KeyEvent_Key(x) ( (0xfffffff0 & x) )

#define H3MouseEvent_Event_Click(x) ( (H3MouseEvent_Click & x) == H3MouseEvent_Click )
#define H3MouseEvent_Event_Release(x) ( (H3MouseEvent_Release & x) == H3MouseEvent_Release )
#define H3MouseEvent_Event_Moving(x) ( (H3MouseEvent_Moving & x) == H3MouseEvent_Moving )
#define H3MouseEvent_Event_Wheel(x) ( (H3MouseEvent_Wheel & x) == H3MouseEvent_Wheel )
#define H3KeyEvent_Button(x) ( (0xfffffff0 & x) )

#endif 