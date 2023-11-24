#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <dwf.h>

#define Wait(ts) Sleep((int)(1000*ts))
#define GETBIT(var, bit)	(((var) >> (bit)) & 1)

void test(HDWF hdwf, char state, char in, char expected_state, char expected_out)
{
    // set value on enabled IO pins
    FDwfDigitalIOOutputSet(hdwf, (state << 11) | (in << 8));

    Sleep(10);

    FDwfDigitalIOStatus(hdwf);

    unsigned int dwRead;
    FDwfDigitalIOInputStatus(hdwf, &dwRead);
    printf("%d  %d  %d  %d  %d  | %c  %c  %c  %c  %c | %d%d%d%d%d\n",
        GETBIT(state, 1),
        GETBIT(state, 0),
        GETBIT(in, 2),
        GETBIT(in, 1),
        GETBIT(in, 0),
        (GETBIT(expected_state, 1) == GETBIT(dwRead, 4)) ? '-' : 'F',
        (GETBIT(expected_state, 0) == GETBIT(dwRead, 3)) ? '-' : 'F',
        (GETBIT(expected_out, 2) == GETBIT(dwRead, 2)) ? '-' : 'F',
        (GETBIT(expected_out, 1) == GETBIT(dwRead, 1)) ? '-' : 'F',
        (GETBIT(expected_out, 0) == GETBIT(dwRead, 0)) ? '-' : 'F',
        GETBIT(dwRead, 4),
        GETBIT(dwRead, 3),
        GETBIT(dwRead, 2),
        GETBIT(dwRead, 1),
        GETBIT(dwRead, 0)
	);
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

    // enable output/mask from DIO 8 to 12
    FDwfDigitalIOOutputEnableSet(hdwf, 0x1F00);

    printf("    DIO Out    |     DIO In\n");
    printf("12 11 10 8  7  | 4  3  2  1  0\n");
    printf("S1 S0 X2 X1 X0 | Q1 Q0 D  C1 C0\n");
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
    test(hdwf, 0b11, 0b010, 0b01, 0b100);
    printf("\n");

    test(hdwf, 0b00, 0b011, 0b00, 0b000);
    test(hdwf, 0b01, 0b011, 0b01, 0b000);
    test(hdwf, 0b10, 0b011, 0b00, 0b100);
    test(hdwf, 0b11, 0b011, 0b01, 0b100);
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