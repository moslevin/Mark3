Imports System.Net.Sockets

Public Class SerialConnect

    Public Structure JoyStruct
        Public bUp As Boolean
        Public bDown As Boolean
        Public bLeft As Boolean
        Public bRight As Boolean
        Public bButon1 As Boolean
        Public bButon2 As Boolean
        Public bButon3 As Boolean
        Public bButon4 As Boolean
        Public bButon5 As Boolean
        Public bButon6 As Boolean
        Public bButon7 As Boolean
        Public bButon8 As Boolean
    End Structure


    Private Enum SlipChannel
        SLIP_CHANNEL_TERMINAL
        SLIP_CHANNEL_UNISCOPE
        SLIP_CHANNEL_NVM
        SLIP_CHANNEL_RESET
        SLIP_CHANNEL_GRAPHICS
        SLIP_CHANNEL_HID
    End Enum

    Private Enum GfxCommand
        DISPLAY_EVENT_SET_PIXEL
        DISPLAY_EVENT_GET_PIXEL
        DISPLAY_EVENT_CLEAR
        DISPLAY_EVENT_LINE
        DISPLAY_EVENT_RECTANGLE
        DISPLAY_EVENT_CIRCLE
        DISPLAY_EVENT_ELLIPSE
        DISPLAY_EVENT_BITMAP
        DISPLAY_EVENT_STAMP
        DISPLAY_EVENT_TEXT
        DISPLAY_EVENT_MOVE
        DISPLAY_EVENT_POLY
    End Enum

    Private Enum HidCommand
        HID_EVENT_MOUSE
        HID_EVENT_KEYBOARD
        HID_EVENT_JOYSTICK
    End Enum

    Private Enum GuiEventType
        EVENT_TYPE_KEYBOARD
        EVENT_TYPE_MOUSE
        EVENT_TYPE_TOUCH
        EVENT_TYPE_JOYSTICK
        EVENT_TYPE_TIMER
    End Enum

    Private Enum GuiTargetType
        TARGET_ID_BROADCAST_Z = 252
        TARGET_ID_BROADCAST = 253
        TARGET_ID_FOCUS = 254
        TARGET_ID_HIGH_Z = 255
    End Enum

    ' References to UI controls used to manage connections
    Dim cmbPorts As ComboBox
    Dim cmbBaud As ComboBox
    Dim txtTcpPort As TextBox
    Dim txtTcpIp As TextBox
    Dim radSerial As RadioButton
    Dim radTcp As RadioButton
    Dim txtConsole As TextBox
    Dim prgProgressBar As ProgressBar
    Dim pbDisplayServer As PictureBox
    Dim radMemFlash As RadioButton
    Dim radMemEeprom As RadioButton

    ' Network data
    Dim WithEvents serPort As New System.IO.Ports.SerialPort
    Dim clientSocket As New System.Net.Sockets.TcpClient()
    Dim serverStream As NetworkStream

    Dim sentRecently As Boolean
    Dim tickCount As Integer
    Dim isReady As Boolean

    ' Local data
    Dim bConnected As Boolean
    Dim bUseSerial As Boolean
    Dim RxBuffer As New List(Of Byte)

    Dim rxFramer As New FunkenSlip
    Dim txFramer As New FunkenSlip

    ' graphics data
    Private Formgraphics As Graphics
    Private Formbitmap As Bitmap

    ' handle serial/network data events

    ' Functions to set the local UI control references.
    Public Sub SetPortComboBox(ByRef cmbPorts_ As ComboBox)
        cmbPorts = cmbPorts_
    End Sub

    Public Sub SetCmbBaud(ByRef cmbBaud_ As ComboBox)
        cmbBaud = cmbBaud_
    End Sub

    Public Sub SetTxtTcpPort(ByRef txtPort_ As TextBox)
        txtTcpPort = txtPort_
    End Sub

    Public Sub SetTcpIp(ByRef txtIp_ As TextBox)
        txtTcpIp = txtIp_
    End Sub

    Public Sub SetRadSerial(ByRef radSerial_ As RadioButton)
        radSerial = radSerial_
    End Sub

    Public Sub SetRadTcp(ByRef radTcp_ As RadioButton)
        radTcp = radTcp_
    End Sub

    Public Sub SetTxtConsole(ByRef txtConsole_ As TextBox)
        txtConsole = txtConsole_
    End Sub
    Public Sub SetProgressBar(ByRef prgBar_ As ProgressBar)
        prgProgressBar = prgBar_
    End Sub
    Public Sub SetRadMemFlash(ByRef radMemFlash_ As RadioButton)
        radMemFlash = radMemFlash_
    End Sub
    Public Sub SetRadMemEeprom(ByRef radMemEeprom_ As RadioButton)
        radMemEeprom = radMemEeprom_
    End Sub
    Public Sub SetPbDisplayServer(ByRef pbPictureBox_ As PictureBox)
        pbDisplayServer = pbPictureBox_
    End Sub
    Public Sub Init()
        Formbitmap = New Bitmap(240, 192)
        Formgraphics = Graphics.FromImage(Formbitmap)
    End Sub

    Public Function Connect() As Integer
        Try
            If radSerial.Checked Then
                bUseSerial = True
            Else
                bUseSerial = False
            End If
            If bUseSerial Then
                serPort.BaudRate = cmbBaud.Text
                serPort.PortName = cmbPorts.Text

                ' 8N1 connnections only
                serPort.Parity = IO.Ports.Parity.None
                serPort.DataBits = 8
                serPort.StopBits = IO.Ports.StopBits.One

                serPort.Open()

                bConnected = True
            Else
                clientSocket = New System.Net.Sockets.TcpClient()
                clientSocket.Connect(txtTcpIp.Text, txtTcpPort.Text)

                serverStream = clientSocket.GetStream()
                clientSocket.ReceiveBufferSize = 32000


                bConnected = True
            End If
        Catch ex As Exception
            MsgBox("Error opening port: " & ex.Message)
            Return -1
        End Try
        Return 0

    End Function

    Public Sub Disconnect()
        Try
            If bConnected Then
                If bUseSerial Then
                    serPort.Close()
                Else
                    clientSocket.Close()
                End If
            End If
            bConnected = False
        Catch ex As Exception
            MsgBox("Error closing port: " & ex.Message)
            bConnected = False
        End Try
    End Sub

    Public Sub SeedPortNames()
        If IsNothing(cmbPorts) Then
            Return
        End If
        Dim astPorts() As String

        cmbPorts.Items.Clear()
        astPorts = System.IO.Ports.SerialPort.GetPortNames
        For i = 0 To astPorts.Length - 1
            cmbPorts.Items.Add(astPorts(i))
        Next

        If cmbPorts.Items.Count > 0 Then
            cmbPorts.Text = cmbPorts.Items(0)
        End If
    End Sub

    Public Sub WriteData(ByRef data As List(Of Byte), ByVal channel As Byte)
        Dim txData As List(Of Byte)

        If IsNothing(data) Then
            Return
        End If
        If data.Count = 0 Then
            Return
        End If
        If bConnected = False Then
            Return
        End If

        Try
            txData = txFramer.Encode(data, channel)

            Dim txArray(txData.Count) As Byte
            Dim i As Integer

            For i = 0 To txData.Count - 1
                txArray(i) = txData(i)
            Next

            If bUseSerial Then
                serPort.Write(txArray, 0, txArray.Count - 1)
            Else
                serverStream.Write(txArray, 0, txArray.Count - 1)
                serverStream.Flush()
            End If

        Catch ex As Exception

        End Try
    End Sub

    Public Function WriteByte(ByVal byte_ As Byte) As Boolean
        Dim txData(1) As Byte

        Try
            If bUseSerial Then
                serPort.Write(txData, 0, 1)
            Else
                serverStream.Write(txData, 0, 1)
                serverStream.Flush()
            End If
        Catch ex As Exception
            Return False
        End Try

        Return True
    End Function

    Public Function IsConnected() As Boolean
        Return bConnected
    End Function

    Public Function ReadByte(ByVal timeout As Integer) As Byte
        If bConnected = False Then
            Return 0
        End If

        If bUseSerial Then
            serPort.ReadTimeout = timeout
            Return serPort.ReadByte()
        Else
            serverStream.ReadTimeout = timeout
            Return serverStream.ReadByte()
        End If

    End Function

    Public Function BytesToRead() As Integer
        If bConnected = False Then
            Return 0
        End If
        If bUseSerial Then
            Return serPort.BytesToRead
        Else
            Return serverStream.DataAvailable
        End If
    End Function

    Public Sub WriteAck()
        Dim b(1) As Byte
        b(0) = 69
        If bUseSerial Then
            serPort.Write(b, 0, 1)
        Else
            serverStream.Write(b, 0, 1)
            serverStream.Flush()
        End If

    End Sub
    Public Sub WriteNack()
        Dim b(1) As Byte
        b(0) = 96
        If bUseSerial Then
            serPort.Write(b, 0, 1)
        Else
            serverStream.Write(b, 0, 1)
            serverStream.Flush()
        End If
    End Sub

    Private Sub SerialPort_DataReceived(ByVal sender As System.Object, ByVal e As System.IO.Ports.SerialDataReceivedEventArgs) Handles serPort.DataReceived
        ' Process bytes.
        Dim tmpByte(1) As Byte
        Dim i As Integer

        Try
            While (serPort.Read(tmpByte, 0, 1))                
                RxBuffer.Add(tmpByte(0))
                If tmpByte(0) = 192 Then
                    Dim decodeData As List(Of Byte)
                    decodeData = rxFramer.Decode(RxBuffer)
                    DecodeMessages(decodeData, rxFramer.GetChannel)
                    rxFramer = New FunkenSlip
                    RxBuffer.Clear()
                End If

            End While
        Catch ex As Exception
            RxBuffer.Clear()
        End Try

    End Sub

    Public Sub NetTimer_Tick()
        If bUseSerial = True Then
            Return
        End If

        If clientSocket.Connected = False Then
            Return
        End If

        tickCount += 1
        Dim keepAlive(4) As Byte
        If tickCount = 10 Then
            tickCount = 0
            keepAlive(0) = 255
            keepAlive(1) = 254
            keepAlive(2) = 255
            keepAlive(3) = 254

            If sentRecently = True Then
                sentRecently = False
            Else
                serverStream.Write(keepAlive, 0, 4)
                serverStream.Flush()
            End If

        End If
        serverStream.ReadTimeout = 100

        If serverStream.DataAvailable Then
            Dim tmpByte(10000) As Byte

            Dim i As Integer

            Try
                While serverStream.DataAvailable
                    Dim readCount As Integer
                    readCount = serverStream.Read(tmpByte, 0, 10000)
                    For i = 0 To readCount - 1
                        RxBuffer.Add(tmpByte(i))
                        If tmpByte(i) = 192 Then
                            Dim decodeData As List(Of Byte)
                            decodeData = rxFramer.Decode(RxBuffer)
                            DecodeMessages(decodeData, rxFramer.GetChannel)
                            rxFramer = New FunkenSlip
                            RxBuffer.Clear()
                        End If
                    Next
                End While
            Catch ex As Exception
            End Try
        End If
        serverStream.Flush()

    End Sub

    Public Function IsBitmapReady() As Boolean
        Return isReady
    End Function

    Private Sub DecodeMessages(ByRef Payload As List(Of Byte), ByVal channel As Integer)
        If IsNothing(Payload) Then
            Return
        End If
        If Payload.Count = 0 Then
            Return
        End If

        Select Case channel
            Case SlipChannel.SLIP_CHANNEL_TERMINAL
                ' Terminal
                DecodeText(Payload)
            Case SlipChannel.SLIP_CHANNEL_UNISCOPE
                ' Uniscope
            Case SlipChannel.SLIP_CHANNEL_NVM
                ' EEProm
            Case SlipChannel.SLIP_CHANNEL_RESET
                ' Reset
            Case SlipChannel.SLIP_CHANNEL_GRAPHICS
                ' Graphics
                isReady = False
                DecodeGraphics(Payload)
                isReady = True
            Case Else
        End Select
    End Sub
    Public Function GetDisplay() As Bitmap
        Return Formbitmap
    End Function
    Private Delegate Sub ConsoleUpdateDelegate(ByRef newText As String)
    Private Sub ConsoleUpdate(ByRef newText As String)
        txtConsole.AppendText(newText)
    End Sub
    Private Sub DecodeText(ByRef Payload As List(Of Byte))
        Dim tempStr As String

        If IsNothing(Payload) Then
            Return
        End If

        tempStr = ""
        For i = 0 To Payload.Count - 1
            tempStr += Chr(Payload(i)).ToString
        Next

        If txtConsole.InvokeRequired Then
            txtConsole.Invoke(New ConsoleUpdateDelegate(AddressOf ConsoleUpdate), tempStr)
        Else
            txtConsole.AppendText(tempStr)
        End If
    End Sub

    Public Sub EncodeJoystick(ByRef joyState As JoyStruct)
        Dim data As New List(Of Byte)
        Dim joydata As Byte

        data.Add(HidCommand.HID_EVENT_JOYSTICK)
        data.Add(GuiEventType.EVENT_TYPE_JOYSTICK)
        data.Add(GuiTargetType.TARGET_ID_BROADCAST)

        If joyState.bUp Then
            joydata += 1
        End If
        If joyState.bDown Then
            joydata += 2
        End If
        If joyState.bLeft Then
            joydata += 4
        End If
        If joyState.bRight Then
            joydata += 8
        End If
        If joyState.bButon1 Then
            joydata += 16
        End If

        data.Add(joydata)
        data.Add(0)

    End Sub

    Public Sub EncodeMouse(ByVal xpos As Integer, ByVal ypos As Integer, ByVal state As Boolean)
        Dim data As New List(Of Byte)
        data.Add(HidCommand.HID_EVENT_MOUSE)
        data.Add(GuiEventType.EVENT_TYPE_MOUSE)
        data.Add(GuiTargetType.TARGET_ID_HIGH_Z)
        data.Add(xpos Mod 256)
        data.Add(xpos \ 256)
        data.Add(ypos Mod 256)
        data.Add(ypos \ 256)

        If (state) Then
            data.Add(1)
        Else
            data.Add(0)
        End If
        WriteData(data, SlipChannel.SLIP_CHANNEL_HID)

    End Sub

    Private Sub DecodeGraphics(ByRef Payload As List(Of Byte))
        Dim cmd As Integer

        If IsNothing(Payload) Then
            Return
        End If
        If Payload.Count = 0 Then
            Return
        End If

        cmd = Payload(0)

        Select Case cmd
            Case GfxCommand.DISPLAY_EVENT_SET_PIXEL
                'Draw Point
                Dim color As Integer
                Dim x As Integer
                Dim y As Integer

                If Payload.Count < 7 Then
                    Return
                End If
                x = Payload(2) * 256
                x += Payload(1)

                y = Payload(4) * 256
                y += Payload(3)

                color = Payload(6) * 256
                color += Payload(5)

                Dim tc As Color = Drawing.Color.FromArgb((color Mod 32) * 255 / 31, ((color \ 32) Mod 32) * 255 / 31, ((color \ (32 * 32)) Mod 32) * 255 / 31)
                Dim pix As New Bitmap(1, 1)
                pix.SetPixel(0, 0, tc)

                Formgraphics.DrawImageUnscaled(pix, x, y)

            Case GfxCommand.DISPLAY_EVENT_CLEAR
                ' clear screen
                Dim tc As Color = Color.Black
                Dim b As New SolidBrush(tc)

                Formgraphics.FillRectangle(b, 0, 0, 240, 192)
            Case GfxCommand.DISPLAY_EVENT_LINE
                'line
                Dim x1 As Integer
                Dim x2 As Integer
                Dim y1 As Integer
                Dim y2 As Integer
                Dim color As Integer

                If Payload.Count < 11 Then
                    Return
                End If

                x1 = Payload(2) * 256
                x1 += Payload(1)

                x2 = Payload(4) * 256
                x2 += Payload(3)

                y1 = Payload(6) * 256
                y1 += Payload(5)

                y2 = Payload(8) * 256
                y2 += Payload(7)

                color = Payload(10) * 256
                color += Payload(9)

                Dim tc As Color = Drawing.Color.FromArgb((color Mod 32) * 255 / 31, ((color \ 32) Mod 32) * 255 / 31, ((color \ (32 * 32)) Mod 32) * 255 / 31)
                Dim p As New Pen(tc)
                p.Width = 1

                Formgraphics.DrawLine(p, x1, y1, x2, y2)

                p.Dispose()

            Case GfxCommand.DISPLAY_EVENT_RECTANGLE
                'rectangle
                Dim left As Integer
                Dim top As Integer
                Dim right As Integer
                Dim bottom As Integer
                Dim line_color As Integer
                Dim fill_color As Integer
                Dim fill As Integer
                If Payload.Count < 11 Then
                    Return
                End If

                left = Payload(2) * 256
                left += Payload(1)

                top = Payload(4) * 256
                top += Payload(3)

                right = Payload(6) * 256
                right += Payload(5)

                bottom = Payload(8) * 256
                bottom += Payload(7)

                fill_color = Payload(10) * 256
                fill_color += Payload(9)

                fill = Payload(11)

                line_color = Payload(13) * 256
                line_color += Payload(12)


                Dim fc As Color = Drawing.Color.FromArgb((fill_color Mod 32) * 255 / 31, _
                                                         ((fill_color \ 32) Mod 32) * 255 / 31, _
                                                         ((fill_color \ (32 * 32)) Mod 32) * 255 / 31)

                Dim lc As Color = Drawing.Color.FromArgb((line_color Mod 32) * 255 / 31, _
                                                         ((line_color \ 32) Mod 32) * 255 / 31, _
                                                         ((line_color \ (32 * 32)) Mod 32) * 255 / 31)
                Dim p As New Pen(lc)
                Dim b As New SolidBrush(fc)
                p.Width = 1

                If fill <> 0 Then
                    Formgraphics.FillRectangle(b, left, top, right - left, bottom - top)
                End If

                Formgraphics.DrawRectangle(p, left, top, right - left, bottom - top)

                p.Dispose()
                b.Dispose()

            Case GfxCommand.DISPLAY_EVENT_CIRCLE
                'circle
                Dim x As Integer
                Dim y As Integer
                Dim radius As Integer
                Dim line_color As Integer
                Dim fill As Integer
                Dim fill_color As Integer

                x = Payload(2) * 256
                x += Payload(1)

                y = Payload(4) * 256
                y += Payload(3)

                radius = Payload(6) * 256
                radius += Payload(5)

                line_color = Payload(8) * 256
                line_color += Payload(7)

                fill = Payload(9)

                fill_color = Payload(10) * 256
                fill_color += Payload(9)

                Dim fc As Color = Drawing.Color.FromArgb((fill_color Mod 32) * 255 / 31, _
                                                         ((fill_color \ 32) Mod 32) * 255 / 31, _
                                                         ((fill_color \ (32 * 32)) Mod 32) * 255 / 31)

                Dim lc As Color = Drawing.Color.FromArgb((line_color Mod 32) * 255 / 31, _
                                                         ((line_color \ 32) Mod 32) * 255 / 31, _
                                                         ((line_color \ (32 * 32)) Mod 32) * 255 / 31)

                Dim p As New Pen(lc)
                Dim b As New SolidBrush(fc)
                p.Width = 1

                If fill <> 0 Then
                    Formgraphics.FillEllipse(b, x, y, radius * 2, radius * 2)
                End If

                Formgraphics.DrawEllipse(p, x, y, radius * 2, radius * 2)

                p.Dispose()
                b.Dispose()
            Case GfxCommand.DISPLAY_EVENT_ELLIPSE
                'ellipse
            Case GfxCommand.DISPLAY_EVENT_STAMP
                'stamp
                Dim x As Integer
                Dim y As Integer
                Dim width As Integer
                Dim height As Integer
                Dim color As Integer

                x = Payload(2) * 256
                x += Payload(1)

                y = Payload(4) * 256
                y += Payload(3)

                width = Payload(6) * 256
                width += Payload(5)

                height = Payload(8) * 256
                height += Payload(7)

                color = Payload(10) * 256
                color += Payload(9)

                Dim data(width * height) As Integer

                Dim data_bit As Integer

                Dim tc As Color = Drawing.Color.FromArgb((color Mod 32) * 255 / 31, ((color \ 32) Mod 32) * 255 / 31, ((color \ (32 * 32)) Mod 32) * 255 / 31)
                Dim pix As New Bitmap(1, 1)

                pix.SetPixel(0, 0, tc)
                data_bit = 128

                For i = 0 To (width * height) - 1
                    Dim data_temp As Integer = Payload(11 + (i \ 8))
                    If data_temp And data_bit Then
                        data(i) = 1
                    Else
                        data(i) = 0
                    End If
                    data_bit \= 2
                    If data_bit = 0 Then
                        data_bit = 128
                    End If
                Next

                For i = 0 To height - 1
                    For j = 0 To width - 1
                        If data((i * width) + j) Then
                            Formgraphics.DrawImageUnscaled(pix, j + x, i + y)
                        End If
                    Next
                Next

                pix.Dispose()
            Case GfxCommand.DISPLAY_EVENT_TEXT
                'Text
                Dim x As Integer
                Dim y As Integer
                Dim color As Integer
                Dim font_size As Integer
                Dim font_name As String
                Dim str_data As String
                Dim tempIdx As Integer

                x = Payload(2) * 256
                x += Payload(1)

                y = Payload(4) * 256
                y += Payload(3)

                color = Payload(6) * 256
                color += Payload(5)

                font_size = Payload(7)
                tempIdx = 8
                font_name = ""
                While (Payload(tempIdx) <> 0)
                    font_name &= Chr(Payload(tempIdx))
                    tempIdx += 1
                End While

                If font_name = "Blox" Then
                    font_name = "Blox (BRK)"
                End If

                tempIdx += 1
                str_data = ""
                While (Payload(tempIdx) <> 0)
                    str_data &= Chr(Payload(tempIdx))
                    tempIdx += 1
                End While
                Dim font As New System.Drawing.Font(font_name, font_size)
                Dim tc As Color = Drawing.Color.FromArgb((color Mod 32) * 255 / 31, ((color \ 32) Mod 32) * 255 / 31, ((color \ (32 * 32)) Mod 32) * 255 / 31)
                Dim b As New SolidBrush(tc)

                Formgraphics.DrawString(str_data, font, b, x, y)

            Case Else
        End Select
    End Sub

    Public Sub FlushRxBuffer()

        If bUseSerial = True Then
            serPort.DiscardInBuffer()
        Else
            'serverStream = clientSocket.GetStream()
            While serverStream.DataAvailable
                serverStream.ReadByte()
            End While
            serverStream.Flush()
        End If

    End Sub

    Public Function ProgramDevice(ByRef hexStream As List(Of Byte), ByVal hexStartAddr As Integer) As Boolean
        Dim slipChannel As Byte

        If radMemFlash.Checked Then
            slipChannel = 127
        Else
            slipChannel = 2
        End If

        If IsNothing(hexStream) Then
            Return False
        End If


        Dim tmpMsg As New List(Of Byte)
        Dim encoder As New FunkenSlip

        Dim txString As String
        Dim tmpChars(2) As Byte

        Dim i As Integer
        Dim j As Integer
        Dim k As Integer

        ' Send a device reset...
        tmpMsg.Clear()
        tmpMsg.Add(0)
        WriteData(tmpMsg, 3)
        FlushRxBuffer()
        Try
            While ReadByte(1000) <> 192
            End While
        Catch ex As Exception
        End Try
        FlushRxBuffer()

        ' WriteByte(192)
        Try
            If ReadByte(1000) <> 69 Then
                Threading.Thread.Sleep(100)
                FlushRxBuffer()
            End If
        Catch ex As Exception
            Threading.Thread.Sleep(100)
            FlushRxBuffer()
        End Try

        ' TextBox1.Text += vbNewLine + "Connected to device..."

        tmpMsg.Clear()

        ' Send a seek to the start address
        tmpMsg.Add(0)
        tmpMsg.Add(hexStartAddr \ 256)
        tmpMsg.Add(hexStartAddr Mod 256)

        FlushRxBuffer()
        WriteData(tmpMsg, 127)
        Dim myByte As Byte
        Try
            myByte = ReadByte(1000)
            If myByte <> 69 Then
                '    GoTo prog_handler
            End If

        Catch ex As Exception
            GoTo prog_handler
        End Try

        'TextBox1.Text += vbNewLine + "OK"
        'TextBox1.Text += vbNewLine + "Programming..." + vbNewLine

        While j < hexStream.Count
            tmpMsg.Clear()
            k = 1
            txString = 0

            'Add CMD byte
            tmpMsg.Add(1)

            prgProgressBar.Value = j / hexStream.Count * 100

            For i = 0 To 15
                If j >= hexStream.Count Then
                    Exit For
                End If
                tmpMsg.Add(hexStream(j))
                j += 1
            Next

            WriteData(tmpMsg, 127)

            Try
                If ReadByte(1000) <> 69 Then
                    GoTo prog_handler
                End If
            Catch ex As Exception
                GoTo prog_handler
            End Try

        End While

        ' Send the end of program message
        tmpMsg.Clear()
        tmpMsg.Add(2)
        WriteData(tmpMsg, 127)

        ' TextBox1.Text += vbNewLine + "Success!"
        prgProgressBar.Value = 0

        Return True
prog_handler:
        MessageBox.Show("Error programming Memory")
        prgProgressBar.Value = 0

        Return False
serial_handler:
        MessageBox.Show("Serial error - check settings and try again")
        prgProgressBar.Value = 0
        Return False

    End Function



End Class
