import re
import os

path_base = "E:\\3D_SSDsim_v1\\3D_SSDsim"
path_off = ["r_buff_result", "mutil_sus_result"]



re1 = ""

for re_path in path_off:
	path = os.path.join(path_base, re_path)
	for root, dirs, files in os.walk(path):
		for f in files:
			abs_path = os.path.join(root, f)
			result_file = open(abs_path)
			for line in result_file:
				if()
			
		