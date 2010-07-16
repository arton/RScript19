#!/usr/local/bin/ruby -Ks
# encoding: cp932

require 'fileutils'

$path = Pathname.new(ARGV[0]).shortname
$altpath = $path.gsub(File::ALT_SEPARATOR, File::SEPARATOR)
FileUtils.rm "#{$altpath}/ssleay32.dll"
FileUtils.rm "#{$altpath}/libeay32.dll"
FileUtils.rm "#{$altpath}/../lib/ruby/1.9.1/i386-mswin32_100/openssl.so"
