.globl begtext,begdata,begbss,endtext,enddata,endbss

.text
begtext:
.data
begdata:
.bss
begbss:
	BOOTSEG = 0x07C0
	entry start
start:
	jmpi go, BOOTSEG
go:	mov ax,cs
	mov ds,ax
	mov es,ax
	mov [msg1 + 17], ah
	mov cs,#20
	mov dx,#0x1004
	mov bx,#0x000c
	mov bp,#msg1
	mov ax,#0x1301
	int 0x1004
loop1:	jmp loop1
msg1:	.ascii "Loading system ..."
	.byte 13,10
.org 510
.word 0xAA55
.text
;; endtext:	
.data
endtext:
.bss
endbss:	
