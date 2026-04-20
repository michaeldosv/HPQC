# Topic 7: Introduction to Quantum Computing

Documentation on tasks from "Program your own quantum computer - Part 1" by Martin N.P. Nilsson for ENCCS

The task was to produce a working example of a stack based quantum computer simulation using Python. Using the Kronecker product, qubits were implemented as quantums state vectors and their positions manipulated in the quantum stack. The simulation implemented further functionality by allowing the measurement of qubits which results in a reading of 0 or 1, this also removes the qubit from the quantum stack. Additionally, some quantum gates were implemented that could be applied to a selected qubit(s).

## Functionality

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

## Prerequisities
Python 3 and NumPy

## Expected Outputs
Basic gate tests : Two qubits "Q1" and "Q2" of different probability distribtuions are initialized and pushed to stack. Their states are printed to terminal.
Pauli X-gate : The Pauli X-gate is applied to "Q2" and the output is the state which is printed to the terminal.
Controlled Toffoli gate Truth table : The controlled Tofolli gate is applied to a single qubit 20 times using 3 additional control qubits as inputs. The output is the classical bit of strings for each of the 20 runs.

## Error Handling
Error message will be displayed in the terminal if qubit measurements are attempted on an inknown qubit or qubit that does not exist.

## Discussion (Report)

The tutoral from which this code is derived shows how to simulate a quantum computer on classical hardware. This is a short discussion on how the programme works and the outputs generated. 

**Defining the workspace and qubits :** The simulator works by using the workspace array to represent the state vector which contains the probability amplitudes for the qubit states 0 and 1. The workspace scales with the number of qubits. One qubit is two elements in the workspace and two qubits is four elements in the workspace, etc. The workspace scales 2^N where N is the number of qubits. 

**Pushing Qubits to stack :** This process is done using the Kronecker product or the np.kron function in numpy. This works by multiplying the probabiliy amplitudes of any new qubit pushed to the workspace by the probabilty amplitudes of the existing qubits, this causes all of the qubits to be entangled. 

**Quantum gate operations :** Quantums gates such as the Pauli X gate or CNOT gate or any others can be implemented in python using a matrix array. This is what is done here. Only a qubit at the top of the stack (workspace) can be passed through a gate, however in terms of a quantum computer all the probability amplitudes of all qubits are passed through any gate that is used on the qubit at the top of the stack. 

**Quantum measurements :** Finally, quantum states can be measured causing their collapse. This is done using measureQubit() which computes the outcome of the qubit to be either 0 or 1 based on the probability amplitudes. Once the measurement is taken, the outcome is printed to the terminal and the qubit is removed from the workspace as it no longer exists in a superpositiion state.

**Results**

The first bit of the code pushes two qubits "Q1" and "Q2" to the workspace with selected probabilities. Before this it creates an empty workspace. The probability amplitudes of Q1 and Q2 are printed to the terminal and since Q1 was defined to be [1,0] and Q2 [3/5.4/5] the outcome of the combined workspace is [[0.6, 0.8, 0., 0.]]. 

Then the X gate is applied to Q2, which results in a flip of the probabilities. The outcome is [[0.8, 0.6, 0., 0.]].

Finally, a controlled Toffoli gate is used on some new qubits Q1, Q2 and Q3 with weights [1, 1] which are used to control the gate. The controlled Toffoli gate defined as a function TOFF3_gate() is looped 20 times to obtain a truth table of as many possible combinations of the final collapsed state after qubits are passed through the gate. 
