rem call Packer.exe -pack Temp\Assets.pack ..\Game\Assets\* ..\Game\Shaders\*
call Packer.exe -pack Temp\Assets.pack ..\Game\Shaders\*
call Packer.exe -text ..\Game\Assets.inc Temp\Assets.pack

rem del /f /s /q ..\Game\Data
md ..\Game\Data
rem copy ..\Game\Assets\* ..\Game\Data
copy ..\Game\Shaders\* ..\Game\Data
call Packer.exe -hash ..\Game\Data\hash Temp\Assets.pack
