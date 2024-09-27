cmake -S . ^
-B .build/windows.x86_64 ^
-A x64 ^
-DCMAKE_VERBOSE_MAKEFILE=ON ^
-DPROJECT_ARCH=x86_64 ^
-DCMAKE_INSTALL_PREFIX:PATH="%cd%/out/windows.x86_64" ^
%*