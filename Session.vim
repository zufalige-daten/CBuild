let SessionLoad = 1
let s:so_save = &g:so | let s:siso_save = &g:siso | setg so=0 siso=0 | setl so=-1 siso=-1
let v:this_session=expand("<sfile>:p")
silent only
silent tabonly
cd ~/Local-Repos/CBuild
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
let s:shortmess_save = &shortmess
if &shortmess =~ 'A'
  set shortmess=aoOA
else
  set shortmess=aoO
endif
badd +9 src/subprocess_handle.cc
badd +210 src/compandlink.cc
badd +1 tests/Make
badd +3 tests/Makefile
badd +1 tests/test_subprocess_handle.c
badd +2 tests/test_subprocess_handle.cc
badd +1 Makefile
badd +32 src/main.cc
badd +16 include/compandlink.hh
badd +6 example/bootmanager/cbuild.make
badd +1 example/kernel/cbuild.make
badd +23 ~/Local-Repos/Tethered-OS/bootmanager/cbuild.make
badd +132 src/parseapp.cc
badd +27 ~/Local-Repos/Tethered-OS/kernel/cbuild.make
badd +19 example/build.sh
badd +26 src/srcstruct.cc
badd +8 example/bootmanager/src/multiboot.asm
badd +3 compile_flags.txt
badd +17 include/main.hh
argglobal
%argdel
$argadd src/subprocess_handle.cc
set stal=2
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabrewind
edit src/subprocess_handle.cc
argglobal
let s:l = 7 - ((6 * winheight(0) + 27) / 55)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 7
normal! 04|
tabnext
edit src/main.cc
argglobal
let s:l = 130 - ((28 * winheight(0) + 27) / 55)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 130
normal! 0
tabnext
edit src/srcstruct.cc
argglobal
let s:l = 40 - ((39 * winheight(0) + 27) / 55)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 40
normal! 0
tabnext
edit Makefile
argglobal
let s:l = 29 - ((28 * winheight(0) + 27) / 55)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 29
normal! 0
tabnext
tabnext
edit example/bootmanager/cbuild.make
argglobal
let s:l = 10 - ((9 * winheight(0) + 27) / 55)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 10
normal! 0
tabnext
edit example/kernel/cbuild.make
argglobal
let s:l = 10 - ((9 * winheight(0) + 27) / 55)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 10
normal! 0
tabnext
edit include/compandlink.hh
argglobal
let s:l = 16 - ((15 * winheight(0) + 27) / 55)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 16
normal! $
tabnext
edit src/compandlink.cc
argglobal
let s:l = 213 - ((54 * winheight(0) + 27) / 55)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 213
normal! 0
tabnext
edit src/parseapp.cc
argglobal
let s:l = 135 - ((30 * winheight(0) + 27) / 55)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 135
normal! 012|
tabnext
edit include/main.hh
argglobal
let s:l = 34 - ((33 * winheight(0) + 27) / 55)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 34
normal! 0
tabnext 5
set stal=1
if exists('s:wipebuf') && len(win_findbuf(s:wipebuf)) == 0 && getbufvar(s:wipebuf, '&buftype') isnot# 'terminal'
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20
let &shortmess = s:shortmess_save
let s:sx = expand("<sfile>:p:r")."x.vim"
if filereadable(s:sx)
  exe "source " . fnameescape(s:sx)
endif
let &g:so = s:so_save | let &g:siso = s:siso_save
set hlsearch
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
