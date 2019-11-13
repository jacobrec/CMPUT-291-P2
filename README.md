# CMPUT-291-P2
## Steps
- [] Read XML file
- [] Build Indicies files
- [] Do querying

## Plan
### XML
Use the xml library
Read one element at a time
process and write to file one at a time
close file
throw error messages if file is invalid

### Build Indices
Read the description and figure this part out
Will likely need to integrate with the ffi library

### Do querying
Parse the text to commands
query ffi with commands
try and reuse sqlify display for those nice box drawing characters
