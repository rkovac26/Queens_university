import random

#number of iterations
iters = 10000
#open input file
with open("nqueens.txt") as f:
    content = f.readlines()
#save it to array content
content = [x.strip() for x in content]

def nqueens():
	output = range(len(content))
	for i in range(len(content)):
		board_size = int(content[i])
		current = init_board(board_size) #creates initial board_size
		print "First part done"
		ith_output = min_conflicts(current, board_size, iters)
		output[i] = ith_output
		print "Second part done"

	with open('nqueens_out.txt', 'w') as f:
		for i in range(len(content)):
			f.write(str(output[i]) + "\n")

	print "done"

def init_board(board_size):
    current = [-1] * board_size
    loop = range(board_size)
    for i in loop:
        possible_confs = [initial_conflicts(current, i, i, row) for row in loop]
        current[i] = random_min(possible_confs, lambda x: x == min(possible_confs), board_size)
    return current


def min_conflicts(current, board_size, iterations):
	for k in range(iterations):
		confs = find_conflicts(current, board_size)

		if sum(confs) == 0:
			return map(lambda x:x+1, current)
		#choose random column (queen) which has a collision
		col = random.choice([i for i in range(board_size) if confs[i] > 0])
		#find how many collisions has the queen in the column
		#When moved to another row
		possible_confs = [conflicts(current, board_size, col, row) for row in range(board_size)]
		#pick the smallest one
		current[col] = random_min(possible_confs, lambda x: x == min(possible_confs), board_size)

	return "Failure"

def random_min(input_list, filter, board_size):
	return random.choice([i for i in range(board_size) if filter(input_list[i])])

def conflicts(current, board_size, col, row):
	total = 0
	for i in range(board_size):
		if i == col:   #that's the column of our queen, we skip it
			continue
		#because we move only alongside columns, we check for rows or
		#diagonal and row collisions only
		if current[i] == row or abs(i - col) == abs(current[i] - row):
			total += 1
	return total

def initial_conflicts(current, board_size, col, row):
	total = 0
	for i in range(board_size):
		if i == col or row == -1 or current[i] == -1:
			continue
		if current[i] == row or abs(i - col) == abs(current[i] - row):
			total += 1
	return total

def find_conflicts(current, board_size):
	return [conflicts(current, board_size, col, current[col]) for col in range(board_size)]

nqueens()
