@echo off

DEL *.pdb 2>NUL


SET MSVC_FLAGS=/std:c++14 /MTd /nologo /GR- /Oi /Zi /EHa-
SET LINKER_FLAGS=/opt:ref /incremental:no
SET WARNINGS=/W4 /WX /wd4201 /wd4100 /wd4189 /wd4505 /wd4702 /D_CRT_SECURE_NO_WARNINGS
SET DEFINES=/DDEBUG=1 /DOS_WINDOWS=1 /DBYTE_ORDER=1234
SET INCLUDES=/I../code
SET LIBS=User32.lib Gdi32.lib Winmm.lib Xinput.lib opengl32.lib


IF NOT EXIST build mkdir build


cl %MSVC_FLAGS% %WARNINGS% %DEFINES% %INCLUDES% /Fespear ../code/main_win32.cpp /link /PDB:spear.pdb %LINKER_FLAGS% %LIBS%
