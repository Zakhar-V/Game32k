crinkler.exe /OUT:%3 /PRINT:IMPORTS /HASHSIZE:10 /LARGEADDRESSAWARE:NO /SUBSYSTEM:WINDOWS /ENTRY:main /COMPMODE:FAST kernel32.lib user32.lib gdi32.lib msvcrt.lib opengl32.lib d3d9.lib %1
kkrunchy_k7.exe --new --out %4 %2
del /q %5
upx.exe -9 -f -v -o %5 %2

