/* Gate definitions and key words */
#include <clang/Quantum/quintrinsics.h>
/* Quantum Runtime Library APIs */
#include <quantum.hpp>
#include <iostream>
/* Declare 2 qubits */
qbit q[2];
/* The quantum logic must be in a function with the keyword quantum_kernel */
/* pre-pended to the signature */
quantum_kernel void prep_and_meas_bell(cbit read_out) {
/* Prepare both qubits in the |0> state */
PrepZ(q[0]);
PrepZ(q[1]);
/* Apply a Hadamard gate to the top qubit */
H(q[0]);
/* Apply a Controlled-NOT gate with the top qubit as
* the control and the bottom qubit as the target */
CNOT(q[0], q[1]);
/* Measure qubit 0 */
MeasZ(q[0], read_out);
}
int main() {
/* Configure the setting of the simulator. */
iqsdk::IqsConfig settings(2, "noiseless");
iqsdk::FullStateSimulator quantum_8086(settings);
if (iqsdk::QRT_ERROR_SUCCESS != quantum_8086.ready()) return 1;
/* Declare 2 measurement readouts */
/* Measurements are stored here as "classical bits" */
cbit classical_bit;
prep_and_meas_bell(classical_bit);
/* Here we can use the FullStateSimulator APIs to get data */
/* or we can write classical logic that interacts with our measurement */
/* results, as below. */
bool result = classical_bit;
if (result) {
std::cout << "True\n";
}
else {
std::cout << "False\n";
}
return 0;
}
