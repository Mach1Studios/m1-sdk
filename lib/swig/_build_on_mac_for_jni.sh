mkdir -p jni/java/com/mach1/spatiallibs
mkdir -p jni/cpp/mach1
swig -c++ -java -outdir jni/java/com/mach1/spatiallibs -I../ -o jni/cpp/mach1/Mach1FloatArrayModule_wrap.cxx -package com.mach1.spatiallibs Mach1FloatArrayModule.i 
swig -c++ -java -outdir jni/java/com/mach1/spatiallibs -I../ -o jni/cpp/mach1/Mach1Point3DModule_wrap.cxx -package com.mach1.spatiallibs Mach1Point3DModule.i 
swig -c++ -java -outdir jni/java/com/mach1/spatiallibs -I../ -o jni/cpp/mach1/Mach1Point4DModule_wrap.cxx -package com.mach1.spatiallibs Mach1Point4DModule.i 
swig -c++ -java -outdir jni/java/com/mach1/spatiallibs -I../ -o jni/cpp/mach1/Mach1DecodeModule_wrap.cxx -package com.mach1.spatiallibs Mach1DecodeModule.i 
swig -c++ -java -outdir jni/java/com/mach1/spatiallibs -I../ -o jni/cpp/mach1/Mach1EncodeModule_wrap.cxx -package com.mach1.spatiallibs Mach1EncodeModule.i 
swig -c++ -java -outdir jni/java/com/mach1/spatiallibs -I../ -o jni/cpp/mach1/Mach1TranscodeModule_wrap.cxx -package com.mach1.spatiallibs Mach1TranscodeModule.i 
swig -c++ -java -outdir jni/java/com/mach1/spatiallibs -I../ -o jni/cpp/mach1/Mach1DecodePositionalModule_wrap.cxx -package com.mach1.spatiallibs Mach1DecodePositionalModule.i 
echo "Removing intermediary modules"
rm -rf jni/java/com/mach1/spatiallibs/*Module.java
yes | cp -rf ./jni ../external/java/