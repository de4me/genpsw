
@echo off

set CURRENTDIR=%cd%

rmdir /S /Q "%CURRENTDIR%\.vs\"
rmdir /S /Q "%CURRENTDIR%\bin\"
rmdir /S /Q "%CURRENTDIR%\obj\"

del /F /Q "genpsw\genpsw.aps"
del /F /Q "genpsw\genpsw.vcxproj.user"
