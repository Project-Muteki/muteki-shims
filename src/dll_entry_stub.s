.section .text

@ DLL entry point on Besta seems to be always ignored. Return nothing immediately to satisfy ld.
.global DllMainCRTStartup
DllMainCRTStartup:
    bx lr
