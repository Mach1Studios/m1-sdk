#!/bin/sh

if [ -z "$IOS_IDENTITY_KEY" ] || [ -z "$IOS_IDENTITY_CERT" ] || [ -z "$IOS_IDENTITY_PROV" ] || [ -z "$IOS_IDENTITY_KEY_PASSWORD" ] ; then
	echo "Export identity key and set path to it at IOS_IDENTITY_KEY IOS_IDENTITY_CERT and IOS_IDENTITY_KEY_PASSWORD env"
	exit 40;
fi
xcode-select --install

travis login --pro -X --github-token ${TRAVIS_GITHUB_TOKEN}
#ios login

echo "Working directory for secret files is `pwd`"

#ios profiles:download --type development
#mv *.mobileprovision profile.mobileprovision
#ios certificates:download --type development
#mv *.cer certificate.cer
cp $IOS_IDENTITY_PROV profile.mobileprovision
cp $IOS_IDENTITY_CERT certificate.cer
cp $IOS_IDENTITY_KEY certificate.p12
PROV_UUID=`/usr/libexec/PlistBuddy -c "Print UUID" /dev/stdin <<< $(security cms -D -i profile.mobileprovision)`
echo "Your provisioning profile id: $PROV_UUID"

CODE_SIGNING_ARGS="CODE_SIGN_IDENTITY=\"iPhone Developer\" PROVISIONING_PROFILE=\"$PROV_UUID\""
echo "Proper parameters for xctool/xcodebuild: $CODE_SIGNING_ARGS"

tar cvf secrets.tar profile.mobileprovision certificate.p12 certificate.cer
mv secrets.tar certs/secrets.tar
travis encrypt KEY_PASSWORD="$IOS_IDENTITY_KEY_PASSWORD" --add
travis encrypt-file certs/secrets.tar certs/secrets.tar.enc --add
rm certs/secrets.tar

# cat > source/scripts/add_signing_key.sh <<- EOM
# #!/bin/sh
# pushd ./source/scripts
# tar xvf secrets.tar
# KEY_CHAIN=ios-build.keychain
# security create-keychain -p travis \$KEY_CHAIN
# security default-keychain -s \$KEY_CHAIN
# security unlock-keychain -p travis \$KEY_CHAIN
# security set-keychain-settings -t 3600 -u \$KEY_CHAIN

# wget https://developer.apple.com/certificationauthority/AppleWWDRCA.cer
# security import AppleWWDRCA.cer  -k  ~/Library/Keychains/\$KEY_CHAIN -T /usr/bin/codesign
# security import certificate.cer  -k  ~/Library/Keychains/\$KEY_CHAIN -T /usr/bin/codesign
# security import certificate.p12  -k  ~/Library/Keychains/\$KEY_CHAIN  -P \$KEY_PASSWORD -T /usr/bin/codesign
# mkdir -p ~/Library/MobileDevice/Provisioning\ Profiles
# cp profile.mobileprovision ~/Library/MobileDevice/Provisioning\ Profiles/
# EOM
# chmod +x source/scripts/add_signing_key.sh

# cat > source/scripts/remove_signing_key.sh <<- EOM
# #!/bin/sh
# security delete-keychain ios-build.keychain
# rm -f ~/Library/MobileDevice/Provisioning\ Profiles/*
# EOM
# chmod +x source/scripts/remove_signing_key.sh

echo "Add to your .travis.yml:"
echo ""
echo "script: xctool  <your parameters> -sdk iphoneos $CODE_SIGNING_ARGS"
echo "before_script:"
echo "- ./source/scripts/add_signing_key.sh"
echo "after_script:"
echo "- ./source/scripts/remove_signing_key.sh"
echo ""
