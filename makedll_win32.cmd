@echo off

echo Compiling: release
gcc -m32 -Wall -O1 -s -mtune=core2 -c *.c || goto :error
echo Linking
gcc -m32 -s -shared -static-libgcc -static-libstdc++ -o gsclinq.dll *.o -L..\ -lcom_plugin || goto :error
echo Cleaning up
del *.o

copy "gsclinq.dll" "C:\Program Files\Activision\CoD4\plugins\gsclinq.dll"
copy "_gsclinq.gsc" "C:\Program Files\Activision\CoD4\mods\aspeed_run\lib\_gsclinq.gsc"
goto :EOF

:error
echo Failed with error #%errorlevel%.
exit /b 1
