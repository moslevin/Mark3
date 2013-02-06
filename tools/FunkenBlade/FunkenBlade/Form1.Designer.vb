<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class FunkenBlade
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container()
        Me.StatusStrip1 = New System.Windows.Forms.StatusStrip()
        Me.lblConnection = New System.Windows.Forms.ToolStripStatusLabel()
        Me.MenuStrip1 = New System.Windows.Forms.MenuStrip()
        Me.FileToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.ExitToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.AboutToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.SplitContainer1 = New System.Windows.Forms.SplitContainer()
        Me.tabSystem = New System.Windows.Forms.TabControl()
        Me.tabConnect = New System.Windows.Forms.TabPage()
        Me.GroupBox2 = New System.Windows.Forms.GroupBox()
        Me.GroupBox3 = New System.Windows.Forms.GroupBox()
        Me.cmbBaud = New System.Windows.Forms.ComboBox()
        Me.Label7 = New System.Windows.Forms.Label()
        Me.Label6 = New System.Windows.Forms.Label()
        Me.cmbPorts = New System.Windows.Forms.ComboBox()
        Me.GroupBox4 = New System.Windows.Forms.GroupBox()
        Me.Label9 = New System.Windows.Forms.Label()
        Me.Label8 = New System.Windows.Forms.Label()
        Me.txtTcpPort = New System.Windows.Forms.TextBox()
        Me.txtTcpIp = New System.Windows.Forms.TextBox()
        Me.GroupBox1 = New System.Windows.Forms.GroupBox()
        Me.chkConnect = New System.Windows.Forms.CheckBox()
        Me.radTcp = New System.Windows.Forms.RadioButton()
        Me.radSerial = New System.Windows.Forms.RadioButton()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.tabProgram = New System.Windows.Forms.TabPage()
        Me.GroupBox7 = New System.Windows.Forms.GroupBox()
        Me.btnProgramHex = New System.Windows.Forms.Button()
        Me.lblProgFile = New System.Windows.Forms.Label()
        Me.prgLoadProgress = New System.Windows.Forms.ProgressBar()
        Me.btnLoadHex = New System.Windows.Forms.Button()
        Me.GroupBox6 = New System.Windows.Forms.GroupBox()
        Me.radEeprom = New System.Windows.Forms.RadioButton()
        Me.radFlash = New System.Windows.Forms.RadioButton()
        Me.Label3 = New System.Windows.Forms.Label()
        Me.tabDisplay = New System.Windows.Forms.TabPage()
        Me.GroupBox8 = New System.Windows.Forms.GroupBox()
        Me.Label11 = New System.Windows.Forms.Label()
        Me.Label10 = New System.Windows.Forms.Label()
        Me.chkForwardMouse = New System.Windows.Forms.CheckBox()
        Me.chkForwardKeyboard = New System.Windows.Forms.CheckBox()
        Me.pbDisplay = New System.Windows.Forms.PictureBox()
        Me.Label4 = New System.Windows.Forms.Label()
        Me.tabUniscope = New System.Windows.Forms.TabPage()
        Me.Label5 = New System.Windows.Forms.Label()
        Me.txtCommand = New System.Windows.Forms.TextBox()
        Me.txtConsole = New System.Windows.Forms.TextBox()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.Timer1 = New System.Windows.Forms.Timer(Me.components)
        Me.OpenFileDialog1 = New System.Windows.Forms.OpenFileDialog()
        Me.Label12 = New System.Windows.Forms.Label()
        Me.StatusStrip1.SuspendLayout()
        Me.MenuStrip1.SuspendLayout()
        CType(Me.SplitContainer1, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SplitContainer1.Panel1.SuspendLayout()
        Me.SplitContainer1.Panel2.SuspendLayout()
        Me.SplitContainer1.SuspendLayout()
        Me.tabSystem.SuspendLayout()
        Me.tabConnect.SuspendLayout()
        Me.GroupBox2.SuspendLayout()
        Me.GroupBox3.SuspendLayout()
        Me.GroupBox4.SuspendLayout()
        Me.GroupBox1.SuspendLayout()
        Me.tabProgram.SuspendLayout()
        Me.GroupBox7.SuspendLayout()
        Me.GroupBox6.SuspendLayout()
        Me.tabDisplay.SuspendLayout()
        Me.GroupBox8.SuspendLayout()
        CType(Me.pbDisplay, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.tabUniscope.SuspendLayout()
        Me.SuspendLayout()
        '
        'StatusStrip1
        '
        Me.StatusStrip1.Items.AddRange(New System.Windows.Forms.ToolStripItem() {Me.lblConnection})
        Me.StatusStrip1.Location = New System.Drawing.Point(0, 302)
        Me.StatusStrip1.Name = "StatusStrip1"
        Me.StatusStrip1.Size = New System.Drawing.Size(437, 22)
        Me.StatusStrip1.TabIndex = 0
        Me.StatusStrip1.Text = "StatusStrip1"
        '
        'lblConnection
        '
        Me.lblConnection.Name = "lblConnection"
        Me.lblConnection.Size = New System.Drawing.Size(79, 17)
        Me.lblConnection.Text = "Disconnected"
        '
        'MenuStrip1
        '
        Me.MenuStrip1.Items.AddRange(New System.Windows.Forms.ToolStripItem() {Me.FileToolStripMenuItem, Me.AboutToolStripMenuItem})
        Me.MenuStrip1.Location = New System.Drawing.Point(0, 0)
        Me.MenuStrip1.Name = "MenuStrip1"
        Me.MenuStrip1.Size = New System.Drawing.Size(437, 24)
        Me.MenuStrip1.TabIndex = 1
        Me.MenuStrip1.Text = "MenuStrip1"
        '
        'FileToolStripMenuItem
        '
        Me.FileToolStripMenuItem.DropDownItems.AddRange(New System.Windows.Forms.ToolStripItem() {Me.ExitToolStripMenuItem})
        Me.FileToolStripMenuItem.Name = "FileToolStripMenuItem"
        Me.FileToolStripMenuItem.Size = New System.Drawing.Size(37, 20)
        Me.FileToolStripMenuItem.Text = "File"
        '
        'ExitToolStripMenuItem
        '
        Me.ExitToolStripMenuItem.Name = "ExitToolStripMenuItem"
        Me.ExitToolStripMenuItem.Size = New System.Drawing.Size(92, 22)
        Me.ExitToolStripMenuItem.Text = "Exit"
        '
        'AboutToolStripMenuItem
        '
        Me.AboutToolStripMenuItem.Name = "AboutToolStripMenuItem"
        Me.AboutToolStripMenuItem.Size = New System.Drawing.Size(52, 20)
        Me.AboutToolStripMenuItem.Text = "About"
        '
        'SplitContainer1
        '
        Me.SplitContainer1.Dock = System.Windows.Forms.DockStyle.Fill
        Me.SplitContainer1.Location = New System.Drawing.Point(0, 24)
        Me.SplitContainer1.Name = "SplitContainer1"
        Me.SplitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal
        '
        'SplitContainer1.Panel1
        '
        Me.SplitContainer1.Panel1.Controls.Add(Me.tabSystem)
        '
        'SplitContainer1.Panel2
        '
        Me.SplitContainer1.Panel2.Controls.Add(Me.txtCommand)
        Me.SplitContainer1.Panel2.Controls.Add(Me.txtConsole)
        Me.SplitContainer1.Panel2.Controls.Add(Me.Label1)
        Me.SplitContainer1.Size = New System.Drawing.Size(437, 278)
        Me.SplitContainer1.SplitterDistance = 165
        Me.SplitContainer1.TabIndex = 2
        '
        'tabSystem
        '
        Me.tabSystem.Controls.Add(Me.tabConnect)
        Me.tabSystem.Controls.Add(Me.tabProgram)
        Me.tabSystem.Controls.Add(Me.tabDisplay)
        Me.tabSystem.Controls.Add(Me.tabUniscope)
        Me.tabSystem.Dock = System.Windows.Forms.DockStyle.Fill
        Me.tabSystem.Location = New System.Drawing.Point(0, 0)
        Me.tabSystem.Name = "tabSystem"
        Me.tabSystem.SelectedIndex = 0
        Me.tabSystem.Size = New System.Drawing.Size(437, 165)
        Me.tabSystem.TabIndex = 0
        '
        'tabConnect
        '
        Me.tabConnect.Controls.Add(Me.GroupBox2)
        Me.tabConnect.Controls.Add(Me.GroupBox1)
        Me.tabConnect.Controls.Add(Me.Label2)
        Me.tabConnect.Location = New System.Drawing.Point(4, 22)
        Me.tabConnect.Name = "tabConnect"
        Me.tabConnect.Size = New System.Drawing.Size(429, 139)
        Me.tabConnect.TabIndex = 0
        Me.tabConnect.Text = "Connection"
        Me.tabConnect.UseVisualStyleBackColor = True
        '
        'GroupBox2
        '
        Me.GroupBox2.Controls.Add(Me.GroupBox3)
        Me.GroupBox2.Controls.Add(Me.GroupBox4)
        Me.GroupBox2.Dock = System.Windows.Forms.DockStyle.Fill
        Me.GroupBox2.Location = New System.Drawing.Point(125, 16)
        Me.GroupBox2.Name = "GroupBox2"
        Me.GroupBox2.Size = New System.Drawing.Size(304, 123)
        Me.GroupBox2.TabIndex = 12
        Me.GroupBox2.TabStop = False
        Me.GroupBox2.Text = "Settings"
        '
        'GroupBox3
        '
        Me.GroupBox3.Controls.Add(Me.cmbBaud)
        Me.GroupBox3.Controls.Add(Me.Label7)
        Me.GroupBox3.Controls.Add(Me.Label6)
        Me.GroupBox3.Controls.Add(Me.cmbPorts)
        Me.GroupBox3.Dock = System.Windows.Forms.DockStyle.Left
        Me.GroupBox3.Location = New System.Drawing.Point(154, 16)
        Me.GroupBox3.Name = "GroupBox3"
        Me.GroupBox3.Size = New System.Drawing.Size(147, 104)
        Me.GroupBox3.TabIndex = 15
        Me.GroupBox3.TabStop = False
        Me.GroupBox3.Text = "Serial Connection"
        '
        'cmbBaud
        '
        Me.cmbBaud.FormattingEnabled = True
        Me.cmbBaud.Items.AddRange(New Object() {"2400", "9600", "14400", "28800", "57600", "115200"})
        Me.cmbBaud.Location = New System.Drawing.Point(22, 73)
        Me.cmbBaud.Name = "cmbBaud"
        Me.cmbBaud.Size = New System.Drawing.Size(100, 21)
        Me.cmbBaud.TabIndex = 13
        Me.cmbBaud.Text = "57600"
        '
        'Label7
        '
        Me.Label7.AutoSize = True
        Me.Label7.Location = New System.Drawing.Point(22, 55)
        Me.Label7.Name = "Label7"
        Me.Label7.Size = New System.Drawing.Size(58, 13)
        Me.Label7.TabIndex = 12
        Me.Label7.Text = "Baud Rate"
        '
        'Label6
        '
        Me.Label6.AutoSize = True
        Me.Label6.Location = New System.Drawing.Point(23, 18)
        Me.Label6.Name = "Label6"
        Me.Label6.Size = New System.Drawing.Size(57, 13)
        Me.Label6.TabIndex = 11
        Me.Label6.Text = "Port Name"
        '
        'cmbPorts
        '
        Me.cmbPorts.FormattingEnabled = True
        Me.cmbPorts.Location = New System.Drawing.Point(22, 31)
        Me.cmbPorts.Name = "cmbPorts"
        Me.cmbPorts.Size = New System.Drawing.Size(100, 21)
        Me.cmbPorts.TabIndex = 10
        '
        'GroupBox4
        '
        Me.GroupBox4.Controls.Add(Me.Label9)
        Me.GroupBox4.Controls.Add(Me.Label8)
        Me.GroupBox4.Controls.Add(Me.txtTcpPort)
        Me.GroupBox4.Controls.Add(Me.txtTcpIp)
        Me.GroupBox4.Dock = System.Windows.Forms.DockStyle.Left
        Me.GroupBox4.Location = New System.Drawing.Point(3, 16)
        Me.GroupBox4.Name = "GroupBox4"
        Me.GroupBox4.Size = New System.Drawing.Size(151, 104)
        Me.GroupBox4.TabIndex = 14
        Me.GroupBox4.TabStop = False
        Me.GroupBox4.Text = "TCP/IP Connection"
        '
        'Label9
        '
        Me.Label9.AutoSize = True
        Me.Label9.Location = New System.Drawing.Point(24, 57)
        Me.Label9.Name = "Label9"
        Me.Label9.Size = New System.Drawing.Size(50, 13)
        Me.Label9.TabIndex = 10
        Me.Label9.Text = "TCP Port"
        '
        'Label8
        '
        Me.Label8.AutoSize = True
        Me.Label8.Location = New System.Drawing.Point(24, 18)
        Me.Label8.Name = "Label8"
        Me.Label8.Size = New System.Drawing.Size(51, 13)
        Me.Label8.TabIndex = 9
        Me.Label8.Text = "Target IP"
        '
        'txtTcpPort
        '
        Me.txtTcpPort.Location = New System.Drawing.Point(24, 73)
        Me.txtTcpPort.Name = "txtTcpPort"
        Me.txtTcpPort.Size = New System.Drawing.Size(100, 20)
        Me.txtTcpPort.TabIndex = 5
        Me.txtTcpPort.Text = "1234"
        '
        'txtTcpIp
        '
        Me.txtTcpIp.Location = New System.Drawing.Point(24, 34)
        Me.txtTcpIp.Name = "txtTcpIp"
        Me.txtTcpIp.Size = New System.Drawing.Size(100, 20)
        Me.txtTcpIp.TabIndex = 4
        Me.txtTcpIp.Text = "127.0.0.1"
        '
        'GroupBox1
        '
        Me.GroupBox1.Controls.Add(Me.chkConnect)
        Me.GroupBox1.Controls.Add(Me.radTcp)
        Me.GroupBox1.Controls.Add(Me.radSerial)
        Me.GroupBox1.Dock = System.Windows.Forms.DockStyle.Left
        Me.GroupBox1.Location = New System.Drawing.Point(0, 16)
        Me.GroupBox1.Name = "GroupBox1"
        Me.GroupBox1.Size = New System.Drawing.Size(125, 123)
        Me.GroupBox1.TabIndex = 11
        Me.GroupBox1.TabStop = False
        Me.GroupBox1.Text = "Connection Type"
        '
        'chkConnect
        '
        Me.chkConnect.Appearance = System.Windows.Forms.Appearance.Button
        Me.chkConnect.Location = New System.Drawing.Point(29, 73)
        Me.chkConnect.Name = "chkConnect"
        Me.chkConnect.Size = New System.Drawing.Size(71, 23)
        Me.chkConnect.TabIndex = 2
        Me.chkConnect.Text = "Connect"
        Me.chkConnect.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
        Me.chkConnect.UseVisualStyleBackColor = True
        '
        'radTcp
        '
        Me.radTcp.AutoSize = True
        Me.radTcp.Checked = True
        Me.radTcp.Location = New System.Drawing.Point(29, 28)
        Me.radTcp.Name = "radTcp"
        Me.radTcp.Size = New System.Drawing.Size(61, 17)
        Me.radTcp.TabIndex = 1
        Me.radTcp.TabStop = True
        Me.radTcp.Text = "TCP/IP"
        Me.radTcp.UseVisualStyleBackColor = True
        '
        'radSerial
        '
        Me.radSerial.AutoSize = True
        Me.radSerial.Location = New System.Drawing.Point(29, 50)
        Me.radSerial.Name = "radSerial"
        Me.radSerial.Size = New System.Drawing.Size(73, 17)
        Me.radSerial.TabIndex = 0
        Me.radSerial.Text = "Serial Port"
        Me.radSerial.UseVisualStyleBackColor = True
        '
        'Label2
        '
        Me.Label2.BackColor = System.Drawing.Color.Blue
        Me.Label2.Dock = System.Windows.Forms.DockStyle.Top
        Me.Label2.ForeColor = System.Drawing.Color.White
        Me.Label2.Location = New System.Drawing.Point(0, 0)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(429, 16)
        Me.Label2.TabIndex = 1
        Me.Label2.Text = "Device Connection Settings"
        '
        'tabProgram
        '
        Me.tabProgram.Controls.Add(Me.GroupBox7)
        Me.tabProgram.Controls.Add(Me.GroupBox6)
        Me.tabProgram.Controls.Add(Me.Label3)
        Me.tabProgram.Location = New System.Drawing.Point(4, 22)
        Me.tabProgram.Name = "tabProgram"
        Me.tabProgram.Size = New System.Drawing.Size(429, 139)
        Me.tabProgram.TabIndex = 1
        Me.tabProgram.Text = "Programming"
        Me.tabProgram.UseVisualStyleBackColor = True
        '
        'GroupBox7
        '
        Me.GroupBox7.Controls.Add(Me.btnProgramHex)
        Me.GroupBox7.Controls.Add(Me.lblProgFile)
        Me.GroupBox7.Controls.Add(Me.prgLoadProgress)
        Me.GroupBox7.Controls.Add(Me.btnLoadHex)
        Me.GroupBox7.Dock = System.Windows.Forms.DockStyle.Fill
        Me.GroupBox7.Location = New System.Drawing.Point(114, 16)
        Me.GroupBox7.Name = "GroupBox7"
        Me.GroupBox7.Size = New System.Drawing.Size(315, 123)
        Me.GroupBox7.TabIndex = 21
        Me.GroupBox7.TabStop = False
        Me.GroupBox7.Text = "Hex File"
        '
        'btnProgramHex
        '
        Me.btnProgramHex.Location = New System.Drawing.Point(42, 62)
        Me.btnProgramHex.Name = "btnProgramHex"
        Me.btnProgramHex.Size = New System.Drawing.Size(60, 23)
        Me.btnProgramHex.TabIndex = 7
        Me.btnProgramHex.Text = "Program"
        Me.btnProgramHex.UseVisualStyleBackColor = True
        '
        'lblProgFile
        '
        Me.lblProgFile.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me.lblProgFile.Location = New System.Drawing.Point(40, 29)
        Me.lblProgFile.Name = "lblProgFile"
        Me.lblProgFile.Size = New System.Drawing.Size(263, 21)
        Me.lblProgFile.TabIndex = 17
        Me.lblProgFile.Text = "No File Selected"
        '
        'prgLoadProgress
        '
        Me.prgLoadProgress.Location = New System.Drawing.Point(111, 62)
        Me.prgLoadProgress.Name = "prgLoadProgress"
        Me.prgLoadProgress.Size = New System.Drawing.Size(198, 23)
        Me.prgLoadProgress.TabIndex = 3
        '
        'btnLoadHex
        '
        Me.btnLoadHex.Location = New System.Drawing.Point(6, 28)
        Me.btnLoadHex.Name = "btnLoadHex"
        Me.btnLoadHex.Size = New System.Drawing.Size(28, 22)
        Me.btnLoadHex.TabIndex = 6
        Me.btnLoadHex.Text = "..."
        Me.btnLoadHex.UseVisualStyleBackColor = True
        '
        'GroupBox6
        '
        Me.GroupBox6.Controls.Add(Me.radEeprom)
        Me.GroupBox6.Controls.Add(Me.radFlash)
        Me.GroupBox6.Dock = System.Windows.Forms.DockStyle.Left
        Me.GroupBox6.Location = New System.Drawing.Point(0, 16)
        Me.GroupBox6.Name = "GroupBox6"
        Me.GroupBox6.Size = New System.Drawing.Size(114, 123)
        Me.GroupBox6.TabIndex = 20
        Me.GroupBox6.TabStop = False
        Me.GroupBox6.Text = "Memory Type"
        '
        'radEeprom
        '
        Me.radEeprom.AutoSize = True
        Me.radEeprom.Location = New System.Drawing.Point(15, 62)
        Me.radEeprom.Name = "radEeprom"
        Me.radEeprom.Size = New System.Drawing.Size(71, 17)
        Me.radEeprom.TabIndex = 11
        Me.radEeprom.Text = "EEPROM"
        Me.radEeprom.UseVisualStyleBackColor = True
        '
        'radFlash
        '
        Me.radFlash.AutoSize = True
        Me.radFlash.Checked = True
        Me.radFlash.Location = New System.Drawing.Point(15, 33)
        Me.radFlash.Name = "radFlash"
        Me.radFlash.Size = New System.Drawing.Size(50, 17)
        Me.radFlash.TabIndex = 10
        Me.radFlash.TabStop = True
        Me.radFlash.Text = "Flash"
        Me.radFlash.UseVisualStyleBackColor = True
        '
        'Label3
        '
        Me.Label3.BackColor = System.Drawing.Color.Blue
        Me.Label3.Dock = System.Windows.Forms.DockStyle.Top
        Me.Label3.ForeColor = System.Drawing.Color.White
        Me.Label3.Location = New System.Drawing.Point(0, 0)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(429, 16)
        Me.Label3.TabIndex = 2
        Me.Label3.Text = "Device Memory Programming"
        '
        'tabDisplay
        '
        Me.tabDisplay.Controls.Add(Me.GroupBox8)
        Me.tabDisplay.Controls.Add(Me.pbDisplay)
        Me.tabDisplay.Controls.Add(Me.Label4)
        Me.tabDisplay.Location = New System.Drawing.Point(4, 22)
        Me.tabDisplay.Name = "tabDisplay"
        Me.tabDisplay.Size = New System.Drawing.Size(429, 139)
        Me.tabDisplay.TabIndex = 2
        Me.tabDisplay.Text = "Display"
        Me.tabDisplay.UseVisualStyleBackColor = True
        '
        'GroupBox8
        '
        Me.GroupBox8.Controls.Add(Me.Label12)
        Me.GroupBox8.Controls.Add(Me.Label11)
        Me.GroupBox8.Controls.Add(Me.Label10)
        Me.GroupBox8.Controls.Add(Me.chkForwardMouse)
        Me.GroupBox8.Controls.Add(Me.chkForwardKeyboard)
        Me.GroupBox8.Dock = System.Windows.Forms.DockStyle.Right
        Me.GroupBox8.Location = New System.Drawing.Point(275, 16)
        Me.GroupBox8.Name = "GroupBox8"
        Me.GroupBox8.Size = New System.Drawing.Size(154, 123)
        Me.GroupBox8.TabIndex = 5
        Me.GroupBox8.TabStop = False
        Me.GroupBox8.Text = "Settings"
        '
        'Label11
        '
        Me.Label11.AutoSize = True
        Me.Label11.Location = New System.Drawing.Point(35, 86)
        Me.Label11.Name = "Label11"
        Me.Label11.Size = New System.Drawing.Size(45, 13)
        Me.Label11.TabIndex = 3
        Me.Label11.Text = "Label11"
        '
        'Label10
        '
        Me.Label10.AutoSize = True
        Me.Label10.Location = New System.Drawing.Point(35, 73)
        Me.Label10.Name = "Label10"
        Me.Label10.Size = New System.Drawing.Size(45, 13)
        Me.Label10.TabIndex = 2
        Me.Label10.Text = "Label10"
        '
        'chkForwardMouse
        '
        Me.chkForwardMouse.AutoSize = True
        Me.chkForwardMouse.Location = New System.Drawing.Point(7, 42)
        Me.chkForwardMouse.Name = "chkForwardMouse"
        Me.chkForwardMouse.Size = New System.Drawing.Size(135, 17)
        Me.chkForwardMouse.TabIndex = 1
        Me.chkForwardMouse.Text = "Forward Mouse Events"
        Me.chkForwardMouse.UseVisualStyleBackColor = True
        '
        'chkForwardKeyboard
        '
        Me.chkForwardKeyboard.AutoSize = True
        Me.chkForwardKeyboard.Location = New System.Drawing.Point(7, 19)
        Me.chkForwardKeyboard.Name = "chkForwardKeyboard"
        Me.chkForwardKeyboard.Size = New System.Drawing.Size(148, 17)
        Me.chkForwardKeyboard.TabIndex = 0
        Me.chkForwardKeyboard.Text = "Forward Keyboard Events"
        Me.chkForwardKeyboard.UseVisualStyleBackColor = True
        '
        'pbDisplay
        '
        Me.pbDisplay.Location = New System.Drawing.Point(8, 19)
        Me.pbDisplay.Name = "pbDisplay"
        Me.pbDisplay.Size = New System.Drawing.Size(243, 193)
        Me.pbDisplay.TabIndex = 4
        Me.pbDisplay.TabStop = False
        '
        'Label4
        '
        Me.Label4.BackColor = System.Drawing.Color.Blue
        Me.Label4.Dock = System.Windows.Forms.DockStyle.Top
        Me.Label4.ForeColor = System.Drawing.Color.White
        Me.Label4.Location = New System.Drawing.Point(0, 0)
        Me.Label4.Name = "Label4"
        Me.Label4.Size = New System.Drawing.Size(429, 16)
        Me.Label4.TabIndex = 3
        Me.Label4.Text = "Remote Display Server"
        '
        'tabUniscope
        '
        Me.tabUniscope.Controls.Add(Me.Label5)
        Me.tabUniscope.Location = New System.Drawing.Point(4, 22)
        Me.tabUniscope.Name = "tabUniscope"
        Me.tabUniscope.Size = New System.Drawing.Size(429, 139)
        Me.tabUniscope.TabIndex = 3
        Me.tabUniscope.Text = "Uniscope"
        Me.tabUniscope.UseVisualStyleBackColor = True
        '
        'Label5
        '
        Me.Label5.BackColor = System.Drawing.Color.Blue
        Me.Label5.Dock = System.Windows.Forms.DockStyle.Top
        Me.Label5.ForeColor = System.Drawing.Color.White
        Me.Label5.Location = New System.Drawing.Point(0, 0)
        Me.Label5.Name = "Label5"
        Me.Label5.Size = New System.Drawing.Size(429, 16)
        Me.Label5.TabIndex = 3
        Me.Label5.Text = "Uniscope Runtime Debugger"
        '
        'txtCommand
        '
        Me.txtCommand.BorderStyle = System.Windows.Forms.BorderStyle.None
        Me.txtCommand.Dock = System.Windows.Forms.DockStyle.Bottom
        Me.txtCommand.Location = New System.Drawing.Point(0, 96)
        Me.txtCommand.Name = "txtCommand"
        Me.txtCommand.Size = New System.Drawing.Size(437, 13)
        Me.txtCommand.TabIndex = 2
        '
        'txtConsole
        '
        Me.txtConsole.BorderStyle = System.Windows.Forms.BorderStyle.None
        Me.txtConsole.Dock = System.Windows.Forms.DockStyle.Top
        Me.txtConsole.Location = New System.Drawing.Point(0, 16)
        Me.txtConsole.Multiline = True
        Me.txtConsole.Name = "txtConsole"
        Me.txtConsole.ScrollBars = System.Windows.Forms.ScrollBars.Vertical
        Me.txtConsole.Size = New System.Drawing.Size(437, 20)
        Me.txtConsole.TabIndex = 1
        '
        'Label1
        '
        Me.Label1.BackColor = System.Drawing.Color.Blue
        Me.Label1.Dock = System.Windows.Forms.DockStyle.Top
        Me.Label1.ForeColor = System.Drawing.Color.White
        Me.Label1.Location = New System.Drawing.Point(0, 0)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(437, 16)
        Me.Label1.TabIndex = 0
        Me.Label1.Text = "Debug Console"
        '
        'Timer1
        '
        '
        'OpenFileDialog1
        '
        Me.OpenFileDialog1.FileName = "OpenFileDialog1"
        '
        'Label12
        '
        Me.Label12.AutoSize = True
        Me.Label12.Location = New System.Drawing.Point(35, 99)
        Me.Label12.Name = "Label12"
        Me.Label12.Size = New System.Drawing.Size(45, 13)
        Me.Label12.TabIndex = 4
        Me.Label12.Text = "Label12"
        '
        'FunkenBlade
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(437, 324)
        Me.Controls.Add(Me.SplitContainer1)
        Me.Controls.Add(Me.StatusStrip1)
        Me.Controls.Add(Me.MenuStrip1)
        Me.MainMenuStrip = Me.MenuStrip1
        Me.Name = "FunkenBlade"
        Me.Text = "FunkenBlade"
        Me.StatusStrip1.ResumeLayout(False)
        Me.StatusStrip1.PerformLayout()
        Me.MenuStrip1.ResumeLayout(False)
        Me.MenuStrip1.PerformLayout()
        Me.SplitContainer1.Panel1.ResumeLayout(False)
        Me.SplitContainer1.Panel2.ResumeLayout(False)
        Me.SplitContainer1.Panel2.PerformLayout()
        CType(Me.SplitContainer1, System.ComponentModel.ISupportInitialize).EndInit()
        Me.SplitContainer1.ResumeLayout(False)
        Me.tabSystem.ResumeLayout(False)
        Me.tabConnect.ResumeLayout(False)
        Me.GroupBox2.ResumeLayout(False)
        Me.GroupBox3.ResumeLayout(False)
        Me.GroupBox3.PerformLayout()
        Me.GroupBox4.ResumeLayout(False)
        Me.GroupBox4.PerformLayout()
        Me.GroupBox1.ResumeLayout(False)
        Me.GroupBox1.PerformLayout()
        Me.tabProgram.ResumeLayout(False)
        Me.GroupBox7.ResumeLayout(False)
        Me.GroupBox6.ResumeLayout(False)
        Me.GroupBox6.PerformLayout()
        Me.tabDisplay.ResumeLayout(False)
        Me.GroupBox8.ResumeLayout(False)
        Me.GroupBox8.PerformLayout()
        CType(Me.pbDisplay, System.ComponentModel.ISupportInitialize).EndInit()
        Me.tabUniscope.ResumeLayout(False)
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents StatusStrip1 As System.Windows.Forms.StatusStrip
    Friend WithEvents MenuStrip1 As System.Windows.Forms.MenuStrip
    Friend WithEvents SplitContainer1 As System.Windows.Forms.SplitContainer
    Friend WithEvents txtCommand As System.Windows.Forms.TextBox
    Friend WithEvents txtConsole As System.Windows.Forms.TextBox
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents tabSystem As System.Windows.Forms.TabControl
    Friend WithEvents tabConnect As System.Windows.Forms.TabPage
    Friend WithEvents tabProgram As System.Windows.Forms.TabPage
    Friend WithEvents Label2 As System.Windows.Forms.Label
    Friend WithEvents Label3 As System.Windows.Forms.Label
    Friend WithEvents tabDisplay As System.Windows.Forms.TabPage
    Friend WithEvents Label4 As System.Windows.Forms.Label
    Friend WithEvents tabUniscope As System.Windows.Forms.TabPage
    Friend WithEvents Label5 As System.Windows.Forms.Label
    Friend WithEvents GroupBox1 As System.Windows.Forms.GroupBox
    Friend WithEvents GroupBox2 As System.Windows.Forms.GroupBox
    Friend WithEvents GroupBox3 As System.Windows.Forms.GroupBox
    Friend WithEvents Label7 As System.Windows.Forms.Label
    Friend WithEvents Label6 As System.Windows.Forms.Label
    Friend WithEvents cmbPorts As System.Windows.Forms.ComboBox
    Friend WithEvents GroupBox4 As System.Windows.Forms.GroupBox
    Friend WithEvents Label9 As System.Windows.Forms.Label
    Friend WithEvents Label8 As System.Windows.Forms.Label
    Friend WithEvents txtTcpPort As System.Windows.Forms.TextBox
    Friend WithEvents txtTcpIp As System.Windows.Forms.TextBox
    Friend WithEvents chkConnect As System.Windows.Forms.CheckBox
    Friend WithEvents radTcp As System.Windows.Forms.RadioButton
    Friend WithEvents radSerial As System.Windows.Forms.RadioButton
    Friend WithEvents FileToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
    Friend WithEvents ExitToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
    Friend WithEvents AboutToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
    Friend WithEvents Timer1 As System.Windows.Forms.Timer
    Friend WithEvents OpenFileDialog1 As System.Windows.Forms.OpenFileDialog
    Friend WithEvents GroupBox8 As System.Windows.Forms.GroupBox
    Friend WithEvents chkForwardMouse As System.Windows.Forms.CheckBox
    Friend WithEvents chkForwardKeyboard As System.Windows.Forms.CheckBox
    Friend WithEvents pbDisplay As System.Windows.Forms.PictureBox
    Friend WithEvents GroupBox7 As System.Windows.Forms.GroupBox
    Friend WithEvents btnProgramHex As System.Windows.Forms.Button
    Friend WithEvents lblProgFile As System.Windows.Forms.Label
    Friend WithEvents prgLoadProgress As System.Windows.Forms.ProgressBar
    Friend WithEvents btnLoadHex As System.Windows.Forms.Button
    Friend WithEvents GroupBox6 As System.Windows.Forms.GroupBox
    Friend WithEvents radEeprom As System.Windows.Forms.RadioButton
    Friend WithEvents radFlash As System.Windows.Forms.RadioButton
    Friend WithEvents lblConnection As System.Windows.Forms.ToolStripStatusLabel
    Friend WithEvents cmbBaud As System.Windows.Forms.ComboBox
    Friend WithEvents Label11 As System.Windows.Forms.Label
    Friend WithEvents Label10 As System.Windows.Forms.Label
    Friend WithEvents Label12 As System.Windows.Forms.Label

End Class
