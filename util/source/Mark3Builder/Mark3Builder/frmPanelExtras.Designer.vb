<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class frmPanelExtras
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
        Me.TableLayoutPanel1 = New System.Windows.Forms.TableLayoutPanel()
        Me.TableLayoutPanel2 = New System.Windows.Forms.TableLayoutPanel()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.chkDebug = New System.Windows.Forms.CheckBox()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.Label3 = New System.Windows.Forms.Label()
        Me.Label4 = New System.Windows.Forms.Label()
        Me.chkAtomic = New System.Windows.Forms.CheckBox()
        Me.chkProfiling = New System.Windows.Forms.CheckBox()
        Me.chkDrivers = New System.Windows.Forms.CheckBox()
        Me.Panel1 = New System.Windows.Forms.Panel()
        Me.btnBack = New System.Windows.Forms.Button()
        Me.btnNext = New System.Windows.Forms.Button()
        Me.TableLayoutPanel1.SuspendLayout()
        Me.TableLayoutPanel2.SuspendLayout()
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
        Me.TableLayoutPanel1.RowStyles.Add(New System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 3.386068!))
        Me.TableLayoutPanel1.RowStyles.Add(New System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 96.61393!))
        Me.TableLayoutPanel1.RowStyles.Add(New System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 32.0!))
        Me.TableLayoutPanel1.Size = New System.Drawing.Size(480, 360)
        Me.TableLayoutPanel1.TabIndex = 3
        '
        'TableLayoutPanel2
        '
        Me.TableLayoutPanel2.CellBorderStyle = System.Windows.Forms.TableLayoutPanelCellBorderStyle.Inset
        Me.TableLayoutPanel2.ColumnCount = 2
        Me.TableLayoutPanel2.ColumnStyles.Add(New System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 36.16438!))
        Me.TableLayoutPanel2.ColumnStyles.Add(New System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 63.83562!))
        Me.TableLayoutPanel2.Controls.Add(Me.Label1, 1, 0)
        Me.TableLayoutPanel2.Controls.Add(Me.chkDebug, 0, 1)
        Me.TableLayoutPanel2.Controls.Add(Me.Label2, 1, 1)
        Me.TableLayoutPanel2.Controls.Add(Me.Label3, 1, 2)
        Me.TableLayoutPanel2.Controls.Add(Me.Label4, 1, 3)
        Me.TableLayoutPanel2.Controls.Add(Me.chkAtomic, 0, 0)
        Me.TableLayoutPanel2.Controls.Add(Me.chkProfiling, 0, 2)
        Me.TableLayoutPanel2.Controls.Add(Me.chkDrivers, 0, 3)
        Me.TableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill
        Me.TableLayoutPanel2.Location = New System.Drawing.Point(13, 14)
        Me.TableLayoutPanel2.Name = "TableLayoutPanel2"
        Me.TableLayoutPanel2.RowCount = 4
        Me.TableLayoutPanel2.RowStyles.Add(New System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 25.0!))
        Me.TableLayoutPanel2.RowStyles.Add(New System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 25.0!))
        Me.TableLayoutPanel2.RowStyles.Add(New System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 25.0!))
        Me.TableLayoutPanel2.RowStyles.Add(New System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 25.0!))
        Me.TableLayoutPanel2.Size = New System.Drawing.Size(453, 310)
        Me.TableLayoutPanel2.TabIndex = 0
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Location = New System.Drawing.Point(175, 12)
        Me.Label1.Margin = New System.Windows.Forms.Padding(10)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(238, 39)
        Me.Label1.TabIndex = 1
        Me.Label1.Text = "Provides support for atomic operations, including addition, subtraction, set, and" & _
    " test-and-set.  Add/Sub/Set contain 8, 16, and 32-bit variants."
        '
        'chkDebug
        '
        Me.chkDebug.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.chkDebug.Appearance = System.Windows.Forms.Appearance.Button
        Me.chkDebug.Location = New System.Drawing.Point(12, 89)
        Me.chkDebug.Margin = New System.Windows.Forms.Padding(10)
        Me.chkDebug.Name = "chkDebug"
        Me.chkDebug.Size = New System.Drawing.Size(141, 37)
        Me.chkDebug.TabIndex = 2
        Me.chkDebug.Text = "Kernel Instrumentation"
        Me.chkDebug.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
        Me.chkDebug.UseVisualStyleBackColor = True
        '
        'Label2
        '
        Me.Label2.AutoSize = True
        Me.Label2.Dock = System.Windows.Forms.DockStyle.Fill
        Me.Label2.Location = New System.Drawing.Point(175, 89)
        Me.Label2.Margin = New System.Windows.Forms.Padding(10)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(266, 55)
        Me.Label2.TabIndex = 3
        Me.Label2.Text = "Provides extra logic for kernel debugging, and instruments the kernel with extra " & _
    "asserts/kernel trace functionality."
        '
        'Label3
        '
        Me.Label3.AutoSize = True
        Me.Label3.Dock = System.Windows.Forms.DockStyle.Fill
        Me.Label3.Location = New System.Drawing.Point(175, 166)
        Me.Label3.Margin = New System.Windows.Forms.Padding(10)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(266, 55)
        Me.Label3.TabIndex = 5
        Me.Label3.Text = "Provides extra classes for profiling code execution.  Useful for debugging and de" & _
    "velopment, but typically uses an additional timer (Platform dependent)."
        '
        'Label4
        '
        Me.Label4.AutoSize = True
        Me.Label4.Dock = System.Windows.Forms.DockStyle.Fill
        Me.Label4.Location = New System.Drawing.Point(175, 243)
        Me.Label4.Margin = New System.Windows.Forms.Padding(10)
        Me.Label4.Name = "Label4"
        Me.Label4.Size = New System.Drawing.Size(266, 55)
        Me.Label4.TabIndex = 7
        Me.Label4.Text = "Enabling device drivers provides a posix-like ""named-file"" interface, with consis" & _
    "tent APIs such as Open()/Close()/Read()/Write()/Control() which are implemented " & _
    "uniquely for each peripheral driver."
        '
        'chkAtomic
        '
        Me.chkAtomic.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.chkAtomic.Appearance = System.Windows.Forms.Appearance.Button
        Me.chkAtomic.Location = New System.Drawing.Point(12, 12)
        Me.chkAtomic.Margin = New System.Windows.Forms.Padding(10)
        Me.chkAtomic.Name = "chkAtomic"
        Me.chkAtomic.Size = New System.Drawing.Size(141, 33)
        Me.chkAtomic.TabIndex = 0
        Me.chkAtomic.Text = "Atomic Operations"
        Me.chkAtomic.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
        Me.chkAtomic.UseVisualStyleBackColor = True
        '
        'chkProfiling
        '
        Me.chkProfiling.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.chkProfiling.Appearance = System.Windows.Forms.Appearance.Button
        Me.chkProfiling.Location = New System.Drawing.Point(12, 166)
        Me.chkProfiling.Margin = New System.Windows.Forms.Padding(10)
        Me.chkProfiling.Name = "chkProfiling"
        Me.chkProfiling.Size = New System.Drawing.Size(141, 36)
        Me.chkProfiling.TabIndex = 9
        Me.chkProfiling.Text = "Profiling Timers"
        Me.chkProfiling.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
        Me.chkProfiling.UseVisualStyleBackColor = True
        '
        'chkDrivers
        '
        Me.chkDrivers.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.chkDrivers.Appearance = System.Windows.Forms.Appearance.Button
        Me.chkDrivers.Location = New System.Drawing.Point(12, 243)
        Me.chkDrivers.Margin = New System.Windows.Forms.Padding(10)
        Me.chkDrivers.Name = "chkDrivers"
        Me.chkDrivers.Size = New System.Drawing.Size(141, 36)
        Me.chkDrivers.TabIndex = 10
        Me.chkDrivers.Text = "Device Drivers"
        Me.chkDrivers.TextAlign = System.Drawing.ContentAlignment.MiddleCenter
        Me.chkDrivers.UseVisualStyleBackColor = True
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
        Me.btnBack.Location = New System.Drawing.Point(297, 1)
        Me.btnBack.Name = "btnBack"
        Me.btnBack.Size = New System.Drawing.Size(75, 23)
        Me.btnBack.TabIndex = 3
        Me.btnBack.Text = "< Prev"
        Me.btnBack.UseVisualStyleBackColor = True
        '
        'btnNext
        '
        Me.btnNext.Anchor = System.Windows.Forms.AnchorStyles.Right
        Me.btnNext.Location = New System.Drawing.Point(378, 1)
        Me.btnNext.Name = "btnNext"
        Me.btnNext.Size = New System.Drawing.Size(75, 23)
        Me.btnNext.TabIndex = 2
        Me.btnNext.Text = "Next >"
        Me.btnNext.UseVisualStyleBackColor = True
        '
        'frmPanelExtras
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.AutoSize = True
        Me.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink
        Me.ClientSize = New System.Drawing.Size(480, 360)
        Me.Controls.Add(Me.TableLayoutPanel1)
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None
        Me.Name = "frmPanelExtras"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent
        Me.Text = "frmPanelExtras"
        Me.WindowState = System.Windows.Forms.FormWindowState.Maximized
        Me.TableLayoutPanel1.ResumeLayout(False)
        Me.TableLayoutPanel2.ResumeLayout(False)
        Me.TableLayoutPanel2.PerformLayout()
        Me.Panel1.ResumeLayout(False)
        Me.ResumeLayout(False)

    End Sub
    Friend WithEvents TableLayoutPanel1 As System.Windows.Forms.TableLayoutPanel
    Friend WithEvents TableLayoutPanel2 As System.Windows.Forms.TableLayoutPanel
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents chkDebug As System.Windows.Forms.CheckBox
    Friend WithEvents Label2 As System.Windows.Forms.Label
    Friend WithEvents Label3 As System.Windows.Forms.Label
    Friend WithEvents Label4 As System.Windows.Forms.Label
    Friend WithEvents chkAtomic As System.Windows.Forms.CheckBox
    Friend WithEvents chkProfiling As System.Windows.Forms.CheckBox
    Friend WithEvents chkDrivers As System.Windows.Forms.CheckBox
    Friend WithEvents Panel1 As System.Windows.Forms.Panel
    Friend WithEvents btnBack As System.Windows.Forms.Button
    Friend WithEvents btnNext As System.Windows.Forms.Button
End Class
