# DTS Parser library
For Commodore Amiga machines running V36 and later OS. 
Provides DTS style hardware configuration files for device and general configuration use.

## Build
Built with SAC/C 6 and smake. Makefile in Src for test code and library build. Makefile in root for supporting header files for library.
TO_DO: VBCC compatible build and alternative make tool outside of SAS/C.
fd2pragma required to build headers. Uses Aminet version https://aminet.net/package/dev/misc/fd2pragma

To build all executable binaries then enter the Src/ dirtory and run
> smake

## Capability
Not everything from all DTS specs will be in this parser due to some custom capabilities not referenced in v0.4 specification.

Some info to note on implementation:
* No DTB is compiled by this implementation. The DTS is parsed and available in memory to query, skipping any conversion to DTB
* Multiline byte arrays and property arrays with comments supported within values 
* Strings interpreted on single lines within quotes
* Property arrays and arrays of u32 values are stored in arrays of Tag Items to handle values which are references
* References to paths are stored as u32 property arrays and therefore Tag Items (singular entry with TAG_END closure)
* Strings are char arrays
* Implementation has a top level node to contain root node, properties and directives/commands. This has no name and no parent node.
* /Include/ directives/commands can be in any node and will load additional file data
* /Include/ files can be called anything. No checks on extensions. This also applies to main DTS file.
* A node with a phandle doesn't need to be defined before it is used in the DTS. The parser will bind references later. Note that missing references could be left hanging (NULL).
* Mixing path and alias/label references, ahead of the actual node with phandle, will create 2 references in memory. Note that both are maintained, but returned ref addresses are different
* Pretty dumb with regards to known property types. Only phandle processed to record a reference
* No formatting checks (see above as parser dumb to known properties) so a value can be set to anything in a property
* No checking for /dts-v1/ directive. User code can still check for it, but parser does not enforce and assumes DTS format
* Parser doesn't treat unrecognised directives/commands as an error. Will continue processing.
* In general the parser isn't very strict and will try to read data as much as possible until it really can't go further. This may create odd results if DTS is not well formatted.
* Overlays with label references or path references supported (similar to Zephyr DTS). Path or node label must exist prior to creating overlay node (cannot forward reference)

## Limitations
* Arithmetic, bitwise or logical expressions in property values (literal string saved as property value but not processed, parser fails if arithmetic in values)
* Max 300 chars for any parsed parameter (changeable in code, but more memory will be used)
* Labels within values (limited purpose and seems useless as not referenced)

Custom DTS capabilities outside of v0.4:
* MACROS are not implemented (no #define, #ifdef, #else, etc)
* #include directives are not used. Use /include/ directive format from v0.4 spec

## Installation
The library is called dts.library and should be installed in LIBS: for general running.
DTS files must be installed to ENVARC: and there must be at least an 'amiga.dts' file in that directory.
There are example DTS files in the DTS/ directory.
> Type smake install from the root build directory to install to ENVARC:

## Running
The first instance to open the library will invoke the parser and create a DTS representation in memory.
If there are any errors then the library will fail to open. Check your files for errors and try again.
There is an test file called 'dtstest' which takes a DTS file as a parameter. Run the following will give output or a line error:
> dtstest ENVARC:amiga.dts

The library will remain in memory with a cache of the DTS. This means that any changes will not be parsed until the library is closed and reinitialised.
In the Amiga OS the library can be closed and released from memory with a call to
>avail flush

All instances of the library must be closed to free (meaning that no processes are using the library). Last resort is a reboot. 

### DTS files
DTS files exist in the DTS/ directory. Copying all to ENVARC: creates an example environment.
DTS entries should be configured to match the actual environment in-use. Examples include a base A500 Rev 6 and A1200 representation. 
All DTS files should overlay from the base A500 version and provide additional hardware. This is done in the 'amiga.dts' which manages this through /include/ directives.
To add or remove a definition then the best practice is to add or remove the /include/ in the 'amiga.dts' and leave the dtsi files as they were in ENVARC. Additional hardware or changes to base hardware can then be added to the 'amiga.dts' directly or added to another dtsi file and included at the end of the include directives. 
Note that all include references must be full paths.

### Library examples
An example file, 'Src/libtest.c' shows how to open the library and run some of the API commands. 
The example covers what most drivers would want to query from the DTS with a compatibility search and address information queried.