from qiskit import QuantumCircuit, transpile, Aer, assemble, execute
from qiskit.visualization import plot_histogram

def QuantumTeleportation(qubitA, qubitB, entangledPair):
    # Create a quantum circuit with three qubits and three classical bits
    qc = QuantumCircuit(3, 3)
    
    # Prepare an entangled pair
    qc.h(entangledPair)
    qc.cx(entangledPair, qubitB)
    
    # Apply Hadamard gate to qubitA
    qc.h(qubitA)
    
    # Apply CNOT gate with qubitA as the control and entangledPair as the target
    qc.cx(qubitA, entangledPair)
    
    # Apply Hadamard gate to qubitA
    qc.h(qubitA)
    
    # Measure qubitA and entangledPair
    qc.measure(qubitA, 0)
    qc.measure(entangledPair, 1)
    
    # Apply Pauli X and/or Z gates based on measurement results
    qc.z(qubitB).c_if(qc, entangledPair)
    qc.x(qubitB).c_if(qc, qubitA)
    
    # Apply Pauli Z gate to qubitB if necessary
    qc.z(qubitB).c_if(qc, qubitA)
    
    # Simulate the quantum circuit
    simulator = Aer.get_backend('qasm_simulator')
    compiled_circuit = transpile(qc, simulator)
    result = execute(compiled_circuit, simulator, shots=1024).result()
    
    # Visualize the results
    counts = result.get_counts(qc)
    plot_histogram(counts)

# Example usage
QuantumTeleportation(0, 2, 1)
