Public Class frmPanelThread
    Private m_Cfg As clMark3Cfg

    Public Sub SetCfg(ByRef clCfg As clMark3Cfg)
        m_Cfg = clCfg
        RescanConfig()
    End Sub

    Private Sub RescanConfig()
        chkDynamic.Checked = m_Cfg.GetUseDynamic()
        chkNamed.Checked = m_Cfg.GetUseNamedThreads()
        chkUnlink.Checked = m_Cfg.GetUseSafeUnlinking()
        NumericUpDown1.Value = m_Cfg.GetTransactionQueueSize()
    End Sub

    Private Sub chkDynamic_CheckedChanged(sender As Object, e As EventArgs) Handles chkDynamic.CheckedChanged
        If (IsNothing(m_Cfg)) Then
            Return
        End If

        m_Cfg.SetUseDynamic(chkDynamic.Checked)
        If (m_Cfg.Validate() = False) Then
            chkDynamic.Checked = False
        End If
        RescanConfig()
    End Sub

    Private Sub chkNamed_CheckedChanged(sender As Object, e As EventArgs) Handles chkNamed.CheckedChanged
        If (IsNothing(m_Cfg)) Then
            Return
        End If

        m_Cfg.SetUseNamedThreads(chkNamed.Checked)
        If (m_Cfg.Validate() = False) Then
            chkNamed.Checked = False
        End If
        RescanConfig()
    End Sub

    Private Sub chkUnlink_CheckedChanged(sender As Object, e As EventArgs) Handles chkUnlink.CheckedChanged
        If (IsNothing(m_Cfg)) Then
            Return
        End If

        m_Cfg.SetUseSafeUnlinking(chkUnlink.Checked)
        If (m_Cfg.Validate() = False) Then
            chkUnlink.Checked = False
        End If
        RescanConfig()
    End Sub

    Private Sub NumericUpDown1_ValueChanged(sender As Object, e As EventArgs) Handles NumericUpDown1.ValueChanged
        If (IsNothing(m_Cfg)) Then
            Return
        End If

        m_Cfg.SetTransactionQueueSize(NumericUpDown1.Value)
        If (m_Cfg.Validate() = False) Then
            NumericUpDown1.Value = 2
        End If
    End Sub

    Private Sub btnNext_Click(sender As Object, e As EventArgs) Handles btnNext.Click
        frmMain.btnTimers.PerformClick()
    End Sub

    Private Sub btnBack_Click(sender As Object, e As EventArgs) Handles btnBack.Click
        frmMain.btnHome.PerformClick()
    End Sub
End Class