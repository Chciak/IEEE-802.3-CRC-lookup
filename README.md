# CRC calculation for IEEE 802.3 ethernet using crc tables

Project implements Ethernet CRC generation and check, with use of lookup tables for speed.

## Description
- File **dopiszCRC** creates a lookup table based on polynomial represented as `unsigned int wielomian` and generates the desired code.
- File **sprawdzCRC** checks whether the calculated value is correct.
