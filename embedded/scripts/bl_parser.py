import struct

class bl_symparser:
    def __init__(self,filename):
        self.str = ""
        self.filename = filename

    def parse(self):
        obj = open(self.filename, "r")
        self.str = obj.read()
        obj.close()

parser = bl_symparser("/home/vm/mark3/trunk/embedded/stage/dbg/avr/atmega328p/gcc/ut_thread.dbg")
parser.parse()

pstate = 0
idx = 0
start_idx = 0
token = 0
line_no = 0
file_no = 0
format_str = ""

while idx < len(parser.str):
   
    if (parser.str[idx]) == b'\x00':
        format_str = parser.str[start_idx:idx]
        pstate = 1
        #print "FMT: %d-%d, format: %s" % (start_idx, idx, format_str)

        (line_no, file_no, token) = struct.unpack('<HHH', parser.str[idx+1:idx+7])
        #print "meta: %d-%d, token: %x, line: %x, file: %x" % (idx+1, idx+7, token, line_no, file_no)

        pstate = 0
        start_idx = idx + 7
        idx += 7
        print "[%d, %d] %s" % (file_no, line_no, format_str)
    else:
        idx += 1
        

