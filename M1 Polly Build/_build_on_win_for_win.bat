python ../../../polly-master/bin/polly.py --clear --install --config Release --toolchain vs-14-2015
python ../../../polly-master/bin/polly.py --clear --install --config Release --toolchain vs-14-2015-win64

del /f /s /q _win 1>nul
rmdir /s /q _win

mkdir _win
cd _win

mkdir x86
mkdir x64

cd %~dp0

xcopy _install\vs-14-2015\lib _win\x86
xcopy _install\vs-14-2015-win64\lib _win\x64
pause
