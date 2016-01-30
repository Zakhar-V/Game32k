del /f /s /q Bin\Debug 
rmdir /s /q Bin\Debug

del /f /s /q Build\Temp 
rmdir /s /q Build\Temp
del /q Build\Packer.exe 

del /f /s /q Packer\Temp 
rmdir /s /q Packer\Temp
 
del /f /s /q Game\Data 
rmdir /s /q Game\Data

del /f /s /q Game\Shaders\Cache 
rmdir /s /q Game\Shaders\Cache 

del /q Game\log.txt 

rem pause