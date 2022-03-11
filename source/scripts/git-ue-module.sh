#!/bin/bash

cd ${TRAVIS_BUILD_DIR}/examples/mach1spatial-c/Unreal\ Engine/UE-Mach1SpatialAPI
git add ${TRAVIS_BUILD_DIR}/examples/mach1spatial-c/Unreal\ Engine/UE-Mach1SpatialAPI/*
git commit -a -m "recompiled mach1spatial-libs"
git rebase master
git checkout master && git pull
git merge tmp
git push https://mach1-bot:${GITHUB_API_KEY}@${GH_REF_UE} HEAD:master