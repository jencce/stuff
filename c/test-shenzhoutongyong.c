#include <alsa/iatomic.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdio.h>

#define		LOOPS		100000000
#define		IN_LOOPS	10000
#define		MAX_THREAD	10

#define		THREAD_STACK_SIZE	(1024 * 1024)
#define		MAX_RANDOM_VALUE	(0x7FFFFFFF)

typedef volatile int 		Atomic_t, *AtomicP_t;
typedef volatile int		SpinLock_t, *SpinLockP_t;
typedef int					Int_t;
typedef struct timeval		TimeType_t;
#define TIMEING(t)			gettimeofday(&t, NULL)
#define TIMEMODE(t)			1
#define GET_TIME_OF_SEC(e, s, f, t)		\
	t = (e.tv_sec - s.tv_sec) +  (double)(e.tv_usec - s.tv_usec)/1000000

TimeType_t			f;
int					dwCheckAtomicLoops = 10000;
Int_t				dwSpinCnt = 0;
#ifdef IA
atomic_t			wAtomicCnt = {0};
#define				get_value(val)	atomic_read(&val)
#else
Atomic_t			wAtomicCnt = 0;
#define				get_value(val)	val
#endif

#define INLINE		__inline
#ifdef USE_ICC
#define THREAD		__declspec(thread)
#else
#define THREAD		__thread
#endif

SpinLock_t		spinLock = 0;

typedef void* (*ThreadFuncFN_t) (void* pArg);	// �߳���ں���

static void OSCreateThreadMult(ThreadFuncFN_t fn, Int_t threadCnt);

static void OSAcquireSpinLock(SpinLockP_t pspinMutex);
static void OSReleaseSpinLock(SpinLockP_t pspinMutex);

static void *sys_test_add_spinlock(void *argv);
static void *sys_test_atomic_add(void *argv);

#ifdef WIN32
#define random	rand
#define SLEEP(_milliseconds) Sleep((DWORD)(_milliseconds))
#elif defined(SOLARIS)
#define SLEEP usleep
#else
#define SLEEP(_millisecond) usleep(1000*(_millisecond))
#endif
#ifndef MAX
#define MAX(a, b)	(((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b)	(((a) < (b)) ? (a) : (b))
#endif

SpinLock_t		lockThread[MAX_THREAD];
Int_t			threadArgv[MAX_THREAD];

int main()
{
	TimeType_t		e,s;
	double			tsec;
	Int_t			i;
	
	TIMEMODE(f);

	for (i=0; i<MAX_THREAD; i++)
	{
		lockThread[i] = 1;
		threadArgv[i] = i;
	}

	TIMEING(s);
	OSCreateThreadMult(sys_test_atomic_add, 1);
	TIMEING(e);
	GET_TIME_OF_SEC(e, s, f, tsec);
	printf("one thread atomic_add:%d, total time:%.3fs\n", get_value(wAtomicCnt), tsec);

	TIMEING(s);
	OSCreateThreadMult(sys_test_add_spinlock, 1);
	TIMEING(e);
	GET_TIME_OF_SEC(e, s, f, tsec);
	printf("one thread spinlock  :%d, total time:%.3fs\n", dwSpinCnt, tsec);

	TIMEING(s);
	OSCreateThreadMult(sys_test_atomic_add, 10);
	TIMEING(e);
	GET_TIME_OF_SEC(e, s, f, tsec);
	printf("ten thread atomic_add:%d, total time:%.3fs\n", get_value(wAtomicCnt), tsec);

	TIMEING(s);
	OSCreateThreadMult(sys_test_add_spinlock, 10);
	TIMEING(e);
	GET_TIME_OF_SEC(e, s, f, tsec);
	printf("ten thread spinlock  :%d, total time:%.3fs\n", dwSpinCnt, tsec);


	getchar();
}


// ����threadCnt���߳�ִ�У�ÿ���̵߳�����ƽ������
void OSCreateThreadMult(ThreadFuncFN_t fn, Int_t	threadCnt)
{
	Int_t			i;
	pthread_t		tid;
	pthread_attr_t	attr;

	dwSpinCnt	= 0;
	dwCheckAtomicLoops = LOOPS/IN_LOOPS/threadCnt;
#ifdef IA
	atomic_set(&wAtomicCnt, 0);
#else
	wAtomicCnt	= 0;
#endif

	for (i=0; i<threadCnt; i++)
	{
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		pthread_attr_setstacksize(&attr, THREAD_STACK_SIZE);

		pthread_create(&tid, &attr, fn, &threadArgv[i]);
	}
	
	for (i=0; i<threadCnt; i++)
	{
		OSAcquireSpinLock(&lockThread[i]);
	}
}

#ifdef IA
#define cmpxchg(ptr,old,new)								\
	({														\
	__typeof__(ptr) _p_ = (ptr);							\
	__typeof__(new) _n_ = (new);							\
	unsigned long _o_, _r_;									\
	_o_ = (unsigned int) (long) (old);						\
	__asm__ __volatile__ ("mov ar.ccv=%0;;" :: "rO"(_o_));	\
	__asm__ __volatile__ ("cmpxchg4.acq %0=[%1],%2,ar.ccv"	\
	: "=r"(_r_) : "r"(_p_), "r"(_n_) : "memory");			\
	(__typeof__(old)) _r_;									\
})
#endif

static INLINE Int_t os_cas (AtomicP_t pdwPtr, Int_t dwCompare, Int_t dwSet)
{
	return cmpxchg(pdwPtr, dwCompare, dwSet);
}

void OSAcquireSpinLock (SpinLockP_t pspinMutex)
{
	Int_t	spinCnt = 0;

#define OS_SPIN_COUNT	4000
	while (os_cas(pspinMutex, 0, 1) != 0)
	{
		if (spinCnt++<OS_SPIN_COUNT)
		{
			spinCnt = 0;
			SLEEP(10);
		}
	}
}

void OSReleaseSpinLock (SpinLockP_t pspinMutex)
{
	*pspinMutex = 0;
	return;
}

void *sys_test_atomic_add(void *argv)
{
	Int_t id = *(Int_t*)argv;
	Int_t i=0;

	for(; i < dwCheckAtomicLoops; i++) 
	{
		Int_t j=0;
		for(; j < IN_LOOPS; j++) 
		{
#ifdef IA
			atomic_inc(&wAtomicCnt);
#elif defined(WIN32)
			_InterlockedIncrement(&wAtomicCnt);
#endif
		} 
	}

	OSReleaseSpinLock(&lockThread[id]);

	return NULL;
}

void *sys_test_add_spinlock(void *argv)
{
	Int_t id = *(Int_t*)argv;
	Int_t i = 0;

	for(; i < dwCheckAtomicLoops; i++) 
	{
		Int_t j=0;
		for(; j < IN_LOOPS; j++) 
		{
			OSAcquireSpinLock(&spinLock);
			dwSpinCnt++;
			OSReleaseSpinLock(&spinLock);
		} 
	}

	OSReleaseSpinLock(&lockThread[id]);

	return NULL;
}

