Imports System.Collections.Generic

Public Class Application

    Shared font_renderer As BitmapFontRenderer

    Shared dest_file As String
    Shared font_size As String
    Shared font_name As String

    Shared is_bold As Boolean
    Shared is_italic As Boolean
    Shared is_underline As Boolean

    Shared num_chars As String
    Shared start_char As String

    Private Shared Function GenerateFSCHeader()
        Dim s As String

        s = "/*" _
            + "    Funkenstein Software Consulting Font Renderer " + vbNewLine _
            + "    (c) 2012, Funkenstein Software Consulting, all rights reserved. " + vbNewLine _
            + "*/" + vbNewLine

        Return s
    End Function

    Private Shared Sub PrintBanner()
        Console.WriteLine("")
        Console.WriteLine("----[ Funkenstein Software Font Generator ]--------------------")
        Console.WriteLine("Usage: FunkRenderConsole <File Prefix> <Font Family> <Font Size> <Optional Flags: -b -i -o> <-num_chars=x> <-start_char=x")
        Console.WriteLine("")
        Console.WriteLine("Copyright (c) 2012 Funkenstein Software Consulting")
        Console.WriteLine("----------------------------------------------------------------")
    End Sub

    Private Shared Sub ReadArgs()
        Dim s() As String = System.Environment.GetCommandLineArgs()

        If UBound(s) < 3 Then
            PrintBanner()
            End
        End If

        dest_file = s(1)
        font_name = s(2)
        font_size = s(3)

        For i = 4 To UBound(s)
            If s(i) = "-i" Then
                is_italic = True
            End If
            If s(i) = "-b" Then
                is_italic = True
            End If
            If s(i) = "-u" Then
                is_italic = True
            End If
            If s(i).StartsWith("-num_chars=") Then
                num_chars = s(i).Substring("-num_chars=".Length)
            End If
            If s(i).StartsWith("-start_char=") Then
                start_char = s(i).Substring("-start_char=".Length)
            End If
        Next

    End Sub

    Private Shared Sub Render()
        Dim header_file As New System.IO.StreamWriter(dest_file + ".h")
        Dim data As String

        data = font_renderer.ExportHeader()

        header_file.WriteLine(GenerateFSCHeader())
        header_file.WriteLine(data)
        header_file.Close()

        Dim source_file As New System.IO.StreamWriter(dest_file + ".cpp")

        data = font_renderer.RenderFont()

        source_file.WriteLine(GenerateFSCHeader())
        source_file.WriteLine(data)
        source_file.Close()
    End Sub

    Public Shared Sub Main()
        num_chars = "256"
        start_char = "0"

        ReadArgs()
        Try
            font_renderer = New BitmapFontRenderer
            font_renderer.SetFontBold(is_bold)
            font_renderer.SetFontBold(is_italic)
            font_renderer.SetFontBold(is_underline)
            font_renderer.SetFontName(font_name)
            font_renderer.SetFontSize(Int(font_size))
            font_renderer.SetNumGlyphs(num_chars)
            font_renderer.SetStartChar(start_char)
            Render()
            Console.WriteLine("Success")
            End

        Catch ex As Exception
            Console.WriteLine("Error:" + ex.Message)
            PrintBanner()
        End Try

    End Sub

End Class
