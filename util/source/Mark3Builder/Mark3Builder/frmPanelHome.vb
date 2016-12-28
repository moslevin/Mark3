Public Class frmPanelHome
    Private m_Cfg As clMark3Cfg

    Public Sub SetCfg(ByRef clCfg As clMark3Cfg)
        m_Cfg = clCfg
    End Sub
    Private Sub Label1_Click(sender As Object, e As EventArgs) Handles Label1.Click

    End Sub

    Private Sub btnNext_Click(sender As Object, e As EventArgs) Handles btnNext.Click
        frmMain.EnableNav()
        frmMain.btnThreads.PerformClick()
    End Sub
End Class