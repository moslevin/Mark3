Imports System.Collections.Generic
Imports System.Threading

Public Class Application

	Dim Shared sp as new System.IO.Ports.SerialPort()
	Dim Shared hex_file_name as String
	Dim Shared serial_port_name as String
	Dim Shared serial_port_baud as String
	Dim Shared hex_file as new IntelHex
	Dim Shared tx_encoder as new FunkenSlip
		
	Private Shared Sub PrintBanner()
		Console.WriteLine( "" )
		Console.WriteLine("----[ Funkenstein Software Firmware Loader ]--------------------")
		Console.WriteLine("Usage: FunkProgMono <source.hex> <serial port name> <baud rate>")
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
		
		hex_file_name = s(1)
		serial_port_name = s(2)
		serial_port_baud = s(3)
		
		Console.WriteLine( "")
		Console.WriteLine( "--[ Programmer Settings ]-------------------------------------")
		Console.WriteLine( "Hex File:    " & hex_file_name )
		Console.WriteLine( "Serial Port: " & serial_port_name )
		Console.WriteLine( "Serial Baud: " & serial_port_baud )
		Console.WriteLine( "--------------------------------------------------------------")
	end sub
	
	Public Shared Sub LoadHex()
		dim byte_stream as List(Of Byte)
		dim hex_size as Integer
		dim start_address as integer
		
		byte_stream = hex_file.ParseHexFile( hex_file_name )
		if IsNothing( byte_stream ) then
			Console.WriteLine("Error reading hex file")
			end
		end if
		
		hex_size = hex_file.GetHexSize()
		start_address = hex_file.GetHexStartAddr()
		
		Console.WriteLine( "")
		Console.WriteLine( "--[ Read Hex File OK ]----------------------------------------")
		Console.WriteLine( "Size:          " & hex_size & " bytes")
		Console.WriteLine( "Start Address: " & start_address )
		Console.WriteLine( "--------------------------------------------------------------")
	end sub
	
	Private Shared Sub SendReset()
		Dim tx_data as new List(Of Byte)
		tx_data.Add(0)
		WriteData( tx_data, 3 )
	end sub
	
	Private Shared function WaitAck() as boolean
		while sp.BytesToRead
			dim tmp_byte as Byte
			tmp_byte = sp.ReadByte
		    if tmp_byte = 69 then
		    	''Console.WriteLine("Received Acknowledge")
		    	return true
		    else
		    	''Console.Writeline("Recevied:" & tmp_byte)
		    end if
		end while
		return false
	end function
	
	Private Shared Sub SetAddress()
		Dim tx_data as new List(Of Byte)
		Dim addr as integer
		addr = hex_file.GetHexStartAddr()
	
		tx_data.Add(0)
		tx_data.Add( addr \ 256 )
		tx_data.Add( addr mod 256 )
		
		WriteData( tx_data, 127 )		
	end sub
	
	
    Public Shared Sub WriteData(ByRef data As List(Of Byte), ByVal channel As Byte)
        Dim tx_data As List(Of Byte)
        
        If IsNothing(data) Then
            Return
        End If
        If data.Count = 0 Then
            Return
        End If

        Try
            tx_data  = tx_encoder.Encode(data, channel)

            Dim tx_array(tx_data.count()) As Byte
            Dim i As Integer

            For i = 0 To tx_data.Count - 1
                tx_array(i) = tx_data(i)
                ''Console.WriteLine("Tx: " & tx_data(i))
                ''sp.Write(tx_array, 0, 1)   
            Next
			
			sp.Write(tx_array, 0, tx_data.Count())
			Thread.Sleep(25)
        Catch ex As Exception

        End Try
    End Sub
    
	Public Shared Sub Program()
		
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
        catch ex as exception
        	Console.WriteLine("Unable to open serial port - aborting")
        	end
        end try

		' Send a reset to the device - probably not necessary since
		' opening the port toggles reset on Arduino.  Try to check for 
		' a reset acknowledge on the port first before attempting to 
		' reset the device (after which we need to wait for ack again...)
		
		Thread.Sleep(200)
		dim tmp_ack(1) as Byte
		tmp_ack(0) = 192
		sp.Write(tmp_ack, 0, 1)
		Thread.Sleep(200)
		if false = WaitAck() then
		
			' Reset and try again
			SendReset()
			
			Thread.Sleep(200)
			sp.Write(tmp_ack, 0, 1)
			Thread.Sleep(200)
			if false = WaitAck() then
				Console.WriteLine("Error: Device not responding to handshake")
				end
			end if
		end if
		Thread.Sleep(100)
		
		' Port is open - send the start address
		SetAddress()		
		Thread.Sleep(100)
		if false = WaitAck() then
			Console.WriteLine("Error: Address Set not acknoweldged")
			end
		end if 
		
		' Iterate through the data, one packet at a time
		Console.WriteLine("Programming")
		try
		    Dim tx_buf as new List(Of Byte)
		    Dim tx_count as Integer
		    Dim hex_stream as List(Of Byte)
		    Dim i as integer
		    
		    hex_stream = hex_file.GetHexData()
		    
		    tx_count = 0
		    while tx_count < hex_file.GetHexSize()
		    	tx_buf.clear()
		    	tx_buf.Add(1)
		    	
		    	for i = 0 to 15
		    	    if tx_count >= hex_file.GetHexSize() then
		    	        exit for
		    	    end if 
		    	    tx_buf.Add(hex_stream(tx_count))
		    		
			  		if tx_count mod 1000 = 0 then
			  			Console.Write(vbNewLine)
			  			Console.Write("[ " & tx_count & " bytes ]")
			  		end if
			  		if tx_count mod 25 = 0 then
			  			Console.Write(".")
			  		end if
			  		
			  		tx_count += 1
		    		
		        next
		    
		    	WriteData( tx_buf, 127 )
		    	
		    	if WaitAck() = false then
		    		Console.WriteLine("Error: No Ack on write")		    		
		    		end
		    	end if 
		    			    			    			    			    	
		    end while
		    
		    tx_buf.clear()
		    tx_buf.Add(2)
		    WriteData( tx_buf, 127 )
		    
		    Console.WriteLine("Success!")
		catch ex as exception
		end try
		
		try
			sp.Close()
		catch ex as exception
			Console.WriteLine("Error closing serial port")			
		end try
	end sub
	
	Public Shared Sub Main()
		
		' Process the command-line arguments		
		GetArgs()
		
		' Load the hex file
		LoadHex()
		
		' Attempt to program the device
		Program()		
		
	End Sub
End Class

