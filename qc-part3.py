# libraries
import numpy as np
import torch as pt
import time

# check if a suitable GPU is available, otherwise default to CPU
if pt.cuda.is_available():
    print("GPU available! Using CUDA.")
    device = pt.device('cuda')
else:
    print("No GPU available. Running on CPU.")
    device = pt.device('cpu')

# initialize the workspace on the chosen device
workspace = pt.tensor([[1.]], device=device, dtype=pt.float32)
namestack = []

# function to push Qubit to the stack taking 'weights' and the name of qubit as inputs
def pushQubit(name, weights):
    global workspace
    global namestack
    if workspace.shape == (1,1):
        namestack = []
    namestack.append(name)

    # normalize the weights using numpy then convert to PyTorch tensor
    weights = weights / np.linalg.norm(weights)
    weights = pt.tensor(weights, device=workspace.device, dtype=workspace[0,0].dtype)
    workspace = pt.reshape(workspace, (1, -1))
    workspace = pt.kron(workspace, weights) 

# to carry out operations on qubits we need to define applyGate by reshaping the workspace
def applyGate(gate, *names):
    global workspace
    if list(names) != namestack[-len(names):]:
        for name in names:
            tosQubit(name)
            
    workspace = pt.reshape(workspace, (-1, 2**len(names)))
    subworkspace = workspace[:, -gate.shape[0]:]
    
    # Convert the gate to a PyTorch tensor and send it to the GPU
    gate_pt = pt.tensor(gate.T, device=workspace.device, dtype=workspace[0,0].dtype)
    
    if workspace.device.type == 'cuda':
        pt.matmul(subworkspace, gate_pt, out=subworkspace) # Fast GPU matmul
    else:
        subworkspace[:,:] = pt.matmul(subworkspace, gate_pt) # CPU fallback

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
    workspace = pt.reshape(workspace, (-1, 2))
    
    # Calculate probabilities using PyTorch's linear algebra package
    prob = pt.linalg.norm(workspace, axis=0)**2
    prob = pt.Tensor.cpu(prob).numpy() # Move back to CPU for numpy random functions
    return prob / prob.sum()


# function to take measurement of qubit
def measureQubit(name):
    global workspace
    global namestack
    prob = probQubit(name) # call probQubit(name) to measure probability of qubit at top of stack
    measurement = np.random.choice(2,p=prob) # select 0 or 1
    workspace = (workspace[:,[measurement]] / np.sqrt(prob[measurement])) # remove measured qubit from stack
    namestack.pop()
    return str(measurement) # return outcome of measured qubit

# Grover's search implementation
def zero_phaseOracle(qubits):            # all qubits zero? 
    for qubit in qubits:                 # negate all inputs
        applyGate(X_gate,qubit)
    applyGate(Z_gate,*namestack)         # controlled Z gate
    for qubit in qubits:                 # restore inputs
        applyGate(X_gate,qubit)

def sample_phaseOracle(qubits):          # sample function 
    applyGate(X_gate,qubits[1])          # negate qubit 1
    applyGate(Z_gate,*namestack)         # controlled Z gate
    applyGate(X_gate,qubits[1])          # restore qubit 1

def groverSearch(n, printProb=True):
    optimalTurns = int(np.pi/4*np.sqrt(2**n)-1/2)   # iterations 
    qubits = list(range(n))                         # generate qubit names
    for qubit in qubits:                            # initialize qubits
        pushQubit(qubit,[1,1])
    for k in range(optimalTurns):                   # Grover iterations:
        sample_phaseOracle(qubits)                  # apply phase oracle
        for qubit in qubits:                        # H-gate all qubits
            applyGate(H_gate,qubit) 
        zero_phaseOracle(qubits)                    # apply 0 phase oracle
        for qubit in qubits:                        # H-gate all qubits
            applyGate(H_gate,qubit) 
        if printProb:                               # peek probabilities
            print(probQubit(qubits[0]))             # to show convergence
    for qubit in reversed(qubits):                  # print result 
        print(measureQubit(qubit),end="")



# define gates
# Pauli X gate
X_gate = np.array([[0,1],
                  [1,0]])

# Hadamard gate
H_gate = np.array([[1,1],
                 [1,-1]]) * np.sqrt(1/2)

# Pauli Z gate
Z_gate = np.array([[1, 0], 
                   [0, -1]])

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

# try Grover's search
workspace = np.array([[1.]])
groverSearch(6)

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
        
