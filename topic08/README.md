# Topic 8: Grover's Search

Reporting on tasks from "Program your own quantum computer - Part 2: Grover's Search" by Martin N.P. Nilsson for ENCCS

This part builds on the simulated quantum computer built in Topic 7. Hence a limited discussion will happen here. 

## Functionality
- Simulated quantum computer (building on Part 1)
- Grover' search

## Discussion (Report)

Grover's search was implemented into the existing code with a few simple additions. 

**Superposition :** The groverSearch() function starts by pushing new qubits to the stack with weights of [1, 1] which creates an equal superposition among them. 

**Phase Oracle :** This function uses the Pauli X gate and Z gate to track a specific solution to a function. When this function locates the solution which corresponds to a quantum state which has not been measured yet, completes a phase flip which reverses the vector. 

**Optimal turns to find solution :** To find the solution to the function, groverSearch() calculates an optimalTurns variable using the formula provided in the article. groverSearch() uses this value as a limit for when to stop searching for the solution and accept the current one. 
