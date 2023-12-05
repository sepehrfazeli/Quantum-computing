// Include the IQS header file and the standard C++ libraries
#include <iostream>
#include "qureg.hpp"

using namespace std;
using namespace iqs;

int main()
{
    // Declare the number of qubits and create a quantum register object
    int num_qubits = 3;
    QubitRegister<ComplexDP> psi(num_qubits);

    // Initialize the quantum register to the |000> state
    psi.Initialize("base", 0);

    // Create an entangled pair of qubits using a Hadamard gate and a CNOT gate
    psi.ApplyHadamard(1);
    psi.ApplyCPauliX(1, 2);

    // Apply a CNOT gate and a Hadamard gate to the qubit that you want to teleport
    psi.ApplyCPauliX(0, 1);
    psi.ApplyHadamard(0);

    // Measure the qubits and store the results in classical bits
    int m0 = psi.Measure(0);
    int m1 = psi.Measure(1);

    // Apply conditional gates to the qubit that receives the teleported state based on the measurement results
    if (m1 == 1)
    {
        psi.ApplyPauliX(2);
    }
    if (m0 == 1)
    {
        psi.ApplyPauliZ(2);
    }

    // Print the final state of the quantum register
    cout << "The final state of the quantum register is: " << endl;
    psi.Print();

    return 0;
}
