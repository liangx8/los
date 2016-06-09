#ifndef UTILS_H
#define UTILS_H
#include <stdint.h>
#define outb(port,val) asm volatile("out %0, %1"::"a"((uint8_t)(val)),"Nd"((uint16_t)(port)))
#define outw(port,val) asm volatile("out %0, %1"::"a"((uint16_t)(val)),"Nd"((uint16_t)(port)))
static inline uint8_t inb(uint16_t port)
{
		uint8_t ret;
		asm volatile ( "in %1, %0"
				: "=a"(ret)
				: "Nd"(port));
		return ret;
}
#endif
