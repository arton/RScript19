#!/usr/local/bin/ruby -Ks
# encoding: cp932
require 'winpath'
require 'swin'
require 'Win32API'
require 'zlib'

CMD_SKIP = 1234
CMD_CONT = 4321

module WMsg
  WS_CHILD          = 0x40000000
  WS_VISIBLECHILD   = 0x50000000
  WS_BORDER         = 0x00800000
  WS_OVERLAPPED     = 0
  WM_COMMAND        = 0x00000111
  WM_RBUTTONUP      = 517

  BS_CHECKBOX =       2
  BS_AUTOCHECKBOX =   3
  BS_RADIOBUTTON =    4
  BS_3STATE =         5
  BS_GROUPBOX =       7
  BS_AUTORADIOBUTTON= 9

  LBS_STANDARD =    0x00a00000 | 1
  LB_ADDSTRING =    0x0180
  LB_INSERTSTRING = 0x0181
  LB_SETSEL =       0x0185
  LB_GETSEL =       0x0187
  LB_GETCURSEL =    0x188
  LB_GETTEXT =      0x189
  LB_GETTEXTLEN =   0x18a
  LBN_SELCHANGE=1
  LBN_DBLCLK=2
  
  CBS_STANDARD    = 0x00a00200 | 3
  CB_ADDSTRING    = 0x143
  CB_DELETESTRING = 0x144
  CB_GETCURSEL =    0x147
  CB_GETLBTEXT =    0x148
  CB_GETLBTEXTLEN = 0x149
  CB_INSERTSTRING = 0x14a
  CB_SETCURSEL =    0x14e
  CBN_SELCHANGE =     1
end

def LOWORD(lParam)
  return (lParam & 0xffff)
end
def HIWORD(lParam)
  return ( (lParam>>16) & 0xffff)
end
def MAKELPARAM(w1,w2)
  return (w2<<16) | w1
end

$failed = false
begin
  $log = File.open("#{ENV['TMP']}\\ruby-1.9-install.log", 'w')
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
  s = nil
  begin
    File.open("#{ARGV[0]}\\#{pn}", 'r') do |f|
      s = f.read
    end
    s.gsub! /^\\usr\\bin\\ruby/, "#{$path}\\ruby"
    s.gsub! %r|/usr/bin/|, "#{$altpath}/"
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
end

Dir.chdir $altpath
Dir.chdir '..'

messagebeep = Win32API.new('user32', 'MessageBeep', %w|i|, 'i')
messagebeep.call(0)

RFactory=SWin::LWFactory.new SWin::Application.hInstance

$wins=[]

$wins.push mw=RFactory.newwindow(nil)
mw.caption = "Ruby-1.9.2-rc2 install optional process"
mw.move(300,200,360,380)
mw.create.show
mw.move(300,200,360,380)

$wins.push $text=RFactory.newwindow(mw)
$text.caption = <<EOD
このステップでは、OpenSSLライセンスのライブラリ（ssleay32.dll,libeay32.dll）とそれを利用するライブラリ（openssl.so）をインストールすることができます。
ただし、Ruby-1.9.2-rc2 インストールパッケージはGPL2の下に配布しているため、GPL非互換のOpenSSLライセンスの下に配布されるssleay32.dll/libeay32.dllを利用するopensslをrequireするRubyプログラムを開発した場合、Ruby-1.9.2-rc2 インストールパッケージと組み合わせて再配布することはできません。
続行した場合、上記プログラムおよびOpenSSL-License.txtがインストールされます。
EOD
$text.classname="STATIC"
$text.style= WMsg::WS_VISIBLECHILD
$text.move 10,10,330,270
$text.etc=$wins.size-1
$text.create.show

$wins.push $bn=RFactory.newwindow(mw)
$bn.classname="BUTTON"
$bn.caption="スキップ"
$bn.etc=CMD_SKIP
$bn.style=WMsg::WS_VISIBLECHILD 
$bn.move 80,290,80,40
$bn.create.show

$wins.push $bt=RFactory.newwindow(mw)
$bt.classname="BUTTON"
$bt.caption="続行"
$bt.etc=CMD_CONT
$bt.style=WMsg::WS_VISIBLECHILD
$bt.move 180,290,80,40
$bt.create.show

def info(fin)
  return nil, nil if fin.eof
  /\A(\d{9})(\S+)/ =~ fin.read(128)
  return $1.to_i, $2
end

mw.addEvent WMsg::WM_COMMAND
def mw.msghandler(msg)
  if msg.msg==WMsg::WM_COMMAND then
    if LOWORD(msg.wParam)==CMD_CONT then     # command button
      File.open('share/blob.data', 'rb') do |data|
        fin = Zlib::GzipReader.new(data)
        loop do
          len, path = info(fin)
          break unless len
          File.open(path, 'wb') do |fout|
            fout.write fin.read(len)
          end
        end
        fin.close
      end
      close
    elsif LOWORD(msg.wParam)==CMD_SKIP then 
      close
    end
  end
end

SWin::Application.messageloop

File.delete 'share/blob.data'

$log.puts "exit=#{$failed}"
$log.close
exit ($failed) ? 1 : 0
