call Packer.exe -pack Temp\Assets.pack ..\Game\Assets\* ..\Game\Shaders\Cache\*
call Packer.exe -text ..\Game\Assets.inc Temp\Assets.pack

rem del ..\timestamp
md ..\Game\Data
copy ..\Game\Assets\* ..\Game\Data
copy ..\Game\Shaders\Cache\* ..\Game\Data
