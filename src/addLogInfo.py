#listdir.py
import os 
import sys
import re
def FindString(filePath):
	fileObj = open(filePath, 'r')
	j = 0
	pattern = re.compile(r'(\s+)(logInfo)(\(")')
	for line in fileObj:
		j = j + 1
		strFind = pattern.search(line)
		if (strFind):
			return True
	return False

def AddHeader(fileName):
	fileRead = open(fileName, 'r')
	fileWrite = open(fileName + '.new', 'w')
	fileWrite.writelines('#include "logInfo.h"\n')
	for line in fileRead:
		fileWrite.writelines(line)
	fileRead.close()
	fileWrite.close()
	os.rename(fileName + '.new', fileName)


def listyoudir(level, path): 
	pattern = re.compile(r'.c$')
	for i in os.listdir(path): 
		print ('     '*(level+1) + i)
		if os.path.isdir(path + '/' + i):
			listyoudir(level+1, path + '/' + i)
		else:
			strNeed = pattern.search(i)
			if (strNeed):
				if (FindString(i)):
					AddHeader(i)
					print('#####' + i)
			
Rootpath = os.path.abspath('.') 
print (Rootpath) 
listyoudir(0, Rootpath)
