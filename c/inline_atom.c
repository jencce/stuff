typedef volatile int Atomic_t, *AtomicP_t;
static int os_cas (AtomicP_t pdwPtr, int dwCompare, int dwSet)
{
    int dwRet;

    __asm__ __volatile__ (
    " lock cmpxchg %3, (%2)\n"
    : "=a" (dwRet)
    : "a" (dwCompare) , "r" (pdwPtr) , "r" (dwSet)
    );
    return dwRet;

}
 
static int os_add (AtomicP_t pdwPtr, int dwIncr)
{

    int  dwRet;
    __asm__ __volatile__ (
    " lock xadd %%eax, (%1)\n"
    : "=a" (dwRet)
    : "r" (pdwPtr) , "a" (dwIncr)
    );
    return dwRet;

}

main()
{
	os_cas(1,2,3);
}
