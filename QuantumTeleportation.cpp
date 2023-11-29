#include <clang/Quantum/quintrinsics.h>

/// Quantum Runtime Library APIs
#include <quantum.hpp>

#include <iostream>

/* Declare 3 qubits */
qbit AliceState;
qbit BellA;
qbit BellB;

/* Declare 3 measurement readouts */
cbit AliceBit1;
cbit AliceBit2;
cbit BobResult;

// Create a bell pair
quantum_kernel void prepBellPair(qbit q0, qbit q1) {
  PrepZ(q0);
  PrepZ(q1);
  H(q0);
  CNOT(q0, q1);
}

quantum_kernel void alicePrepState() {
  // Prep all
  PrepZ(AliceState);
  PrepZ(BellA);
  PrepZ(BellB);

  // Alice creates her quantum state--here an X rotation
  RX(AliceState, 1);
}

quantum_kernel void aliceProtocol() {
  // Create a bell pair
  prepBellPair(BellA, BellB);

  // Alice entangles her qubit with the first element of the Bell pair
  CNOT(AliceState, BellA);
  H(AliceState);
}

// Alice measures her two bits and sends them to Bob.
quantum_kernel void measAlice() {
  MeasZ(AliceState, AliceBit1);
  MeasZ(BellA, AliceBit2);
}

// Corrections are invoked from the top level, so they must be wrapped in
// individual quantum_kernels
quantum_kernel void xBob() {
  X(BellB);
}

quantum_kernel void zBob() {
  Z(BellB);
}

// Alternatively, push the corrections into a quantum kernel with CX and CZ
// operations.
quantum_kernel void bobCorrections() {
  CNOT(BellA, BellB);
  CZ(AliceState, BellB);
}

quantum_kernel void teleportCircuit() {
  alicePrepState();
  aliceProtocol();
  measAlice();
  bobCorrections();
}

int main() {
  using namespace iqsdk;

  // Set up the simulator
  IqsConfig iqs_config(/*num_qubits*/ 3,
                       /*simulation_type*/ "noiseless");
  FullStateSimulator iqs_device(iqs_config);
  if (QRT_ERROR_SUCCESS != iqs_device.ready()) {
    std::printf("Device not ready\n");
    return 1;
  }

  // Structures for reporting probabilities and amplitudes.
  std::vector<std::reference_wrapper<qbit>> qids = {
      std::ref(AliceState), std::ref(BellA), std::ref(BellB)};
  QssMap<double> probability_map;
  QssMap<std::complex<double>> amplitude_map;

  // Alice preps her state
  alicePrepState();

  std::cout << "\n==Alice's state before teleportation==\n";
  // Only display non-zero amplitudes above 0.01
  amplitude_map = iqs_device.getAmplitudes(qids, {}, 0.01);
  FullStateSimulator::displayAmplitudes(amplitude_map);

  std::cout << "\n==Alice's corresponding probability distribution==\n";
  std::vector<std::reference_wrapper<qbit>> alice_qid_ref = {
      std::ref(AliceState)};
  // By specifying only AliceState for the qid argument, we get out the
  // conditional probability of Alice's state being in |0> or |1> respectively
  probability_map = iqs_device.getProbabilities(alice_qid_ref, {});
  FullStateSimulator::displayProbabilities(probability_map);

  aliceProtocol();

  // Before measurement, qubits are in superposition
  std::cout << "\n==Alice's state before measAlice()==\n";
  // Only display non-zero amplitudes above 0.01
  amplitude_map = iqs_device.getAmplitudes(qids, {}, 0.01);
  FullStateSimulator::displayAmplitudes(amplitude_map);

  measAlice();

  std::cout << "\n==Bob's state immediately after measurement==\n";
  // When we look at the state vector, we see that Alice's two qubits
  // have in fact been measured, and have collapsed so there are only two
  // non-zero amplitudes.
  amplitude_map = iqs_device.getAmplitudes(qids, {}, 0.01 /*threshold--only display non-zero amplitudes*/);
  FullStateSimulator::displayAmplitudes(amplitude_map);

  // If we look at the
