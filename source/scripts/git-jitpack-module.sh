#!/bin/bash

cd ${TRAVIS_BUILD_DIR}/examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI
git add -u ${TRAVIS_BUILD_DIR}/examples/mach1spatial-c/android/JitPack-Mach1SpatialAPI/*
git commit -a -m "recompiled mach1spatial-libs"
git rebase master
git checkout master && git pull
git merge tmp
git push https://mach1-bot:${GITHUB_API_KEY}@${GH_REF_JITPACK} HEAD:master