# CMPUT-291-P2
## Steps
- [x] Read XML file
- [x] Build Indicies files
- [x] Parse Commands
- [ ] Do querying

## Plan
### XML
Use the xml library
Read one element at a time
process and write to file one at a time
close file
throw error messages if file is invalid

### Build Indices
Basically some bash stuff to build the indicies, its in phase2.sh

### Do querying
Parse the text to commands. This part is done

query ffi with commands

See here for the c interface
https://docs.oracle.com/cd/E17276_01/html/api_reference/C/frame_main.html

## Assumptions
- All Email rows will have a row id that is greater then or equal to 1
- All dates will be in the range 0000/01/01 to 9999/12/31 inclusive
