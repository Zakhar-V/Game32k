crinkler.exe /OUT:%2 /PRINT:IMPORTS /HASHSIZE:10 /LARGEADDRESSAWARE:NO /SUBSYSTEM:WINDOWS /ENTRY:main /COMPMODE:SLOW kernel32.lib user32.lib gdi32.lib msvcrt.lib d3d9.lib %1
del /q %4
upx.exe -9 -f -v -o %4 %3
