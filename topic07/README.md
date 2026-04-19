# Topic 7: Introduction to Quantum Computing

Documentation on tasks from "Program your own quantum computer - Part 1" by Martin N.P. Nilsson for ENCCS

Report available in separate file "Report.md".

The task was to produce a working example of a stack based quantum computer simulation using Python. Using the Kronecker product, qubits were implemented as quantums state vectors and their positions manipulated in the quantum stack. The simulation implemented further functionality by allowing the measurement of qubits which results in a reading of 0 or 1, this also removes the qubit from the quantum stack. Additionally, some quantum gates were implemented that could be applied to a selected qubit(s).

### Functionality

- Initialize qubits with specific probability weights
- Push qubits to quantum stack
- Push a qubit to the top of the quantum stack
- Calculate normalized probabilities of |0> and |1> quantum state vector
- Measure qubit and remove from stack
- Apply quantum gates to qubit
-   Pauli X-gate
-   Hadamard gate
-   SWAP gate
-   Toffoli gate
-   Controlled Toffili gate with 3 control qubits

### Prerequisities
Python 3 and NumPy

### Expected Outputs
Basic gate tests : Two qubits "Q1" and "Q2" of different probability distribtuions are initialized and pushed to stack. Their states are printed to terminal.
Pauli X-gate : The Pauli X-gate is applied to "Q2" and the output is the state which is printed to the terminal.
Controlled Toffoli gate Truth table : The controlled Tofolli gate is applied to a single qubit 20 times using 3 additional control qubits as inputs. The output is the classical bit of strings for each of the 20 runs.

### Error Handling
Error message will be displayed in the terminal if qubit measurements are attempted on an inknown qubit or qubit that does not exist.
