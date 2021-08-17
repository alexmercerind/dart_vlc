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
    :name => 'Build common lib',
    :show_env_vars_in_log => true,
    :script => 'cmake -Bdartvlc_core ${PODS_TARGET_SRCROOT}/../dartvlc -DCMAKE_INSTALL_PREFIX:PATH=${PODS_TARGET_SRCROOT}/deps && pwd && make -C dartvlc_core install',
    :execution_position => :before_compile
  },
  {
    :name => 'Change VLCKit ID',
    :show_env_vars_in_log => true,
    :script => 'echo ${PODS_ROOT} && install_name_tool -id "@executable_path/../Frameworks/VLCKit.framework/Versions/A/VLCKit" ${PODS_ROOT}/VLCKit/VLCKit.framework/VLCKit',
    :execution_position => :before_compile
  }]
  s.source           = { :path => '.' }
  s.source_files     = 'Classes/**/*'
  s.dependency 'FlutterMacOS'
  s.dependency 'VLCKit', '~>3.3'
  s.platform = :osx, '10.11'
  s.library = 'c++'
  s.pod_target_xcconfig = {
    'DEFINES_MODULE' => 'YES',
    'LIBRARY_SEARCH_PATHS' => '$(PODS_TARGET_SRCROOT)/deps/lib',
    'OTHER_LDFLAGS' => [
      '-ldart_vlc_core',
      '-Wl,-force_load,${PODS_TARGET_SRCROOT}/deps/lib/libdart_vlc_core.a',
    ],
    'CLANG_CXX_LANGUAGE_STANDARD' => 'c++17',
    'CLANG_CXX_LIBRARY' => 'libc++',
  }
  s.swift_version = '5.0'
end
