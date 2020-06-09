# Mach1 web example deployment

PLAYER = mach1-spatialaudioplayer

# getting OS type
ifeq ($(OS),Windows_NT)
	detected_OS := Windows
else
	detected_OS := $(shell uname)
endif

install: 
	# install all deps

# improve caching when scaled up, apply cache age 0 only to files that change often
player:
	aws s3 sync ../mach1spatial-web  s3://$(PLAYER) --exclude 'node_modules/*' --exclude 'audio/*' --exclude 'video/*' --exclude 'Makefile' --exclude '.DS_Store' --cache-control 'max-age 0,no-cache,no-store,must-revalidate' --metadata-directive REPLACE --profile mach1