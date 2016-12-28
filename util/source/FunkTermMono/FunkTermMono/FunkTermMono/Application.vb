Imports System.Collections.Generic
Imports System.Threading

Public Class Application

	Dim Shared sp as new System.IO.Ports.SerialPort()
	
	Dim Shared serial_port_name as String
	Dim Shared serial_port_baud as String
	Dim Shared encoding as String
	
	Dim Shared tx_encoder as new FunkenSlip
	Dim Shared rx_decoder as new FunkenSlip
	
	Private Shared Sub PrintBanner()
		Console.WriteLine( "" )
		Console.WriteLine("----[ Funkenstein Software Terminal ]---------------------------")
		Console.WriteLine("Usage: FunkTermMono <serial port name> <baud rate> <encoding>")
		Console.WriteLine("Where <encoding> is either FunkenSlip or ASCII")
		Console.WriteLine("")
		Console.WriteLine("Copyright (c) 2012 Funkenstein Software Consulting")
		Console.WriteLine("----------------------------------------------------------------")
	End Sub
	
	Private Shared Sub GetArgs()
		dim s() as string = System.Environment.GetCommandLineArgs()

		' Make sure we only have as many arguments as we expect...
		if UBound(s) <> 3
			Console.WriteLine("Expected 3 arguments, " & UBound(s) & " provided.")
			PrintBanner()
			end 
		end if
		
		serial_port_name = s(1)
		serial_port_baud = s(2)
		encoding = s(3)
		
		Console.WriteLine( "")
		Console.WriteLine( "--[ Port Settings ]-------------------------------------------")
		Console.WriteLine( "Serial Port: " & serial_port_name )
		Console.WriteLine( "Serial Baud: " & serial_port_baud )
		Console.WriteLine( "Encoding: " & encoding )
		Console.WriteLine( "--------------------------------------------------------------")
		
		if encoding <> "FunkenSlip" And encoding <> "ASCII" then
			Console.WriteLine("Error: Unknown encoding " & encoding)
			end
		end if
	end sub
	
	Public Shared Sub ReadData()
		Dim rx_data as new List(Of Byte)
		Dim rx_decoded as List(Of Byte)
		Dim str as String
		sp.BaudRate = serial_port_baud
        sp.PortName = serial_port_name

        ' 8N1 connnections only
        sp.Parity = IO.Ports.Parity.None
        sp.StopBits = IO.Ports.StopBits.One

		try
        	sp.Open()
        	sp.DtrEnable = true
        	Thread.Sleep(100)
        	sp.DtrEnable = false
        	Thread.Sleep(100)
        	sp.DiscardInBuffer()
        	sp.DiscardOutBuffer()
        catch ex as Exception
        	Console.WriteLine("Error opening port")
        	end
        end try
		
		
		str = ""
		while true			
				while sp.BytesToRead
					Dim tmp_byte as Byte
					Dim i as Integer					
														
					if encoding = "FunkenSlip" then					
						tmp_byte = sp.ReadByte
						rx_data.Add(tmp_byte)
						if tmp_byte = 192 then
							rx_decoded = rx_decoder.Decode(rx_data)
							if Not IsNothing(rx_decoded) then
								for i = 0 to rx_decoded.count() - 1							
									str = str & Chr(rx_decoded(i))
								next
								Console.WriteLine(str)								
							end if
							str = ""
							rx_data.Clear						
							Thread.Sleep(1)
						end if 
						
					elseif encoding = "ASCII" then
						Console.Write(Chr(sp.ReadByte))
					else 
						sp.ReadByte					
					end if
				end while
				Thread.Sleep(25)

		end while
		
	end sub
	
	Public Shared Sub Main()
		
		' Process the command-line arguments		
		GetArgs()
		ReadData()		
	End Sub
End Class

