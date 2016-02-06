call Packer.exe -pack Temp\Assets.pack ..\Game\Assets\* ..\Game\Shaders\Cache\*
call Packer.exe -text ..\Game\Assets.inc Temp\Assets.pack

rem del /f /s /q ..\Game\Data
md ..\Game\Data
copy ..\Game\Assets\* ..\Game\Data
copy ..\Game\Shaders\Cache\* ..\Game\Data
call Packer.exe -hash ..\Game\Data\hash Temp\Assets.pack
