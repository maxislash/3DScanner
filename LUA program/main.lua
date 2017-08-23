rs232 = require("luars232")

-- Linux
 port_name = "/dev/cu.usbmodem1411"

-- (Open)BSD
-- port_name = "/dev/cua00"

-- Windows
--port_name = "COM1"

local out = io.stderr

-- open port
local e, p = rs232.open(port_name)
if e ~= rs232.RS232_ERR_NOERROR then
	-- handle error
	out:write(string.format("can't open serial port '%s', error: '%s'\n",
			port_name, rs232.error_tostring(e)))
	return
end

-- set port settings
assert(p:set_baud_rate(rs232.RS232_BAUD_9600) == rs232.RS232_ERR_NOERROR)
assert(p:set_data_bits(rs232.RS232_DATA_8) == rs232.RS232_ERR_NOERROR)
assert(p:set_parity(rs232.RS232_PARITY_NONE) == rs232.RS232_ERR_NOERROR)
assert(p:set_stop_bits(rs232.RS232_STOP_1) == rs232.RS232_ERR_NOERROR)
assert(p:set_flow_control(rs232.RS232_FLOW_OFF)  == rs232.RS232_ERR_NOERROR)

out:write(string.format("OK, port open with values '%s'\n", tostring(p)))

--[[nom_fichier = system.startOSK(".txt","trame")
fichier = io.open(nom_fichier, "w")
fichier:close()
System.message("Le fichier "..nom_fichier.." a ete cree avec succes",0)
]]--
local file = io.open("test.txt" ,"w")

-- read with timeout
local read_len = 6 -- read one byte
local timeout = 500 -- in milliseconds



while (true) do 

	local err, data_read, size = p:read(read_len, timeout)
	assert(e == rs232.RS232_ERR_NOERROR)
	
	if(data_read == 'P' or data_read == 'V') do
		io.write('\n')
	else if (data_read = 'T') do
		io.write('\n')
		io.write('\n')
	else
		io.write(data_read);
	end

end

file:close()

--[[
-- write without timeout
err, len_written = p:write("test")
assert(e == rs232.RS232_ERR_NOERROR)


-- write with timeout 100 msec
err, len_written = p:write("test\n", timeout)
assert(e == rs232.RS232_ERR_NOERROR)

]]--

-- close
assert(p:close() == rs232.RS232_ERR_NOERROR)