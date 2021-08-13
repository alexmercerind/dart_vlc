#
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html.
# Run `pod lib lint dart_vlc.podspec` to validate before publishing.
#
Pod::Spec.new do |s|
  s.name             = 'dart_vlc'
  s.version          = '0.0.1'
  s.summary          = 'Implementation of dart_vlc for macOS.'
  s.description      = <<-DESC
Implementation of dart_vlc for macOS.
                       DESC
  s.homepage         = 'http://example.com'
  s.license          = { :file => '../LICENSE' }
  s.author           = { 'Your Company' => 'email@example.com' }
  s.source           = { :path => '.' }

  s.dependency 'FlutterMacOS'

  s.dependency 'VLCKit', '~>3.3'

  s.source_files          = 'Classes/**/*', 'src/**/*'
  s.private_header_files  = 'src/**/*.{h,hpp}'
  s.preserve_paths        = 'src/**/*', 'src/*'
  s.header_mappings_dir   = 'src'

  # Currently must be statically linked.
  # https://github.com/CocoaPods/CocoaPods/issues/10859
  s.static_framework = true

  s.platform = :osx, '10.11'
  s.library = 'c++'
  s.pod_target_xcconfig = {
    'DEFINES_MODULE' => 'YES',
    'HEADER_SEARCH_PATHS' => '$(PODS_TARGET_SRCROOT)/src',
    'OTHER_CFLAGS' => [
      '-Wno-documentation',
    ],
    'OTHER_CXXFLAGS' => [
      '-Wno-documentation',
    ],
    'OTHER_LDFLAGS' => [
        '-framework', 'VLCKit.framework',
    ],
    'CLANG_CXX_LANGUAGE_STANDARD' => 'c++17',
    'CLANG_CXX_LIBRARY' => 'libc++',
  }
  s.user_target_xcconfig = {
    'OTHER_LDFLAGS' => [
        # Prevent functions like Player_create, Player_play, etc, being stripped
        # by linker. This should have been done by marking those functions as 
        # `__attribute__((used))` according to clang's documentation. However,
        # they are still stripped due to unkwon reason.
        # TODO: remove this linker flag when the issue is fixed, if possible.
        '-all_load',
    ],
  }
  s.swift_version = '5.0'
end
