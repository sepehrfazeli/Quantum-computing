#include "qhipster/IqsSimulator.h"

int main() {
  // Create a quantum register with 3 qubits
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

  return 0;
}