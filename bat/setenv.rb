path = File::dirname($0).gsub('/', '\\')
ENV['PATH'] = "#{path};#{ENV['PATH']}"
if ARGV.size > 0
  cmd = 'start ruby -Ks "' + ARGV[0] + '"'
else
  if ENV['OS'] == 'Windows_NT'
    cmd = "start cmd"
  else
    cmd = "start command"
  end
end
system(cmd)


