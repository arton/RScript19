Set ruby = CreateObject("ruby.object")
WSH.echo ruby.Version
WSH.echo ruby.RubyVersion
Set intobj = ruby.rubyize(5)
WSH.echo "Integer:"
WSH.echo intobj.next
Set strobj = ruby.rubyize("HOHOHOHO")
WSH.echo "String:"
WSH.echo strobj.gsub("O", "I")
For Each x in ruby.rubyize("ABC" & vbCrLf & "DEF" & vbCrLf & "GHI")
  WSH.echo ruby.rubyize(x).chomp
Next
WSH.echo "Dir:"
Set dir = ruby.erubyize("Dir")
WSH.echo dir.pwd
For Each f in dir.open(".")
  WSH.echo f
Next
WSH.echo "Kernel:"
Set kern = ruby.erubyize("Kernel")
WSH.echo kern.sprintf("%s = %d + %d", "15", 10, 5)
