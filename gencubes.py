## color gradient generator

rgb = []
rgb.append([])
rgb.append([])
rgb.append([])
f = open("cubes.txt", "r")
lines = f.readlines()
f.close()
f = open("temp.txt", "w")

wr = []
wr.append("struct rgb_t gradient1[] = {\n")

i = 0
index = 0
for line in lines:
    flt = str(int(round(float(line.strip('\n')))))
    rgb[index].append(flt)
    i += 1
    if i == (index + 1)*100:
        index += 1

for x in range(len(rgb[0])):
    wr.append("\t(rgb_t){" + rgb[0][x] + ', ' + rgb[1][x] + ', ' + rgb[2][x] + "},\n")

wr.append("};")

f.writelines(wr)
f.close()