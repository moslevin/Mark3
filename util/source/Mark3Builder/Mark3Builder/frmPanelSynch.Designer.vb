<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class frmPanelSynch
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
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(frmPanelSynch))
        Me.TableLayoutPanel1 = New System.Windows.Forms.TableLayoutPanel()
        Me.TableLayoutPanel2 = New System.Windows.Forms.TableLayoutPanel()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.chkMutex = New System.Windows.Forms.CheckBox()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.Label3 = New System.Windows.Forms.Label()
        Me.Label4 = New System.Windows.Forms.Label()
        Me.chkSem = New System.Windows.Forms.CheckBox()
        Me.TableLayoutPanel3 = New System.Windows.Forms.TableLayoutPanel()
        Me.TableLayoutPanel4 = New System.Windows.Forms.TableLayoutPanel()
        Me.Label5 = New System.Windows.Forms.Label()
        Me.NumericUpDown1 = New System.Windows.Forms.NumericUpDown()
        Me.chkQueue = New System.Windows.Forms.CheckBox()
        Me.chkEvent = New System.Windows.Forms.CheckBox()
        Me.Panel1 = New System.Windows.Forms.Panel()
        Me.btnBack = New System.Windows.Forms.Button()
        Me.btnNext = New System.Windows.Forms.Button()
        Me.TableLayoutPanel1.SuspendLayout()
        Me.TableLayoutPanel2.SuspendLayout()
        Me.TableLayoutPanel3.SuspendLayout()
        Me.TableLayoutPanel4.SuspendLayout()
        CType(Me.NumericUpDown1, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.Panel1.SuspendLayout()
        Me.SuspendLayout()
        '
        'TableLayoutPanel1
        '
        Me.TableLayoutPanel1.ColumnCount = 3
        Me.TableLayoutPanel1.ColumnStyles.Add(New System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 2.159827!))
        Me.TableLayoutPanel1.ColumnStyles.Add(New System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 95.68034!))
        Me.TableLayoutPanel1.ColumnStyles.Add(New System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 2.159827!))
        Me.TableLayoutPanel1.Controls.Add(Me.TableLayoutPanel2, 1, 1)
        Me.TableLayoutPanel1.Controls.Add(Me.Panel1, 1, 2)
        Me.TableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill
        Me.TableLayoutPanel1.Location = New System.Drawing.Point(0, 0)
        Me.TableLayoutPanel1.Name = "TableLayoutPanel1"
        Me.TableLayoutPanel1.RowCount = 3
        Me.TableLayoutPanel1.RowStyles.Add(New System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 3.375844!))
        Me.TableLayoutPanel1.RowStyles.Add(New System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 96.62415!))
        Me.TableLayoutPanel1.RowStyles.Add(New System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 32.0!))
        Me.TableLayoutPanel1.Size = New System.Drawing.Size(480, 360)
        Me.TableLayoutPanel1.TabIndex = 2
        '
        'TableLayoutPanel2
        '
        Me.TableLayoutPanel2.CellBorderStyle = System.Windows.Forms.TableLayoutPanelCellBorderStyle.Inset
        Me.TableLayoutPanel2.ColumnCount = 2
        Me.TableLayoutPanel2.ColumnStyles.Add(New System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 36.16438!))
        Me.TableLayoutPanel2.ColumnStyles.Add(New System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 63.83562!))
        Me.TableLayoutPanel2.Controls.Add(Me.Label1, 1, 0)
        Me.TableLayoutPanel2.Controls.Add(Me.chkMutex, 0, 1)
        Me.TableLayoutPanel2.Controls.Add(Me.Label2, 1, 1)
        Me.TableLayoutPanel2.Controls.Add(Me.Label3, 1, 2)
        Me.TableLayoutPanel2.Controls.Add(Me.Label4, 1, 3)
        Me.TableLayoutPanel2.Controls.Add(Me.chkSem, 0, 0)
        Me.TableLayoutPanel2.Controls.Add(Me.TableLayoutPanel3, 0, 3)
        Me.TableLayoutPanel2.Controls.Add(Me.chkEvent, 0, 2)
        Me.TableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill
        Me.TableLayoutPanel2.Location = New System.Drawing.Point(13, 14)
        Me.TableLayoutPanel2.Name = "TableLayoutPanel2"
        Me.TableLayoutPanel2.RowCount = 4
        Me.TableLayoutPanel2.RowStyles.Add(New System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 20.0!))
        Me.TableLayoutPanel2.RowStyles.Add(New System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 30.0!))
        Me.TableLayoutPanel2.RowStyles.Add(New System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 20.0!))
        Me.TableLayoutPanel2.RowStyles.Add(New System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 30.0!))
        Me.TableLayoutPanel2.Size = New System.Drawing.Size(453, 310)
        Me.TableLayoutPanel2.TabIndex = 0
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Location = New System.Drawing.Point(175, 12)
        Me.Label1.Margin = New System.Windows.Forms.Padding(10)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(261, 40)
        Me.Label1.TabIndex = 1
        Me.Label1.Text = "Enabling this features provides both counting and binary semaphores for thread sy" & _
    "nchronization. If you have to only pick one blocking mechanism, this is the one " & _
    "to choose."
        '
        'chkMutex
        '
        Me.chkMutex.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.chkMutex.Appearance = System.Windows.Forms.Appearance.Button
        Me.chkMutex.Location = New System.Drawing.Point(12, 74)
        Me.chkMutex.Margin = New System.Windows.Forms.Padding(10)
        Me.chkMutex.Name = "chkMutex"
        Me.chkMutex.Size = New System.Drawing.Size(141, 37)
        Me.chkMutex.TabIndex = 2
        Me.chkMutex.Text = "Mutexes"
        Me.chkMutex.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
        Me.chkMutex.UseVisualStyleBackColor = True
        '
        'Label2
        '
        Me.Label2.AutoSize = True
        Me.Label2.Dock = System.Windows.Forms.DockStyle.Fill
        Me.Label2.Location = New System.Drawing.Point(175, 74)
        Me.Label2.Margin = New System.Windows.Forms.Padding(10)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(266, 70)
        Me.Label2.TabIndex = 3
        Me.Label2.Text = resources.GetString("Label2.Text")
        '
        'Label3
        '
        Me.Label3.AutoSize = True
        Me.Label3.Dock = System.Windows.Forms.DockStyle.Fill
        Me.Label3.Location = New System.Drawing.Point(175, 166)
        Me.Label3.Margin = New System.Windows.Forms.Padding(10)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(266, 40)
        Me.Label3.TabIndex = 5
        Me.Label3.Text = resources.GetString("Label3.Text")
        '
        'Label4
        '
        Me.Label4.AutoSize = True
        Me.Label4.Dock = System.Windows.Forms.DockStyle.Fill
        Me.Label4.Location = New System.Drawing.Point(175, 228)
        Me.Label4.Margin = New System.Windows.Forms.Padding(10)
        Me.Label4.Name = "Label4"
        Me.Label4.Size = New System.Drawing.Size(266, 70)
        Me.Label4.TabIndex = 7
        Me.Label4.Text = "Enable inter-thread communication using message queue objects.  Individual messag" & _
    "es are allocated from a global message pool, which should be sized appropriately" & _
    " to prevent exhaustion in the system."
        '
        'chkSem
        '
        Me.chkSem.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.chkSem.Appearance = System.Windows.Forms.Appearance.Button
        Me.chkSem.Location = New System.Drawing.Point(12, 12)
        Me.chkSem.Margin = New System.Windows.Forms.Padding(10)
        Me.chkSem.Name = "chkSem"
        Me.chkSem.Size = New System.Drawing.Size(141, 33)
        Me.chkSem.TabIndex = 0
        Me.chkSem.Text = "Semaphores"
        Me.chkSem.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
        Me.chkSem.UseVisualStyleBackColor = True
        '
        'TableLayoutPanel3
        '
        Me.TableLayoutPanel3.ColumnCount = 1
        Me.TableLayoutPanel3.ColumnStyles.Add(New System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100.0!))
        Me.TableLayoutPanel3.ColumnStyles.Add(New System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 20.0!))
        Me.TableLayoutPanel3.Controls.Add(Me.TableLayoutPanel4, 0, 1)
        Me.TableLayoutPanel3.Controls.Add(Me.chkQueue, 0, 0)
        Me.TableLayoutPanel3.Dock = System.Windows.Forms.DockStyle.Fill
        Me.TableLayoutPanel3.Location = New System.Drawing.Point(5, 221)
        Me.TableLayoutPanel3.Name = "TableLayoutPanel3"
        Me.TableLayoutPanel3.RowCount = 2
        Me.TableLayoutPanel3.RowStyles.Add(New System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50.0!))
        Me.TableLayoutPanel3.RowStyles.Add(New System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50.0!))
        Me.TableLayoutPanel3.Size = New System.Drawing.Size(155, 84)
        Me.TableLayoutPanel3.TabIndex = 8
        '
        'TableLayoutPanel4
        '
        Me.TableLayoutPanel4.ColumnCount = 2
        Me.TableLayoutPanel4.ColumnStyles.Add(New System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 72.72727!))
        Me.TableLayoutPanel4.ColumnStyles.Add(New System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 27.27273!))
        Me.TableLayoutPanel4.Controls.Add(Me.Label5, 0, 0)
        Me.TableLayoutPanel4.Controls.Add(Me.NumericUpDown1, 1, 0)
        Me.TableLayoutPanel4.Dock = System.Windows.Forms.DockStyle.Fill
        Me.TableLayoutPanel4.Location = New System.Drawing.Point(3, 45)
        Me.TableLayoutPanel4.Name = "TableLayoutPanel4"
        Me.TableLayoutPanel4.RowCount = 1
        Me.TableLayoutPanel4.RowStyles.Add(New System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100.0!))
        Me.TableLayoutPanel4.Size = New System.Drawing.Size(149, 36)
        Me.TableLayoutPanel4.TabIndex = 0
        '
        'Label5
        '
        Me.Label5.AutoSize = True
        Me.Label5.Dock = System.Windows.Forms.DockStyle.Fill
        Me.Label5.Location = New System.Drawing.Point(10, 5)
        Me.Label5.Margin = New System.Windows.Forms.Padding(10, 5, 5, 5)
        Me.Label5.Name = "Label5"
        Me.Label5.Size = New System.Drawing.Size(93, 26)
        Me.Label5.TabIndex = 0
        Me.Label5.Text = "Global Queue Size"
        Me.Label5.TextAlign = System.Drawing.ContentAlignment.TopRight
        '
        'NumericUpDown1
        '
        Me.NumericUpDown1.Location = New System.Drawing.Point(111, 3)
        Me.NumericUpDown1.Minimum = New Decimal(New Integer() {2, 0, 0, 0})
        Me.NumericUpDown1.Name = "NumericUpDown1"
        Me.NumericUpDown1.Size = New System.Drawing.Size(34, 20)
        Me.NumericUpDown1.TabIndex = 1
        Me.NumericUpDown1.Value = New Decimal(New Integer() {2, 0, 0, 0})
        '
        'chkQueue
        '
        Me.chkQueue.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.chkQueue.Appearance = System.Windows.Forms.Appearance.Button
        Me.chkQueue.Location = New System.Drawing.Point(10, 10)
        Me.chkQueue.Margin = New System.Windows.Forms.Padding(10)
        Me.chkQueue.Name = "chkQueue"
        Me.chkQueue.Size = New System.Drawing.Size(135, 22)
        Me.chkQueue.TabIndex = 1
        Me.chkQueue.Text = "Message Queues"
        Me.chkQueue.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
        Me.chkQueue.UseVisualStyleBackColor = True
        '
        'chkEvent
        '
        Me.chkEvent.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.chkEvent.Appearance = System.Windows.Forms.Appearance.Button
        Me.chkEvent.Location = New System.Drawing.Point(12, 166)
        Me.chkEvent.Margin = New System.Windows.Forms.Padding(10)
        Me.chkEvent.Name = "chkEvent"
        Me.chkEvent.Size = New System.Drawing.Size(141, 36)
        Me.chkEvent.TabIndex = 9
        Me.chkEvent.Text = "Event Flags"
        Me.chkEvent.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
        Me.chkEvent.UseVisualStyleBackColor = True
        '
        'Panel1
        '
        Me.Panel1.Controls.Add(Me.btnBack)
        Me.Panel1.Controls.Add(Me.btnNext)
        Me.Panel1.Dock = System.Windows.Forms.DockStyle.Fill
        Me.Panel1.Location = New System.Drawing.Point(13, 330)
        Me.Panel1.Name = "Panel1"
        Me.Panel1.Size = New System.Drawing.Size(453, 27)
        Me.Panel1.TabIndex = 1
        '
        'btnBack
        '
        Me.btnBack.Anchor = System.Windows.Forms.AnchorStyles.Right
        Me.btnBack.Location = New System.Drawing.Point(294, 0)
        Me.btnBack.Name = "btnBack"
        Me.btnBack.Size = New System.Drawing.Size(75, 23)
        Me.btnBack.TabIndex = 3
        Me.btnBack.Text = "< Prev"
        Me.btnBack.UseVisualStyleBackColor = True
        '
        'btnNext
        '
        Me.btnNext.Anchor = System.Windows.Forms.AnchorStyles.Right
        Me.btnNext.Location = New System.Drawing.Point(375, 0)
        Me.btnNext.Name = "btnNext"
        Me.btnNext.Size = New System.Drawing.Size(75, 23)
        Me.btnNext.TabIndex = 2
        Me.btnNext.Text = "Next >"
        Me.btnNext.UseVisualStyleBackColor = True
        '
        'frmPanelSynch
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.AutoSize = True
        Me.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink
        Me.ClientSize = New System.Drawing.Size(480, 360)
        Me.Controls.Add(Me.TableLayoutPanel1)
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None
        Me.Name = "frmPanelSynch"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent
        Me.Text = "frmPanelSynch"
        Me.WindowState = System.Windows.Forms.FormWindowState.Maximized
        Me.TableLayoutPanel1.ResumeLayout(False)
        Me.TableLayoutPanel2.ResumeLayout(False)
        Me.TableLayoutPanel2.PerformLayout()
        Me.TableLayoutPanel3.ResumeLayout(False)
        Me.TableLayoutPanel4.ResumeLayout(False)
        Me.TableLayoutPanel4.PerformLayout()
        CType(Me.NumericUpDown1, System.ComponentModel.ISupportInitialize).EndInit()
        Me.Panel1.ResumeLayout(False)
        Me.ResumeLayout(False)

    End Sub
    Friend WithEvents TableLayoutPanel1 As System.Windows.Forms.TableLayoutPanel
    Friend WithEvents TableLayoutPanel2 As System.Windows.Forms.TableLayoutPanel
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents chkMutex As System.Windows.Forms.CheckBox
    Friend WithEvents Label2 As System.Windows.Forms.Label
    Friend WithEvents Label3 As System.Windows.Forms.Label
    Friend WithEvents Label4 As System.Windows.Forms.Label
    Friend WithEvents chkSem As System.Windows.Forms.CheckBox
    Friend WithEvents TableLayoutPanel3 As System.Windows.Forms.TableLayoutPanel
    Friend WithEvents TableLayoutPanel4 As System.Windows.Forms.TableLayoutPanel
    Friend WithEvents Label5 As System.Windows.Forms.Label
    Friend WithEvents NumericUpDown1 As System.Windows.Forms.NumericUpDown
    Friend WithEvents chkQueue As System.Windows.Forms.CheckBox
    Friend WithEvents chkEvent As System.Windows.Forms.CheckBox
    Friend WithEvents Panel1 As System.Windows.Forms.Panel
    Friend WithEvents btnBack As System.Windows.Forms.Button
    Friend WithEvents btnNext As System.Windows.Forms.Button
End Class
