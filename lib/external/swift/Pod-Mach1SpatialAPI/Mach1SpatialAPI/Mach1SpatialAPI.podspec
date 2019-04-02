#
# Be sure to run `pod lib lint Mach1SpatialAPI.podspec' to ensure this is a
# valid spec before submitting.

Pod::Spec.new do |s|
  s.name             = 'Mach1SpatialAPI'
  s.version          = '0.1.0'
  s.summary          = 'This is the Mach1 Spatial SDK for iOS'

  s.description      = <<-DESC
Mach1 Spatial APIs are all contained in this pod which includes:
    [Mach1EncodeCAPI, Mach1DecodeCAPI, Mach1DecodePositionalCAPI]
                       DESC

  s.homepage         = 'http://www.mach1studios.com'
  # s.screenshots     = 'www.example.com/screenshots_1', 'www.example.com/screenshots_2'
  s.license          = { :type => 'Commercial', :file => 'LICENSE' }
  s.author           = { 'Mach1' => 'http://www.mach1studios.com' }
  s.documentation_url = 'http://dev.mach1.xyz'
  s.source           = { :git => 'https://github.com/Mach1Studios/Mach1SpatialAPI.git', :tag => s.version.to_s }

  s.ios.deployment_target = '8.0'
  
  s.resource_bundles = {
    'Mach1SpatialAPI' => ['Mach1SpatialAPI/Assets/*.aif']
  }

  s.source_files = '../../include/swift/*.swift'
  s.public_header_files = '../../include/swift/*.h'
  s.vendored_libraries = '../../../../../binaries/ios/lib/*.a'
end
