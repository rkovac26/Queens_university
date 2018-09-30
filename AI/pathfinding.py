import itertools
from heapq import heappush
from heapq import heappop
import math

#Global coordinates of start and goal
s1 = 0
s2 = 0
g1 = 0
g2 = 0
#Store the final path
the_path = []

#Processes multiple grids and ouputs them in an array contents
def process_file(content):
    content = [x.strip() for x in content]
    content = [list(x) for x in content]

    number_of_grids = 1
    grid_sizes = [len(content[0])]
    grid_starts = [0]
    grid_ends = []
    for i in range(len(content)):
    	if content[i] == []:
    		number_of_grids += 1
    		grid_starts.append(i+1)
    		grid_ends.append(i)

    grid_ends.append(len(content))

    contents = list(range(number_of_grids))
    for i in range(number_of_grids):
    	contents[i] = content[grid_starts[i]:grid_ends[i]]


    return contents


#Finds starting and finish point of one grid
def find_start_goal(content):
    for i in range(len(content)):
        for j in range(len(content[0])):
            if content[i][j]=='S':
                s1 = i
                s2 = j
            if content[i][j]=='G':
                g1 = i
                g2 = j
    return [(s1,s2), (g1,g2)]


#------------------Implemented priority Q---------------------
pq = []                         # list of entries arranged in a heap
entry_finder = {}               # mapping of tasks to entries
REMOVED = '<removed-task>'      # placeholder for a removed task
counter = itertools.count()     # unique sequence count

#Add a new task or update the priority of an existing task
def add_task(task, priority=0):
    if task in entry_finder:
        remove_task(task)
    count = next(counter)
    entry = [priority, count, task]
    entry_finder[task] = entry
    heappush(pq, entry)

#Mark an existing task as REMOVED.  Raise KeyError if not found
def remove_task(task):
    entry = entry_finder.pop(task)
    entry[-1] = REMOVED

#Remove and return the lowest priority task. Raise KeyError if empty
def pop_task():
    while pq:
        priority, count, task = heappop(pq)
        if task is not REMOVED:
            del entry_finder[task]
            return task
    raise KeyError('pop from an empty priority queue')

#------------------------end of implementation if priority queue------------

#Finds neighbours of a current position
#when we can move in 4 directions
def neighbours(a, content):
    currentX = a[0]
    currentY = a[1]
    neighbours_list = list(range(4))
    ind = 0
    if content[currentX - 1][currentY] == '_' or content[currentX - 1][currentY] == 'G':
        neighbours_list[ind] = (currentX - 1,currentY)
        ind = ind + 1
    if content[currentX][currentY - 1] == '_' or content[currentX][currentY - 1] == 'G':
        neighbours_list[ind] = (currentX,currentY - 1)
        ind = ind + 1
    if content[currentX + 1][currentY] == '_' or content[currentX + 1][currentY] == 'G':
        neighbours_list[ind] = (currentX + 1,currentY)
        ind = ind + 1
    if content[currentX][currentY + 1] == '_' or content[currentX][currentY + 1] == 'G':
        neighbours_list[ind] = (currentX,currentY + 1)
        ind = ind + 1

    return neighbours_list[0:ind] # if less than 4 neighbours are accessible


#Finds neighbours of a current position
#when we can move in 8 directions
def neighboursB(a, content):
    currentX = a[0]
    currentY = a[1]
    neighbours_list = list(range(8))
    ind = 0
    if content[currentX - 1][currentY] == '_' or content[currentX - 1][currentY] == 'G':
        neighbours_list[ind] = (currentX - 1,currentY)
        ind = ind + 1
    if content[currentX][currentY - 1] == '_' or content[currentX][currentY - 1] == 'G':
        neighbours_list[ind] = (currentX,currentY - 1)
        ind = ind + 1
    if content[currentX + 1][currentY] == '_' or content[currentX + 1][currentY] == 'G':
        neighbours_list[ind] = (currentX + 1,currentY)
        ind = ind + 1
    if content[currentX][currentY + 1] == '_' or content[currentX][currentY + 1] == 'G':
        neighbours_list[ind] = (currentX,currentY + 1)
        ind = ind + 1
    if content[currentX + 1][currentY + 1] == '_' or content[currentX + 1][currentY + 1] == 'G':
        neighbours_list[ind] = (currentX + 1,currentY + 1)
        ind = ind + 1
    if content[currentX - 1][currentY - 1] == '_' or content[currentX - 1][currentY - 1] == 'G':
        neighbours_list[ind] = (currentX - 1,currentY - 1)
        ind = ind + 1
    if content[currentX + 1][currentY - 1] == '_' or content[currentX + 1][currentY - 1] == 'G':
        neighbours_list[ind] = (currentX + 1,currentY - 1)
        ind = ind + 1
    if content[currentX - 1][currentY + 1] == '_' or content[currentX - 1][currentY + 1] == 'G':
        neighbours_list[ind] = (currentX - 1,currentY + 1)
        ind = ind + 1

    return neighbours_list[0:ind]


#Euclidian distance
def heuristic(goal, next):
    return math.sqrt(pow(next[0] - goal[0], 2) + pow(next[1] - goal[1], 2))

#Chebyshev distance
def heuristic_chebDistance(goal, next):
    return max(abs(goal[0] - goal[0]), abs(next[1] - next[1]))
#manhattan distance (unused)
def heuristic_man(goal, next):
    return abs(next[0] - goal[0]) + abs(next[1] - goal[1])

#Tracks back from goal to start to find the path
def create_path(position, list):
    if list[position] == None:
        return the_path
    else:
        the_path.append(list[position])
        create_path(list[position], list)


#Greedy algorithm for 4 directions
def greedyAlg(content):
    del pq[:]
    entry_finder.clear()
    start_goal = find_start_goal(content)
    start = start_goal[0]
    goal = start_goal[1]
    add_task(start,0)
    came_from = {}
    came_from[start] = None


    while len(pq)!=0:
        current = pop_task()

        if current == goal:
            break

        for next_node in neighbours(current, content):
            if next_node not in came_from.values():
                priority = heuristic(goal, next_node)
                add_task(next_node, priority)
                came_from[next_node] = current

    content_out = [row[:] for row in content]

    #Finds path from came_from, starting at goal finishing at start
    create_path(goal, came_from)

    for value in the_path:
        content_out[value[0]][value[1]] = 'P'

    content_out[start[0]][start[1]] = 'S'
    del the_path[:]


    return content_out

#A* algorithm for 4 directions
def aStarAlg(content):
    del pq[:]
    entry_finder.clear()
    start_goal = find_start_goal(content)
    start = start_goal[0]
    goal = start_goal[1]
    add_task(start,0)
    came_from = {}
    cost_so_far = {}
    came_from[start] = None
    cost_so_far[start] = 0

    while len(pq)!=0:
        current = pop_task()

        if current == goal:
            break

        for next_node in neighbours(current, content):
            new_cost = cost_so_far[current] + 1
            if next_node not in cost_so_far.keys() or new_cost < cost_so_far[next_node]:
                cost_so_far[next_node] = new_cost
                priority = new_cost + heuristic(goal, next_node)
                add_task(next_node, priority)
                came_from[next_node] = current



    #writes out the path taken
    content_out = content

    #Finds path from came_from, starting at goal finishing at start
    create_path(goal, came_from)

    for value in the_path:
        content_out[value[0]][value[1]] = 'P'

    content_out[start[0]][start[1]] = 'S'
    del the_path[:]

    return content_out

#Greedy algorithm for 8 directions
def greedyAlgB(content):
    del pq[:]
    entry_finder.clear()
    start_goal = find_start_goal(content)
    start = start_goal[0]
    goal = start_goal[1]
    add_task(start,0)
    came_from = {}
    came_from[start] = None


    while len(pq)!=0:
        current = pop_task()

        if current == goal:
            break

        for next_node in neighboursB(current, content):
            if next_node not in came_from.values():
                priority = heuristic_chebDistance(goal, next_node)
                add_task(next_node, priority)
                came_from[next_node] = current

    content_out = [row[:] for row in content] #copy array

    #Finds path from came_from, starting at goal finishing at start
    create_path(goal, came_from)

    for value in the_path:
        content_out[value[0]][value[1]] = 'P'

    content_out[start[0]][start[1]] = 'S'
    del the_path[:]

    return content_out

#A* algorithm for 8 directions
def aStarAlgB(content):
    del pq[:]
    entry_finder.clear()
    start_goal = find_start_goal(content)
    start = start_goal[0]
    goal = start_goal[1]
    add_task(start,0)
    came_from = {}
    cost_so_far = {}
    came_from[start] = None
    cost_so_far[start] = 0

    while len(pq)!=0:
        current = pop_task()

        if current == goal:
            break

        for next_node in neighboursB(current, content):
            new_cost = cost_so_far[current] + 1
            if next_node not in cost_so_far.keys() or new_cost < cost_so_far[next_node]:
                cost_so_far[next_node] = new_cost
                priority = new_cost + heuristic_chebDistance(goal, next_node)
                add_task(next_node, priority)
                came_from[next_node] = current


    content_out = content

    #Finds path from came_from, starting at goal finishing at start
    create_path(goal, came_from)

    for value in the_path:
        content_out[value[0]][value[1]] = 'P'

    content_out[start[0]][start[1]] = 'S'
    del the_path[:]


    return content_out

#---------------Part A---------------
with open("pathfinding_a.txt") as f:
    contentA = f.readlines()

grids = process_file(contentA)

output = ""
for i in range(len(grids)):
    output += "Greedy" + "\n"
    contentG = greedyAlg(grids[i])
    for j in contentG:
        output = output + ''.join(j)
        output = output + "\n"
    output += "A*" + "\n"
    contentA = aStarAlg(grids[i])
    for x in contentA:
        output = output + ''.join(x)
        output = output + "\n"

with open('pathfinding_a_out.txt', 'w') as f:
    f.write(output)

#-------------End of Part A---------------

#--------------Part B---------------------

with open("pathfinding_b.txt") as f:
    contentB = f.readlines()

grids = process_file(contentB)

output = ""
for i in range(len(grids)):
    output += "Greedy" + "\n"
    contentG = greedyAlgB(grids[i])
    for j in contentG:
        output = output + ''.join(j)
        output = output + "\n"
    output += "A*" + "\n"
    contentA = aStarAlgB(grids[i])
    for x in contentA:
        output = output + ''.join(x)
        output = output + "\n"

with open('pathfinding_b_out.txt', 'w') as f:
    f.write(output)

#-------------End of part B---------------
