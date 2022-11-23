#!/bin/bash

if [[ "$PWD" == *Unity-Mach1SpatialAPI ]]
then
	echo "Script called from correct path: $PWD"
	git add -u ./*
	git commit -a -m "recompiled mach1spatial-libs"
	git rebase master
	git checkout master && git pull
	git merge tmp
	git push https://mach1-bot:${GITHUB_API_KEY}@${GH_REF_UNITY} HEAD:master
else
	echo "ERROR: Script called from wrong directory: $PWD"
fi