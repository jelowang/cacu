
/*
+	HpeGeometry.h
+	Jelo Wang

+	Hello Particle
+	realtime VFX engine
																				
																												
+	(C) 量子动力实验室 Quantum Dynamics Lab.
+		Community : http://www.qdl.studio
+		Contact Us : jelo.wang@gmail.com

*/ 

#include "schal.h"
#include "HpThread.h"

#ifdef HP_USE_THREAD
static int locker = 0 ;

/*
brief :
*/
HpThreadPool::HpThreadPool()
{

	SCClQueueInit( &m_queueTask ) ;
	memset( &m_thread , 0 , sizeof(m_thread) ) ;

	m_thread.queueTask.front = 0 ;
	m_thread.queueTask.rear = 0 ;
	m_exit = false ;

}

/*
brief :
*/
HpThreadPool::~HpThreadPool()
{

	m_exit = true ;
	for ( int counter = 0 ; counter < m_thread.totall ; counter ++ ) {

		m_thread.id[counter]->join () ;
		delete m_thread.id[counter] ;

	}

}

/*
brief :
*/
void HpThreadPool::init(int totall)
{
	
	assert ( totall <= MAX_THREAD );
	m_exit = false ;

	for ( int counter = 0 ; counter < totall ; counter ++ ) {

		m_thread.id[counter] = new std::thread( std::bind(&HpThreadPool::run, this) ) ;
		m_thread.totall ++ ;
	}

}

/*
brief : 
*/
void HpThreadPool::exit () 
{
	m_exit = true ;
}

/*
brief :
*/
void HpThreadPool::addTask( HpEmitterLogic* logic , HpCB_ThreadTask func , void* para1 , void* para2 )
{

	std::unique_lock<std::mutex> ul(m_queueMutex); 

	if ( m_thread.queueTask.front == ( m_thread.queueTask.rear + 1 ) % MAX_QUEUE ) {
		return ;
	}

	m_thread.queueTask.task [m_thread.queueTask.rear].target = logic ;
	m_thread.queueTask.task [m_thread.queueTask.rear].func = func ;
	m_thread.queueTask.task [m_thread.queueTask.rear].para1 = para1 ;
	m_thread.queueTask.task [m_thread.queueTask.rear].para2 = para2 ;
	m_thread.queueTask.rear = ( m_thread.queueTask.rear + 1 ) % MAX_QUEUE ; 

	m_queueMutex_notify.notify_one();  



}

/*
brief :
*/
void HpThreadPool::run()
{

	while ( false == m_exit )
	{
		 
		std::unique_lock<std::mutex> ul(m_queueMutex); 
		if ( m_thread.queueTask.front == m_thread.queueTask.rear ) {
			m_queueMutex_notify.wait(ul); 
			continue ;
		}
		
		HpEmitterLogic* logic = m_thread.queueTask.task [m_thread.queueTask.front].target ;
		HpCB_ThreadTask callback = m_thread.queueTask.task [m_thread.queueTask.front].func ;
		(logic->*callback)(m_thread.queueTask.task [m_thread.queueTask.front].para1,m_thread.queueTask.task [m_thread.queueTask.front].para2);

		m_thread.queueTask.front = ( m_thread.queueTask.front + 1 ) % MAX_QUEUE ;
		
	}
}

#if 0
/*
	brief : 
*/
HpThread::HpThread() 
{
	m_thread = NULL ;
	m_next_finish = false ;
}

/*
	brief : 
*/
HpThread::~HpThread() 
{
	m_next_finish = false ;
	m_list_tasks.clear(); 
	if ( NULL != m_thread ) {
		delete m_thread ;
	}
}

/*
	brief : 
*/
void HpThread::join() 
{
	if ( NULL == m_thread ) {
		return ;
	}
	terminate();  
	m_thread->join() ;
}

/*
	brief : 
*/
void HpThread::wait() 
{
	while(load()) {
		std::this_thread::sleep_for (std::chrono::microseconds(1)) ;
	}
		
}

/*
	brief : 
*/	
void HpThread::terminate() 
{
	m_next_finish = true ;
}

/*
	brief : 
*/	
size_t HpThread::load() 
{
	size_t sz = 0;  
	m_list_tasks_mutex.lock();  
    sz = m_list_tasks.size();  
    m_list_tasks_mutex.unlock();  
    return sz;
}

/*
	brief : 
*/	
size_t HpThread::appendTask( std::function< void (void) > func ) 
{

	if ( NULL == m_thread ) {
		m_thread = new std::thread(std::bind(&HpThread::run,this)) ;
	}

    size_t sz = 0;  
    
	m_list_tasks_mutex.lock();  
    m_list_tasks.push_back(func);  
    sz = m_list_tasks.size();  
    m_list_tasks_mutex.unlock();  
    
	return sz;  

}

/*
	brief : 
*/	
void HpThread::run()  
{  

	std::function<void()> task;
    while(true)
    {
        {   // acquire lock
            std::unique_lock<std::mutex> 
                lock(pool.queue_mutex);
             
            // look for a work item
            while(!pool.stop && pool.tasks.empty())
            { // if there are none wait for notification
                pool.condition.wait(lock);
            }
 
            if(pool.stop) // exit if the pool is stopped
                return;
 
            // get the task from the queue
            task = pool.tasks.front();
            pool.tasks.pop_front();
 
        }   // release lock
 
        // execute the task
        task();
    }

    while ( false == m_next_finish)  
    {  
        std::function< void (void)> curr_task ;  
        bool bHasTasks = false;  

        m_list_tasks_mutex.lock();  
        if (m_list_tasks.empty()==false)  
        {  
            bHasTasks = true;  
            curr_task = *m_list_tasks.begin();  
        }  
        m_list_tasks_mutex.unlock();  

        //doing task  
        if (bHasTasks)  
        {  
            curr_task();  
            m_list_tasks_mutex.lock();  
            m_list_tasks.pop_front();  
            m_list_tasks_mutex.unlock();  
        }  

    }  
}  

/*
	HpThreadPool
*/

/*
	brief : 
*/	
HpThreadPool::HpThreadPool ( int threads ) 
{

	assert(threads>0 && threads<=512);

	for (int i = 0; i< threads ;i++)  
		m_vec_threads.push_back(std::shared_ptr<HpThread>(new HpThread()));  

	m_n_threads = threads ;
}

/*
	brief : 
*/	
HpThreadPool::~HpThreadPool () 
{
}

/*
	brief : 
*/
void HpThreadPool::join() 
{
	for_each(m_vec_threads.begin(),m_vec_threads.end(),[this](std::shared_ptr<HpThread> & item)
	{
		item->terminate();  
		item->join();  
	}) ;
}

/*
	brief : 
*/
void HpThreadPool::wait() 
{

	int n_tasks = 0;  

	do  
	{  
		if (n_tasks)  
			std::this_thread::sleep_for (std::chrono::microseconds(1)) ;

		n_tasks = 0;  
		for_each(m_vec_threads.begin(),m_vec_threads.end(),[this,&n_tasks](std::shared_ptr<HpThread> & item)  
		{  
			n_tasks += item->load();  
		});  

	}while (n_tasks);  
  
        
}

/*
	brief : 
*/	
void HpThreadPool::terminate() 
{
	for_each(m_vec_threads.begin(),m_vec_threads.end(),[this](std::shared_ptr<HpThread> & item)  
	{  
		 item->terminate();  
	});  
}

/*
	brief : 
*/	
size_t HpThreadPool::load(int n) 
{
	return (n>=m_vec_threads.size())?0:m_vec_threads[n]->load();  
}

/*
	brief : 
*/	
void HpThreadPool::append(std::function< void (void) > func)  
{  

    int nIdx = -1;  
    unsigned int nMinLoad = -1;  

    for (unsigned int i=0;i<m_n_threads;i++)  
    {  
        if (nMinLoad> m_vec_threads[i]->load())  
        {  
            nMinLoad = m_vec_threads[i]->load();  
            nIdx = i;  
        }  
    }  

    assert(nIdx>=0 && nIdx<m_n_threads);  
    m_vec_threads[nIdx]->appendTask(func);  

}  

/*
	brief : 
*/
int HpThreadPool::count() {
	return m_vec_threads.size();
}

#endif

#endif //#ifdef HP_USE_THREAD