Imports System.Windows.Forms

Public Class frmMain

    Private m_Cfg As clMark3Cfg

    ' Enumeration used to track the current form displayed in Panel2
    Private Enum CurrentForm
        CurrentFormHome
        CurrentFormThread
        CurrentFormTimers
        CurrentFormSynch
        CurrentFormExtras
        CurrentFormExport
        ' 
        CurrentFormNone
    End Enum

    ' Objects for each form that may be displayed in Panel2
    Private pHome As frmPanelHome
    Private pThread As frmPanelThread
    Private pTimers As frmPanelTimers
    Private pSynch As frmPanelSynch
    Private pExtras As frmPanelExtras
    Private pExport As frmPanelExport
    Private eCurrentForm As CurrentForm


    Private Sub Panel1_Paint(sender As Object, e As PaintEventArgs)

    End Sub

    Private Sub TableLayoutPanel1_Paint(sender As Object, e As PaintEventArgs)

    End Sub

    Private Sub TableLayoutPanel1_Paint_1(sender As Object, e As PaintEventArgs) Handles TableLayoutPanel1.Paint

    End Sub

    Private Sub SplitContainer1_Panel2_Paint(sender As Object, e As PaintEventArgs) Handles SplitContainer1.Panel2.Paint

    End Sub

    Private Sub Button1_Click(sender As Object, e As EventArgs)

    End Sub

    Private Sub ResetButtonColors()
        btnExport.BackColor = SystemColors.Control
        btnHome.BackColor = SystemColors.Control
        btnThreads.BackColor = SystemColors.Control
        btnTimers.BackColor = SystemColors.Control
        btnSync.BackColor = SystemColors.Control
        btnFeatures.BackColor = SystemColors.Control
    End Sub

    Private Sub ButtonHome_Click(sender As Object, e As EventArgs) Handles btnHome.Click
        ShowHomeForm()
        ResetButtonColors()
        btnHome.BackColor = SystemColors.ControlLightLight
    End Sub

    Private Sub CreateForms()
        m_Cfg = New clMark3Cfg
        m_Cfg.SetDefaults()

        pHome = New frmPanelHome
        pSynch = New frmPanelSynch
        pThread = New frmPanelThread
        pTimers = New frmPanelTimers
        pExport = New frmPanelExport
        pExtras = New frmPanelExtras

        pHome.SetCfg(m_Cfg)
        pThread.SetCfg(m_Cfg)
        pTimers.SetCfg(m_Cfg)
        pExport.SetCfg(m_Cfg)
        pExtras.SetCfg(m_Cfg)
        pSynch.SetCfg(m_Cfg)
    End Sub
    Private Sub HideForms()
        pHome.Hide()
        pThread.Hide()
        pTimers.Hide()
        pExport.Hide()
        pExtras.Hide()
        pSynch.Hide()
    End Sub
    Private Sub ShowHomeForm()
        If eCurrentForm <> CurrentForm.CurrentFormHome Then
            HideForms()
            Me.SplitContainer1.Panel2.Controls.Clear()
            pHome.TopLevel = False
            Me.SplitContainer1.Panel2.Controls.Add(pHome)
            pHome.Show()
            eCurrentForm = CurrentForm.CurrentFormHome
        End If
    End Sub
    Private Sub ShowThreadForm()
        If eCurrentForm <> CurrentForm.CurrentFormThread Then
            HideForms()
            Me.SplitContainer1.Panel2.Controls.Clear()
            pThread.TopLevel = False
            Me.SplitContainer1.Panel2.Controls.Add(pThread)
            pThread.Show()
            eCurrentForm = CurrentForm.CurrentFormThread
        End If
    End Sub
    Private Sub ShowTimersForm()
        If eCurrentForm <> CurrentForm.CurrentFormTimers Then
            HideForms()
            Me.SplitContainer1.Panel2.Controls.Clear()
            pTimers.TopLevel = False
            Me.SplitContainer1.Panel2.Controls.Add(pTimers)
            pTimers.Show()
            eCurrentForm = CurrentForm.CurrentFormTimers
        End If
    End Sub
    Private Sub ShowSynchForm()
        If eCurrentForm <> CurrentForm.CurrentFormSynch Then
            HideForms()
            Me.SplitContainer1.Panel2.Controls.Clear()
            pSynch.TopLevel = False
            Me.SplitContainer1.Panel2.Controls.Add(pSynch)
            pSynch.Show()
            eCurrentForm = CurrentForm.CurrentFormSynch
        End If
    End Sub
    Private Sub ShowExtrasForm()
        If eCurrentForm <> CurrentForm.CurrentFormExtras Then
            HideForms()
            Me.SplitContainer1.Panel2.Controls.Clear()
            pExtras.TopLevel = False
            Me.SplitContainer1.Panel2.Controls.Add(pExtras)
            pExtras.Show()
            eCurrentForm = CurrentForm.CurrentFormExtras
        End If
    End Sub
    Private Sub ShowExportForm()
        If eCurrentForm <> CurrentForm.CurrentFormExport Then
            HideForms()
            Me.SplitContainer1.Panel2.Controls.Clear()
            pExport.TopLevel = False
            Me.SplitContainer1.Panel2.Controls.Add(pExport)
            pExport.Show()
            pExport.RefreshData()
            eCurrentForm = CurrentForm.CurrentFormExport
        End If
    End Sub
    Public Sub DisableNav()
        btnExport.Enabled = False
        btnFeatures.Enabled = False
        btnSync.Enabled = False
        btnThreads.Enabled = False
        btnTimers.Enabled = False
    End Sub

    Public Sub EnableNav()
        btnExport.Enabled = True
        btnFeatures.Enabled = True
        btnSync.Enabled = True
        btnThreads.Enabled = True
        btnTimers.Enabled = True
    End Sub

    Private Sub FormLoad(sender As Object, e As EventArgs) Handles MyBase.Load
        eCurrentForm = CurrentForm.CurrentFormNone
        CreateForms()
        ShowHomeForm()
        DisableNav()
        btnHome.BackColor = SystemColors.ControlLightLight
    End Sub

    Private Sub btnThreads_Click(sender As Object, e As EventArgs) Handles btnThreads.Click
        ShowThreadForm()
        ResetButtonColors()
        btnThreads.BackColor = SystemColors.ControlLightLight
    End Sub

    Private Sub btnTimers_Click(sender As Object, e As EventArgs) Handles btnTimers.Click
        ShowTimersForm()
        ResetButtonColors()
        btnTimers.BackColor = SystemColors.ControlLightLight
    End Sub

    Private Sub btnSync_Click(sender As Object, e As EventArgs) Handles btnSync.Click
        ShowSynchForm()
        ResetButtonColors()
        btnSync.BackColor = SystemColors.ControlLightLight
    End Sub

    Private Sub btnFeatures_Click(sender As Object, e As EventArgs) Handles btnFeatures.Click
        ShowExtrasForm()
        ResetButtonColors()
        btnFeatures.BackColor = SystemColors.ControlLightLight
    End Sub

    Private Sub btnExport_Click(sender As Object, e As EventArgs) Handles btnExport.Click
        ShowExportForm()
        ResetButtonColors()
        btnExport.BackColor = SystemColors.ControlLightLight
    End Sub
End Class
