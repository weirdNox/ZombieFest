@echo off

taskkill /im win32_zfest.exe >NUL 2>NUL
IF NOT EXIST "%~dp0\..\build" mkdir "%~dp0\..\build"
pushd "%~dp0\..\build"

REM Set variables
set PLATFORM=amd64
set BUILD_TYPE=debug
set TOOLSET_LOCATION=C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC
if exist "%~dp0\env.bat" call "%~dp0\env.bat"

REM Load compiler
call "%TOOLSET_LOCATION%\vcvarsall.bat" %PLATFORM%

REM Compile with correct flags
set compilerFlags=/nologo /fp:fast /FC /Gm- /GR- /EHa- /Oi /W4 /wd4505 /wd4201 /wd4100 /DPLATFORM_WIN=1

if /i %BUILD_TYPE% == debug (
    set compilerFlags=%compilerFlags% /MTd /Zi /Ob0 /Od /RTC1 /DEBUG /DDEBUG_BUILD=1
)
if /i %BUILD_TYPE% == relDeb (
    set compilerFlags=%compilerFlags% /MT /Zo /Zi /O2 /Ob1 /DNDEBUG /DRELDEB_BUILD=1
)
if /i %BUILD_TYPE% == release (
    set compilerFlags=%compilerFlags% /MT /O2 /Ob2 /DNDEBUG /DRELEASE_BUILD=1
)

del *.pdb >NUL 2>NUL

rem echo WAITING FOR PDB > lock.tmp
rem cl ..\src\zfest.cpp %compilerFlags% /Fmzfest.map /LD /link /incremental:no /opt:ref /PDB:zfest_%random%.pdb
rem del lock.tmp

cl ..\src\win32_zfest.cpp %compilerFlags% /Fmwin32_zfest.map /link /incremental:no /opt:ref user32.lib gdi32.lib winmm.lib opengl32.lib

popd
