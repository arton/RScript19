VERSION 5.00
Begin VB.Form LifeCanvas 
   BorderStyle     =   3  'ŒÅ’èÀÞ²±Û¸Þ
   Caption         =   "VB Life"
   ClientHeight    =   6345
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   8160
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   423
   ScaleMode       =   3  'Ëß¸¾Ù
   ScaleWidth      =   544
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows ‚ÌŠù’è’l
   Begin VB.CommandButton QuitButton 
      Caption         =   "&Quit"
      Height          =   375
      Left            =   4920
      TabIndex        =   3
      Top             =   4800
      Width           =   1215
   End
   Begin VB.CommandButton NewButton 
      Caption         =   "&New"
      Height          =   375
      Left            =   3360
      TabIndex        =   2
      Top             =   4800
      Width           =   1095
   End
   Begin VB.CommandButton GoButton 
      Caption         =   "&Go"
      Height          =   375
      Left            =   1920
      TabIndex        =   1
      Top             =   4800
      Width           =   1095
   End
   Begin VB.CommandButton NextButton 
      Caption         =   "&Next"
      Height          =   375
      Left            =   480
      TabIndex        =   0
      Top             =   4800
      Width           =   1095
   End
End
Attribute VB_Name = "LifeCanvas"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Public LifeRectSize As Long
Public CanvasHeight As Long
Public CanvasWidth As Long
Public Itf As Canvas

Private Sub Form_Load()
    Me.ScaleWidth = (CanvasWidth + 2) * LifeRectSize
    Me.ScaleHeight = (CanvasHeight + 5) * LifeRectSize _
        + NextButton.Height
    NextButton.Top = (CanvasHeight + 3) * LifeRectSize
    GoButton.Top = NextButton.Top
    NewButton.Top = NextButton.Top
    QuitButton.Top = NextButton.Top
    QuitButton.Left = Me.ScaleWidth - QuitButton.Width _
        - LifeRectSize
    NextButton.Left = LifeRectSize
    GoButton.Left = LifeRectSize * 2 + NextButton.Width
    NewButton.Left = GoButton.Left + LifeRectSize _
        + GoButton.Width
End Sub

Private Sub Form_MouseDown(Button As Integer, _
    Shift As Integer, X As Single, Y As Single)
    Itf.OnClick X, Y
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, _
    UnloadMode As Integer)
    If GoButton.Caption = "&Stop" Then
        Itf.OnGo
    End If
    Set Itf = Nothing
End Sub

Private Sub GoButton_Click()
    If GoButton.Caption = "&Go" Then
        GoButton.Caption = "&Stop"
    Else
        GoButton.Caption = "&Go"
    End If
    Itf.OnGo
End Sub

Private Sub NewButton_Click()
    Itf.OnNew
End Sub

Private Sub NextButton_Click()
    Itf.OnNext
End Sub

Private Sub QuitButton_Click()
    Itf.OnQuit
End Sub
