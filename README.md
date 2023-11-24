# AD2/AD3 Test Program for ECE 212 Circuit 3

This is a program that tests input/outputs using the AD2 for the ECE 212
circuit 3 vending machine project. It uses the DIO pins to feed states/inputs
into the circuit, and reads the next state/outputs and verifies they are 
the same as my expected outputs. The benefit is that all 21 test cases
can be run at a time, making it easier to catch circuit wiring problems.

To run, just plug in the AD2/AD3. With waveforms closed, run the program.
A terminal should pop up that shows which test cases succeeded and failed.

## Build Instructions
To build, make sure that the Waveforms SDK is installed (should be by default).
If not, just re-run the waveforms installer but select the SDK instead of the
program.

The SDK should be available on Windows in: `C:\Program Files(x86)\Digilent\WaveformsSDK`.
In that folder is the reference manual as well.

### Configuring the CMake for non-Windows
The SDK will be installed in different locations on MacOS and Linux, just check
the waveforms website for it's location. Then, update the cmake paths for the 
library file and include directory.

### Building
* CLion:
    * Open the cmake file and build.
* Visual Studio:
    * Create a build folder, in it run the command: `cmake -G"Visual Studio 17 2022" -A x64 ..`
    * Open the solution, set "Circuit3" as the default runtime.
    * Press run and it should build and run the script.