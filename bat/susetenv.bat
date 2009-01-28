@echo off
"%~dp0ruby" -x "%~f0" %*
@goto endofruby
#!/bin/ruby
require 'suexec'
path = File::dirname($0).gsub(File::SEPARATOR, File::ALT_SEPARATOR)
ENV['PATH'] = "#{path};#{ENV['PATH']}"
SuExec.exec 'cmd', *['/C', %|"#{File::dirname($0)}\\setenvpath.bat"|]
__END__
:endofruby
