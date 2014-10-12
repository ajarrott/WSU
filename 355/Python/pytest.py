stackDict = []
testStack = []

stackLevel = 0
codeLevel = 10
while (stackLevel < 10):

	stackDict.append({stackLevel:{codeLevel:"somecodeinhere"+str(codeLevel)}})
	stackLevel = stackLevel + 1
	codeLevel = codeLevel + 1

print stackDict

for key in stackDict:
	print key 
	print key.keys()
	print key.values()
	code = key.values()
	try:
		stackval = code.pop()
		print stackval.keys()
		print stackval.values()
	except:
		pass
	#for i in testStack:
#	print testStack[i]


