mkdir -p python/cpp/
swig -c++ -python -outdir python/ -I../ -o python/cpp/Mach1FloatArrayModule_wrap.cxx Mach1FloatArrayModule.i 
swig -c++ -python -outdir python/ -I../ -o python/cpp/Mach1Point3DModule_wrap.cxx Mach1Point3DModule.i 
swig -c++ -python -outdir python/ -I../ -o python/cpp/Mach1Point4DModule_wrap.cxx Mach1Point4DModule.i 
swig -c++ -python -outdir python/ -I../ -o python/cpp/Mach1DecodeModule_wrap.cxx Mach1DecodeModule.i 
swig -c++ -python -outdir python/ -I../ -o python/cpp/Mach1EncodeModule_wrap.cxx Mach1EncodeModule.i 
swig -c++ -python -outdir python/ -I../ -o python/cpp/Mach1DecodePositionalModule_wrap.cxx Mach1DecodePositionalModule.i 
swig -c++ -python -outdir python/ -I../ -o python/cpp/Mach1TranscodeModule_wrap.cxx Mach1TranscodeModule.i 
