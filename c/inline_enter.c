int main()
{
	__asm__ __volatile__( "enter $65535, $31"
			      :
			      :
			      : );
	return 0;
}
