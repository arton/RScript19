#!/usr/local/bin/ruby -Ks
require 'winpath'
if ARGV.size == 0
  puts 'no install directory, bye'
  gets
end
$failed = false
$log = File.open("#{ENV['TMP']}\\ruby-install.log", 'w')
$path = Pathname.new(ARGV[0]).shortname
$path.gsub!('\\\\', '\\')
$log.puts "path='#{$path}'"
$altpath = $path.gsub(File::ALT_SEPARATOR, File::SEPARATOR)
$log.puts "altpath='#{$altpath}'"
def reset_path(pn)
  s = nil
  begin
    File.open("#{$path}\\#{pn}", 'r') do |f|
      s = f.read
    end
    s.gsub! /^\\bin\\ruby/, %|"#{$path}\\ruby"|
    s.gsub! /^c\:\\home\\ruby\\bin\\ruby/i, %|"#{$path}\\ruby"|
    s.gsub! %r{\#\!\s*/bin/}, %|#!#{$altpath}/|
    s.gsub! %r{"\s*/bin/}, %|"#{$altpath}/|      
    s.gsub! %r{c\:/home/ruby/bin/}i, "#{$altpath}/"
    if pn =~ /\Amkexy.bat\Z/i
      s.gsub! /"ruby"/, "'#{$path}\\ruby.exe'"
    end
    File.open("#{ARGV[0]}\\#{pn}", 'w') do |dst|
      dst.write s
    end
  rescue
    $log.puts "#{$!}"
    $failed = true
  end
  $log.puts "#{pn} done"
end

Dir.open("#{ARGV[0]}").each do |nm|
  reset_path(nm) if /\.(cmd|bat)\Z/i =~ nm
end

$log.close

exit ($failed) ? 1 : 0
