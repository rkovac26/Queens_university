#--------------Assignment 3-----------------#
#Final output written to the outLife
output = ""

#Process the input file into 2D array
def process_file(input):
    input = [x.strip() for x in input]
    gen_0 = [list(x) for x in input]
    gen_0 = gen_0[1:]

    return gen_0

#Return the number of ones around a point which has position x and y
#in a "matrix"
def neighbours(matrix, x, y):
    count = 0
    if x>0:
        if matrix[x-1][y] == '1':
            count+=1
        if y>0:
            if matrix[x-1][y-1] == '1':
                count+=1
    if y>0:
        if matrix[x][y-1] == '1':
            count+=1
        if x+1<rows:
            if matrix[x+1][y-1] == '1':
                count+=1
    if x+1<rows:
        if matrix[x+1][y] == '1':
            count+=1
        if y+1<cols:
            if matrix[x+1][y+1] == '1':
                count+=1
    if y+1<cols:
        if matrix[x][y+1] == '1':
            count+=1
        if x>0:
            if matrix[x-1][y+1] == '1':
                count+=1

    return count

#Opens the file
with open("inLife.txt") as f:
    content = f.readlines()

#Stores the number of generations
gens_number = int(content[0])
#Generation 0
current_generation = process_file(content)
#Dimensions of the input matrix
rows = len(current_generation)
cols = len(current_generation[0])

#Ouputs the unchanged generation 0
output += "Generation 0" + "\n"
for line in current_generation:
    output += ''.join(line) + "\n"

#----------------Life Algorithm-------------------------#

for k in range(gens_number):
    #Ensures that the next_generation is always a 2D array
    #without any '1' or '0', fills it with 'x'
    next_generation = [x[:] for x in [['x'] * cols] * rows]
    #First line of each iteration
    output += "Generation " + str(k + 1) + "\n"

    for i in range(rows):
        for j in range(cols):
            score = neighbours(current_generation, i, j)
            if current_generation[i][j] == '1':
                if score == 2 or score == 3:
                    next_generation[i][j] = '1'
                else:
                    next_generation[i][j] = '0'
            if current_generation[i][j] == '0':
                if score == 3:
                    next_generation[i][j] = '1'
                else:
                    next_generation[i][j] = '0'

    for i in range(rows):
        for j in range(cols):
            current_generation[i][j] = next_generation[i][j]

    for line in current_generation:
        output += ''.join(line) + "\n"

#--------------End life algorithm-----------------------#


with open("outLife.txt", 'w') as f:
    f.write(output)
