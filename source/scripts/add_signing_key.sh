#!/bin/sh
pushd ${TRAVIS_BUILD_DIR}/source/scripts/certs
tar xvf secrets.tar
KEY_CHAIN=ios-build.keychain
security create-keychain -p travis $KEY_CHAIN
security default-keychain -s $KEY_CHAIN
security unlock-keychain -p travis $KEY_CHAIN
security set-keychain-settings -t 3600 -u $KEY_CHAIN

wget https://developer.apple.com/certificationauthority/AppleWWDRCA.cer
security import AppleWWDRCA.cer  -k  ~/Library/Keychains/$KEY_CHAIN -T /usr/bin/codesign
security import certificate.cer  -k  ~/Library/Keychains/$KEY_CHAIN -T /usr/bin/codesign
security import certificate.p12  -k  ~/Library/Keychains/$KEY_CHAIN  -P $KEY_PASSWORD -T /usr/bin/codesign
mkdir -p ~/Library/MobileDevice/Provisioning\ Profiles
cp profile.mobileprovision ~/Library/MobileDevice/Provisioning\ Profiles/

security set-key-partition-list -S apple-tool:,apple: -s -k travis $KEY_CHAIN
