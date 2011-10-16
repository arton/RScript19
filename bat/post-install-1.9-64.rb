#!/usr/local/bin/ruby -Ks
# encoding: cp932
require 'winpath'

$failed = false
begin
  $log = File.open("#{ENV['TMP']}\\ruby-1.9-64-install.log", 'w')
rescue
  puts $!
  $log = $stderr
end
if ARGV.size == 0
  $log.puts 'no install directory, bye'
  $log.close
  gets
  exit(1)
end
$path = Pathname.new(ARGV[0]).shortname
$log.puts "path='#{$path}'"
$altpath = $path.gsub(File::ALT_SEPARATOR, File::SEPARATOR)
$log.puts "altpath='#{$altpath}'"
def reset_path(pn)
  s = "@echo off\r\n"
  begin
    start = false
    File.open("#{ARGV[0]}\\#{pn}", 'r').each_line do |line|
      unless start
        next unless /\A:WinNT/ =~ line
        start = true
      else
        s << line + "\r\n"
      end
    end.close
    if !start
      $log.puts "skip -- #{pn}"
      return
    end
    s.gsub! /^[cC]:\\.+\\ruby/, "#{$path}\\ruby"
    s.gsub! %r|[cC]:/.+/bin/|, "#{$altpath}/"
    s.gsub! %r|#!([cC]:)?/.+/bin/|, "#!#{$altpath}/"
    File.open("#{ARGV[0]}\\#{pn}", 'w') do |dst|
      dst.write s
    end
  rescue
    $log.puts "#{$!}"
    $failed = true
  end
  $log.puts "#{pn} done"
end

def reset_shbang(pn)
  s = nil
  begin
    File.open("#{ARGV[0]}\\#{pn}", 'r') do |f|
      s = f.read
    end
    unless s =~ /\A#!.+ruby.+$/
      $log.puts "skip -- #{pn}"
      return
    end  
    s.gsub! /\A#!.+ruby.+$/, "#!#{$altpath}/ruby.exe"
    File.open("#{ARGV[0]}\\#{pn}", 'w') do |dst|
      dst.write s
    end
  rescue
    $log.puts "#{$!}"
    $failed = true
  end
  $log.puts "#{pn} done"
end

Dir.open(ARGV[0]).each do |nm|
  reset_path(nm) if /\.(cmd|bat)\Z/i =~ nm
  reset_shbang(nm) if /\A[^.]+\Z/ =~ nm
end

$log.puts "exit=#{$failed}"
$log.close
exit ($failed) ? 1 : 0
