   ;# DRIVER INSTANTIATION
   ;# Sets up an input terminal and two Timex hi-res FZX output terminals.
   ;# The input terminal is connected to the output terminal fd=1.

   EXTERN _ff_ao_Prefect

   ;# fd=0, stdin, input terminal with 256 characters edit buffer
   include(`src/zx_01_input_kbd_inkey_custom.m4')dnl
   m4_zx_01_input_kbd_inkey_custom(_stdin, __i_fcntl_fdstruct_1, CRT_ITERM_TERMINAL_FLAGS, 256, CRT_ITERM_INKEY_DEBOUNCE, CRT_ITERM_INKEY_REPEAT_START, CRT_ITERM_INKEY_REPEAT_RATE)dnl

   ;# fd=1, window_in, tshr fzx output terminal
   include(`driver/terminal/tshr_01_output_fzx.m4')dnl
   m4_tshr_01_output_fzx(_window_in, 0x2030, 0, 0, 0, 64, 23, 1, 0, _ff_ao_Prefect, 0, 512, 184, 8, M4__CRT_OTERM_FZX_DRAW_MODE, CRT_OTERM_FZX_LINE_SPACING, CRT_OTERM_FZX_LEFT_MARGIN, CRT_OTERM_FZX_SPACE_EXPAND)dnl

   ;# fd=2, window_out, tshr fzx output terminal
   include(`src/tshr_01_output_fzx_custom.m4')dnl
   m4_tshr_01_output_fzx_custom(_window_out, 0x2030, 0, 0, 0, 64, 17, 6, 0, _ff_ao_Prefect, 0, 512, 136, 48, M4__CRT_OTERM_FZX_DRAW_MODE, CRT_OTERM_FZX_LINE_SPACING, CRT_OTERM_FZX_LEFT_MARGIN, CRT_OTERM_FZX_SPACE_EXPAND)dnl
