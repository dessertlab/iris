import os

dir1 = './seeds_folder'

list_mon = [os.path.abspath(os.path.join(dir1, p)) for p in os.listdir(dir1)]
list_mon.sort(key=lambda x: int(''.join(filter(str.isdigit, x))))

file_cat = open("seeds_cat", "a")
iter = 0
for mon_file in list_mon:

	with open(mon_file) as f:
        	for line in f:
			file_cat.write(line)


file_cat.close()
