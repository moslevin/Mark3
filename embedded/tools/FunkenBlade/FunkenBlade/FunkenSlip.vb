Public Class FunkenSlip

    Private Payload As List(Of Byte)
    Private CalcCRC As UInt16
    Private MsgCRC As UInt16
    Private Channel As Byte
    Private MsgSize As UInt16
    Private CrcOK As Boolean
    Private Mode As Boolean

    Public Function Encode(ByRef SourceData As List(Of Byte), ByVal Channel As Byte) As List(Of Byte)
        Dim TmpBuf As New List(Of Byte)
        Dim Framer As New SlipFramer
        Dim PayloadLen As UInt16
        Dim PayloadCRC As UInt16
        Dim i As Integer

        Mode = False

        PayloadLen = SourceData.Count

        TmpBuf.Add(Channel)
        TmpBuf.Add(PayloadLen \ 256)
        TmpBuf.Add(PayloadLen Mod 256)

        For i = 0 To TmpBuf.Count - 1
            PayloadCRC += TmpBuf(i)
        Next

        For i = 0 To SourceData.Count - 1
            TmpBuf.Add(SourceData(i))
            PayloadCRC += SourceData(i)
        Next

        MsgSize = TmpBuf.Count
        CalcCRC = PayloadCRC
        MsgCRC = CalcCRC

        TmpBuf.Add(PayloadCRC \ 256)
        TmpBuf.Add(PayloadCRC Mod 256)

        Payload = Framer.SlipEncode(TmpBuf)

        Return Payload
    End Function

    Public Function Decode(ByRef SourceData As List(Of Byte)) As List(Of Byte)
        Dim TmpBuf As List(Of Byte)
        Dim i As Integer
        Dim Framer As New SlipFramer
        Mode = True

        TmpBuf = Framer.SlipDecode(SourceData)

        Payload = New List(Of Byte)

        If TmpBuf.Count < 5 Then
            Return Nothing
        End If

        For i = 0 To TmpBuf.Count - 3
            CalcCRC += TmpBuf(i)
        Next

        Channel = TmpBuf(0)
        MsgSize = (TmpBuf(1) * 256) + TmpBuf(2)
        MsgCRC = TmpBuf(TmpBuf.Count - 2) * 256 + TmpBuf(TmpBuf.Count - 1)

        If CalcCRC = MsgCRC Then
            CrcOK = True
        Else
            CrcOK = False
        End If

        For i = 3 To TmpBuf.Count - 3
            Payload.Add(TmpBuf(i))
        Next

        Return Payload
    End Function

    Public Function GetPayload() As List(Of Byte)
        Return Payload
    End Function

    Public Function IsCrcOk() As Boolean
        Return CrcOK
    End Function

    Public Function GetSize() As UInt16
        Return MsgSize
    End Function

    Public Function GetCrc() As UInt16
        Return MsgCRC
    End Function

    Public Function GetChannel() As Byte
        Return Channel
    End Function

    Public Function IsDecoder() As Boolean
        Return Mode
    End Function
End Class
