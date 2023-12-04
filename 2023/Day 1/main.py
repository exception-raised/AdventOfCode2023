import re

input_file = open("Day 1/input.txt")

total = 0
while(True):
    line = input_file.readline()
    if(not line):
        break

    left_pointer = -1
    right_pointer = -1

    
    for i in range(len(line)):
        if(line[i].isdigit()):
            if(left_pointer == -1):
                left_pointer = int(line[i])
                right_pointer = int(line[i])
            else:
                right_pointer = int(line[i])

    total += left_pointer*10 + right_pointer # Example: 8, 5 ; 8 * 10 = 80 ; 80 + 5 = 85

print(total)
input_file.close()