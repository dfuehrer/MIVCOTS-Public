with open("testdata.csv") as f:
    data = f.readlines()
day = '20180416'
time = 12303000
j=0;
f = open('new_test_data.csv', 'w')
for line in data:
    line = line.split(',')
    i = 0;
    for strnew in line:
        #print(strnew)
        if(strnew == "ID"):
            id = line[i+1]
        if(strnew == "LG"):
            la = line[i+1]
        if(strnew == "LT"):
            lt = line[i+1]
        if(strnew == "CS"):
            cs = line[i+1]
        if(strnew == "AL"):
            al = line[i+1]

        i+=1
    new = "#," + str(j)
    new +=  ',AA,' + (id)
    new += ',AB,' + (day)
    #new += ',AC,' + str(time+j)
    new += ',AD,' + (la)
    new += ',AE,' + (lt)
    new += ',AF,' + (al)
    new += ',AG,' + (cs)
    new += ',!\n'
    print(new)
    f.write(new)

    j+=1
f.close()
