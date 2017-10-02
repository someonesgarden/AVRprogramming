/*
	== avrcommon.h ==
	common functions
	written by mojo@uec-koken 2009-5
*/

#ifndef outb
	#define	outb(addr, data) addr = (data)
#endif
#ifndef inb
	#define	inb(addr) (addr)
#endif
#ifndef outw
	#define	outw(addr, data) addr = (data)
#endif
#ifndef inw
	#define	inw(addr) (addr)
#endif
#ifndef BV
	#define BV(bit)	(1<<(bit))
#endif
#ifndef cbi
	#define cbi(reg,bit) reg &= ~(BV(bit))
#endif
#ifndef sbi
	#define sbi(reg,bit) reg |= (BV(bit))
#endif
#ifndef cli
	#define cli() __asm__ __volatile__ ("cli" ::)
#endif
#ifndef sei
	#define sei() __asm__ __volatile__ ("sei" ::)
#endif
