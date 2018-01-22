;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; TSHR_01_OUTPUT_FZX_CUSTOM
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; This output terminal driver subclasses the tshr_01_output_fzx driver.
; It overrides the scroll pause handling by printing a scroll prompt
; message in another output terminal, waiting for a key press and finally
; clearing the scroll prompt message.
;
; ;;;;;;;;;;;;;;;;;;;;
; DRIVER CLASS DIAGRAM
; ;;;;;;;;;;;;;;;;;;;;
;
; CONSOLE_01_OUTPUT_TERMINAL (root, abstract)
; CONSOLE_01_OUTPUT_TERMINAL_FZX (abstract)
; TSHR_01_OUTPUT_FZX (concrete)
; TSHR_01_OUTPUT_FZX_CUSTOM (concrete)

SECTION code_driver
SECTION code_driver_terminal_output

PUBLIC tshr_01_output_fzx_custom

EXTERN tshr_01_output_fzx
EXTERN asm_im2_push_registers, asm_im2_pop_registers
EXTERN asm_in_wait_nokey, asm_in_wait_key
EXTERN l_jpix

EXTERN OTERM_MSG_PAUSE
EXTERN STDIO_MSG_WRIT, STDIO_MSG_ICTL
EXTERN IOCTL_OTERM_CLS
EXTERN _window_in ; scroll prompt window

tshr_01_output_fzx_custom:

   ; let parent class handle other messages than OTERM_MSG_PAUSE
   cp OTERM_MSG_PAUSE
   jp nz, tshr_01_output_fzx

oterm_msg_pause:

   ; OTERM_MSG_PAUSE:
   ; The scroll count has reached zero so the driver should pause the output somehow.
   ;
   ; can use: af, bc, de, hl

   ; STDIO_MSG_WRIT:
   ; Write a buffer to the stream.
   ;
   ; input:
   ;  IX = FDSTRUCT *output_terminal
   ;  A  = STDIO_MSG_WRIT
   ; BC' = buffer length > 0
   ; HL' = void *buffer
   ; HL  = buffer length > 0

   ; STDIO_MSG_ICTL:
   ; IOCTL message delivery to driver.
   ;
   ; input:
   ;  IX = FDSTRUCT *output_terminal
   ;   A = STDIO_MSG_ICTL
   ;  BC = first parameter
   ;  DE = ioctl request
   ;  HL = void *arg (!= 0 for custom handling)

   call asm_im2_push_registers

   ; set output terminal for scroll prompt
IF __SDCC_IY
   ld iy,(_window_in)
ELSE
   ld ix,(_window_in)
ENDIF

   ; send message STDIO_MSG_WRIT to output terminal to print the scroll prompt
   ld a,STDIO_MSG_WRIT
   ld bc,6
   ld hl,scroll_prompt
   exx
   ld hl,6
   call l_jpix

   ; wait for a key press
   call asm_in_wait_nokey
   call asm_in_wait_key
   call asm_in_wait_nokey

   ; send message STDIO_MSG_ICTL with request IOCTL_OTERM_CLS to output terminal to clear it
   ld a,STDIO_MSG_ICTL
   ld bc,0
   ld de,IOCTL_OTERM_CLS
   ld hl,1
   call l_jpix

   call asm_im2_pop_registers

   ret

scroll_prompt:
   defm "<MORE>"
