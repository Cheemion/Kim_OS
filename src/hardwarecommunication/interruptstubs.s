.set IRQ_BASE, 0x20

.section .text

.extern _ZN4myos21hardwarecommunication16InterruptManager15handleInterruptEhj



.macro HandleException num
.global _ZN4myos21hardwarecommunication16InterruptManager16HandleException\num\()Ev
_ZN4myos21hardwarecommunication16InterruptManager16HandleException\num\()Ev:
	movb $\num, (interruptnumber)
	jmp int_bottom
.endm

.macro HandleInterruptRequest num
.global _ZN4myos21hardwarecommunication16InterruptManager26HandleInterruptRequest\num\()Ev
_ZN4myos21hardwarecommunication16InterruptManager26HandleInterruptRequest\num\()Ev:
	movb $\num + IRQ_BASE, (interruptnumber)
	pushl $0         #this is for error code, Exception will push this automatically but interrupt won't
	jmp int_bottom
.endm


HandleInterruptRequest 0x00
HandleInterruptRequest 0x01
HandleInterruptRequest 0x0C
	
int_bottom:

	pushl %ebp
	pushl %edi
	pushl %esi
	pushl %edx
	pushl %ecx
	pushl %ebx
	pushl %eax
	
	#call c++ handler
	pushl %esp
	pushl (interruptnumber)
	call _ZN4myos21hardwarecommunication16InterruptManager15handleInterruptEhj
	movl %eax, %esp	# switch the stack 

	popl %eax
	popl %ebx
	popl %ecx
	popl %edx
	popl %esi
	popl %edi
	popl %ebp

	add $4, %esp #previous push 0, to count offset, its actually the error flag in the CPUState

.global _ZN4myos21hardwarecommunication16InterruptManager22IgnoreInterruptRequestEv
_ZN4myos21hardwarecommunication16InterruptManager22IgnoreInterruptRequestEv:	
	iret
.data
    interruptnumber: .byte 0
