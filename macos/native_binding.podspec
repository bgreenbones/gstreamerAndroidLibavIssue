#
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html.
# Run `pod lib lint native_binding.podspec` to validate before publishing.
#


Pod::Spec.new do |s|
  s.name             = 'native_binding'
  s.version          = '0.0.1'
  s.summary          = 'A new Flutter FFI plugin project.'
  s.description      = <<-DESC
A new Flutter FFI plugin project.
                       DESC
  s.homepage         = 'http://example.com'
  s.license          = { :file => '../LICENSE' }
  s.author           = { 'Your Company' => 'email@example.com' }

  # This will ensure the source files in Classes/ are included in the native
  # builds of apps using this FFI plugin. Podspec does not support relative
  # paths, so Classes contains a forwarder C file that relatively imports
  # `../src/*` so that the C sources can be shared among all target platforms.
  s.source           = { :path => '.' }
  s.source_files     = 'Classes/**/*'
  s.dependency 'FlutterMacOS'

  s.platform = :osx, '10.11'
  s.pod_target_xcconfig = { 'DEFINES_MODULE' => 'YES',
   'HEADER_SEARCH_PATHS' => ['"/Library/Frameworks/GStreamer.framework/Headers"',
                            '"/opt/homebrew/Cellar/sdl2/2.28.5/include"'],
  #  '"/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include"'],
        # '"/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks/Kernel.framework/Versions/A/Headers"']
   'LD_RUNPATH_SEARCH_PATHS' => ['"/Library/Frameworks"','"/Library/Frameworks/GStreamer.framework/Libraries"',
                                  '"/opt/homebrew/Cellar/sdl2/2.28.5/lib"'],
   'OTHER_LDFLAGS' => ['" -L/Library/Frameworks/GStreamer.framework/Libraries -F/Library/Frameworks -framework GStreamer "',
                '" -L/opt/homebrew/lib -lSDL2 "']
  #  'HEADER_SEARCH_PATHS' => ['"/Library/Frameworks/GStreamer.framework/Headers"', '"/opt/homebrew/Cellar/sdl2/2.28.5/include"'],
  #  'LD_RUNPATH_SEARCH_PATHS' => ['"/Library/Frameworks"','"/Library/Frameworks/GStreamer.framework/Libraries"', '"/opt/homebrew/Cellar/sdl2/2.28.5/lib"'],
  #  'OTHER_LDFLAGS' => ['" -L/Library/Frameworks/GStreamer.framework/Libraries -F/Library/Frameworks -framework GStreamer "', '" -L/opt/homebrew/lib -lSDL2main -lSDL2 "']
  }
  s.swift_version = '5.0'
end
