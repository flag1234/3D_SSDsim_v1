import re
import os

path_base = "E:\\3D_SSDsim_v1\\3D_SSDsim"
path_off = ["r_buff_result", "mutil_sus_result"]

ana_result = open("result.txt", 'w')
data_f = open("data.txt", 'w')

out = ".*_ex.*"
re1 = "read count.*"
re2 = "program count.*"
re3 = "erase count.*"
re4 = "direct erase count.*"
re5 = "gc count.*"
re6 = "multi-plane program count.*"
re7 = "multi-plane read count.*"
re8 = "mutli plane one shot program count.*"
re9 = "one shot program count.*"
re10 = "one shot read count.*"
re11 = "mutli plane one shot read count.*"
re12 = "erase suspend count.*"
re13 = "erase resume  count.*"
re14 = "suspend read  count.*"
re15 = "suspend write  count.*"
re16 = "read request average size.*"
re17 = "write request average size.*"
re18 = "read request average response time.*"
re19 = "write request average response time.*"

for re_path in path_off:
	path = os.path.join(path_base, re_path)
	ana_result.write(re_path)
	ana_result.write("\n*******************\n")
	
	for root, dirs, files in os.walk(path):
		for f in files:
			if re.match(out, f) == None:
				ana_result.write(f+":")
				ana_result.write("\n")
				data_f.write(f+":")
				data_f.write("\n")
				abs_path = os.path.join(root, f)
				result_file = open(abs_path, 'r')
				for line in result_file:
					if re.match(re1, line) != None or re.match(re2, line) != None or re.match(re3, line) != None or re.match(re4, line) != None\
					or re.match(re5, line) != None or re.match(re6, line) != None or re.match(re7, line) != None or re.match(re8, line) != None\
					or re.match(re9, line) != None or re.match(re10, line) != None or re.match(re11, line) != None or re.match(re12, line) != None\
					or re.match(re13, line) != None or re.match(re14, line) != None or re.match(re15, line) != None or re.match(re16, line) != None\
					or re.match(re17, line) != None or re.match(re18, line) != None or re.match(re19, line) != None:
						res = line.strip()
						r_c = res.split(":")
						#print(r_c[0] + ':' + r_c[1].replace(' ', ''))
						ana_result.write(r_c[0] + ':' + r_c[1].replace(' ', ''))
						ana_result.write("\n")
						data_f.write(r_c[1].replace(' ', ''))
						data_f.write("\n")
				ana_result.write("**************\n")
				result_file.close()
	ana_result.write("\n\n\n")
ana_result.close()
data_f.close()
					
			