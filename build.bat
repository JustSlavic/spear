@echo off

DEL *.pdb 2>NUL


SET MSVC_FLAGS=/MTd /nologo /permissive- /Zi
SET WARNINGS=/W4 /WX /wd4201 /wd4100 /wd4189 /wd4505 /wd4702 /D_CRT_SECURE_NO_WARNINGS
SET DEFINES=/DDEBUG=1 /DDLL_BUILD=1
SET INCLUDES=/Icode
rem Gdi32.lib has functions:
rem     ChoosePixelFormat, CreateSolidBrush, DescribePixelFormat, SetPixelFormat
rem User32.lib has functions:
rem     RegisterClassA, CreateWindowExA, DestroyWindow, GetSystemMetrics, GetDC, ReleaseDC, AdjustWindowRect, LoadCursorA,
SET LIBS=opengl32.lib Gdi32.lib User32.lib
SET LINKER_FLAGS=/opt:ref /incremental:no

rem   Winmm.lib Xinput.lib D3D11.lib D3DCompiler.lib

echo WAITING FOR PDB > lock.tmp
for /f "delims=" %%i in ('powershell -command "(New-TimeSpan -Start (Get-Date "01/01/1970") -End (Get-Date)).TotalSeconds"') do SET PDB_FILENAME="bin/%%i.pdb"


cl %MSVC_FLAGS% %WARNINGS% %DEFINES% %INCLUDES% /Febin/spear_engine /Tccode/engine/main_win32.c /link /SUBSYSTEM:WINDOWS /PDB:spear.pdb %LINKER_FLAGS% %LIBS%
cl %MSVC_FLAGS% %WARNINGS% %DEFINES% %INCLUDES% /Febin/spear_game /Tccode/game_rpg/game.c /LD /link /PDB:%PDB_FILENAME% %LINKER_FLAGS%

rem clang -std=c89 -g -Wall -Werror -Wno-comment -DDLL_BUILD -Icode -o bin/spear_engine_clang.exe code/engine/main_win32.c -lopengl32 -lGdi32 -lUser32


del lock.tmp
