#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <dwf.h>

#define Wait(ts) Sleep((int)(1000*ts))
#define GETBIT(var, bit)	(((var) >> (bit)) & 1)

// Circuit inputs, AD2 outputs (number corresponds to DIO pin):
const int S1 = 14;
const int S0 = 13;
const int X2 = 10;
const int X1 = 9;
const int X0 = 8;

// Circuit outputs, AD2 inputs (number corresponds to DIO pin):
const int NextS1 = 6;
const int NextS0 = 5;
const int Dispense = 2;
const int Change1 = 1;
const int Change0 = 0;

/* Tests and input/output combination, printing test results to command line
 * ('-' when expected bit matches the read bit, and 'F' when it does not).
 *
 * @param hdwf              AD2/AD3 device handle
 * @param state             Test state:         0b[S1][S0]
 * @param in                Test input:         0b[X2][X1][X0]
 * @param expected_state    Expected output:    0b[NextS1][NextS0]
 * @param expected_out      Expected output:    0b[Dispense][Change1][Change0]
 */
void test(HDWF hdwf, char state, char in, char expected_state, char expected_out)
{
    // Set digital output values
    unsigned int outputMask = 0;
    outputMask |= (GETBIT(state, 1) << S1);
    outputMask |= (GETBIT(state, 0) << S0);
    outputMask |= (GETBIT(in, 2) << X2);
    outputMask |= (GETBIT(in, 1) << X1);
    outputMask |= (GETBIT(in, 0) << X0);
    FDwfDigitalIOOutputSet(hdwf, outputMask);

    // Wait for circuit gates to change (each gate should change in ~17ns so this may not be necessary)
    Sleep(100);

    // Read circuit output (AD2 input)
    FDwfDigitalIOStatus(hdwf);
    unsigned int dwRead;
    FDwfDigitalIOInputStatus(hdwf, &dwRead);

    // Output whether each case succeeded or failed
    // Output order:    S1 S0 X2 X1 X0 | Expected NextS1 NextS0 Dispense Change1 Change0 |
    //                  Actual NextS1 NextS0 Dispense Change1 Change0 |
    //                  Pass/Fail NextS1 NextS0 Dispense Change1 Change0
    printf("%d  %d  %d  %d  %d  | %d  %d  %d  %d  %d  | %d  %d  %d  %d  %d  | %c  %c  %c  %c  %c\n",
        // Test inputs (AD2 outputs)
        GETBIT(state, 1),
        GETBIT(state, 0),
        GETBIT(in, 2),
        GETBIT(in, 1),
        GETBIT(in, 0),

        // Expected output
        GETBIT(expected_state, 1),
        GETBIT(expected_state, 0),
        GETBIT(expected_out, 2),
        GETBIT(expected_out, 1),
        GETBIT(expected_out, 0),

        // Actual output (AD2 inputs)
        GETBIT(dwRead, NextS1),
        GETBIT(dwRead, NextS0),
        GETBIT(dwRead, Dispense),
        GETBIT(dwRead, Change1),
        GETBIT(dwRead, Change0),

        // Tests
        (GETBIT(expected_state, 1) == GETBIT(dwRead, NextS1)) ? '-' : 'F',
        (GETBIT(expected_state, 0) == GETBIT(dwRead, NextS0)) ? '-' : 'F',
        (GETBIT(expected_out, 2) == GETBIT(dwRead, Dispense)) ? '-' : 'F',
        (GETBIT(expected_out, 1) == GETBIT(dwRead, Change1)) ? '-' : 'F',
        (GETBIT(expected_out, 0) == GETBIT(dwRead, Change0)) ? '-' : 'F'
	);
    Sleep(100);
}

int main()
{
    HDWF hdwf;
    STS sts;
    unsigned int dwRead;
    char szError[512] = {0};

    printf("Open automatically the first available device\n");
    if(!FDwfDeviceOpen(-1, &hdwf)) {
        FDwfGetLastErrorMsg(szError);
        printf("Device open failed\n\t%s", szError);
        return 0;
    }

    // DIO enable output mask
    unsigned int outputEnableMask = 0;
    outputEnableMask |= (1 << S1);
    outputEnableMask |= (1 << S0);
    outputEnableMask |= (1 << X2);
    outputEnableMask |= (1 << X1);
    outputEnableMask |= (1 << X0);
    FDwfDigitalIOOutputEnableSet(hdwf, outputEnableMask);

    printf("   Test Input  |     Expected   |      Actual    |    Pass/Fail  \n");
    printf("S1 S0 X2 X1 X0 | Q1 Q0 D  C1 C0 | Q1 Q0 D  C1 C0 | Q1 Q0 D  C1 C0\n");
    test(hdwf, 0b00, 0b000, 0b00, 0b000);
    test(hdwf, 0b01, 0b000, 0b01, 0b000);
    test(hdwf, 0b10, 0b000, 0b10, 0b000);
    test(hdwf, 0b11, 0b000, 0b11, 0b000);
    printf("\n");

    test(hdwf, 0b00, 0b001, 0b01, 0b000);
    test(hdwf, 0b01, 0b001, 0b10, 0b000);
    test(hdwf, 0b10, 0b001, 0b11, 0b000);
    test(hdwf, 0b11, 0b001, 0b11, 0b001);
    printf("\n");

    test(hdwf, 0b00, 0b010, 0b00, 0b000);
    test(hdwf, 0b01, 0b010, 0b01, 0b000);
    test(hdwf, 0b10, 0b010, 0b00, 0b100);
    test(hdwf, 0b11, 0b010, 0b00, 0b101);
    printf("\n");

    test(hdwf, 0b00, 0b011, 0b00, 0b000);
    test(hdwf, 0b01, 0b011, 0b01, 0b000);
    test(hdwf, 0b10, 0b011, 0b00, 0b100);
    test(hdwf, 0b11, 0b011, 0b00, 0b101);
    printf("\n");

    test(hdwf, 0b00, 0b100, 0b00, 0b000);
    test(hdwf, 0b01, 0b100, 0b01, 0b000);
    test(hdwf, 0b10, 0b100, 0b10, 0b000);
    test(hdwf, 0b11, 0b100, 0b00, 0b100);
    printf("\n");

    test(hdwf, 0b00, 0b101, 0b00, 0b000);
    test(hdwf, 0b01, 0b101, 0b01, 0b000);
    test(hdwf, 0b10, 0b101, 0b10, 0b000);
    test(hdwf, 0b11, 0b101, 0b00, 0b100);
    printf("\n");

    test(hdwf, 0b00, 0b111, 0b00, 0b000);
    test(hdwf, 0b01, 0b111, 0b00, 0b001);
    test(hdwf, 0b10, 0b111, 0b00, 0b010);
    test(hdwf, 0b11, 0b111, 0b00, 0b011);
    printf("\n");

    FDwfDeviceClose(hdwf);
}