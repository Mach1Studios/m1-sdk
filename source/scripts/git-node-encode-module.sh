#!/bin/bash

cd ${TRAVIS_BUILD_DIR}/examples/mach1spatial-nodejs/mach1spatial-encode
git add -u ${TRAVIS_BUILD_DIR}/examples/mach1spatial-nodejs/mach1spatial-encode/*
git commit -a -m "recompiled mach1spatial-libs"
git rebase main
git checkout main && git pull
git merge tmp
git push https://mach1-bot:${GITHUB_API_KEY}@${GH_REF_NODE_ENCODE} HEAD:main