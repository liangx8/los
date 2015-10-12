# Address modes

|Register Addressing        |
|:-------------------------:|
|intel syntax |AT&T syntax|
|mov ax,bx    |mov %bx,%ax|

### Immediate
  mov ax,01h
  mov $0x01,%ax
### Direct memory addressing
  .data
  my_var dw 12345
  .code
    mov ax,[my_var]
    mov my_var,%ax
### Direct offset addressing
  mov ax,[my_var+12]
  mov ax,my_var[12]
### Register Indirect
  mov ax,[di] # The registers used for indirect addressing are BX, BP, SI, DI
### Base-index
  mov ax,[bx + di]
### Base-index with displacement
  mov ax,[bx + di + 10]