
@echo off

set CURRENTDIR=%cd%
set CURRENTPROJECT=genpsw

rmdir /S /Q "%CURRENTDIR%\.vs\"
rmdir /S /Q "%CURRENTDIR%\bin\"
rmdir /S /Q "%CURRENTDIR%\obj\"

del /F /Q "%CURRENTPROJECT%\%CURRENTPROJECT%.aps"
del /F /Q "%CURRENTPROJECT%\%CURRENTPROJECT%.vcxproj.user"
