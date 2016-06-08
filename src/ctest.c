#include <stdio.h>
#include <stdint.h>

extern uint32_t *cpu_id(uint32_t);
void aschar(uint32_t v){
  int i;
  for(i=0;i<4;i++){
	int c=v & 0xff;
	if (c > 127 || c < ' ')
	  putchar('.');
	else
	  putchar(c);
	v = v >> 8;
  }
  printf("\n");
}
int main(int argc, char* argv[]){
  uint32_t *p;
  uint32_t src;
  for(src=0;src<0x18;src++){
	  p=cpu_id(src);
	  printf("initial eax:0x%08x\n",src);
	  printf("0x%08x ",*p);aschar(*p);
	  printf("0x%08x ",*(p+1));aschar(*(p+1));
	  printf("0x%08x ",*(p+2));aschar(*(p+2));
	  printf("0x%08x ",*(p+3));aschar(*(p+3));
  }
  return 0;
}
