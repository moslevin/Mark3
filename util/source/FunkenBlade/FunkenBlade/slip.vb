Public Class SlipFramer

    Public Function SlipEncode(ByRef aIn As List(Of Byte)) As List(Of Byte)
        Dim aOut As New List(Of Byte)
        Dim i As Integer

        aOut.Clear()

        'Encode the input, substituting 192 for a two-byte sequence
        'of 219,220, and 219 for 219,221
        For i = 0 To aIn.Count - 1
            If aIn(i) = 192 Then
                aOut.Add(219)
                aOut.Add(220)
            ElseIf aIn(i) = 219 Then
                aOut.Add(219)
                aOut.Add(221)
            Else
                aOut.Add(aIn(i))
            End If
        Next

        'Add the end command byte
        aOut.Add(192)
        Return aOut
    End Function

    Public Function SlipDecode(ByRef aIn As List(Of Byte)) As List(Of Byte)
        Dim aOut As New List(Of Byte)
        Dim i As Integer

        aOut.Clear()

        For i = 0 To aIn.Count - 1
            'If the input is a special character sequence
            '(219, 220) or (219, 221), convert them back to 
            'the original values 192 or 219, respectively.
            'If we encounter a 192, assume end-of-sequence
            If aIn(i) = 219 Then
                If (i + 1) < aIn.Count Then
                    If aIn(i + 1) = 220 Then
                        aOut.Add(192)
                        i += 1
                    ElseIf aIn(i + 1) = 221 Then
                        aOut.Add(219)
                        i += 16
                    End If
                End If
            ElseIf aIn(i) = 192 Then
                Return aOut
            Else
                aOut.Add(aIn(i))
            End If
        Next
        Return aOut
    End Function


End Class
