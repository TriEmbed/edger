# Linux install script testing scheme
This is in lieu of a test plan that is forthcoming

## Manual test procedures
1. Start with ~/.edger empty and only the que_tools repo with changes unless changesin another repo are also required (only needed until repo unification done).
2.  Connect a hardware flavor and invoke the install script
3. Follow the directions at the end of the install script, if any to complete installation
4. Run Aaardvark in a browser and use the "test" and "browse" buttons to load an FPGA .aap file and confirm correct operation.
5. Go back to 1 and iterate over hardware types (currently "white" board and versions 0.60 and 0.70 of "purple" boards. 
