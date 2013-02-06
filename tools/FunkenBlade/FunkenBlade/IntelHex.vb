Public Class IntelHex

    Private hexReadOk As Boolean
    Private hexDataLen As Integer
    Private hexData As List(Of Byte)
    Private hexStartAddr As Integer

    Private Function HexCharsToByte(ByRef srcChars() As Char) As Byte
        Dim tempByte As Byte
        Dim retVal As Byte

        Dim i As Integer
        For i = 0 To 1
            Select Case srcChars(i)
                Case "0"
                    tempByte = 0
                Case "1"
                    tempByte = 1
                Case "2"
                    tempByte = 2
                Case "3"
                    tempByte = 3
                Case "4"
                    tempByte = 4
                Case "5"
                    tempByte = 5
                Case "6"
                    tempByte = 6
                Case "7"
                    tempByte = 7
                Case "8"
                    tempByte = 8
                Case "9"
                    tempByte = 9
                Case "A"
                    tempByte = 10
                Case "B"
                    tempByte = 11
                Case "C"
                    tempByte = 12
                Case "D"
                    tempByte = 13
                Case "E"
                    tempByte = 14
                Case "F"
                    tempByte = 15
            End Select
            If i = 0 Then
                retVal = tempByte * 16
            Else
                retVal += tempByte
            End If
        Next

        Return retVal
    End Function

    Public Function ParseHexFile(ByRef fileName As String) As List(Of Byte)

        Dim fileReader As New System.IO.StreamReader(fileName)
        Dim fileLine As String
        Dim firstLine As Boolean

        hexReadOk = False
        hexData = New List(Of Byte)

        firstLine = True

        If IsNothing(fileReader) Then
            Return Nothing
        End If

        While fileReader.EndOfStream = False
            Dim lineArray() As Char
            Dim i As Integer
            Dim lineBytes As Integer
            Dim tempChars(2) As Char

            fileLine = fileReader.ReadLine()

            'Normal line in hex format is at least 9 bytes...
            If (fileLine.Length < 10) Then
                GoTo cleanup
            End If

            'Only handle simple hex files, starting with :1
            If Not fileLine.StartsWith(":") Then
                GoTo cleanup
            End If


            'Add data...
            lineArray = fileLine.ToCharArray()

            'If this is the first line, set the start address
            If firstLine Then
                Dim addrHigh As Byte
                Dim addrLow As Byte

                tempChars(0) = lineArray(3)
                tempChars(1) = lineArray(4)

                addrHigh = HexCharsToByte(tempChars)

                tempChars(0) = lineArray(5)
                tempChars(1) = lineArray(6)

                addrLow = HexCharsToByte(tempChars)
                hexStartAddr = addrHigh * 256 + addrLow
                firstLine = False
            End If

            tempChars(0) = lineArray(1)
            tempChars(1) = lineArray(2)
            lineBytes = HexCharsToByte(tempChars)

            ' Ignore the first 9 bytes, and the last 2
            For i = 0 To lineBytes - 1

                Dim tempByte As Byte

                tempChars(0) = lineArray(9 + (2 * i))
                tempChars(1) = lineArray(10 + (2 * i))
                tempByte = HexCharsToByte(tempChars)

                hexData.Add(tempByte)
            Next

        End While
        hexReadOk = True
cleanup:
        Return hexData
    End Function

    Public Function GetHexSize() As Integer
        Return hexDataLen
    End Function

    Public Function GetHexStartAddr() As Integer
        Return hexStartAddr
    End Function

    Public Function GetHexData() As List(Of Byte)
        If hexReadOk Then
            Return hexData
        End If
        Return Nothing
    End Function
End Class
