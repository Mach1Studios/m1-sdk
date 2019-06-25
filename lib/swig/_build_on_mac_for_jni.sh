mkdir -p jni/java/com/Mach1
mkdir -p jni/cpp/Mach1
swig -c++ -java -outdir jni/java/com/Mach1 -I../ -o jni/cpp/Mach1/FloatArrayModule_wrap.cxx -package com.Mach1 FloatArrayModule.i 
swig -c++ -java -outdir jni/java/com/Mach1 -I../ -o jni/cpp/Mach1/Mach1Point3DModule_wrap.cxx -package com.Mach1 Mach1Point3DModule.i 
swig -c++ -java -outdir jni/java/com/Mach1 -I../ -o jni/cpp/Mach1/Mach1Point4DModule_wrap.cxx -package com.Mach1 Mach1Point4DModule.i 
swig -c++ -java -outdir jni/java/com/Mach1 -I../ -o jni/cpp/Mach1/Mach1DecodeModule_wrap.cxx -package com.Mach1 Mach1DecodeModule.i 
swig -c++ -java -outdir jni/java/com/Mach1 -I../ -o jni/cpp/Mach1/Mach1EncodeModule_wrap.cxx -package com.Mach1 Mach1EncodeModule.i 
swig -c++ -java -outdir jni/java/com/Mach1 -I../ -o jni/cpp/Mach1/Mach1DecodePositionalModule_wrap.cxx -package com.Mach1 Mach1DecodePositionalModule.i 
echo "Removing intermediary modules"
rm -rf jni/java/com/Mach1/*Module.java