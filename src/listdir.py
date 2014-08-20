#listdir.py
import os 
import sys
import re
def FindString(filePath):
	fileObj = open(filePath, 'r')
	j = 0
	pattern = re.compile(r'(\s+)(dd)(\(")')
	for line in fileObj:
		j = j + 1
		strFind = pattern.search(line)
		if (strFind):
			return True
	return False

def ReplaceFile(filePath):
	fileRead = open(filePath, 'r')
	fileWrite = open(filePath + '.new', 'w')
	pattern = re.compile(r'(\s+)(dd)(\(")')
	count = 0
	for line in fileRead:
		strNew = line
		strFind = pattern.search(line)
		if (strFind):
			if(count == 0):
				count = 1
			strNew = pattern.sub(r'\1logInfo\3', strNew)
			print('###' + strNew)
		fileWrite.writelines(strNew)
	fileRead.close()
	fileWrite.close()
	os.rename(filePath + '.new', filePath)
		

def listyoudir(level, path): 
	ignore = re.compile(r'^ddebug.h$')
	pattern = re.compile(r'.[ch]$')
	for i in os.listdir(path): 
		print ('     '*(level+1) + i)
		if os.path.isdir(path + '/' + i):
			listyoudir(level+1, path + '/' + i)
		else:
			strNeed = pattern.search(i)
			if (strNeed):
				strIgn = ignore.search(i)
				if (strIgn):
					continue
				if (FindString(i)):
					ReplaceFile(i)

Rootpath = os.path.abspath('.') 
print (Rootpath) 
print (sys.argv[1])
listyoudir(0, Rootpath)
