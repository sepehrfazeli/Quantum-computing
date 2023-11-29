# #include "qhipster/IqsSimulator.h"
# Import the Python library with the C++ class and methods of Intel Quantum Simulator.
# If the library is not contained in the same folder of this notebook, its path has to be added.
import sys
sys.path.insert(0, '../lib_python')
import intelqs as simulator

#  Import NumPy library with Intel specialization.
import numpy as np
from numpy import random_intel

#  Import graphical library for plots.
import matplotlib.pyplot as plt

# Create a quantum register with 3 qubits
iqs::QubitRegister<ComplexDP> psi(3);

// Prepare an entangled pair
psi.ApplyHadamard(1);
psi.ApplyCPauliZ(1, 2);

// Apply Hadamard gate to qubitA
psi.ApplyHadamard(0);

// Apply CNOT gate with qubitA as the control and entangledPair as the target
psi.ApplyCPauliX(0, 1);

// Apply Hadamard gate to qubitA
psi.ApplyHadamard(0);

// Measure qubitA and entangledPair
auto resultA = psi.Measure(0);
auto resultEntangled = psi.Measure(1);

// Apply Pauli X and/or Z gates based on measurement results
if (resultEntangled == 1) {
  psi.ApplyPauliZ(2);
}
if (resultA == 1) {
  psi.ApplyPauliX(2);
}

// Print the final state
psi.Print("Final state:");
