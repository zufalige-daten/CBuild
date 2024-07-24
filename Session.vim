let SessionLoad = 1
let s:so_save = &g:so | let s:siso_save = &g:siso | setg so=0 siso=0 | setl so=-1 siso=-1
let v:this_session=expand("<sfile>:p")
silent only
silent tabonly
cd ~/Desktop/cbuild/cbuild-better
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
let s:shortmess_save = &shortmess
if &shortmess =~ 'A'
  set shortmess=aoOA
else
  set shortmess=aoO
endif
badd +28 src/compandlink.cc
badd +1 src/lexer.cc
badd +123 src/main.cc
badd +19 src/msafe.cc
badd +31 src/parseapp.cc
badd +29 src/srcstruct.cc
badd +1 include/main.hh
badd +10 include/compandlink.hh
badd +2 compile_flags.txt
badd +4 include/lexer.hh
badd +9 include/msafe.hh
badd +6 include/parseapp.hh
badd +5 include/srcstruct.hh
argglobal
%argdel
$argadd src/compandlink.cc
$argadd src/lexer.cc
$argadd src/main.cc
$argadd src/msafe.cc
$argadd src/parseapp.cc
$argadd src/srcstruct.cc
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
tabnew +setlocal\ bufhidden=wipe
tabrewind
edit src/compandlink.cc
argglobal
let s:l = 47 - ((42 * winheight(0) + 23) / 47)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 47
normal! 0
tabnext
edit include/compandlink.hh
argglobal
if bufexists(fnamemodify("include/compandlink.hh", ":p")) | buffer include/compandlink.hh | else | edit include/compandlink.hh | endif
if &buftype ==# 'terminal'
  silent file include/compandlink.hh
endif
let s:l = 11 - ((10 * winheight(0) + 23) / 47)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 11
normal! 0
tabnext
edit src/lexer.cc
argglobal
2argu
balt src/compandlink.cc
let s:l = 32 - ((31 * winheight(0) + 23) / 47)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 32
normal! 0
tabnext
edit include/lexer.hh
argglobal
1argu
if bufexists(fnamemodify("include/lexer.hh", ":p")) | buffer include/lexer.hh | else | edit include/lexer.hh | endif
if &buftype ==# 'terminal'
  silent file include/lexer.hh
endif
let s:l = 17 - ((16 * winheight(0) + 23) / 47)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 17
normal! 0
tabnext
edit src/main.cc
argglobal
3argu
balt src/compandlink.cc
let s:l = 9 - ((8 * winheight(0) + 23) / 47)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 9
normal! 02|
tabnext
edit include/main.hh
argglobal
1argu
if bufexists(fnamemodify("include/main.hh", ":p")) | buffer include/main.hh | else | edit include/main.hh | endif
if &buftype ==# 'terminal'
  silent file include/main.hh
endif
balt src/compandlink.cc
let s:l = 27 - ((26 * winheight(0) + 23) / 47)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 27
normal! 0
tabnext
edit src/msafe.cc
argglobal
4argu
balt src/compandlink.cc
let s:l = 14 - ((13 * winheight(0) + 23) / 47)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 14
normal! 0
tabnext
edit include/msafe.hh
argglobal
if bufexists(fnamemodify("include/msafe.hh", ":p")) | buffer include/msafe.hh | else | edit include/msafe.hh | endif
if &buftype ==# 'terminal'
  silent file include/msafe.hh
endif
balt include/msafe.hh
let s:l = 12 - ((11 * winheight(0) + 23) / 47)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 12
normal! 0
tabnext
edit src/parseapp.cc
argglobal
5argu
balt src/compandlink.cc
let s:l = 1 - ((0 * winheight(0) + 23) / 47)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 1
normal! 0
tabnext
edit include/parseapp.hh
argglobal
if bufexists(fnamemodify("include/parseapp.hh", ":p")) | buffer include/parseapp.hh | else | edit include/parseapp.hh | endif
if &buftype ==# 'terminal'
  silent file include/parseapp.hh
endif
balt include/parseapp.hh
let s:l = 6 - ((5 * winheight(0) + 23) / 47)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 6
normal! 020|
tabnext
edit src/srcstruct.cc
argglobal
6argu
balt src/compandlink.cc
let s:l = 1 - ((0 * winheight(0) + 23) / 47)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 1
normal! 0
tabnext
edit include/srcstruct.hh
argglobal
if bufexists(fnamemodify("include/srcstruct.hh", ":p")) | buffer include/srcstruct.hh | else | edit include/srcstruct.hh | endif
if &buftype ==# 'terminal'
  silent file include/srcstruct.hh
endif
balt include/srcstruct.hh
let s:l = 5 - ((4 * winheight(0) + 23) / 47)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 5
normal! 018|
tabnext 3
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
