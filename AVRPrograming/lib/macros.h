
/* Standard Macros */
/* You can totally get by without these, but why? */

/* Make sure we've already got io / sfr / pindefs loaded */
#ifndef   _AVR_IO_H_
#include  <avr/io.h>
#endif

/* Reminder: the following useful bit-twiddling macros are
   always included in avr/sfr_defs.h, which is called from
   avr/io.h

 bit_is_set(sfr, bit)
 bit_is_clear(sfr, bit)
 loop_until_bit_is_set(sfr, bit)
 loop_until_bit_is_clear(sfr, bit)

*/

/* Define up the full complement of bit-twiddling macros */
#ifndef BV
#define BV(bit)	(1<<(bit))
#endif
#ifndef  set_bit
#define set_bit(sfr, bit)     (_SFR_BYTE(sfr) |= BV(bit))  // old sbi()
#endif
#ifndef clear_bit
#define clear_bit(sfr, bit)   (_SFR_BYTE(sfr) &= ~BV(bit)) // old cbi()
#endif
#ifndef toggle_bit
#define toggle_bit(sfr, bit)  (_SFR_BYTE(sfr) ^= BV(bit))
#endif
/*
#ifndef cli
	#define cli() __asm__ __volatile__ ("cli" ::)
#endif
#ifndef sei
	#define sei() __asm__ __volatile__ ("sei" ::)
#endif
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