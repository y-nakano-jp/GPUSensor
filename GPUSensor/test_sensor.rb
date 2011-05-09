require 'GPUSensor'
attr = [
	:GPU_TEMPERATURE, 
	:CORE_THRESHOLD, 
	:DEFAULT_CORE_THRESHOLD, 
	:MAX_CORE_THRESHOLD,
	:AMBIENT_TEMPERATURE, 
	:INTAKE_TEMPERATURE, 
	:EXHAUST_TEMPERATURE, 
	:BOARD_TEMPERATURE,
	:FAN_SPEED,
	:RAM_USED,
	:RAM_SIZE,
	:GPU_USED]

attr.each do |a|
	ret = GPUSensor::getAttribute(a)
	if ret >= 0
		print GPUSensor::attribute(a) + " ",ret,"\n"
	end
end 
