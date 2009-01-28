VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "vbCanvas"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Option Explicit

Public Sub setrect(ByVal x As Long, ByVal y As Long)
    Dim l As Long, t As Long
    l = (x + 1) * LifeRectSize
    t = (y + 1) * LifeRectSize
    LifeCanvas.Line (l, t)-Step(LifeRectSize, LifeRectSize), vbBlue, BF
End Sub

Public Sub resetrect(ByVal x As Long, ByVal y As Long)
    Dim l As Long, t As Long
    l = (x + 1) * LifeRectSize
    t = (y + 1) * LifeRectSize
    LifeCanvas.Line (l, t)-Step(LifeRectSize, LifeRectSize), LifeCanvas.BackColor, BF
End Sub

Public Sub clear(ByVal x As Long, ByVal y As Long)
    Dim l As Long, t As Long
    l = x * LifeRectSize
    t = y * LifeRectSize
    LifeCanvas.Line (LifeRectSize, LifeRectSize)-Step(l, t), LifeCanvas.BackColor, BF
End Sub
