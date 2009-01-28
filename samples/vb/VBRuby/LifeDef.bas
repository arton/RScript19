Attribute VB_Name = "LifeDef"
'This program demonstrate how to host RubyScript in VB
' see KB article Q184739 for usging Script Control
Option Explicit

Public Declare Function SetEnvironmentVariable Lib "kernel32" Alias "SetEnvironmentVariableA" _
    (ByVal lpName As String, ByVal lpValue As String) As Long
    
Public Declare Function GetShortPathName Lib "kernel32" Alias "GetShortPathNameA" (ByVal lpszLongPath As String, ByVal lpszShortPath As String, ByVal cchBuffer As Long) As Long
    

Public Const CanvasWidth = 80
Public Const CanvasHeight = 80
Public Const LifeRectSize = 6

Sub Main()
    Dim spath As String
    spath = String(Len(App.Path), " ")
    GetShortPathName App.Path, spath, Len(App.Path)
    SetEnvironmentVariable "RUBYLIB", spath
    LifeCanvas.Show vbModeless
End Sub
