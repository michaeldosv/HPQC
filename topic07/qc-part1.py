# libraries
import numpy as np

# function to push Qubit to the stack taking 'weights' and the name of qubit as inputs
def pushQubit(name, weights):
        global workspace
        global namestack
        if workspace.shape == (1,1):
                namestack = []
        namestack.append(name)
        weights = weights / np.linalg.norm(weights)
        weights = np.array(weights, dtype=workspace[0,0].dtype)
        workspace = np.reshape(workspace, (1,-1))
        workspace = np.kron(workspace, weights)

# to carry out operations on qubits we need to define applyGate by reshaping the workspace
def applyGate(gate, *names):
        global workspace
        if list(names) != namestack[-len(names):]:
                for name in names:
                        tosQubit(name)
        workspace = np.reshape(workspace, (-1,2**(len(names))))
        subworkspace = workspace[:,-gate.shape[0]:]
        np.matmul(workspace,gate.T,out=workspace)

# function to push a qubit to the top of stack
def tosQubit(name):
        global workspace
        global namestack
        k = len(namestack) - namestack.index(name)
        if k > 1:
                namestack.append(namestack.pop(-k))
                workspace = np.reshape(workspace, (-1, 2, 2**(k-1)))
                workspace = np.swapaxes(workspace, -2, -1)

# function to calculate probability of 0 or 1 of qubit at top of stack
def probQubit(name):
        global workspace
        tosQubit(name)
        workspace = np.reshape(workspace,(-1,2))
        prob = np.linalg.norm(workspace,axis=0)**2
        return prob / prob.sum()


# function to take measurement of qubit
def measureQubit():
        global workspace
        global namestack
        prob = probQubit() # call probQubit() to measure probability of qubit at top of stack
        measurement = np.random.choice(2,p=prob) # select 0 or 1
        workspace = (workspace[:,[measurement]] / np.sqrt(prob[measurement])) # remove measured qubit from stack
        namestack.pop()
        return str(measurement) # return outcome of measured qubit



# define gates
# Pauli X gate
X_gate = np.array([[0,1],
                  [1,0]])

# Hadamard gate
H_gate = np.array([[1,1],
                 [1,-1]]) * np.sqrt(1/2)

# SWAP gate
SWAP_gate = np.array([[1, 0, 0, 0],
                      [0, 0, 1, 0],
                      [0, 1, 0, 0],
                      [0, 0, 0, 1]])

# Toffoli gate
TOFF_gate = np.array([[1, 0, 0, 0, 0, 0, 0, 0],
                     [0, 1, 0, 0, 0, 0, 0, 0],
                     [0, 0, 1, 0, 0, 0, 0, 0],
                     [0, 0, 0, 1, 0, 0, 0, 0],
                     [0, 0, 0, 0, 1, 0, 0, 0],
                     [0, 0, 0, 0, 0, 1, 0, 0],
                     [0, 0, 0, 0, 0, 0, 0, 1],
                     [0, 0, 0, 0, 0, 0, 1, 0]])

# Controlled Toffoli gate
def TOFF3_gate(q1,q2,q3,q4):
        pushQubit("temp",[1,0])
        applyGate(TOFF_gate,q1,q2,"temp")
        applyGate(TOFF_gate,"temp",q3,q4)
        applyGate(TOFF_gate,q1,q2,"temp") 
        measureQubit("temp")                      


workspace = np.array([[1.]]) # create a new empty stack of qubits
pushQubit("Q1", [1,0]) # push the first qubit which is definitely 0
print("After Q1 : ", np.reshape(workspace,(1,-1)))
pushQubit("Q2", [3/5,4/5]) # push another qubit with a different probablility
tosQubit("Q2")
print("After Q2 : ", np.reshape(workspace,(1,-1)))

# apply X_gate to qubit at the top of the stack
applyGate(X_gate, "Q2")
print("Output of X gate : ",workspace)

# generate a truth table for the controlled Toffoli gate
workspace = np.array([[1.]])  
for i in range(20): 
        pushQubit("Q1",[1,1])
        pushQubit("Q2",[1,1])
        pushQubit("Q3",[1,1])
        pushQubit("Q4",[1,0])   

TOFF3_gate("Q1","Q2","Q3","Q4")  
print("".join([measureQubit(q) for q in
                   ["Q1","Q2","Q3","Q4"]]), end=",")

