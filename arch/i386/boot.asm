;-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+;
;                                                                              ;
;                            FOSOS i386 bootloader                             ;
;                                                                              ;
;                  Copyright (c) 2022, the FOSOS developers.                   ;
;                    SPDX-License-Identifier: BSD-2-Clause                     ;
;                                                                              ;
;-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+;
[bits 16]    ; We start in 16-bit real mode
[org 0x7c00] ; The BIOS loads us at 7c00

push dx ; Save the boot drive number

cld ; Clear the direction flag so string ops count up

; Clear the host console by printing \033c (reset device)
mov dx, 0x3f8
mov al, 27
out dx, al
mov al, "c"
out dx, al

; Print a message so the user knows what we're doing
mov si, loadingMsg
call printSerial
mov si, startingMsg
call printBIOS

; Set up a buffer for the kernel at 512K
mov bx, 0x8000
mov es, bx
xor bx, bx

; Load the kernel code
mov ah, 0x02
mov al, 72
mov cx, 10
pop dx ; Get that drive number from earlier
xor dh, dh
int 0x13

; Check carry and return code for errors
jc diskError
cmp ah, 0
jne diskError

cli ; Disable interrupts

; Enable A20 (8042)
call empty8042
mov al, 0xd1
out 0x64, al
call empty8042
mov al, 0xdf
out 0x60, al
call empty8042

lgdt [gdt_descriptor] ; Load the GDT

; Set Protect Enable bit in control register 0
mov eax, cr0
or al, 1
mov cr0, eax

jmp 8:initProtected

loadingMsg: db "[Boot] Loading kernel...", 10, 0
startingMsg: db "Starting FOSOS...", 13, 10, 0
diskErrorMsg: db "[Boot] A disk read error occurred.", 13, 10, 0

; Print a string to the serial console
printSerial:
	mov dx, 0x3f8

	printSerial_loop:
		mov al, [si]
		cmp al, 0
		je printSerial_exit

		out dx, al ; Output to COM1/serial console
		inc si
		jmp printSerial_loop
	printSerial_exit:
		ret

; Print a string using BIOS interrupts
printBIOS:
	mov ah, 0x0e

	printBIOS_loop:
		mov al, [si]
		cmp al, 0
		je printBIOS_exit

		int 0x10 ; Output to screen with BIOS
		inc si
		jmp printBIOS_loop
	printBIOS_exit:
		ret

diskError:
	mov si, diskErrorMsg
	call printSerial
	mov si, diskErrorMsg
	call printBIOS

	cli ; No interrupts
	hlt ; Hang :~(

empty8042:
	dw 0x00eb, 0x00eb
	in al, 0x64
	test al, 2
	jnz empty8042
	ret

gdt_start:
	dd 0, 0
	dd 0x0000ffff
	dd 0x00cf9a00
	dd 0x0000ffff
	dd 0x00cf9200
gdt_end:

; Like a handle to the GDT
gdt_descriptor:
	dw gdt_end - gdt_start - 1
	dd gdt_start

[bits 32] ; We have 32 bits now!
initProtected:
	mov ax, 16
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov esp, 0x2000 ; Set up an 8KB 32-bit stack

	jmp 0x80000 ; Jump to the kernel

times 510-($-$$) db 0 ; Make sure this is one sector (boot sector)
dw 0xaa55             ; Magic 'bootable' marker :~)