# Address modes


|intel syntax|AT&T syntax|description|
|:--|:--|:--|
|Register Addressing|||
|`mov ax,bx`|`mov %bx,%ax`|
|Immediate|
|`mov ax,01h`|`mov $0x01,%ax`|
|Direct memory addressing|
|`mov ax,[my_var]`|`mov my_var,%ax`|
|Direct offset addressing|
|`mov ax,[my_var+12]` or `mov ax,my_var[12]`||
|Register Indirect|
|`mov ax,[di]`||The registers used for indirect addressing are BX, BP, SI, DI|
|Base-index|
|`mov ax,[bx + di]`||
|Base-index with displacement|
|`mov ax,[bx + di + 10]`|`mov 10(%bx,%di,1),%ax`|

# Other
    mov -4(%ebp),%eax
"()" 间接寻址(Indirect addressing),"-4" 偏移寻址(displacement),上面的语句是吧ebp中的值-4的地址内容给 eax,在
括号前面的数值表示偏移值

    movl -4(%ebp,%edx,4),%eax   # 装入 ebp - 4 + edx * 4 地址中的内容到 eax
    leal (8,%eax,4),%eax        # eax * 4 + 8 => eax
    leal (%eax,%eax,2),%eax     # eax * 2 + eax => eax

# INTEL 官方文档的翻译
## MEMORY MANAGEMENT OVERVIEW
(摘自 INTEL 官方文档 SYSTEM PROGRAMMING GUID 3.1 )

IA-32架构的内存管理设备是分开两部分:`分段`(segmentation)和`分页`(paging).`分段`提供一个代码,数据,栈模块独立分开的
机制,以便于多程序(或任务)能够运行在同一个处理器中而没有互相干扰.`分页`提供一个机制去实现conventional deman-paged(??),
virtual-memory system where 程序部分执行环境根据需要被映射(mapped)到物理地址.`分页`也能分隔多任务。当操作在保护模式
必须使用某些分段.*There is no mode bit to disable segmentation*.然而,使用`分页`是一个可选项.

两种机制能被设置用在单程序(或者单任务)系统,多任务系统或者共享内存的多处理器系统中.

看表 3-1， `分段`为把处理器的可访问地址空间(*linear address space*)分开到更小的保护地址空间(*segments*)提供一个机制
*segment* 为程序存放代码，数据和栈或者存放系统数据结构（例如TSS或者LDT).如果超过一个程序(或任务)运行在处理器中,每个程
序会被安排到自己的*segment*中.处理器负责划分*segment*的边界和保障程序不会互相之间干扰(The processor then enforces
the boundaries between these segments and insures that one program does onot interfer with the execution of
another program by writing into the other program's segments).分段机制也允许typing of segments(??)因此,执行常
规type of segemnt 能够被限制(The segmentation mechanism also allows typing of segments so that the operations
that may be performed on a particular type of segment can be restricted).

在系统中全部的segment被放在处理器的linear address space. 在一个常规segment定位一个字节,必须提供逻辑地址(logical address)
或者叫far pointer.一个逻辑地址由段选择(segment selector)和偏移(offset)组成.段选择器是唯一对应一个段,它提供一个描述表
(descriptor table)(例如GDT)的偏移,这个偏移中存放的数据结构叫segment descriptor.每一个segment有一个segment descriptor.
segment descriptor定义size,访问权限,privilege level,segment type,在linear address space的开始位置.偏移加base address
就能定位内存了

如果`分页`没有使用,linear address space 就会直接映射到物理内存空间(physical address space).物理内存空间的范围由处理器的
地址总线宽度决定(IA-32是32位,因此是4G)

因为多任务计算机系统通常定义一个大于最经济的实际物理内存的linear address space(Because multitasking computing systems
commonly define a linear address space much larger than it is economically feasible to contain all at once in physical
memory), 就需要某些"虚拟"的linear address space.这些虚拟linear address space就使用了`分页`机制.

`分页`支持"虚拟内存"环境,让一个大的linear address space 模拟在一个小的物理内存(RAM和ROM)和磁盘存储器上.当使用`分页`时,
每一个segment被分成多个页(基本是4KB一页),并被存放在内存和磁盘上.操作系统维护一个页目录(page directory)和设置分页表(page
table),以便于跟踪每个page,处理器用page directory和page table 翻译linear address 到一个物理地址然后执行一个在内存上的读写
请求.

如果被访问的page不在物理内存上,处理器会中断执行(产生一个page-fault exception).操作系统从磁盘上读page到物理内存然后继续执
行程序.

当`分页`被正确的实现在操作系统中.page在物理内存和磁盘之间的切换是对程序透明的.当运行在virtual-8086模式,16位的程序也能做
IA-32架构下透明的分页.

## USING SEGMENTS
(摘自 INTEL 官方文档 SYSTEM PROGRAMMING GUID 3.2 )

IA-32支持的的`分段`机制能够被用于各种系统设计中.这种设计从 flat models 到 mult-segmented models,flat models 最小的使用
segmentation 来保护程序, multi-segments 应用段来建立一个强壮的操作环境而令多任务能够被稳定的执行.

### Basic Flat Model
用于系统最简单的内存模式是"flat model",操作系统和应用程序共享一个连续的没有分段的地址空间.尽可能地,basic flat model为系
统设计者和程序员隐藏分段机制.
在IA-32机制中实现flat model,最少2个段描术必需被建立,一个是代码段一个是数据端,2个段都影射到整个linear address space:这就
是说,2个段都有一个基本地址0,和最大的访问空间4G. 限制segment在 4G以内，就算没有物理内存配置在系统上，段机制防止发生超出内
存限制异常，ROM(EPROM)被放置在内存的顶部空间,因为处理器从FFFF_FFF0H开始执行,RAM(DRAM) 放置在底部,因为DS 寄存器会被初始化
为0

### Protected Flat Model
The protected flat model is similar to the basic flat model,
### Mult-Segment Model
### Segmentation in IA-32e Mode
### Paging and Segmentation
`分页` 能被用在一上所有模式
### PHYSICAL ADDRESS SPACE
#### Intel 64 Processors and Physical Address Sapce
### LOGICAL AND LINEAR ADDRESSES
在保护模式的系统架构层,处理器使2种地址翻译阶段得到物理地址:逻辑地址翻译(logical-address translation)和线性地址空间分页(linear address space paging).

就算是最小的使用分段,处理器在访问每个字节都使用逻辑地址(logical address).逻辑地址由16位段选择器和32位偏移组成.
#### Logical Address Translation in IA-32e Mode
#### Segment Selectors
段选择器是一个16位的段ID,它不是直接指向段,而是指向段描述(segment descriptor).段选择器由下面几部分组成:
- Index (bit3 到 bit15) 在8192个(GDT or LDT)中选择. index * 8 加上GDT或LDT的开始地址.
- TI(Table indicator) flag (bit2) 指定表类型, 0- GDT 1-LDT
- Requested Privilege Level(RPL) (Bit0/1) 指定权限选择器,权限可选 0~3, See Section 5.5, "privilege Levels", for a description of the relationship of the RPL to the CPL of the executing program(or task) and teh descriptor privilege level(DPL) of the descriptor the segement selector points to.

GDT的第一个数据不给处理器使用. ... ...

#### Segment Registers

#### Segment Loading Instructions in IA-32e Mode

#### Segment Descriptor
8字节(64位)接组成,
- Segment Limit15:0  [15:0]
- Base Address15:0   [31:16]
- Base address 23:16 [39:32]
- Segment Type       [43:40]
- S Descriptor Type  [44]
- DPL                [46:45]
- Segment Present    [47]
- Segment Limit19:16 [51:48]
- Available for use by system software [52]
- 64-bit code segment(IA-32e mode only) [53]
- Default operation size (0=16-bit segment; 1= 32bit segment) [54]
- Granularity [55]
- Base Address31:24 [63:55]

1. Segment limit field
    定义段的大小，处理器根据 Granularity 标记解释2种Segment limit
    - 如果Granularity为0，段以1字节为单位，最大能定义 1MByte
    - 如果Granularity为1，段以4KByte为单位，最大能定义 4GByte。

    如果logical address中的offset大于(增加)／小于(减少)limi，就会发生一个general-protection exceptionst 或者 stack-fault excpetions

2. Base address field
    段的开始地址,16字节对齐
3. Segment Type field
    指定段的类型,生长的方向.类型的编码是根据代码,数据,系统描述而不同
4. S(Descriptor type)flag
    1 system segment, 0 code or data segment
5. DPL(descriptor privilege level) field
    指定权限级别.
6. P(segment-present) flag
    Indicates whether the segment is present in memory(set) or not present(clear).
7. D/B (default operation size/default stack pointer size and/or uper bund) flag
        * 可执行代码段(Executable code segment). 这个标记称作D,设1, 代码指令32bit/8bit.设0,16bit,指令前缀66和67仍然有效
		* 栈段(stack segment) 这个标记称作B, 指定某些指令(如:push pop call)操作符大小 1- 32 0-16
		* 向下生长段(Expand-down data segment)这个标记称作B, 1 段的上边界为0xFFFFFFFF(4 GBytes) 0 0xFFFF(64KBytes)
8. G(granularity)flag
9. L(64-bit code segment)flag
   		* 在IA-32e模式，1代表这个代码段的指令是64-bit模式，0代表兼容模式。如果L bit置位，D bit必须清除,如果不是IA-32e模
		式或在没有代码，此位保留并且为0
##CHAPTER 6 INTERRUPT AND EXCEPTION HANDLING
(6.2 exception and interrupt vectors)
协助处理异常和中断，每种架构定义异常和每种中断条件要求的特殊处理，处理器定义了唯一的ID号码叫vector number,处理器使用vector
number 关联到一个异常或中断的IDT(Interrupt Descriptor Table)中
vector number 从 0 到 255， 0 到 31 为Intel 64和 IA-32架构保留，这个范围内的vector number不是全部都使用的
Vector number 32 到 255为用户(我理解为系统设计者)使用,这些中断让外部I/O设备发出硬件中断到处理器通过其中一个外部硬件中断机制

### 6.10 INTERRUPT DESCRIPTOR TABLE
IDT基础地址必须8字节对齐