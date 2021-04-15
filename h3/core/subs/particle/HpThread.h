
/*
+	HpEmitterLogic.h
+	Jelo Wang

+	Hello Particle
+	realtime VFX engine
																				
																												
+	(C) 量子动力实验室 Quantum Dynamics Lab.
+		Community : http://www.qdl.studio
+		Contact Us : jelo.wang@gmail.com

*/ 

#ifndef __HP_THREAD_H__
#define __HP_THREAD_H__

#include "HpAdapter.h"

#ifdef HP_USE_THREAD
#include <condition_variable>
#include <thread>
#include <mutex>
#include <chrono>
#include <algorithm>
#include <list>  
#include <atomic>
#include <vector>  
#include <time.h>  
#include <assert.h>  
#include <memory>  
#include "sccl.h"
#include "HpEmitterLogic.h"

#define MAX_THREAD 1000
#define MAX_QUEUE 100000

typedef void (HpEmitterLogic::*HpCB_ThreadTask)(void* et , void* particle ) ;

typedef struct {

	HpCB_ThreadTask func;
	HpEmitterLogic* target ;

	void* para1 ;
	void* para2 ;

} HpThreadTask ;


/*
brief :
*/
class HpThreadPool
{

public:

	/*
	brief :
	*/
	HpThreadPool();

	/*
	brief :
	*/
	~HpThreadPool();

	/*
	brief :
	*/
	void init(int totall) ;

	/*
	brief : 
	*/
	void exit () ;

	/*
	brief :
	*/
	void addTask( HpEmitterLogic* logic , HpCB_ThreadTask func , void* para1 , void* para2 ) ;

private:

	/*
	brief :
	*/
	void run() ;

private :

	bool m_exit ;

	std::mutex m_queueMutex ;
	std::condition_variable m_queueMutex_notify ;
	SCClQueue m_queueTask;

	struct {
		std::thread* id[MAX_THREAD];
		struct {
			HpThreadTask task[MAX_QUEUE] ;
			int front ;
			int rear ;
		} queueTask ;
		int totall;
	} m_thread;

};

#if 0
/*
	brief : 
*/
class HpThread
{
public:

	/*
		brief : 
	*/
	HpThread() ;

	/*
		brief : 
	*/
	~HpThread() ;

	/*
		brief : 
	*/
	void join() ;
	
	/*
		brief : 
	*/
	void wait() ;
	
	/*
		brief : 
	*/	
	void terminate() ;
	
	/*
		brief : 
	*/	
	size_t load() ;
	
	/*
		brief : 
	*/	
	size_t appendTask( std::function< void (void) > func ) ;

private: 
	void run () ;

private: 

	bool m_next_finish ;
	std::thread* m_thread ;
	std::list<std::function< void (void)> >	m_list_tasks ;           
	std::mutex m_list_tasks_mutex ;      
    std::mutex m_cond_mutex ; 
	std::condition_variable condition;

} ;

/*
	brief : 
*/
class HpThreadPool
{

public :

	/*
		brief : 
	*/	
	HpThreadPool ( int threads ) ;

	/*
		brief : 
	*/	
	~HpThreadPool () ;

	/*
		brief : 
	*/
	void join() ;
	
	/*
		brief : 
	*/
	void wait() ;
	
	/*
		brief : 
	*/	
	void terminate() ;
	
	/*
		brief : 
	*/	
	void append(std::function< void (void) > func) ;

	/*
		brief : 
	*/	
	size_t load(int n) ;

	/*
		brief : 
	*/
	int count() ;
	
private :

	int m_n_threads;  
	std::vector<std::shared_ptr<HpThread> > m_vec_threads;  

} ;

#endif

#endif	//	#ifdef HP_USE_THREAD

#endif
