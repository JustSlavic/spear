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
for /f "delims=" %%i in ('powershell -command "(New-TimeSpan -Start (Get-Date "01/01/1970") -End (Get-Date)).TotalSeconds"') do SET PDB_FILENAME="%%i.pdb"


cl %MSVC_FLAGS% %WARNINGS% %DEFINES% %INCLUDES% /Febin/spear_engine /Tccode/engine/main_win32.c /link /SUBSYSTEM:WINDOWS /PDB:spear.pdb %LINKER_FLAGS% %LIBS%


rem cl %MSVC_FLAGS% %WARNINGS% %DEFINES% %INCLUDES% /Fespear ../code/main_win32.cpp /link /PDB:spear.pdb %LINKER_FLAGS% %LIBS%
rem cl %MSVC_FLAGS% %WARNINGS% %DEFINES% %INCLUDES% /Fegame ../code/game/game.cpp /LD /link /PDB:%PDB_FILENAME% %LINKER_FLAGS%

rem cl /nologo /Fetest_pointers ../code/main_test.cpp

del lock.tmp

rem cl %MSVC_FLAGS% %WARNINGS% %DEFINES% %INCLUDES% /Fegame ../code/geometric_algebra_visualized/app.cpp /LD /link /PDB:%PDB_FILENAME% %LINKER_FLAGS%
rem cl %MSVC_FLAGS% %WARNINGS% %DEFINES% %INCLUDES% /Fegame ../code/game_platformer/game.cpp /LD /link /PDB:%PDB_FILENAME% %LINKER_FLAGS%
    
