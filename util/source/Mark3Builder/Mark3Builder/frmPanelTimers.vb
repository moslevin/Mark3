Public Class frmPanelTimers

    Private m_Cfg As clMark3Cfg

    Public Sub SetCfg(ByRef clCfg As clMark3Cfg)
        m_Cfg = clCfg
        RescanConfig()
    End Sub

    Private Sub RescanConfig()
        chkTimers.Checked = m_Cfg.GetUseTimers()
        chkTickless.Checked = m_Cfg.GetUseTickless()
        chkSleep.Checked = m_Cfg.GetUseSleep()
        chkRR.Checked = m_Cfg.GetUseQuanta()
        NumericUpDown1.Value = m_Cfg.GetTransactionQueueSize()
    End Sub
    Private Sub chkTimers_CheckedChanged(sender As Object, e As EventArgs) Handles chkTimers.CheckedChanged
        If (IsNothing(m_Cfg)) Then
            Return
        End If

        m_Cfg.SetUseTimers(chkTimers.Checked)
        If (m_Cfg.Validate() = False) Then
            chkTimers.Checked = False
        End If
        RescanConfig()
    End Sub

    Private Sub chkTickless_CheckedChanged(sender As Object, e As EventArgs) Handles chkTickless.CheckedChanged
        If (IsNothing(m_Cfg)) Then
            Return
        End If

        m_Cfg.SetUseTickless(chkTickless.Checked)
        If (m_Cfg.Validate() = False) Then
            chkTickless.Checked = False
        End If
        RescanConfig()
    End Sub

    Private Sub chkSleep_CheckedChanged(sender As Object, e As EventArgs) Handles chkSleep.CheckedChanged
        If (IsNothing(m_Cfg)) Then
            Return
        End If

        m_Cfg.SetUseSleep(chkSleep.Checked)
        If (m_Cfg.Validate() = False) Then
            chkSleep.Checked = False
        End If
        RescanConfig()
    End Sub

    Private Sub chkRR_CheckedChanged(sender As Object, e As EventArgs) Handles chkRR.CheckedChanged
        If (IsNothing(m_Cfg)) Then
            Return
        End If

        m_Cfg.SetUseQuanta(chkRR.Checked)
        If (m_Cfg.Validate() = False) Then
            chkRR.Checked = False
        Else
            m_Cfg.SetDefaultQuanta(NumericUpDown1.Value)
            If (m_Cfg.Validate() = False) Then
                NumericUpDown1.Value = 2
            End If
        End If
        RescanConfig()
    End Sub

    Private Sub NumericUpDown1_ValueChanged(sender As Object, e As EventArgs) Handles NumericUpDown1.ValueChanged
        If (IsNothing(m_Cfg)) Then
            Return
        End If

        m_Cfg.SetDefaultQuanta(NumericUpDown1.Value)
        If (m_Cfg.Validate() = False) Then
            NumericUpDown1.Value = 2
        End If
    End Sub

    Private Sub btnBack_Click(sender As Object, e As EventArgs) Handles btnBack.Click
        frmMain.btnThreads.PerformClick()
    End Sub

    Private Sub btnNext_Click(sender As Object, e As EventArgs) Handles btnNext.Click
        frmMain.btnSync.PerformClick()
    End Sub
End Class