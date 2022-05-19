#!/bin/bash

# Git push main repo
#cd ${TRAVIS_BUILD_DIR}
# add compiled library changes
git add -u ./mach1spatial-libs/*
git add -u ./executables/*
# add updated submodules
git add -u ./examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI
git add -u ./examples/mach1spatial-c/ios/Pod-Mach1SpatialAPI
git add -u ./examples/mach1spatial-c/Unity/Unity-Mach1SpatialAPI
git add -u ./examples/mach1spatial-c/Unreal\ Engine/UE-Mach1SpatialAPI
git add -u ./examples/mach1spatial-web/m1-web-spatialaudioplayer
git add -u ./examples/mach1spatial-nodejs/mach1spatial-decode
git add -u ./examples/mach1spatial-nodejs/mach1spatial-encode
git add -u ./examples/mach1spatial-nodejs/mach1spatial-transcode
git commit -a -m "recompiled mach1spatial-libs"
git rebase master
git checkout master && git pull
git merge tmp
git push https://mach1-bot:${GITHUB_API_KEY}@${GH_REF} HEAD:master