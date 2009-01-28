VERSION 5.00
Object = "{0E59F1D2-1FBE-11D0-8FF2-00A0D10038BC}#1.0#0"; "msscript.ocx"
Begin VB.Form LifeCanvas 
   BorderStyle     =   3  'ŒÅ’èÀÞ²±Û¸Þ
   Caption         =   "My LIFE As A Ruby"
   ClientHeight    =   6990
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   7575
   Icon            =   "LifeCanvas.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   466
   ScaleMode       =   3  'Ëß¸¾Ù
   ScaleWidth      =   505
   StartUpPosition =   3  'Windows ‚ÌŠù’è’l
   Begin VB.CommandButton NewButton 
      Caption         =   "New"
      Height          =   375
      Left            =   3240
      TabIndex        =   3
      Top             =   6480
      Width           =   975
   End
   Begin VB.Timer Timer1 
      Enabled         =   0   'False
      Interval        =   20
      Left            =   4680
      Top             =   6480
   End
   Begin MSScriptControlCtl.ScriptControl Ruby 
      Left            =   5280
      Top             =   6360
      _ExtentX        =   1005
      _ExtentY        =   1005
      Language        =   "RubyScript"
      AllowUI         =   -1  'True
   End
   Begin VB.CommandButton QuitButton 
      Caption         =   "Quit"
      Height          =   375
      Left            =   6120
      TabIndex        =   2
      Top             =   6480
      Width           =   1215
   End
   Begin VB.CommandButton GoButton 
      Caption         =   "Go"
      Height          =   375
      Left            =   1920
      TabIndex        =   1
      Top             =   6480
      Width           =   975
   End
   Begin VB.CommandButton NextButton 
      Caption         =   "Next"
      Height          =   375
      Left            =   360
      TabIndex        =   0
      Top             =   6480
      Width           =   975
   End
End
Attribute VB_Name = "LifeCanvas"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Dim Shapes(79, 79) As Shape
Dim CanvasClass As New vbCanvas
Dim life As Object

Private Sub Form_Load()
    Dim fn As Integer, stext As String
    
    Me.ScaleWidth = (CanvasWidth + 2) * LifeRectSize
    Me.ScaleHeight = (CanvasHeight + 5) * LifeRectSize + NextButton.Height
    NextButton.Top = (CanvasHeight + 3) * LifeRectSize
    GoButton.Top = NextButton.Top
    NewButton.Top = NextButton.Top
    QuitButton.Top = NextButton.Top
    QuitButton.Left = Me.ScaleWidth - QuitButton.Width - LifeRectSize
    NextButton.Left = LifeRectSize
    GoButton.Left = LifeRectSize * 2 + NextButton.Width
    NewButton.Left = GoButton.Left + LifeRectSize + GoButton.Width
    Ruby.SitehWnd = Me.hWnd
    fn = FreeFile
    Open App.Path & "\VBLife.rb" For Input Access Read As fn
    stext = InputB(LOF(fn), fn)
    Close fn
    Ruby.AddObject "canvas", CanvasClass
    Ruby.AddCode StrConv(stext, vbUnicode)
    Set life = Ruby.CodeObject.createlife(CanvasClass, CanvasWidth, CanvasHeight, LifeRectSize)
'    Set life = Ruby.Eval(StrConv(stext, vbUnicode))
End Sub

Private Sub GoButton_Click()
    If GoButton.Caption = "Stop" Then
        Timer1.Enabled = False
        GoButton.Caption = "Go"
        NextButton.Enabled = True
        NewButton.Enabled = True
    Else
        GoButton.Caption = "Stop"
        NextButton.Enabled = False
        NewButton.Enabled = False
        life.display
        Timer1.Enabled = True
    End If
End Sub

Private Sub NewButton_Click()
    life.initnew
End Sub

Private Sub NextButton_Click()
    life.Next
End Sub

Private Sub QuitButton_Click()
    End
End Sub

Private Sub Ruby_Error()
    MsgBox Ruby.Error.Description
End Sub

Private Sub Timer1_Timer()
    life.Next
End Sub
