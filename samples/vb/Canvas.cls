VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "Canvas"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Option Explicit

Public Event OnNext()
Public Event OnGo()
Public Event OnNew()
Public Event OnQuit()
Public Event OnClick(ByVal X As Long, ByVal Y As Long)

Public Sub setrect(ByVal X As Long, ByVal Y As Long)
    Dim l As Long, t As Long
    With LifeCanvas
        l = (X + 1) * .LifeRectSize
        t = (Y + 1) * .LifeRectSize
        LifeCanvas.Line (l, t)-Step(.LifeRectSize, .LifeRectSize), _
            vbBlue, BF
    End With
End Sub

Public Sub resetrect(ByVal X As Long, ByVal Y As Long)
    Dim l As Long, t As Long
    With LifeCanvas
        l = (X + 1) * .LifeRectSize
        t = (Y + 1) * .LifeRectSize
        LifeCanvas.Line (l, t)-Step(.LifeRectSize, .LifeRectSize), _
            .BackColor, BF
    End With
End Sub

Public Sub clear(ByVal X As Long, ByVal Y As Long)
    Dim l As Long, t As Long
    With LifeCanvas
        l = X * .LifeRectSize
        t = Y * .LifeRectSize
        LifeCanvas.Line (.LifeRectSize, .LifeRectSize)-Step(l, t), _
            .BackColor, BF
    End With
End Sub

Public Sub CreateCanvas(ByVal w As Long, ByVal h As Long, _
                        ByVal rectsize As Long)
    With LifeCanvas
        .CanvasWidth = w
        .CanvasHeight = h
        .LifeRectSize = rectsize
        Set .Itf = Me
        .Show vbModal
    End With
End Sub

Friend Sub OnNext()
    RaiseEvent OnNext
End Sub

Friend Sub OnGo()
    RaiseEvent OnGo
End Sub

Friend Sub OnNew()
    RaiseEvent OnNew
End Sub

Friend Sub OnQuit()
    RaiseEvent OnQuit
End Sub

Friend Sub OnClick(ByVal X As Long, ByVal Y As Long)
    Dim l As Long, t As Long
    With LifeCanvas
        l = X / .LifeRectSize - 1
        t = Y / .LifeRectSize - 1
    End With
    RaiseEvent OnClick(l, t)
End Sub

