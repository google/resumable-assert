Pod::Spec.new do |s|
  s.name        = 'ResumableAssert'
  s.version     = '1.0.0'
  s.authors     = 'Google Inc.'
  s.license     = { :type => 'Apache', :file => 'LICENSE' }
  s.homepage    = 'https://github.com/google/resumable-assert'
  s.source      = { :git => 'https://github.com/google/resumable-assert.git', :tag => s.version }
  s.summary     = 'Assert replacement to continue execution in debugger'
  s.description = <<-DESC

  ResumableAssert allows to ignore or disable assertion failure and continue execution in debugger.
                     DESC

  s.ios.deployment_target  = '9.0'
  s.osx.deployment_target  = '10.10'
  s.tvos.deployment_target = '9.0'
  s.swift_version = '4.0'

  s.prefix_header_file = false
  s.public_header_files = "Sources/#{s.name}/include/**/*.h"
  s.source_files = "Sources/#{s.name}/**/*.{h,m}"
  s.xcconfig = {
    'HEADER_SEARCH_PATHS' => "\"${PODS_TARGET_SRCROOT}/Sources/#{s.name}/include\""
  }
end
