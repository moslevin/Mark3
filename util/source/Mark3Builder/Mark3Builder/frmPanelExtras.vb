Public Class frmPanelExtras
    Private m_Cfg As clMark3Cfg

    Public Sub SetCfg(ByRef clCfg As clMark3Cfg)
        m_Cfg = clCfg
        RescanConfig()
    End Sub

    Private Sub RescanConfig()
        chkAtomic.Checked = m_Cfg.GetUseAtomic()
        chkDrivers.Checked = m_Cfg.GetUseDrivers()
        chkDebug.Checked = m_Cfg.GetUseDebug()
        chkProfiling.Checked = m_Cfg.GetUseProfilingTimers()
    End Sub

    Private Sub chkAtomic_CheckedChanged(sender As Object, e As EventArgs) Handles chkAtomic.CheckedChanged
        If (IsNothing(m_Cfg)) Then
            Return
        End If

        m_Cfg.SetUseAtomic(chkAtomic.Checked)
        If (m_Cfg.Validate() = False) Then
            chkAtomic.Checked = False
        End If
        RescanConfig()
    End Sub

    Private Sub chkDebug_CheckedChanged(sender As Object, e As EventArgs) Handles chkDebug.CheckedChanged
        If (IsNothing(m_Cfg)) Then
            Return
        End If

        m_Cfg.SetUseDebug(chkDebug.Checked)
        If (m_Cfg.Validate() = False) Then
            chkDebug.Checked = False
        End If
        RescanConfig()
    End Sub

    Private Sub chkProfiling_CheckedChanged(sender As Object, e As EventArgs) Handles chkProfiling.CheckedChanged
        If (IsNothing(m_Cfg)) Then
            Return
        End If

        m_Cfg.SetUseProfilingTimers(chkProfiling.Checked)
        If (m_Cfg.Validate() = False) Then
            chkProfiling.Checked = False
        End If
        RescanConfig()
    End Sub

    Private Sub chkDrivers_CheckedChanged(sender As Object, e As EventArgs) Handles chkDrivers.CheckedChanged
        If (IsNothing(m_Cfg)) Then
            Return
        End If

        m_Cfg.SetUseDrivers(chkDrivers.Checked)
        If (m_Cfg.Validate() = False) Then
            chkDrivers.Checked = False
        End If
        RescanConfig()
    End Sub

    Private Sub btnNext_Click(sender As Object, e As EventArgs) Handles btnNext.Click
        frmMain.btnExport.PerformClick()
    End Sub

    Private Sub btnBack_Click(sender As Object, e As EventArgs) Handles btnBack.Click
        frmMain.btnSync.PerformClick()
    End Sub
End Class