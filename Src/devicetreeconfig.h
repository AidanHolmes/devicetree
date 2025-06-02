#ifndef __DEVICETREECONFIG_HDR
#define __DEVICETREECONFIG_HDR

#include <exec/types.h>
#include <dos/dos.h>
#include <utility/tagitem.h>

#define DT_MAX_NODE_LABEL    	31 + 1
#define DT_MAX_NODE_NAME    	50
#define DT_MAX_NODE_ADDRESS  	50
#define DT_MAX_PROPERTY_NAME 	31 + 1
#define DT_MAX_COMMAND_NAME    	50
#define DT_MAX_COMMAND_VALUE   	50
//#define DT_MAX_TEMP_STR 		DT_MAX_NODE_NAME + DT_MAX_NODE_ADDRESS + 2
#define DT_MAX_TEMP_STR			300
#define DT_MAX_REFERENCE		300

#define DT_OBJECT_PROPERTY	 	1
#define DT_OBJECT_COMMAND		2

#define DT_VALUE_UNDEFINED		0
#define DT_VALUE_ULONG_ARRAY	1
#define DT_VALUE_STRING			2
#define DT_VALUE_BYTE_STRING	3
#define DT_VALUE_LOGIC			4

#define DT_RETURN_NOERROR			0
#define DT_RETURN_NOMEM				1
#define DT_RETURN_FILE_ERROR		2
#define DT_RETURN_PARAM_ERROR		3
#define DT_RETURN_REPLAY			4
#define DT_RETURN_SYSERR			5

#define DT_ENCODED_VALUE_U32		TAG_USER+0
#define DT_ENCODED_VALUE_REFERENCE 	TAG_USER+1

struct devicetreeObject;
struct devicetreeValue;
struct devicetreeProperty;
struct devicetreeNode;

struct devicetreeObject
{
	struct devicetreeNode *node;
	struct devicetreeObject *next;
	struct devicetreeObject *prev;
	UWORD type;
};

struct devicetreeValue
{
	struct devicetreeProperty *property;
	struct devicetreeValue *next;
	struct devicetreeValue *prev;
	void *value;
	ULONG size ;
	UWORD type;
};

struct devicetreeEncodedArrayValue // effectively a tag item
{
	ULONG flags;
	ULONG value; // could be a value or reference memory address
};

struct devicetreeReference
{
	struct devicetreeReference *next;
	struct devicetreeReference *prev;
	char strPath[DT_MAX_REFERENCE];
	char referenceName[DT_MAX_NODE_LABEL]; // Although node holds the label name this is needed when node not yet found and may be orphan ref
	struct devicetreeNode *node;
	BOOL phandleValid;
	ULONG phandleRef;
};

struct devicetreeProperty
{
	struct devicetreeObject _obj;
	char name[DT_MAX_PROPERTY_NAME];
	char label[DT_MAX_NODE_LABEL];
	struct devicetreeValue *values;
};

struct devicetreeCommand
{
	struct devicetreeObject _obj;
	char name[DT_MAX_COMMAND_NAME];
	char value[DT_MAX_COMMAND_VALUE];
};

struct devicetreeNode{
	UWORD id;
	char label[DT_MAX_NODE_LABEL];
	char name[DT_MAX_NODE_NAME];
	char unitAddress[DT_MAX_NODE_ADDRESS];
	struct devicetreeNode *parent;
	struct devicetreeNode *child;
	struct devicetreeNode *next;
	struct devicetreeNode *prev;
	struct devicetreeObject *firstObject;
};

typedef UWORD (*dt_object_callback)(struct devicetreeObject *obj) ;

enum enConfigState {dtconfigStateNode, dtconfigStateProperty, dtconfigStateCommand, dtconfigStateComment, dtconfigStateCommentBlock};
enum enPropertyState {dtpropUnknown, dtpropLogic, dtpropArray, dtpropByteString, dtpropText, dtpropReference};
enum enCommentState {dtcommentNode, dtcommentVar};
enum enCommandState {dtcmdName, dtcmdValue};

struct devicetreeStream;

struct devicetreeStream{
	struct devicetreeStream *next;
	struct devicetreeStream *prev;
	BOOL configOpened;
	BPTR f;
	UBYTE *buffer;
	ULONG streamLen;
	ULONG offset;
	ULONG atLine;
};

struct devicetreeConfig{
	struct Library *dos;
	struct devicetreeStream *stream;	// Multiple streams open depending on directive
	struct devicetreeNode topNode;		// Root node
	// config state variables
	dt_object_callback fn;
	enum enConfigState state;
	enum enPropertyState propertystate ;
	enum enCommandState cmdstate;
	enum enCommentState commentstate;
	char temp[DT_MAX_TEMP_STR];
	UWORD tempIndex;
	char lastchar;
	char label[DT_MAX_NODE_LABEL];
	struct devicetreeReference *refTop; //Store all known references in this list
	struct devicetreeNode *currentNode;
	struct devicetreeObject *currentObject;
	struct devicetreeStream *currentStream;
	struct devicetreeValue *currentValue ;
	UWORD lastid;
	BOOL escaping ;
	BOOL quoting ;
};

// Initialise a config object for the DTS parser. Call this before you do anything else
// Returns zero on success, otherwise an error occurred
UWORD dtInitialise(struct devicetreeConfig *config);

// Include a file or memory buffer to setup the parser. One of these must be called at least once to prime the parser.
// File streams must be opened prior to making the dtOpenFile call. 
// Note that these stack with previously opened files or buffers and the parser will unwind all streams
// Can fail if not enough memory available to open a new stream.
UWORD dtOpenFile(struct devicetreeConfig *config, BPTR f, ULONG len);
UWORD dtOpenBuffer(struct devicetreeConfig *config, UBYTE *buffer, ULONG len);

// Parse all configuration in a DTS. The device tree config will be populated with all config information
// Any parse errors are returned by this function. fn_callback is optional and will trigger for any discovered object in the file
UWORD dtParseConfig(struct devicetreeConfig *config, dt_object_callback fn_callback);

// Close the parser and release resources
void dtClose(struct devicetreeConfig *config);

// Iterate to last object held in a node, object, value or stream list.
struct devicetreeObject *getLastObject(struct devicetreeNode *n);
struct devicetreeNode *getLastChildNode(struct devicetreeNode *n);
struct devicetreeNode *getLastSiblingNode(struct devicetreeNode *n);
struct devicetreeValue *getLastValue(struct devicetreeProperty *prop);
struct devicetreeStream *getLastStream(struct devicetreeConfig *config);

// string copy with length restrictions
void dtStrCpy(char *to, char *from, UWORD maxbuf);

// Compare strings and ignore case
BOOL dtStriCmp(char *a, char *b, UWORD max);

// Report last line number processed in current stream. Useful when errors happen
ULONG lastLineNumber(struct devicetreeConfig *config);

// Fetches the node using a node path. Addresses can be in the path. If no address provided in path then first match 
// will be returned.
// Returns NULL if no node found
struct devicetreeNode* getNode(struct devicetreeConfig *config, char *path);

// Find first matching child in a node-path or continue to search for next matching child node in node-path
// This only searches immediate children of the node specified by path
// fromNode - set to last discovered node or NULL for first search
// path - path to parent node to start iteration
// compatible - optional string to filter found nodes by matching compatible value
struct devicetreeNode* iterateChildNodes(struct devicetreeConfig *config, struct devicetreeNode *fromNode, char *path, char *compatible);

// Search for a property in a node using the name prameter. Case insensitive.
// Returns NULL if not found
struct devicetreeProperty* getProperty(struct devicetreeConfig *config, struct devicetreeNode *node, char *name);

// Get properties belonging to a node.
// Returns first property if lastProperty is NULL
// If lastProperty is set with the last result, then the function will return the next property or NULL if no more found.
struct devicetreeProperty* iterateProperty(struct devicetreeConfig *config, struct devicetreeNode *node, struct devicetreeProperty *lastProperty);


#endif