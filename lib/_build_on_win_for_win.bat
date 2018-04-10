python ../../polly-master/bin/polly.py --clear --install --config Release --toolchain vs-14-2015
python ../../polly-master/bin/polly.py --clear --install --config Release --toolchain vs-14-2015-win64
python ../../polly-master/bin/polly.py --clear --install --config Release --toolchain vs-15-2017
python ../../polly-master/bin/polly.py --clear --install --config Release --toolchain vs-15-2017-win64

del /f /s /q _win 1>nul
rmdir /s /q _win

mkdir _win
cd _win

mkdir -p vs14\x86
mkdir -p vs14\x64
mkdir -p vs15\x86
mkdir -p vs15\x64

cd %~dp0

xcopy _install\vs-14-2015\lib _win\vs14\x86
xcopy _install\vs-14-2015-win64\lib _win\vs14\x64
xcopy _install\vs-15-2017\lib _win\vs15\x86
xcopy _install\vs-15-2017-win64\lib _win\vs15\x64

pause
