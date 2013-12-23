<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class frmMain
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
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(frmMain))
        Me.SplitContainer1 = New System.Windows.Forms.SplitContainer()
        Me.TableLayoutPanel1 = New System.Windows.Forms.TableLayoutPanel()
        Me.btnThreads = New System.Windows.Forms.Button()
        Me.btnHome = New System.Windows.Forms.Button()
        Me.btnExport = New System.Windows.Forms.Button()
        Me.btnFeatures = New System.Windows.Forms.Button()
        Me.btnSync = New System.Windows.Forms.Button()
        Me.btnTimers = New System.Windows.Forms.Button()
        CType(Me.SplitContainer1, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SplitContainer1.Panel1.SuspendLayout()
        Me.SplitContainer1.SuspendLayout()
        Me.TableLayoutPanel1.SuspendLayout()
        Me.SuspendLayout()
        '
        'SplitContainer1
        '
        Me.SplitContainer1.Dock = System.Windows.Forms.DockStyle.Fill
        Me.SplitContainer1.FixedPanel = System.Windows.Forms.FixedPanel.Panel1
        Me.SplitContainer1.IsSplitterFixed = True
        Me.SplitContainer1.Location = New System.Drawing.Point(0, 0)
        Me.SplitContainer1.Margin = New System.Windows.Forms.Padding(1)
        Me.SplitContainer1.Name = "SplitContainer1"
        '
        'SplitContainer1.Panel1
        '
        Me.SplitContainer1.Panel1.Controls.Add(Me.TableLayoutPanel1)
        '
        'SplitContainer1.Panel2
        '
        Me.SplitContainer1.Size = New System.Drawing.Size(786, 415)
        Me.SplitContainer1.SplitterDistance = 224
        Me.SplitContainer1.SplitterWidth = 1
        Me.SplitContainer1.TabIndex = 0
        '
        'TableLayoutPanel1
        '
        Me.TableLayoutPanel1.ColumnCount = 1
        Me.TableLayoutPanel1.ColumnStyles.Add(New System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100.0!))
        Me.TableLayoutPanel1.Controls.Add(Me.btnThreads, 0, 1)
        Me.TableLayoutPanel1.Controls.Add(Me.btnHome, 0, 0)
        Me.TableLayoutPanel1.Controls.Add(Me.btnExport, 0, 5)
        Me.TableLayoutPanel1.Controls.Add(Me.btnFeatures, 0, 4)
        Me.TableLayoutPanel1.Controls.Add(Me.btnSync, 0, 3)
        Me.TableLayoutPanel1.Controls.Add(Me.btnTimers, 0, 2)
        Me.TableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill
        Me.TableLayoutPanel1.Location = New System.Drawing.Point(0, 0)
        Me.TableLayoutPanel1.Margin = New System.Windows.Forms.Padding(1)
        Me.TableLayoutPanel1.Name = "TableLayoutPanel1"
        Me.TableLayoutPanel1.RowCount = 6
        Me.TableLayoutPanel1.RowStyles.Add(New System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 16.66667!))
        Me.TableLayoutPanel1.RowStyles.Add(New System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 16.66667!))
        Me.TableLayoutPanel1.RowStyles.Add(New System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 16.66667!))
        Me.TableLayoutPanel1.RowStyles.Add(New System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 16.66667!))
        Me.TableLayoutPanel1.RowStyles.Add(New System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 16.66667!))
        Me.TableLayoutPanel1.RowStyles.Add(New System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 16.66667!))
        Me.TableLayoutPanel1.RowStyles.Add(New System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20.0!))
        Me.TableLayoutPanel1.RowStyles.Add(New System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20.0!))
        Me.TableLayoutPanel1.RowStyles.Add(New System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20.0!))
        Me.TableLayoutPanel1.Size = New System.Drawing.Size(224, 415)
        Me.TableLayoutPanel1.TabIndex = 1
        '
        'btnThreads
        '
        Me.btnThreads.AccessibleRole = System.Windows.Forms.AccessibleRole.TitleBar
        Me.btnThreads.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.btnThreads.AutoSize = True
        Me.btnThreads.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink
        Me.btnThreads.BackColor = System.Drawing.SystemColors.Control
        Me.btnThreads.DialogResult = System.Windows.Forms.DialogResult.Cancel
        Me.btnThreads.FlatAppearance.MouseDownBackColor = System.Drawing.SystemColors.ControlLight
        Me.btnThreads.FlatAppearance.MouseOverBackColor = System.Drawing.SystemColors.ControlLightLight
        Me.btnThreads.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.btnThreads.Font = New System.Drawing.Font("Verdana", 14.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.btnThreads.Image = CType(resources.GetObject("btnThreads.Image"), System.Drawing.Image)
        Me.btnThreads.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.btnThreads.Location = New System.Drawing.Point(1, 70)
        Me.btnThreads.Margin = New System.Windows.Forms.Padding(1)
        Me.btnThreads.Name = "btnThreads"
        Me.btnThreads.Size = New System.Drawing.Size(222, 67)
        Me.btnThreads.TabIndex = 3
        Me.btnThreads.Text = "Threads"
        Me.btnThreads.TextAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.btnThreads.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText
        Me.btnThreads.UseVisualStyleBackColor = False
        '
        'btnHome
        '
        Me.btnHome.AccessibleRole = System.Windows.Forms.AccessibleRole.TitleBar
        Me.btnHome.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.btnHome.AutoSize = True
        Me.btnHome.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink
        Me.btnHome.DialogResult = System.Windows.Forms.DialogResult.Cancel
        Me.btnHome.FlatAppearance.MouseDownBackColor = System.Drawing.SystemColors.ControlLight
        Me.btnHome.FlatAppearance.MouseOverBackColor = System.Drawing.SystemColors.ControlLightLight
        Me.btnHome.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.btnHome.Font = New System.Drawing.Font("Verdana", 14.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.btnHome.Image = CType(resources.GetObject("btnHome.Image"), System.Drawing.Image)
        Me.btnHome.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.btnHome.Location = New System.Drawing.Point(1, 1)
        Me.btnHome.Margin = New System.Windows.Forms.Padding(1)
        Me.btnHome.Name = "btnHome"
        Me.btnHome.Size = New System.Drawing.Size(222, 67)
        Me.btnHome.TabIndex = 2
        Me.btnHome.Text = "Home"
        Me.btnHome.TextAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.btnHome.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText
        Me.btnHome.UseVisualStyleBackColor = False
        '
        'btnExport
        '
        Me.btnExport.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.btnExport.AutoSize = True
        Me.btnExport.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink
        Me.btnExport.BackColor = System.Drawing.SystemColors.Control
        Me.btnExport.DialogResult = System.Windows.Forms.DialogResult.Cancel
        Me.btnExport.FlatAppearance.MouseDownBackColor = System.Drawing.SystemColors.ControlLight
        Me.btnExport.FlatAppearance.MouseOverBackColor = System.Drawing.SystemColors.ControlLightLight
        Me.btnExport.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.btnExport.Font = New System.Drawing.Font("Verdana", 14.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.btnExport.Image = CType(resources.GetObject("btnExport.Image"), System.Drawing.Image)
        Me.btnExport.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.btnExport.Location = New System.Drawing.Point(1, 346)
        Me.btnExport.Margin = New System.Windows.Forms.Padding(1)
        Me.btnExport.Name = "btnExport"
        Me.btnExport.Size = New System.Drawing.Size(222, 68)
        Me.btnExport.TabIndex = 1
        Me.btnExport.Text = "Export"
        Me.btnExport.TextAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.btnExport.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText
        Me.btnExport.UseVisualStyleBackColor = False
        '
        'btnFeatures
        '
        Me.btnFeatures.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.btnFeatures.AutoSize = True
        Me.btnFeatures.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink
        Me.btnFeatures.FlatAppearance.MouseDownBackColor = System.Drawing.SystemColors.ControlLight
        Me.btnFeatures.FlatAppearance.MouseOverBackColor = System.Drawing.SystemColors.ControlLightLight
        Me.btnFeatures.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.btnFeatures.Font = New System.Drawing.Font("Verdana", 14.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.btnFeatures.Image = CType(resources.GetObject("btnFeatures.Image"), System.Drawing.Image)
        Me.btnFeatures.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.btnFeatures.Location = New System.Drawing.Point(1, 277)
        Me.btnFeatures.Margin = New System.Windows.Forms.Padding(1)
        Me.btnFeatures.Name = "btnFeatures"
        Me.btnFeatures.Size = New System.Drawing.Size(222, 67)
        Me.btnFeatures.TabIndex = 1
        Me.btnFeatures.Text = "Features"
        Me.btnFeatures.TextAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.btnFeatures.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText
        Me.btnFeatures.UseVisualStyleBackColor = False
        '
        'btnSync
        '
        Me.btnSync.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.btnSync.AutoSize = True
        Me.btnSync.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink
        Me.btnSync.FlatAppearance.MouseDownBackColor = System.Drawing.SystemColors.ControlLight
        Me.btnSync.FlatAppearance.MouseOverBackColor = System.Drawing.SystemColors.ControlLightLight
        Me.btnSync.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.btnSync.Font = New System.Drawing.Font("Verdana", 14.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.btnSync.Image = CType(resources.GetObject("btnSync.Image"), System.Drawing.Image)
        Me.btnSync.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.btnSync.Location = New System.Drawing.Point(1, 208)
        Me.btnSync.Margin = New System.Windows.Forms.Padding(1)
        Me.btnSync.Name = "btnSync"
        Me.btnSync.Size = New System.Drawing.Size(222, 67)
        Me.btnSync.TabIndex = 1
        Me.btnSync.Text = "Synchronization"
        Me.btnSync.TextAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.btnSync.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText
        Me.btnSync.UseVisualStyleBackColor = False
        '
        'btnTimers
        '
        Me.btnTimers.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.btnTimers.AutoSize = True
        Me.btnTimers.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink
        Me.btnTimers.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None
        Me.btnTimers.FlatAppearance.MouseDownBackColor = System.Drawing.SystemColors.ControlLight
        Me.btnTimers.FlatAppearance.MouseOverBackColor = System.Drawing.SystemColors.ControlLightLight
        Me.btnTimers.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.btnTimers.Font = New System.Drawing.Font("Verdana", 14.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.btnTimers.Image = CType(resources.GetObject("btnTimers.Image"), System.Drawing.Image)
        Me.btnTimers.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.btnTimers.Location = New System.Drawing.Point(1, 139)
        Me.btnTimers.Margin = New System.Windows.Forms.Padding(1)
        Me.btnTimers.Name = "btnTimers"
        Me.btnTimers.Size = New System.Drawing.Size(222, 67)
        Me.btnTimers.TabIndex = 1
        Me.btnTimers.Text = "Timers"
        Me.btnTimers.TextAlign = System.Drawing.ContentAlignment.MiddleLeft
        Me.btnTimers.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText
        Me.btnTimers.UseVisualStyleBackColor = False
        '
        'frmMain
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(786, 415)
        Me.Controls.Add(Me.SplitContainer1)
        Me.Cursor = System.Windows.Forms.Cursors.Arrow
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow
        Me.MaximizeBox = False
        Me.MinimizeBox = False
        Me.Name = "frmMain"
        Me.Text = "Mark3 Kernel Configurator"
        Me.SplitContainer1.Panel1.ResumeLayout(False)
        CType(Me.SplitContainer1, System.ComponentModel.ISupportInitialize).EndInit()
        Me.SplitContainer1.ResumeLayout(False)
        Me.TableLayoutPanel1.ResumeLayout(False)
        Me.TableLayoutPanel1.PerformLayout()
        Me.ResumeLayout(False)

    End Sub
    Friend WithEvents SplitContainer1 As System.Windows.Forms.SplitContainer
    Friend WithEvents TableLayoutPanel1 As System.Windows.Forms.TableLayoutPanel
    Friend WithEvents btnExport As System.Windows.Forms.Button
    Friend WithEvents btnFeatures As System.Windows.Forms.Button
    Friend WithEvents btnSync As System.Windows.Forms.Button
    Friend WithEvents btnTimers As System.Windows.Forms.Button
    Friend WithEvents btnThreads As System.Windows.Forms.Button
    Friend WithEvents btnHome As System.Windows.Forms.Button

End Class
