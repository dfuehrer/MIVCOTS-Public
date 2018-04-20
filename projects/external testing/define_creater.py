with open("Keys_Define_Sheet.csv") as f:
    data = f.readlines()

f = open('key_defines.h', 'w')

defines = []
vectorList = []
for line in data:
    line = line.replace(",\n","").replace("\n","").split(',')
    if(len(line) == 2):
        print("#define " + line[1] + " \"" + line[0] + "\"")
        defines.append("#define " + line[1] + " \"" + line[0] + "\"\n")
        vectorList.append(line[1] + ",")
        #f.write("#define " + line[1] + " \"" + line[0] + "\"\n")

vectorList[-1] = vectorList[-1].replace(",", "")
print (vectorList)

f.write("#include<vector>\n#include<string>\n")

f.write("#define numKeys " + str(len(defines)) + "\n")
for line in defines:
    f.write(line)

f.write("std::vector<std::string> keyList {")
for keyID in vectorList:
	f.write(keyID)

f.write("}")
