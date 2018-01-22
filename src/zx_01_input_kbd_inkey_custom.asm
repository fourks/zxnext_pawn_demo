;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; ZX_01_INPUT_KBD_INKEY_CUSTOM
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; This input terminal driver subclasses the zx_01_input_kbd_inkey driver.
; It is intended to be connected to an fzx output terminal and limits the
; input to one line.
;
; ;;;;;;;;;;;;;;;;;;;;
; DRIVER CLASS DIAGRAM
; ;;;;;;;;;;;;;;;;;;;;
;
; CONSOLE_01_INPUT_TERMINAL (root, abstract)
; ZX_01_INPUT_KBD_INKEY (concrete)
; ZX_01_INPUT_KBD_INKEY_CUSTOM (concrete)

SECTION code_driver
SECTION code_driver_terminal_input

PUBLIC zx_01_input_kbd_inkey_custom

EXTERN ITERM_MSG_REJECT
EXTERN zx_01_input_kbd_inkey

zx_01_input_kbd_inkey_custom:

   ; let parent class handle other messages than ITERM_MSG_REJECT
   cp ITERM_MSG_REJECT
   jp nz, zx_01_input_kbd_inkey

iterm_msg_reject:

   ; ITERM_MSG_REJECT:
   ; Indicate whether typed character should be rejected.
   ;
   ;   enter: ix = FDSTRUCT.JP *input_terminal
   ;          c = ascii code
   ;    exit: carry reset indicates the character should be rejected.
   ; can use: af, bc, de, hl

   ; unconditionally accept enter
   ld a,c
   cp '\n'
   scf
   ret z

   ; hl = FDSTRUCT *output_terminal (an fzx output terminal)
   ; at offset 14 in FDSTRUCT.JP *input_terminal
IF __SDCC_IY
   ld l,(iy+14)
   ld h,(iy+15)
ELSE
   ld l,(ix+14)
   ld h,(ix+15)
ENDIF

   ; if there is no output terminal return indicating char is accepted
   ld a,h
   or l
   scf
   ret z

   ; paper.width is at offset 41 in the fzx driver + 3 more because
   ; the output_terminal points at a lock, see tshr_01_output_fzx.asm.
   ; bc = paper.width - 12 as the x limit. The width of the cursor is
   ; assumed to be 12 pixels.

   ld bc,41+3
   add hl,bc

   ld a,(hl)
   inc hl
   ld h,(hl)
   ld l,a

   ld bc,12
   sbc hl,bc

   ld b,h
   ld c,l

   ; hl = FDSTRUCT *output_terminal
IF __SDCC_IY
   ld l,(iy+14)
   ld h,(iy+15)
ELSE
   ld l,(ix+14)
   ld h,(ix+15)
ENDIF

   ; current x pixel coordinate is at offset 35 + 3 in the fzx driver
   ; hl = current x pixel coordinate (relative the left edge of the window)

   ld de,35+3
   add hl,de

   ld a,(hl)
   inc hl
   ld h,(hl)
   ld l,a

   ; set carry flag if current x pixel coordinate < paper.width
   sbc hl,bc

   ret
