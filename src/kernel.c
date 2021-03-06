
#include <stdbool.h> /* C doesn't have booleans by default. */

#include <stddef.h>
#include <stdint.h>

#include "utils.h"
//#define LOS_DECL __cdecl

/* Check if the compiler thinks if we are targeting the wrong operating system. */
//#if defined(__linux__)
//#error "You are not using a cross-compiler, you will most certainly run into trouble"
//#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

/* Hardware text mode color constants. */
enum vga_color {
		COLOR_BLACK = 0,
		COLOR_BLUE = 1,
		COLOR_GREEN = 2,
		COLOR_CYAN = 3,
		COLOR_RED = 4,
		COLOR_MAGENTA = 5,
		COLOR_BROWN = 6,
		COLOR_LIGHT_GREY = 7,
		COLOR_DARK_GREY = 8,
		COLOR_LIGHT_BLUE = 9,
		COLOR_LIGHT_GREEN = 10,
		COLOR_LIGHT_CYAN = 11,
		COLOR_LIGHT_RED = 12,
		COLOR_LIGHT_MAGENTA = 13,
		COLOR_LIGHT_BROWN = 14,
		COLOR_WHITE = 15,
};
const  uint8_t hex[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
uint8_t make_color(enum vga_color fg, enum vga_color bg)
{
		return fg | bg << 4;
}

uint16_t make_vgaentry(char c, uint8_t color)
{
		uint16_t c16 = c;
		uint16_t color16 = color;
		return c16 | color16 << 8;
}
/*
  size_t strlen(const char* str) {
  size_t ret = 0;
  while ( str[ret] != 0 )
  ret++;
  return ret;
  }
*/
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

int __attribute__((stdcall)) update_cursor(size_t row,size_t col);
uint32_t cpu_flag(void);
uint32_t* cr_flag(void);

void set_cursor(size_t row,size_t col)
{
		size_t offset=col + row * 80;
		//crt control reigister 14
		outb(0x3d4,14);
		outb(0x3d5,(offset >> 8) );
		//crt control reigister 15
		outb(0x3d4,15);
		outb(0x3d5,offset);
}
void terminal_initialize()
{
		terminal_row = 0;
		terminal_column = 0;
		terminal_color = make_color(COLOR_RED, COLOR_BLACK);
		// 这是一个超过32位的地址访问,因此可以判断肯定在保护模式
		terminal_buffer = (uint16_t*) 0xB8000;
		for (size_t y = 0; y < VGA_HEIGHT; y++) {
				for (size_t x = 0; x < VGA_WIDTH; x++) {
						const size_t index = y * VGA_WIDTH + x;
						terminal_buffer[index] = make_vgaentry(' ', terminal_color);
				}
		}
}

void scroll_screen()
{
		for(size_t row=1;row<VGA_HEIGHT;row++){
				for(size_t col=0;col<VGA_WIDTH;col++){
						terminal_buffer[(row-1)*VGA_WIDTH+col]=terminal_buffer[row*VGA_WIDTH+col];
				}
		}
		uint16_t space=make_vgaentry(' ',terminal_color);
		for(size_t col=0;col<VGA_WIDTH;col++){
				terminal_buffer[(VGA_HEIGHT-1)*VGA_WIDTH+col]=space;
		}
}
void terminal_setcolor(uint8_t color)
{
		terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
		const size_t index = y * VGA_WIDTH + x;
		terminal_buffer[index] = make_vgaentry(c, color);
}


void terminal_putchar(char c)
{
		if(c == '\n'){
				terminal_column = 0;
				if (++terminal_row == VGA_HEIGHT) {
						terminal_row = VGA_HEIGHT-1;
						scroll_screen();
				}
				return;
		}
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
		if (++terminal_column == VGA_WIDTH) {
				terminal_column = 0;
				if (++terminal_row == VGA_HEIGHT) {
						terminal_row = VGA_HEIGHT-1;
						scroll_screen();
				}
		}
}

void terminal_writestring(const char* data)
{
		size_t i=0;
		while(data[i]){
				terminal_putchar(data[i++]);
		}
		set_cursor(terminal_row,terminal_column);
}
void hput(uint32_t v)
{
		uint32_t a=v/16;
		uint32_t b=v%16;
		if (a) hput(a);
		terminal_putchar(hex[b]);
}
void iput(long v)
{
		long a=v/10;
		int b=v%10;
		if (a) iput(a);
		terminal_putchar(hex[b]);
}
#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif

void kernel_main()
{
		uint32_t flag;
		uint8_t b=0;
//		uint32_t *pcr;
		/* Initialize terminal interface */
		terminal_initialize();
		update_cursor(0,0);
		terminal_writestring("size of int ");
		iput(sizeof(int));
		terminal_putchar('\n');

		for(int a=0;a<5;a++){
				iput(a+100000);
				terminal_writestring(" 0x");
				hput(0x1f000+a);
				terminal_writestring(" Hello, kernel World!\n");
		}
		flag=cpu_flag();
		hput(flag);
		terminal_putchar(' ');
		for (int a=0;a<300;a++){
				uint32_t flag1;
				b ++;
				flag1=cpu_flag();
				if (flag1 != flag){
						flag=flag1;
						hput(flag);
						terminal_putchar(' ');
						hput(b);
						terminal_putchar('|');
				}
		}
		/*
		pcr=cr_flag();
		terminal_putchar('\n');
		for(int a=0;a<4;a++){
				hput(*(pcr+a));
				terminal_putchar('\n');
				}*/
}
