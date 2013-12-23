Public Class frmPanelSynch
    Private m_Cfg As clMark3Cfg

    Public Sub SetCfg(ByRef clCfg As clMark3Cfg)
        m_Cfg = clCfg
        RescanConfig()
    End Sub

    Private Sub RescanConfig()
        chkSem.Checked = m_Cfg.GetUseSemaphores()
        chkMutex.Checked = m_Cfg.GetUseMutex()
        chkEvent.Checked = m_Cfg.GetUseEventFlag()
        chkQueue.Checked = m_Cfg.GetUseMessages()
        NumericUpDown1.Value = m_Cfg.GetGlobalMessageQueue()
    End Sub

    Private Sub chkSem_CheckedChanged(sender As Object, e As EventArgs) Handles chkSem.CheckedChanged
        If (IsNothing(m_Cfg)) Then
            Return
        End If

        m_Cfg.SetUseSemaphores(chkSem.Checked)
        If (m_Cfg.Validate() = False) Then
            chkSem.Checked = False
        End If
        RescanConfig()
    End Sub

    Private Sub chkMutex_CheckedChanged(sender As Object, e As EventArgs) Handles chkMutex.CheckedChanged
        If (IsNothing(m_Cfg)) Then
            Return
        End If

        m_Cfg.SetUseMutex(chkMutex.Checked)
        If (m_Cfg.Validate() = False) Then
            chkMutex.Checked = False
        End If
        RescanConfig()
    End Sub

    Private Sub chkEvent_CheckedChanged(sender As Object, e As EventArgs) Handles chkEvent.CheckedChanged
        If (IsNothing(m_Cfg)) Then
            Return
        End If

        m_Cfg.SetUseEventFlag(chkEvent.Checked)
        If (m_Cfg.Validate() = False) Then
            chkEvent.Checked = False
        End If
        RescanConfig()
    End Sub

    Private Sub chkQueue_CheckedChanged(sender As Object, e As EventArgs) Handles chkQueue.CheckedChanged
        If (IsNothing(m_Cfg)) Then
            Return
        End If

        m_Cfg.SetUseMessages(chkQueue.Checked)
        If (m_Cfg.Validate() = False) Then
            chkQueue.Checked = False
        Else
            m_Cfg.SetGlobalMessageQueue(NumericUpDown1.Value)
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

        m_Cfg.SetGlobalMessageQueue(NumericUpDown1.Value)
        If (m_Cfg.Validate() = False) Then
            NumericUpDown1.Value = 2
        End If
        RescanConfig()
    End Sub

    Private Sub btnNext_Click(sender As Object, e As EventArgs) Handles btnNext.Click
        frmMain.btnFeatures.PerformClick()
    End Sub

    Private Sub btnBack_Click(sender As Object, e As EventArgs) Handles btnBack.Click
        frmMain.btnTimers.PerformClick()
    End Sub
End Class