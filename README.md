# Bot-Searching-in-Partially-Observable-Environment

  The problem is to for the intelligent Agent to reach the target location in a given environment.
  The agent is partially observable to only its neighbouring 4 cells.
  
The Agent employs these 3 algorithms to reach the target location.

1. Repeated Forward A* Algorithm
2. Repeated Backward A* Algorithm
3. Adaptive A* Algorithm.


The code works as follows:

First I generated the maze/corridor like structure with Depth First Search by using
random tie-breaking and backtracking. 50 such Environments are generated and
stored in the files separately. Refer AgentTest.cpp to see how its done.

Then based on the inputs given to AgentTest.cpp, the program runs the agent on the created 50 gridworld environments.
We can also set the timer to 1 second to actually see how the agent's internal view of the environment at each step.

Future Work:

1. Create a visual application using Python to see how actually the agent moved, its calculated path at each step and
   its internal view of the environment vs the actual environment.
