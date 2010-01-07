echo -=- %PACKAGE% -=-
dir /s /b ..\src\common\*.cpp > filelist.txt
dir /s /b ..\include\common\*.h >> filelist.txt
if NOT "%PACKAGE%"=="common" dir /s /b ..\src\%PACKAGE%\*.cpp >> filelist.txt
if NOT "%PACKAGE%"=="common" dir /s /b ..\include\%PACKAGE%\*.h >> filelist.txt
if NOT "%PACKAGE%"=="common" (set MACRO=_) ELSE (set MACRO=gtc_)
..\..\tools\gettext\xgettext -k%MACRO% --c++ -f filelist.txt -p pot -d %PACKAGE%
cd pot
IF EXIST %PACKAGE%.pot del %PACKAGE%.pot
rename %PACKAGE%.po %PACKAGE%.pot
cd ..\
del filelist.txt
