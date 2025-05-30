# DTS Parser library
For Commodore Amiga machines running V36 and later OS. 
Provides DTS style hardware configuration files for device and general configuration use.

## Build
Built with SAC/C 6 and smake. Makefile in Src for test code and library build. Makefile in root for supporting header files for library.
TO_DO: VBCC compatible build and alternative make tool outside of SAS/C.
fd2pragma required to build headers. Uses Aminet version https://aminet.net/package/dev/misc/fd2pragma

## Capability
Not everything from all DTS specs will be in this parser due to some custom capabilities not referenced in v0.4 specification.

Some info to note on implementation:
* Multiline byte arrays and property arrays with comments supported within values 
* Strings interpreted on single lines within quotes
* Property arrays and arrays of u32 values are stored in arrays of Tag Items to handle values which are references
* Strings are straight-forward char arrays
* Implementation has a top level node to contain root node, properties and directives/commands
* /Include/ directives/commands can be in any node and will load additional file data
* /Include/ files can be called anything. No checks on extensions. This also applies to main DTS file.
* A node with a phandle doesn't need to be defined before it is used in the DTS. The parser will bind references later. Note that missing references could be left hanging (NULL).
* Pretty dumb with regards to known property types. Only phandle processed to record a reference
* No formatting checks (see above as parser dumb to known properties) so a value can be anything
* No checking for /dts-v1/ directive. Can be added to DTS and it will be stored for processing. User can decide to enforce or check for it.
* Parser doesn't treat unrecognised directives/commands as an error. Will continue processing.
* In general the parser isn't very strict and will try to read data as much as possible until it really can't go further. This may create odd results if DTS is not well formatted.

## Limitations
* No overlay support
* Deletion of property (are recorded against node but no action taken)
* Deletion of node (are recorded against node but no action taken)
* Arithmetic, bitwise or logical expressions in property values (literal string saved as property value but not processed, parser fails if in values)
* Max 300 chars for any parsed parameter (changeable in code)
* Aliases expanding to paths (saved as undefined strings),
* Labels within values (limited purpose and seems useless as not referenced)
* References with paths e.g. < &{/soc/interrupt-controller@40000} > are not supported. This will be just parsed as a reference name
* Expansion of references to full paths are not supported. Saved as unknown value so could still be looked up by client.

Custom DTS capabilities outside of 0.4:--
* MACROS are not implemented (no #define, #ifdef, #else, etc)
* #include directives are not used. Use /include/ directive format from v0.4 spec