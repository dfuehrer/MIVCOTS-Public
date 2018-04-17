with open("Keys_Define_Sheet.csv") as f:
    data = f.readlines()

f = open('key_defines.h', 'w')

for line in data:
    line = line.replace(",\n","").replace("\n","").split(',')
    if(len(line) == 2):
        print("#define " + line[1] + " \"" + line[0] + "\"")
        f.write("#define " + line[1] + " \"" + line[0] + "\"\n")
