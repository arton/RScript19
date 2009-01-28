COMRuby.exe: COMRuby.obj
	link /out:COMRuby.exe COMRuby.obj ole32.lib oleaut32.lib

.cpp.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 \
		$<

clean:
	@del COMRuby.obj
	@del COMRuby.exe
