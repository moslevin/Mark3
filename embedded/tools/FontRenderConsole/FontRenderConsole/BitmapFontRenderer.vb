Imports System.Drawing

Public Class BitmapFontRenderer
    Private my_gfx As Graphics
    Private my_tmp_gfx As Graphics
    Private my_img As Image
    Private my_bmp As Bitmap

    Private min_x As Integer
    Private min_y As Integer

    Private max_x As Integer
    Private max_y As Integer

    Private line_string As String
    Private bitmap_string As String

    Private font_size As Integer
    Private font_name As String

    Private font_bold As Boolean
    Private font_italic As Boolean
    Private font_underline As Boolean

    Private num_glyphs As Integer
    Private start_glyph As Integer

    Public Sub SetFontName(ByRef name_ As String)
        font_name = name_
    End Sub
    Public Sub SetFontSize(ByVal size_ As Integer)
        font_size = size_
    End Sub
    Public Sub SetFontBold(ByVal bold_ As Boolean)
        font_bold = bold_
    End Sub
    Public Sub SetFontItalic(ByVal italic_ As Boolean)
        font_italic = italic_
    End Sub
    Public Sub SetFontUnderline(ByVal underline_ As Boolean)
        font_underline = underline_
    End Sub
    Public Sub SetStartChar(ByVal start_code As Integer)
        start_glyph = start_code
    End Sub
    Public Sub SetNumGlyphs(ByVal num_glyphs_ As Integer)
        num_glyphs = num_glyphs_
    End Sub
    Private Sub Render_Glyph(ByVal glyph_text As String)
        Dim my_font As New System.Drawing.Font(font_name, font_size)
        Dim my_brush As System.Drawing.Brush = System.Drawing.Brushes.Black
        Dim my_font_brush As System.Drawing.Brush = System.Drawing.Brushes.White

        my_bmp = New Bitmap(font_size * 3, font_size * 3)
        my_img = my_bmp
        my_tmp_gfx = Graphics.FromImage(my_img)
        my_tmp_gfx.TextRenderingHint = System.Drawing.Text.TextRenderingHint.SingleBitPerPixelGridFit

        my_tmp_gfx.FillRectangle(my_brush, 0, 0, font_size * 3 - 1, font_size * 3 - 1)
        my_tmp_gfx.DrawString(glyph_text, my_font, my_font_brush, 0, 0)

        my_tmp_gfx.Flush()
        my_tmp_gfx.Dispose()

    End Sub

    Private Sub Find_Boundaries()
        Dim my_pixel As Color

        min_x = font_size * 3 - 1
        min_y = font_size * 3 - 1

        max_x = 0
        max_y = 0

        ' Scan the bitmap to find the max & min boundaries for the glyph
        For x = 0 To font_size * 3 - 1
            For y = 0 To font_size * 3 - 1
                my_pixel = my_bmp.GetPixel(x, y)
                If my_pixel.R <> 0 Then
                    If x < min_x Then
                        min_x = x
                    End If
                    If y < min_y Then
                        min_y = y
                    End If
                    If x > max_x Then
                        max_x = x
                    End If
                    If y > max_y Then
                        max_y = y
                    End If
                End If
            Next
        Next

    End Sub

    Private Sub Font_GenerateHeader()
        Dim option_string As String
        option_string = "_" + font_bold.ToString + "_" + font_italic.ToString + "_" + font_underline.ToString + "_"
        line_string = "#include ""font.h"" " + vbNewLine + vbNewLine
        line_string += "const FONT_STORAGE_TYPE " + font_name.Replace(" ", "_") + "_" + font_size.ToString + option_string + "[] FONT_ATTRIBUTE_TYPE = {" + vbNewLine

    End Sub

    Private Sub Glyph_GenerateData(ByVal glyph_text As String)
        Dim my_pixel As Color

        Dim x As Integer
        Dim y As Integer
        Dim pixel_val As Byte
        Dim shift_val As Byte

        If max_x = 0 Then ' We didn't find anything here... stub out the glyph            
            line_string += vbTab + "0, " + " /* ucWidth */" + vbNewLine
            line_string += vbTab + "0, " + " /* ucHeight */" + vbNewLine
            line_string += vbTab + "0, " + " /* ucVOffset */" + vbNewLine
            line_string += vbTab + "//--[Glyph Data]--" + vbNewLine
            line_string += vbTab + "/* No glyph data*/" + vbNewLine
            Return
        End If

        line_string += vbTab + ((max_x - min_x) + 1).ToString + ", /* ucWidth */" + vbNewLine
        line_string += vbTab + ((max_y - min_y) + 1).ToString + ", /* ucHeight */" + vbNewLine
        line_string += vbTab + (min_y).ToString + ", /* ucVOffset */" + vbNewLine
        line_string += vbTab + "//--[Glyph Data]--" + vbNewLine + vbTab

        For y = min_y To max_y
            shift_val = 128
            pixel_val = 0
            bitmap_string = ""
            For x = min_x To max_x
                If shift_val = 0 Then
                    line_string += pixel_val.ToString
                    line_string += "," + vbTab

                    shift_val = 128
                    pixel_val = 0
                Else

                End If

                my_pixel = my_bmp.GetPixel(x, y)
                If my_pixel.R = 0 Then
                    bitmap_string += "0"
                Else
                    pixel_val += shift_val
                    bitmap_string += "1"
                End If

                shift_val /= 2
            Next

            line_string += pixel_val.ToString + ","
            line_string += vbTab + " /* " + bitmap_string + " */"

            line_string += vbNewLine + vbTab

        Next
    End Sub

    Public Sub Font_GenerateFooter()
        Dim option_string As String
        Dim flags As Integer

        option_string = "_" + font_bold.ToString + "_" + font_italic.ToString + "_" + font_underline.ToString + "_"

        If font_bold = True Then
            flags += 1
        End If
        If font_italic = True Then
            flags += 2
        End If
        If font_underline = True Then
            flags += 4
        End If

        line_string += vbNewLine + vbTab + "0" + vbNewLine
        line_string += "};" + vbNewLine + vbNewLine

        line_string += "Font_t fnt" + (font_name).Replace(" ", "_") + "_" + font_size.ToString + option_string + " = {" + vbNewLine
        line_string += vbTab + font_size.ToString + ", /* ucSize */" + vbNewLine
        line_string += vbTab + flags.ToString + ", /* ucFlags */" + vbNewLine
        line_string += vbTab + start_glyph.ToString + ", /* ucStartChar */" + vbNewLine
        line_string += vbTab + (Int(num_glyphs) - 1).ToString + ", /* ucMaxChar */" + vbNewLine
        line_string += vbTab + """" + (font_name).Replace(" ", "_") + """" + ", /* szName */" + vbNewLine
        line_string += vbTab + (font_name).Replace(" ", "_") + "_" + font_size.ToString + option_string + " /* pucData */" + vbNewLine
        line_string += "};" + vbNewLine
    End Sub

    Public Function RenderFont() As String
        Dim i As Integer

        line_string = ""

        Font_GenerateHeader()
        For i = start_glyph To num_glyphs - 1
            Render_Glyph(Chr(i))
            Find_Boundaries()
            line_string += vbNewLine + "//-- Glyph for ASCII Character code: " + i.ToString + vbNewLine
            Glyph_GenerateData(Chr(i))
        Next

        Font_GenerateFooter()
        Return line_string
    End Function


    Public Function ExportHeader() As String

        Dim option_string As String
        option_string = "_" + font_bold.ToString + "_" + font_italic.ToString + "_" + font_underline.ToString + "_"

        line_string = ""

        line_string += "#ifndef __" + font_name.Replace(" ", "_") + "_" + font_size.ToString + option_string + "_H__" + vbNewLine
        line_string += "#define __" + font_name.Replace(" ", "_") + "_" + font_size.ToString + option_string + "_H__" + vbNewLine

        line_string += vbNewLine + vbNewLine
        line_string += "#include ""font.h"" " + vbNewLine + vbNewLine

        line_string += "//--[ Public Symbol Exports ]--" + vbNewLine

        line_string += "extern const FONT_STORAGE_TYPE " + (font_name).Replace(" ", "_") + "_" + font_size.ToString + option_string + "[];" + vbNewLine
        line_string += "extern Font_t fnt" + (font_name).Replace(" ", "_") + "_" + font_size.ToString + option_string + ";" + vbNewLine

        line_string += vbNewLine
        line_string += "#endif" + vbNewLine

        Return line_string
    End Function
End Class

