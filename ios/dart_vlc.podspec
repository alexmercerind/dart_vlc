#
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html.
# Run `pod lib lint dart_vlc.podspec` to validate before publishing.
#
Pod::Spec.new do |s|
    s.name             = 'dart_vlc'
    s.version          = '0.0.1'
    s.summary          = 'A new flutter plugin project.'
    s.description      = <<-DESC
  A new flutter plugin project.
                         DESC
    s.homepage         = 'http://example.com'
    s.license          = { :file => '../LICENSE' }
    s.author           = { 'Your Company' => 'email@example.com' }
    s.script_phases     = [{
        :name => 'Fetch submodules... only needed if using git in pubspec.yaml',
        :show_env_vars_in_log => true,
        :script => 'cd ${PODS_TARGET_SRCROOT}/.. && git submodule update --init --recursive || true',
        :execution_position => :before_compile
    }, {
        :name => 'Build Simulator lib',
        :show_env_vars_in_log => true,
        :script => 'cmake -Bdartvlc_core '\
        '${PODS_TARGET_SRCROOT}/../dartvlc '\
        '-DCMAKE_INSTALL_PREFIX:PATH=${PODS_TARGET_SRCROOT}/deps '\
        '-GXcode '\
        '-DCMAKE_SYSTEM_NAME=iOS && '\
        'pwd && '\
        'xcodebuild -project dartvlc_core/dart_vlc_core.xcodeproj '\
        '-sdk iphonesimulator '\
        '-scheme dart_vlc_core '\
        'CONFIGURATION_BUILD_DIR=${PODS_TARGET_SRCROOT}/deps/simulator',
        :execution_position => :before_compile
    }, {
        :name => 'Build Device lib',
        :show_env_vars_in_log => true,
        :script => 'xcodebuild -project dartvlc_core/dart_vlc_core.xcodeproj '\
        '-sdk iphoneos '\
        '-scheme dart_vlc_core '\
        'CONFIGURATION_BUILD_DIR=${PODS_TARGET_SRCROOT}/deps/device',
        :execution_position => :before_compile
    }]
    s.source           = { :path => '.' }
    s.source_files     = 'Classes/**/*'
    s.dependency 'Flutter'
    s.dependency 'MobileVLCKit', '~>3.3.0'
    s.platform = :ios, '9'
    s.library = 'c++'
    s.pod_target_xcconfig = {
      'DEFINES_MODULE' => 'YES',
      'LIBRARY_SEARCH_PATHS[sdk=iphoneos*]' => '$(inherited) $(PODS_TARGET_SRCROOT)/deps/device',
      'OTHER_LDFLAGS[sdk=iphoneos*]' => '$(inherited) -ldart_vlc_core -Wl,-force_load,${PODS_TARGET_SRCROOT}/deps/device/libdart_vlc_core.a',
      'LIBRARY_SEARCH_PATHS[sdk=iphonesimulator*]' => '$(inherited) $(PODS_TARGET_SRCROOT)/deps/simulator',
      'OTHER_LDFLAGS[sdk=iphonesimulator*]' => '$(inherited) -ldart_vlc_core -Wl,-force_load,${PODS_TARGET_SRCROOT}/deps/simulator/libdart_vlc_core.a',
      'CLANG_CXX_LANGUAGE_STANDARD' => 'c++17',
      'CLANG_CXX_LIBRARY' => 'libc++',
    }
    s.swift_version = '5.0'
  end