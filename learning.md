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
*segment* 为程序存放代码，数据和栈或者存放系统数据结构（例如TSS或者LDT).如果超过一个程序(或任务)运行在处理器中,每个
程序会被安排到自己的*Segment*中.如果超过一个程序(或任务)运行在处理器中,每个程序会被安排到自己的*segment*中.处理器负
责划分*segment*的边界和保障程序不会互相之间干扰(The processor then enforces the boundaries between these segments
and insures that one program does onot interfer with the execution f another program by writing into the other program's
segments).分段机制也允许typing of segments(??)因此,执行常规type of segemnt 能够被限制(The segmentation mechanism
also allows typing of segments so that the operations that may be performed on a particular type of segment can be
restricted).

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
table),以便于跟踪没个page,处理器用page directory和page table 翻译linear address 到一个物理地址然后执行一个在内存上的读写
请求.

如果被访问的page不在物理内存上,处理器会中断执行(产生一个page-fault exception).操作系统从磁盘上读page到物理内存然后继续执
行程序.

当`分页`被正确的实现在操作系统中.page在物理内存和磁盘之间的切换是对程序透明的.当运行在virtual-8086模式,16位的程序也能做
IA-32架构下透明的分页.

## USING SEGMENTS
(摘自 INTEL 官方文档 SYSTEM PROGRAMMING GUID 3.2 )