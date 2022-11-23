#!/bin/bash

if [[ "$PWD" == *mach1spatial-transcode ]]
then
	echo "Script called from correct path: $PWD"
	git add -u ./*
	git commit -a -m "recompiled mach1spatial-libs"
	git rebase main
	git checkout main && git pull
	git merge tmp
	git push https://mach1-bot:${GITHUB_API_KEY}@${GH_REF_NODE_TRANSCODE} HEAD:main
else
	echo "ERROR: Script called from wrong directory: $PWD"
fi