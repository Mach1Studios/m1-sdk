mkdir -p jni/java/com/mach1/Mach1SpatialLibs
mkdir -p jni/cpp/mach1
swig -c++ -java -outdir jni/java/com/mach1/Mach1SpatialLibs -I../ -o jni/cpp/mach1/Mach1FloatArrayModule_wrap.cxx -package com.mach1.Mach1SpatialLibs Mach1FloatArrayModule.i 
swig -c++ -java -outdir jni/java/com/mach1/Mach1SpatialLibs -I../ -o jni/cpp/mach1/Mach1Point3DModule_wrap.cxx -package com.mach1.Mach1SpatialLibs Mach1Point3DModule.i 
swig -c++ -java -outdir jni/java/com/mach1/Mach1SpatialLibs -I../ -o jni/cpp/mach1/Mach1Point4DModule_wrap.cxx -package com.mach1.Mach1SpatialLibs Mach1Point4DModule.i 
swig -c++ -java -outdir jni/java/com/mach1/Mach1SpatialLibs -I../ -o jni/cpp/mach1/Mach1DecodeModule_wrap.cxx -package com.mach1.Mach1SpatialLibs Mach1DecodeModule.i 
swig -c++ -java -outdir jni/java/com/mach1/Mach1SpatialLibs -I../ -o jni/cpp/mach1/Mach1EncodeModule_wrap.cxx -package com.mach1.Mach1SpatialLibs Mach1EncodeModule.i 
swig -c++ -java -outdir jni/java/com/mach1/Mach1SpatialLibs -I../ -o jni/cpp/mach1/Mach1DecodePositionalModule_wrap.cxx -package com.mach1.Mach1SpatialLibs Mach1DecodePositionalModule.i 
echo "Removing intermediary modules"
rm -rf jni/java/com/mach1/Mach1SpatialLibs/*Module.java
yes | cp -rf ./jni ../external/java/