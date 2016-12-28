Public Class frmPanelExport
    Private m_Cfg As clMark3Cfg

    Public Sub SetCfg(ByRef clCfg As clMark3Cfg)
        m_Cfg = clCfg
    End Sub
    Private Sub frmPanelExport_Load(sender As Object, e As EventArgs) Handles MyBase.Load

    End Sub

    Private Sub TableLayoutPanel1_Paint(sender As Object, e As PaintEventArgs) Handles TableLayoutPanel1.Paint

    End Sub

    Public Sub RefreshData()
        lblFeatures.Text = m_Cfg.GetFeatureSummary()
        lblSize.Text = Str(m_Cfg.EstimateSize()) + " Bytes"
        Dim newVal As Double
        newVal = m_Cfg.GetEstimatePercent() * 100.0
        If newVal > 100.0 Then
            newVal = 100.0
        End If
        ProgressBar2.Value = newVal
    End Sub
    Private Sub frmPanelExport_Shown(sender As Object, e As EventArgs) Handles MyBase.Shown
        RefreshData()
    End Sub

    Private Sub TableLayoutPanel2_Paint(sender As Object, e As PaintEventArgs) Handles TableLayoutPanel2.Paint

    End Sub

    Private Sub lblSize_Click(sender As Object, e As EventArgs) Handles lblSize.Click

    End Sub

    Private Sub btnBack_Click(sender As Object, e As EventArgs) Handles btnBack.Click
        frmMain.btnFeatures.PerformClick()
    End Sub

    Private Sub btnExport_Click(sender As Object, e As EventArgs) Handles btnExport.Click        
        Dim strSaveFile As String
        Dim strSaveData As String

        SaveFileDialog1.FileName = "mark3cfg.h"
        SaveFileDialog1.DefaultExt = ".h"
        SaveFileDialog1.AddExtension = True

        If SaveFileDialog1.ShowDialog() <> Windows.Forms.DialogResult.OK Then
            Return
        End If

        strSaveData = m_Cfg.GenerateHeader
        strSaveFile = SaveFileDialog1.FileName

        Dim ioStream As New System.IO.FileStream(strSaveFile, IO.FileMode.Create, IO.FileAccess.Write)
        Dim ioWriter As New System.IO.StreamWriter(ioStream)

        ioWriter.Write(strSaveData)
        ioWriter.Flush()

        ioStream.Close()

        MessageBox.Show("Mark3 Configuration Exported to: " + strSaveFile, "Export", MessageBoxButtons.OK)
    End Sub
End Class