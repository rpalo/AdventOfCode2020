"""Day 15: Rambunctions Recitation

Help the elves play a memory game where they remember which numbers
they said at which timestep.

It's not pretty, but it gets the job done, and I'm busy.
"""

history = dict()
starting = list(reversed([9,3,1,0,8,4]))

for i in range(1, 30000001):
    if starting:
        current = starting.pop()
    elif first:
        current = 0
    else:
        current = time_since_last
    
    first = (current not in history)
    if not first:
        time_since_last = i - history[current]
    history[current] = i

print(current)
    