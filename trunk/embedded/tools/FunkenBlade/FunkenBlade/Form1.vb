Public Class FunkenBlade
    Private serial As New SerialConnect

    Private frameBuf1 As Bitmap
    Private frameBuf2 As Bitmap
    Private frame1 As Boolean
    Private frameResize As Boolean

    Private hexFileName As String
    Private hexObj As New IntelHex

    Private joyStatus As SerialConnect.JoyStruct

    Private Sub Console_Resize()
        Dim delta As Integer
        delta = txtCommand.Top - txtConsole.Top
        txtConsole.Height = delta - 1
    End Sub
    Private Sub Video_Resize()
        Dim delta As Double
        delta = tabDisplay.Bottom - pbDisplay.Top
        If delta > 192 Then
            delta = 192
        End If
        pbDisplay.Height = delta
        pbDisplay.Width = delta * 1.25
        frameResize = True
    End Sub

    Private Sub Form1_Resize(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Resize
        Console_Resize()
        Video_Resize()
    End Sub

    Private Sub NetThreadCb()
        While 1
            serial.NetTimer_Tick()
            Threading.Thread.Sleep(25)
        End While
    End Sub

    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        Console_Resize()
        Video_Resize()
        serial.SetPortComboBox(cmbPorts)
        serial.SetCmbBaud(cmbBaud)
        serial.SetTcpIp(txtTcpIp)
        serial.SetTxtTcpPort(txtTcpPort)
        serial.SetRadTcp(radTcp)
        serial.SetRadSerial(radSerial)
        serial.SetProgressBar(prgLoadProgress)
        serial.SetRadMemEeprom(radEeprom)
        serial.SetRadMemFlash(radFlash)
        serial.SetPbDisplayServer(pbDisplay)
        serial.SeedPortNames()
        serial.SetTxtConsole(txtConsole)
        serial.Init()

        MyBase.SetStyle(ControlStyles.OptimizedDoubleBuffer, True)
        MyBase.SetStyle(ControlStyles.AllPaintingInWmPaint, True)
        MyBase.SetStyle(ControlStyles.UserPaint, True)
        Timer1.Interval = 33
        Timer1.Start()


        Dim netThreadParam As New Threading.ThreadStart(AddressOf Me.NetThreadCb)
        Dim netThread As New Threading.Thread(netThreadParam)

        netThread.Start()

    End Sub

    Private Sub CheckBox1_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles chkConnect.CheckedChanged
        If chkConnect.Checked = True Then
            If 0 = serial.Connect() Then
                chkConnect.Text = "Disconnect"
            End If

        Else
            serial.Disconnect()
            chkConnect.Text = "Connect"
        End If
    End Sub

    Private Sub SplitContainer1_SplitterMoved(ByVal sender As System.Object, ByVal e As System.Windows.Forms.SplitterEventArgs) Handles SplitContainer1.SplitterMoved
        Console_Resize()
        Video_Resize()
    End Sub

    Private Sub pbDisplay_Paint(ByVal sender As System.Object, ByVal e As System.Windows.Forms.PaintEventArgs) Handles pbDisplay.Paint

        Dim tmpImg As Bitmap
        Dim tmpGfx As Graphics
        Dim myBmp As Bitmap

        If frameResize Then
            Dim tmpImg1 As New Bitmap(pbDisplay.Width, pbDisplay.Height)
            frameBuf1 = tmpImg1
        End If

        If frameResize Then
            Dim tmpImg2 As New Bitmap(pbDisplay.Width, pbDisplay.Height)
            frameBuf2 = tmpImg2
        End If
        frameResize = False

        If frame1 = True Then
            tmpImg = frameBuf1
            frame1 = False
        Else
            tmpImg = frameBuf2
            frame1 = True
        End If

        myBmp = serial.GetDisplay()
        If IsNothing(myBmp) Then
            Return
        End If

        If serial.IsBitmapReady Then
            tmpGfx = Graphics.FromImage(tmpImg)
            tmpGfx.DrawImage(myBmp, New Rectangle(0, 0, pbDisplay.Width, pbDisplay.Height), 0, 0, myBmp.Width, myBmp.Height, GraphicsUnit.Pixel)
            pbDisplay.BackgroundImage = tmpImg
            pbDisplay.Image = tmpImg
        End If

    End Sub

    Private Sub pbDisplay_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles pbDisplay.Click

    End Sub

    Private Sub Timer1_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Timer1.Tick
        serial.NetTimer_Tick()
        pbDisplay.Refresh()

    End Sub

    Private Sub FunkenBlade_Paint(ByVal sender As System.Object, ByVal e As System.Windows.Forms.PaintEventArgs) Handles MyBase.Paint

    End Sub

    Private Sub MenuStrip1_ItemClicked(ByVal sender As System.Object, ByVal e As System.Windows.Forms.ToolStripItemClickedEventArgs) Handles MenuStrip1.ItemClicked

    End Sub

    Private Sub btnLoadHex_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnLoadHex.Click
        If OpenFileDialog1.ShowDialog() = Windows.Forms.DialogResult.OK Then
            hexFileName = OpenFileDialog1.FileName
            If IsNothing(hexObj.ParseHexFile(hexFileName)) Then
                MsgBox("Failed to read hex file!")
            End If
            lblProgFile.Text = hexFileName
        End If

    End Sub

    Private Sub btnProgramHex_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnProgramHex.Click
        Dim hexData As List(Of Byte)
        Dim hexStart As Integer

        hexData = hexObj.GetHexData()
        hexStart = hexObj.GetHexStartAddr()

        If IsNothing(hexData) Then
            Return
        End If

        If serial.IsConnected = False Then
            Return
        End If

        serial.ProgramDevice(hexData, hexStart)

    End Sub


    Private Sub pbDisplay_MouseDown(sender As Object, e As MouseEventArgs) Handles pbDisplay.MouseDown
        '
        Dim xpos As Integer
        Dim ypos As Integer

        xpos = e.X
        ypos = e.Y

        Label10.Text = xpos
        Label11.Text = ypos

        serial.EncodeMouse(xpos, ypos, 1)
    End Sub


    Private Sub pbDisplay_MouseUp(sender As Object, e As MouseEventArgs) Handles pbDisplay.MouseUp
        '
        Dim xpos As Integer
        Dim ypos As Integer

        xpos = e.X
        ypos = e.Y

        Label10.Text = xpos
        Label11.Text = ypos

        serial.EncodeMouse(xpos, ypos, 0)
    End Sub

    Private Sub pbDisplay_MouseMove(sender As Object, e As MouseEventArgs) Handles pbDisplay.MouseMove
        Dim xpos As Integer
        Dim ypos As Integer

        xpos = e.X
        ypos = e.Y

        Label10.Text = xpos
        Label11.Text = ypos

        serial.EncodeMouse(xpos, ypos, 0)
    End Sub

    Private Sub FunkenBlade_KeyDown(sender As Object, e As KeyEventArgs) Handles MyBase.KeyDown

    End Sub

    Private Sub FunkenBlade_KeyUp(sender As Object, e As KeyEventArgs) Handles MyBase.KeyUp
        
    End Sub

    Private Sub tabSystem_KeyDown(sender As Object, e As KeyEventArgs) Handles tabSystem.KeyDown
        Select Case e.KeyCode
            Case Keys.W
                joyStatus.bUp = True
            Case Keys.S
                joyStatus.bDown = True
            Case Keys.A
                joyStatus.bLeft = True
            Case Keys.D
                joyStatus.bRight = True
            Case Keys.L
                joyStatus.bButon1 = True
        End Select

        Label12.Text = ""
        If joyStatus.bUp = True Then
            Label12.Text += "U"
        Else
            Label12.Text += " "
        End If
        If joyStatus.bDown = True Then
            Label12.Text += "D"
        Else
            Label12.Text += " "
        End If
        If joyStatus.bLeft = True Then
            Label12.Text += "L"
        Else
            Label12.Text += " "
        End If
        If joyStatus.bRight = True Then
            Label12.Text += "R"
        Else
            Label12.Text += " "
        End If
        If joyStatus.bButon1 = True Then
            Label12.Text += "X"
        Else
            Label12.Text += " "
        End If
    End Sub

    Private Sub tabSystem_KeyUp(sender As Object, e As KeyEventArgs) Handles tabSystem.KeyUp
        Select Case e.KeyCode
            Case Keys.W
                joyStatus.bUp = False
            Case Keys.S
                joyStatus.bDown = False
            Case Keys.A
                joyStatus.bLeft = False
            Case Keys.D
                joyStatus.bRight = False
            Case Keys.L
                joyStatus.bButon1 = False
        End Select

        Label12.Text = ""
        If joyStatus.bUp = True Then
            Label12.Text += "U"
        Else
            Label12.Text += " "
        End If
        If joyStatus.bDown = True Then
            Label12.Text += "D"
        Else
            Label12.Text += " "
        End If
        If joyStatus.bLeft = True Then
            Label12.Text += "L"
        Else
            Label12.Text += " "
        End If
        If joyStatus.bRight = True Then
            Label12.Text += "R"
        Else
            Label12.Text += " "
        End If
        If joyStatus.bButon1 = True Then
            Label12.Text += "X"
        Else
            Label12.Text += " "
        End If
    End Sub
End Class
