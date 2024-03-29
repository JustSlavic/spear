@echo off

DEL *.pdb 2>NUL


SET MSVC_FLAGS=/std:c++17 /MTd /nologo /GR- /Oi /Zi /EHa-
SET LINKER_FLAGS=/opt:ref /incremental:no
SET WARNINGS=/W4 /WX /wd4201 /wd4100 /wd4189 /wd4505 /wd4702 /D_CRT_SECURE_NO_WARNINGS
SET DEFINES=/DDEBUG=1 /DOS_WINDOWS=1 /DARCH_64BIT=1 /DBYTE_ORDER=1234 /DUI_EDITOR_ENABLED=1 /DDLL_BUILD=1
SET INCLUDES=/I../code /I../code/based
SET LIBS=User32.lib Gdi32.lib Winmm.lib Xinput.lib opengl32.lib D3D11.lib D3DCompiler.lib


echo WAITING FOR PDB > lock.tmp
for /f "delims=" %%i in ('powershell -command "(New-TimeSpan -Start (Get-Date "01/01/1970") -End (Get-Date)).TotalSeconds"') do SET PDB_FILENAME="%%i.pdb"

cl %MSVC_FLAGS% %WARNINGS% %DEFINES% %INCLUDES% /Fespear ../code/main_win32.cpp /link /PDB:spear.pdb %LINKER_FLAGS% %LIBS%
cl %MSVC_FLAGS% %WARNINGS% %DEFINES% %INCLUDES% /Fegame ../code/game_stub.cpp /LD /link /PDB:%PDB_FILENAME% %LINKER_FLAGS%

del lock.tmp

rem cl %MSVC_FLAGS% %WARNINGS% %DEFINES% %INCLUDES% /Fegame ../code/geometric_algebra_visualized/app.cpp /LD /link /PDB:%PDB_FILENAME% %LINKER_FLAGS%
rem cl %MSVC_FLAGS% %WARNINGS% %DEFINES% %INCLUDES% /Fegame ../code/game_platformer/game.cpp /LD /link /PDB:%PDB_FILENAME% %LINKER_FLAGS%
