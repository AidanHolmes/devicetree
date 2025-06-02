#include "devicetreeconfig.h"
#include <string.h>
#include <proto/exec.h>
#include <exec/exec.h>
#include <dos/dos.h>
#include <proto/exec.h>
#include <proto/dos.h>

#define DOSBase config->dos

/*inline*/ static void _pushInChar(char *str, char c, UWORD *index, ULONG max)
{
	if (*index < max-1){
		str[*index] = c;
		*index +=1;
		str[*index] = '\0'; // ensure terminated
	}
}

/*inline*/ static void _popOutChar(char *str, UWORD *index)
{
	if (*index > 0){
		*index -= 1;
		str[*index] = '\0';
	}
}

static UWORD _getNodePath(struct devicetreeNode *node, char *strPath, UWORD maxLength)
{
	// Paths can be long so they will be allocated in memory and returned via strPath
	UWORD length = 0, i=0, templen = 0;
	struct devicetreeNode *n;
	char *p = NULL;
	
	// 2 passes to determine length and then write path
	for (i=0;i<2;i++){
		for (n=node;n->parent;n=n->parent){
			if (!(n->name[0] == '/' && n->name[1] == '\0')){
				if (n->unitAddress[0] != '\0'){
					templen = strlen(n->unitAddress); 
					if (i==1){
						p -= templen +1;// add 1 for @
						*p = '@';
						memcpy(p+1, n->unitAddress, templen);
					}
					length += templen+1;
				}
				templen = strlen(n->name);
				if (i==1){
					p -= templen;
					memcpy(p, n->name, templen);
				}
				length += templen;
				
				
				if (i==1){
					p -= 1;
					*p = '/';
				}
				length += 1; // add for path separator
			}
		}
		if (i==0){
			if (length > maxLength-1){
				return DT_RETURN_PARAM_ERROR;
			}
			p=strPath+length;
			*p = '\0';
		}
	}

	return DT_RETURN_NOERROR;
}

static struct devicetreeReference* _getReference(struct devicetreeConfig *config, char *name, char *path, ULONG *phandle)
{
	// Searches by name, path or phandle or any of these if they are set. If name or path is empty or NULL then will not be used for search
	// If no reference found then returns NULL.
	struct devicetreeReference *r ;
	struct devicetreeNode *n;
	char *p = NULL;
	BOOL braces = FALSE;
	
	for	(r=config->refTop; r; r=r->next){
		if (name && name[0] != '\0'){
			if (dtStriCmp(name, r->referenceName, DT_MAX_NODE_LABEL)){
				return r; // found the node requested
			}
			
		}
		if (path && path[0] != '\0'){
			// If braces then skip first character and terminate at closing brace
			if (path[0] == '{'){
				for (p=path; *p; p++){
					if (*p == '}'){
						*p='\0';
						path += 1;
						braces = TRUE ;
						break;
					}
				}
			}
			// Compare path string to cached path in reference list or get node
			// from device tree and compare that node.
			if (dtStriCmp(path, r->strPath, DT_MAX_REFERENCE)){
				return r;
			}
			if ((n=getNode(config, path))){
				if (r->node == n){
					return r;
				}
			}
			if (braces){
				*p = '}';
			}
			
		}
		if (phandle){
			// no name given, so search by phandle value
			if (*phandle == r->phandleRef){
				return r; // found node by phandle
			}
		}
	}
	
	return r;
}

static struct devicetreeReference* _createReference(struct devicetreeConfig *config, char *name, char *path, ULONG *phandle)
{
	// No duplicate check performed in this call. Use _getReference to find an existing record and only
	// use this function if the reference doesn't exist.
	struct devicetreeReference *r, *attachTo ;
	char *p = NULL;
	BOOL braces = FALSE ;
	
	r = AllocVec(sizeof(struct devicetreeReference), MEMF_ANY | MEMF_CLEAR);
	if (r){
		// Add ref name if not null and not empty or add a phandle if no name
		// Any entry by name is likely a forward reference and empty phandle, but could be a node with phandle property and label
		if (name && name[0] != '\0'){
			dtStrCpy(r->referenceName, name, DT_MAX_NODE_LABEL);
		}
		if (path && path[0] != '\0'){
			// If braces then skip first character and terminate at closing brace
			if (path[0] == '{'){
				for (p=path; *p; p++){
					if (*p == '}'){
						*p='\0';
						path += 1;
						braces = TRUE ;
						break;
					}
				}
			}
			// Copy path to reference
			dtStrCpy(r->strPath, path, DT_MAX_REFERENCE);
			r->node=getNode(config, path); // could be null if forward ref
			if (braces){
				*p = '}';
			}
			
		}
		if (phandle){
			// Set the phandle if specified in parameters
			r->phandleRef = *phandle;
			r->phandleValid = TRUE;
		}
		
		if (!config->refTop){
			config->refTop = r;
		}else{
			// Find last reference in linked list and add to end
			for(attachTo=config->refTop; attachTo->next != NULL; attachTo=attachTo->next);
			attachTo->next = r;
			r->prev = attachTo ;
		}
	}

	return r;
}

static struct devicetreeStream* _addStream(struct devicetreeConfig *config)
{
	struct devicetreeStream *p = NULL, *attachTo = NULL;
	
	p = AllocVec(sizeof(struct devicetreeStream), MEMF_ANY | MEMF_CLEAR);
	if (p){
		if (config->stream){
			// Find last stream in linked list and add new stream
			for(attachTo=config->stream; attachTo->next != NULL; attachTo=attachTo->next);
			attachTo->next = p;
			p->prev = attachTo ;
		}else{
			// Allocate initial stream
			config->stream = p;
		}
	}
	return p;
}

static UWORD _handleCommand(struct devicetreeConfig *config, struct devicetreeCommand *cmd)
{
	UWORD ret ;
	BPTR f;

	if (dtStriCmp("include", cmd->name, DT_MAX_COMMAND_VALUE)){
		if(!(f = Open(cmd->value, MODE_OLDFILE))){
			return DT_RETURN_PARAM_ERROR;
		}
		if ((ret=dtOpenFile(config, f, 0)) != DT_RETURN_NOERROR){
			return ret;
		}
		config->currentStream->configOpened = TRUE ; // This was opened automatically. Needs closing by this code
		
	}
	return DT_RETURN_NOERROR;
}

static void _linkToNode(struct devicetreeNode *parent, struct devicetreeObject *obj)
{
	struct devicetreeObject *attachTo = NULL ;
	
	attachTo = getLastObject(parent);
	obj->prev = attachTo ; // could be null if first.
	if (attachTo){
		attachTo->next = obj;
	}else{
		// first object in section
		parent->firstObject = obj;
	}
	obj->node = parent ;
}

static struct devicetreeProperty *_createProperty(struct devicetreeConfig *config, struct devicetreeNode *parent)
{
	struct devicetreeProperty *newProperty = NULL;
		
	newProperty = (struct devicetreeProperty *)AllocVec(sizeof(struct devicetreeProperty), MEMF_ANY | MEMF_CLEAR);
	if (!newProperty){
		return NULL;
	}
	newProperty->_obj.type = DT_OBJECT_PROPERTY;
	_linkToNode(parent, (struct devicetreeObject*)newProperty);
	
	return newProperty;
}

static struct devicetreeCommand *_createCommand(struct devicetreeConfig *config, struct devicetreeNode *parent)
{
	struct devicetreeCommand *newCmd= NULL;
		
	newCmd = (struct devicetreeCommand *)AllocVec(sizeof(struct devicetreeCommand), MEMF_ANY | MEMF_CLEAR);
	if (!newCmd){
		return NULL;
	}
	newCmd->_obj.type = DT_OBJECT_COMMAND;
	_linkToNode(parent, (struct devicetreeObject*)newCmd);
	
	return newCmd;
}

static struct devicetreeNode *_createNode(struct devicetreeConfig *config, struct devicetreeNode *parent)
{
	struct devicetreeNode *newNode = NULL, *attachTo = NULL;
		
	newNode = (struct devicetreeNode *)AllocVec(sizeof(struct devicetreeNode), MEMF_ANY | MEMF_CLEAR);
	if (!newNode){
		return NULL;
	}
	if (!parent->child){
		parent->child = newNode ;
	}else{
		attachTo = getLastSiblingNode(parent);
		attachTo->next = newNode;
		newNode->prev = attachTo ;
	}
	
	newNode->parent = parent;
	
	return newNode;
}

static struct devicetreeValue *_createValue(struct devicetreeConfig *config, struct devicetreeProperty *property)
{
	struct devicetreeValue *newValue = NULL, *attachTo = NULL;
		
	newValue = (struct devicetreeValue *)AllocVec(sizeof(struct devicetreeValue), MEMF_ANY | MEMF_CLEAR);
	if (!newValue){
		return NULL;
	}
	newValue->type = DT_VALUE_UNDEFINED;
	newValue->property = property;
	if (!property->values){
		property->values = newValue ;
	}else{
		attachTo = getLastValue(property);
		attachTo->next = newValue;
		newValue->prev = attachTo ;
	}
	
	return newValue;
}
static UWORD _strToBytes(char *str, struct devicetreeValue *value)
{
	char *p = NULL;
	UBYTE val = 0, multi = 0, *array = NULL;
	UWORD elements = 0, run = 0;
	
	for (run = 0; run < 2; run++){
		elements = 0 ;
		val = 0;
		multi = 0;
		for (p=str; *p; p++){
			switch(*p){
				case ' ':
				case '\t':
				case '\r':
				case '\n':
					break;
				default:
					if (*p >= 'A' && *p <= 'F'){
						*p += 32; // convert to lower
					}
					if (*p >= '0' && *p <= '9'){
						val = (val << multi) + *p - '0';
					}else if ((*p >= 'a' && *p <= 'f')){
						val = (val << multi) + *p - 'a' + 10;
					}else{
						// unexpected char in stream
						return DT_RETURN_PARAM_ERROR;
					}
					if (multi){
						if (run == 1){
							array[elements] = val ;
							val = 0;
						}
						elements++;
						multi = 0;
					}else{
						multi = 4;
					}
			}
		}
		
		if (run == 0){
			value->size = elements;
			array = (UBYTE*)AllocVec(value->size, MEMF_ANY | MEMF_CLEAR);
			if (!array){
				value->size = 0;
				return DT_RETURN_NOERROR;
			}
			value->value = array ;
		}
	}
	
	return DT_RETURN_NOERROR;
}

static UWORD _handleKnownPropertyEncodedValue(struct devicetreeConfig *config, struct devicetreeProperty *property, UWORD index, ULONG val)
{
	struct devicetreeReference *ref=NULL, *ref2=NULL;
	char path[DT_MAX_REFERENCE] ;

	path[0] = '\0';
	
	if (dtStriCmp(property->name, "phandle", DT_MAX_PROPERTY_NAME)){
		if (index == 0){
			_getNodePath(property->_obj.node, path, DT_MAX_REFERENCE);
			if ((ref = _getReference(config, property->_obj.node->label, path, &val))){
				if (ref->phandleValid && ref->phandleRef == val){
					// Reference already exists.
					return DT_RETURN_PARAM_ERROR;
				}
			}
			if (!ref){ // Reference hasn't been created yet
				if(!(ref = _createReference(config, property->_obj.node->label, path, &val))){
					// no memory
					return DT_RETURN_NOMEM;
				}
			}
			
			// Set the node in reference list
			ref->phandleValid = TRUE ;
			ref->phandleRef = val ;
			ref->node = property->_obj.node;
			if (path[0]){
				// If we have a 2nd forward copy with the path reference then populate again - this is quite stupid
				if ((ref2 = _getReference(config, NULL, path, NULL))){
					if (ref != ref2){
						if (!ref2->node){
							ref2->node = ref->node;
						}
						if (!ref2->phandleValid){
							ref2->phandleValid = ref->phandleValid;
							ref2->phandleRef = ref->phandleRef;
						}
					}
				}
				dtStrCpy(ref->strPath, path, DT_MAX_REFERENCE);
			}
		}
	}
	return DT_RETURN_NOERROR;
}

static UWORD _addToEncodedArray(struct devicetreeConfig *config, struct devicetreeEncodedArrayValue *encval, UWORD index,  BOOL isRef, ULONG val, char *refName)
{
	struct devicetreeReference *ref=NULL;
	struct devicetreeProperty *p=NULL;
	UWORD ret = 0;
	char *path = NULL; 
	
	// Convert to property object
	if (config->currentObject->type == DT_OBJECT_PROPERTY){
		p = (struct devicetreeProperty*)config->currentObject ;
	}else{
		return DT_RETURN_PARAM_ERROR;
	}
	
	if (isRef){
		// Not a value, but a reference to a node
		if (refName[0] == '{'){
			path = refName;
			refName = NULL;
		}
		if (!(ref = _getReference(config, refName, path, NULL))){
			if (!(ref = _createReference(config, refName, path, NULL))){ // create forward reference
				return DT_RETURN_NOMEM;
			}
		}
		encval[index].flags = DT_ENCODED_VALUE_REFERENCE;
		encval[index].value = (ULONG)ref;
	}else{
		if ((ret=_handleKnownPropertyEncodedValue(config,p,index,val)) != DT_RETURN_NOERROR){
			return ret ;
		}
		encval[index].flags = DT_ENCODED_VALUE_U32;
		encval[index].value = val;
	}
	
	return DT_RETURN_NOERROR;
}

static UWORD _strToArray(struct devicetreeConfig *config, char *str, struct devicetreeValue *value)
{
	char *p = NULL, tempRef[DT_MAX_REFERENCE];
	BOOL readingValue = FALSE, isHex = FALSE, readingRef = FALSE;
	ULONG val = 0;
	struct devicetreeEncodedArrayValue *array = NULL;
	UWORD elements = 0, run = 0, tempRefIndex = 0, ret=0;
	
	for (run = 0; run < 2; run++){
		elements = 0 ;
		readingValue = FALSE ;
		readingRef = FALSE ;
		isHex = FALSE ;
		val =0;
		for (p=str; *p; p++){
			switch(*p){
				case '\r':
					break;
				case '\n':
				case ' ':
				case '\t':
					if (readingValue){
						if (run == 1){
							if ((ret=_addToEncodedArray(config, array, elements, readingRef, val, tempRef)) != DT_RETURN_NOERROR){
								return ret ;
							}
						}
						elements++ ;
						readingValue = FALSE ;
						readingRef = FALSE ;
					}
					val = 0;
					isHex = FALSE ;
					break;
				case '&':
					// reference
					readingRef = TRUE ;
					tempRefIndex = 0;
					break;
				default:
					readingValue = TRUE ;
					if (readingRef){
						_pushInChar(tempRef, *p, &tempRefIndex, DT_MAX_NODE_LABEL);
					}else{
						if (*p >= 'A' && *p <= 'F'){
							*p += 32; // convert to lower
						}
						if (*p >= '0' && *p <= '9'){
							if (isHex){
								val = (val * 16) + *p-48;
							}else{
								val = (val * 10) + *p-48;
							}
						}else if ((*p >= 'a' && *p <= 'f') && isHex){
							val = (val * 16) + *p-87;
						}else{
							if ((*p == 'x' || *p == 'X') && p != str && *(p-1) == '0'){
								isHex = TRUE ;
							}else{
								// unexpected char in stream
								return DT_RETURN_PARAM_ERROR;
							}
						}
					}
			}
		}
		if (readingValue){
			if (run == 1){
				if ((ret=_addToEncodedArray(config, array, elements,readingRef, val, tempRef)) != DT_RETURN_NOERROR){
					return ret ;
				}
			}
			elements++ ;
		}
		
		if (run == 0){
			value->size = elements+1;
			array = (struct devicetreeEncodedArrayValue*)AllocVec(value->size * sizeof(struct devicetreeEncodedArrayValue), MEMF_ANY | MEMF_CLEAR);
			if (!array){
				value->size = 0;
				return DT_RETURN_NOMEM;
			}
			value->value = array ;
			array[elements].flags = TAG_END;
		}
	}
	
	return DT_RETURN_NOERROR;
}

static UWORD _parseProperty(struct devicetreeConfig *config, char c)
{
	struct devicetreeValue *value = NULL;
	struct devicetreeProperty *prop = NULL;
	struct devicetreeReference *r = NULL;
	UWORD ret ;
	
	prop = (struct devicetreeProperty*)config->currentObject;
	
	switch(config->propertystate){
		case dtpropUnknown:
			switch(c){
				case '<':
					// Array
					if (config->tempIndex == 0){
						config->propertystate = dtpropArray;
						if (!(value =_createValue(config, prop))){
							return DT_RETURN_NOMEM;
						}
						value->type = DT_VALUE_ULONG_ARRAY;
						config->currentValue = value ;
					}
					break;
				case '"':
					// Text and quoting
					if (config->tempIndex == 0){
						config->propertystate = dtpropText;
						config->quoting = TRUE ;
						if (!(value =_createValue(config, prop))){
							return DT_RETURN_NOMEM;
						}
						value->type = DT_VALUE_STRING;
						config->currentValue = value ;
					}
					break;
				case '[':
					// Byte string
					if (config->tempIndex == 0){
						config->propertystate = dtpropByteString;
						if (!(value =_createValue(config, prop))){
							return DT_RETURN_NOMEM;
						}
						value->type = DT_VALUE_BYTE_STRING;
						config->currentValue = value ;
					}
					break;
				case '&':
					// Reference to label
					if (config->tempIndex == 0){ // check if text already added and if not then is a reference
						config->propertystate = dtpropReference;
						if (!(value =_createValue(config, prop))){
							return DT_RETURN_NOMEM;
						}
						value->type = DT_VALUE_ULONG_ARRAY; // reference in ULONG tag array
						config->currentValue = value ;
					}
					break;
				case ',':
					// new value separator
					if (config->tempIndex > 0){
						// We've captured some text already, assume logic
						if (!(value =_createValue(config, prop))){
							return DT_RETURN_NOMEM;
						}
						value->type = DT_VALUE_LOGIC;
						value->size = strlen(config->temp)+1;
						value->value = AllocVec(value->size, MEMF_ANY);
						if (!value->value){
							return DT_RETURN_NOMEM;
						}
						dtStrCpy(value->value, config->temp, DT_MAX_TEMP_STR);

						config->tempIndex = 0;
					}
					break;
				case ';':
					if (!config->quoting){
						if (config->fn){
							if ((ret=config->fn(config->currentObject)) != DT_RETURN_NOERROR){
								return ret;
							}
						}
						config->state = dtconfigStateNode;
					}
					break;
				case ':':
					// Label
					if (config->tempIndex > 0){
						// Do nothing with these at the moment. Not sure how to store
						config->tempIndex = 0;
					}
					break;
				case ' ':
				case '\t':
				case '\r':
				case '\n':
					break ; // ignore
				default:
					// other text - is this a label or calculation?
					_pushInChar(config->temp, c, &config->tempIndex, DT_MAX_TEMP_STR);
			}
			break;
		case dtpropArray:
			if (c == '>'){
				config->propertystate = dtpropUnknown;
			
				if ((ret=_strToArray(config, config->temp, config->currentValue)) != DT_RETURN_NOERROR){
					return ret;
				}
				config->tempIndex = 0;
			}else{
				if(config->lastchar == '/'){
					if (c == '/' || c == '*'){
						if (c == '*'){
							config->state = dtconfigStateCommentBlock;
						}else if(c == '/'){
							config->state = dtconfigStateComment;
						}
						config->commentstate = dtcommentVar;
					}
				}
				if (config->state == dtconfigStateProperty){
					_pushInChar(config->temp, c, &config->tempIndex, DT_MAX_TEMP_STR);
				}else{
					_popOutChar(config->temp, &config->tempIndex);
				}
			}
			break;
		case dtpropText:
			if (c == '"'){
				config->propertystate = dtpropUnknown;
				config->quoting = FALSE ;
				config->currentValue->size = strlen(config->temp)+1;
				config->currentValue->value = AllocVec(config->currentValue->size, MEMF_ANY);
				if (!config->currentValue->value){
					return DT_RETURN_NOMEM;
				}
				dtStrCpy(config->currentValue->value, config->temp, config->currentValue->size);
				
				config->tempIndex = 0;
			}else{
				_pushInChar(config->temp, c, &config->tempIndex, DT_MAX_TEMP_STR);
			}
			break;
		case dtpropReference:
			if (c == ' ' || c == '\t' || c== ';' || c == '\n' || c == '\r'){
				if(config->tempIndex > 0){
					if (!(r=_getReference(config, config->temp, NULL, NULL))){
						if(!(r = _createReference(config, config->temp, NULL, NULL))){
							return DT_RETURN_NOMEM;
						}
					}else if(r->node && !r->strPath[0]){
						// Add node path if not set
						_getNodePath(r->node, r->strPath, DT_MAX_REFERENCE);
					}
					config->currentValue->value = AllocVec(sizeof(struct devicetreeEncodedArrayValue)*2, MEMF_ANY | MEMF_CLEAR);
					if (!config->currentValue->value){
						return DT_RETURN_NOMEM;
					}
					config->currentValue->size = 2;
					((struct devicetreeEncodedArrayValue*)config->currentValue->value)[0].flags = DT_ENCODED_VALUE_REFERENCE;
					((struct devicetreeEncodedArrayValue*)config->currentValue->value)[0].value = (ULONG)r;
					config->tempIndex = 0;
				}
				config->propertystate = dtpropUnknown;
			}else{
				_pushInChar(config->temp, c, &config->tempIndex, DT_MAX_TEMP_STR);
			}
			break;
		case dtpropByteString:
			if (c == ']'){
				config->propertystate = dtpropUnknown;
			
				if ((ret=_strToBytes(config->temp, config->currentValue)) != DT_RETURN_NOERROR){
					return ret;
				}
				config->tempIndex = 0;
			}else{
				if(config->lastchar == '/'){
					if (c == '/' || c == '*'){
						if (c == '*'){
							config->state = dtconfigStateCommentBlock;
						}else if(c == '/'){
							config->state = dtconfigStateComment;
						}
						config->commentstate = dtcommentVar;
					}
				}
				if (config->state == dtconfigStateProperty){
					if (c != ' ' && c != '\t' && c != '\n' && c != '\r'){
						// Only add chars which are not white space as these are ignored anyway
						_pushInChar(config->temp, c, &config->tempIndex, DT_MAX_TEMP_STR);
					}
				}else{
					_popOutChar(config->temp, &config->tempIndex);
				}
			}
			break;
		default:
			return DT_RETURN_PARAM_ERROR;
	}
	
	return DT_RETURN_NOERROR;
}

static UWORD _parseComment(struct devicetreeConfig *config, char c, BOOL block)
{
	BOOL bExitComment = FALSE ;
	
	switch(c){
		//case '\r':
		case '\n':
			if (!block){
				bExitComment = TRUE ;
			}
			break;
		default:
			if (block){
				if (config->lastchar == '*' && c == '/'){
					bExitComment = TRUE ;
				}
			}
	}
	
	if (bExitComment){
		switch(config->commentstate){
			case dtcommentVar:
				config->state = dtconfigStateProperty;
				// Property state should remain untouched as entered by the comment state
				break;
			case dtcommentNode:
			default:
				config->state = dtconfigStateNode;
				config->propertystate = dtpropUnknown;
		}
	}
	
	return DT_RETURN_NOERROR;
}

static UWORD _parseCommand(struct devicetreeConfig *config, char c)
{
	struct devicetreeCommand *cmd = NULL ;
	UWORD ret = 0;
	
	switch(config->cmdstate){
		case dtcmdName:
			switch(c){
				case '/':
					if (config->tempIndex > 0){
						if (!(cmd = _createCommand(config, config->currentNode))){
							return DT_RETURN_NOMEM;
						}
						config->currentObject = (struct devicetreeObject*)cmd ;
						dtStrCpy(cmd->name, config->temp, DT_MAX_COMMAND_NAME);
						config->tempIndex = 0;
						config->cmdstate = dtcmdValue;
						break;
					}
				case '*':
					if (config->tempIndex == 0){
						// This is a comment or comment block
						if (c == '*'){
							config->state = dtconfigStateCommentBlock;
							config->commentstate = dtcommentNode;
						}else{
							// this is a // comment
							config->state = dtconfigStateComment;
							config->commentstate = dtcommentNode;
						}
						break;
					}
					// Fall through * or / to be just another character for command name
					_pushInChar(config->temp, c, &config->tempIndex, DT_MAX_TEMP_STR);
					break;
				case ' ':
				case '\t': // Whitespace in command is an error but may be a root char
					config->state = dtconfigStateNode;
					config->tempIndex = 0;
					_pushInChar(config->temp, '/', &config->tempIndex, DT_MAX_TEMP_STR);
					break;
				default:
					_pushInChar(config->temp, c, &config->tempIndex, DT_MAX_TEMP_STR);
			}
			break;
		case dtcmdValue:
			switch(c){
				case '\r':
				case '\n':
					break;
				case '"':
					if (config->quoting){
						config->quoting = FALSE;
					}else{
						config->quoting = TRUE;
						break;
					}
				case ';':
					if (!config->quoting){
						if (config->tempIndex > 0){
							// Captured a value
							cmd = (struct devicetreeCommand*)config->currentObject;
							dtStrCpy(cmd->value, config->temp, DT_MAX_COMMAND_VALUE);
							config->tempIndex = 0 ;
						}

						if (c == ';'){
							if ((ret=_handleCommand(config, (struct devicetreeCommand*)config->currentObject)) != DT_RETURN_NOERROR){
								return ret;
							}
							if (config->fn){
								if ((ret=config->fn(config->currentObject)) != DT_RETURN_NOERROR){
									return ret;
								}
							}
							config->state = dtconfigStateNode;
							config->tempIndex = 0 ;
						}
						break;
					}
					// else fall through
				case ' ':
				case '\t':
					if (config->tempIndex == 0){
						break;
					}
				default:
					_pushInChar(config->temp, c, &config->tempIndex, DT_MAX_TEMP_STR);
			}
			break;
		default:
			// Unknown state
			return DT_RETURN_PARAM_ERROR;
	}
	
	return DT_RETURN_NOERROR;
}

static void _strToNode(char *str, struct devicetreeNode *node)
{
	char *p = NULL ;
	BOOL hasAddress = FALSE ;
	// split the string into name and address parts
	for (p=str; *p; p++){
		if (*p == '@'){
			hasAddress = TRUE;
			*p = '\0';
			p += 1; // advance to address char
			break;
		}
	}
	dtStrCpy(node->name, str, DT_MAX_NODE_NAME);
	if (hasAddress){
		dtStrCpy(node->unitAddress, p, DT_MAX_NODE_ADDRESS);
		*(p-1) = '@' ; // restore string
	}
}

static UWORD _parseNode(struct devicetreeConfig *config, char c)
{
	struct devicetreeProperty *property = NULL ;
	struct devicetreeNode *node = NULL ;
	
	switch(c){
		case ':': // Label identifier
			if (config->tempIndex > 0){
				// Text exists for a label to be saved
				// Will not know if for a property or a node
				dtStrCpy(config->label, config->temp, DT_MAX_NODE_LABEL);
				config->tempIndex = 0;
			}else{
				_pushInChar(config->temp, c, &config->tempIndex, DT_MAX_TEMP_STR);
			}
			break;
		case '/': // Command identifier
			if (config->tempIndex > 0){
				// Already have text before this character
				_pushInChar(config->temp, c, &config->tempIndex, DT_MAX_TEMP_STR);
			}else{
				config->state = dtconfigStateCommand;
				config->cmdstate = dtcmdName;
				config->tempIndex = 0; // reset temp string processing
				config->label[0] = '\0';
			}
			break; 
		case ';':// End marker
		case '=':// Property assignment 
			if (config->tempIndex > 0){
				// Chars in the temp buffer - this could be a property with no value
				if (!(property = _createProperty(config, config->currentNode))){
					return DT_RETURN_NOMEM;
				}
				config->currentObject = (struct devicetreeObject*)property ;
				dtStrCpy(property->name, config->temp, DT_MAX_PROPERTY_NAME);
				dtStrCpy(property->label, config->label, DT_MAX_NODE_LABEL); // copy any set label
				config->label[0] = '\0';
				config->tempIndex = 0; // reset for capture of new value
				config->state = dtconfigStateProperty;
				config->propertystate = dtpropUnknown; // reset state of value parser
				
			}else{
				_pushInChar(config->temp, c, &config->tempIndex, DT_MAX_TEMP_STR);
			}
			if (c == ';'){ // no values so reset and continue parsing node
				config->state = dtconfigStateNode;
				config->tempIndex = 0; // reset temp string processing
				config->label[0] = '\0';
			}
			break;
		// Line Terminators
		case '\n':
		case '\r':
			config->tempIndex = 0; // reset temp string processing
			config->label[0] = '\0';
			break; 
		case '{':
			// Opening of a node definition
			if (!(node = _createNode(config, config->currentNode))){
				return DT_RETURN_NOMEM;
			}
			config->currentNode = node ;
			if (config->tempIndex > 0){
				_strToNode(config->temp, node);
			}
			dtStrCpy(node->label, config->label, DT_MAX_NODE_LABEL);
			config->tempIndex = 0 ;
			config->label[0] = '\0';
			break;
		case '}':
			// Closing node definition
			config->currentNode = config->currentNode->parent; 
			if (!config->currentNode){
				// unmatched braces??
				config->currentNode = &config->topNode;
			}
			config->tempIndex = 0 ;
			config->label[0] = '\0';
			break;
		// Whitespace
		case ' ':
		case '\t':
			break;
		default: // text that could be a label or node identifier
			_pushInChar(config->temp, c, &config->tempIndex, DT_MAX_TEMP_STR);
			
	}

	return DT_RETURN_NOERROR;
}

WORD _terminatePath(char *path)
{
	WORD i = 0;
	for (; path[i] ;i++){
		if (path[i]=='/'){
			path[i] = '\0';
			return i; // divider at offset
		}
	}
	return -1; // No more path dividers
}

char* _nextPathItem(char *path, WORD lastDivider)
{
	if (lastDivider >= 0){
		path[lastDivider] = '/' ; // retore path
		path += lastDivider+1 ;
	}
	return path;
}

WORD _terminateAddress(char *pathNodeName)
{
	WORD i = 0;
	for (; pathNodeName[i] ;i++){
		if (pathNodeName[i]=='@'){
			pathNodeName[i] = '\0';
			return i; // address marker at offset
		}
	}
	return -1; // No address in path
}

char* _restoreNodeName(char *pathNodeName, WORD addressDivider)
{
	if (addressDivider >= 0){
		pathNodeName[addressDivider] = '@' ; // retore address marker
		pathNodeName += addressDivider+1 ;
	}
	return pathNodeName;
}

//////////////////////////////////////////////
//
// Public functions
//

#define _conftoupper(x) ((x >= 'a' && x <= 'z')?x-32:x)
BOOL dtStriCmp(char *a, char *b, UWORD max)
{
	UWORD i = 0;
	
	for(;i< (max-1) && a[i] != '\0';i++){
		if (_conftoupper(a[i]) != _conftoupper(b[i])){
			return FALSE ;
		}
	}
	if (a[i] == '\0' && b[i] == '\0'){
		return TRUE;
	}
	return FALSE;
}

void dtStrCpy(char *to, char *from, UWORD maxbuf)
{
	size_t len = 0;
	if (to && from){
		len = strlen(from);
		if (len > maxbuf - 1){
			len = maxbuf - 1;
		}
		memcpy(to, from, len);
		to[len] = '\0';
	}
}

struct devicetreeObject *getLastObject(struct devicetreeNode *n)
{
	struct devicetreeObject *p = NULL ;
	if (!n->firstObject){
		return NULL;
	}
	for(p=n->firstObject; p->next != NULL; p=p->next);
	
	return p;
}

struct devicetreeValue *getLastValue(struct devicetreeProperty *prop)
{
	struct devicetreeValue *p = NULL ;
	if (!prop->values){
		return NULL;
	}
	for(p=prop->values; p->next != NULL; p=p->next);
	
	return p;
}

struct devicetreeNode *getLastSiblingNode(struct devicetreeNode *n)
{
	struct devicetreeNode *p = NULL ;
	if (!n->child){
		return NULL;
	}
	for(p=n->child; p->next != NULL; p=p->next);
	
	return p;
}

struct devicetreeNode *getLastChildNode(struct devicetreeNode *n)
{
	struct devicetreeNode *p = NULL ;
	if (!n->child){
		return NULL;
	}
	for(p=n->child; p->child != NULL; p=p->child);
	
	return p;
}

struct devicetreeStream *getLastStream(struct devicetreeConfig *config)
{
	struct devicetreeStream *p = NULL ;
	if (!config->stream){
		return NULL;
	}
	for(p=config->stream; p->next != NULL; p=p->next);
	
	return p;
}

UWORD dtInitialise(struct devicetreeConfig *config)
{
	memset(config,0,sizeof(struct devicetreeConfig));
	config->dos = OpenLibrary(DOSNAME, 0);
	if (!config->dos){
		return DT_RETURN_SYSERR;
	}
	return DT_RETURN_NOERROR;
}

UWORD dtOpenFile(struct devicetreeConfig *config, BPTR f, ULONG len)
{
	struct devicetreeStream *dtStream = NULL;
	LONG curpos = 0, filelen = 0;
	
	// Open stream and set the current stream pointer to this new file
	if (!(dtStream = _addStream(config))){
		return DT_RETURN_NOMEM;
	}
	if (len == 0){
		curpos = Seek(f,0,OFFSET_CURRENT);
		if (IoErr()!=0){
			return DT_RETURN_FILE_ERROR;
		}
		filelen = Seek(f,0,OFFSET_END);
		if (IoErr()!=0){
			return DT_RETURN_FILE_ERROR;
		}
		filelen = Seek(f,0,OFFSET_CURRENT);
		if (IoErr()!=0){
			return DT_RETURN_FILE_ERROR;
		}
		len = filelen - curpos;
		Seek(f,curpos,OFFSET_BEGINNING);
		if (IoErr()!=0){
			return DT_RETURN_FILE_ERROR;
		}
	}
	
	dtStream->f = f;
	dtStream->streamLen = len;
	
	config->currentStream = dtStream ;
	
	return DT_RETURN_NOERROR;
}

UWORD dtOpenBuffer(struct devicetreeConfig *config, UBYTE *buffer, ULONG len)
{
	struct devicetreeStream *dtStream = NULL;
	// Open stream and set the current stream pointer to this new file
	if (!(dtStream = _addStream(config))){
		return DT_RETURN_NOMEM;
	}
	
	dtStream->buffer = buffer;
	dtStream->streamLen = len;
	
	config->currentStream = dtStream ;
	
	return DT_RETURN_NOERROR;
}

UWORD dtParseConfig(struct devicetreeConfig *config, dt_object_callback fn_callback)
{
	char c='\0';
	UWORD ret = DT_RETURN_NOERROR ;
	LONG fret = 0;
	BOOL commentBlock = FALSE ;
	
	// initialise with top section
	config->currentNode = &config->topNode;
	config->fn = fn_callback;
	
	for (;config->currentStream;config->currentStream = config->currentStream->prev){
		for(;config->currentStream->offset < config->currentStream->streamLen;config->currentStream->offset++){
			config->lastchar = c;
			// Read next character from file stream or memory buffer
			if (config->currentStream->f){
				if ((fret=Read(config->currentStream->f,&c,1)) != 1){
					if (fret == 0){
						// Exit reading this stream
						break;
					}else{
						return DT_RETURN_FILE_ERROR ; //error occurred
					}
				}
			}else if(config->currentStream->buffer){
				c = config->currentStream->buffer[config->currentStream->offset];
			}else{
				// No data - error parsing
				return DT_RETURN_PARAM_ERROR;
			}
			
			if (c == '\n'){
				config->currentStream->atLine++;
			}
		
replay:
			commentBlock = FALSE ;
			switch(config->state){
				case dtconfigStateNode:
					ret = _parseNode(config,c);
					break;
				case dtconfigStateProperty:
					ret = _parseProperty(config,c);
					break;
				case dtconfigStateCommand:
					ret = _parseCommand(config,c);
					break;
				case dtconfigStateCommentBlock:
					commentBlock = TRUE ;
				case dtconfigStateComment:
					_parseComment(config, c, commentBlock);
					break;
				default:
					return DT_RETURN_PARAM_ERROR; // unexpected state
					break;
			}
			if (ret == DT_RETURN_REPLAY){
				goto replay;
			}
			if (ret != DT_RETURN_NOERROR){
				return ret;
			}
		}
	}
	return DT_RETURN_NOERROR;
}

void dtClose(struct devicetreeConfig *config)
{
	void *freeMe; // Need to save this as freeing the var invalidates the change in linked list ref
	struct devicetreeNode *dtslvl, *dtssib ;
	struct devicetreeObject *dtsobjs;
	struct devicetreeValue *dtsvals;
	struct devicetreeProperty *dtsprop;
	struct devicetreeStream *dtstream;
	
	for(dtslvl = getLastChildNode(&config->topNode);dtslvl;){
		for(dtssib=getLastSiblingNode(dtslvl); dtssib;){
			for(dtsobjs = getLastObject(dtssib); dtsobjs;){
				if (dtsobjs->type == DT_OBJECT_PROPERTY){
					dtsprop = (struct devicetreeProperty*)dtsobjs;
					for(dtsvals = getLastValue(dtsprop); dtsvals;){
						if (dtsvals->value && dtsvals->size >0){
							FreeVec(dtsvals->value);
						}
						freeMe = dtsvals;
						dtsvals = dtsvals->prev;
						FreeVec(freeMe);
					}
				}
				freeMe = dtsobjs;
				dtsobjs = dtsobjs->prev;
				FreeVec(freeMe);
			}
			freeMe = dtssib;
			dtssib = dtssib->prev;
			FreeVec(freeMe);
		}
		if (dtslvl != &config->topNode){
			freeMe = dtslvl;
			if ((dtslvl = dtslvl->parent)){
				dtslvl->child = NULL;
			}
			
			FreeVec(freeMe);
		}else{
			dtslvl = NULL;
		}
	}
	
	for(dtstream=getLastStream(config);dtstream;){
		if (dtstream->configOpened){
			Close(dtstream->f);
			dtstream->f = NULL;
		}
		freeMe = dtstream;
		dtstream = dtstream->prev;
		FreeVec(freeMe);
	}
	
	CloseLibrary(config->dos);
	config->dos = NULL ;
	memset(&config->topNode, 0, sizeof(struct devicetreeNode));
}

ULONG lastLineNumber(struct devicetreeConfig *config)
{
	if (!config->currentStream){
		return 0;
	}
	return config->currentStream->atLine + 1;
}

struct devicetreeNode* getNode(struct devicetreeConfig *config, char *path)
{
	WORD dividerAt = 0, addressAt = 0;
	struct devicetreeNode *n = NULL;	
	BOOL justRoot = FALSE, matchNode=FALSE;
	
	n = config->topNode.child;
	
	// Root node is just stupid exception in the path format
	// it's like a blank named node with trailing path marker
	if (path[0] == '/' && path[1] == '\0'){
		justRoot = TRUE;
	}
	
	do{
		matchNode = FALSE;
		dividerAt = _terminatePath(path);
		// Look for node
		for (;n && !matchNode;n=n->next){
			addressAt = _terminateAddress(path);
			if (path[0] == '\0'){ // assume root node or no name node
				if (n->name[0] == '\0' || n->name[0] == '/'){
					if (justRoot){
						return n;
					}
					matchNode=TRUE; // could have an address component (odd name, should fail?)
				}
			}else{
				// Look for named node
				if (dtStriCmp(n->name, path, DT_MAX_NODE_NAME)){
					matchNode=TRUE;
				}
			}
			if (addressAt >= 0){ // there is an address component to the path name
				path = _restoreNodeName(path, addressAt); // restore string and move path pointer to address
				matchNode = FALSE ;
				if (dtStriCmp(n->unitAddress, path, DT_MAX_NODE_ADDRESS)){
					matchNode=TRUE;
				}
			}
			if (matchNode){
				break; // exit loop if match found
			}
		}
		if (!n){ // valid node pointer is a must-have to proceed
			// No node found, exit
			return NULL ;
		}
		
		// Found node, iterate to next child and move on path
		if (dividerAt >=0){
			path = _nextPathItem(path, dividerAt);
			n=n->child;
		}
	}while(dividerAt >= 0);
	
	// exit loop when all path items found. Return node
	return n;
}

struct devicetreeNode* iterateChildNodes(struct devicetreeConfig *config, struct devicetreeNode *fromNode, char *path, char *compatible)
{
	struct devicetreeNode *containingNode=NULL, *n=NULL;
	struct devicetreeObject *o=NULL;
	struct devicetreeProperty *p=NULL;
	struct devicetreeValue *v = NULL;
	
	// If we don't have a previous search node then
	// look for the parent containing node.
	// If not found then exit
	if (!fromNode){
		containingNode = getNode(config, path);
		if (!containingNode){
			return NULL;
		}
		n = containingNode->child; // n starts with first child
	}else{
		n = fromNode->next; // continue a search to next sibling
	}
	
	// Check if compatible filter exists and has a value
	if (compatible && compatible[0] != '\0'){
		for ( ; n; n=n->next){ // iterate all siblings
			for (o=n->firstObject; o;o=o->next){ // iterate all associated objects
				if (o->type == DT_OBJECT_PROPERTY){
					p = (struct devicetreeProperty*)o;
					if (dtStriCmp(p->name, "compatible", DT_MAX_PROPERTY_NAME)){
						for (v=p->values; v; v=v->next){
							if (v->type == DT_VALUE_STRING && v->size > 0 && dtStriCmp((char*)v->value, compatible, v->size)){
								// Found compatible property with corresponding value
								return n;
							}
						}
					}
				}
			}
		}
	}
	
	return n; // Could be null for failed result
}

struct devicetreeProperty* getProperty(struct devicetreeConfig *config, struct devicetreeNode *node, char *name)
{
	struct devicetreeObject *o = NULL;
	struct devicetreeProperty *p = NULL;
	
	for(o=node->firstObject;o;o=o->next){
		if (o->type == DT_OBJECT_PROPERTY){
			p = (struct devicetreeProperty*)o;
			if (dtStriCmp(p->name, name, DT_MAX_NODE_LABEL)){
				return p;
			}
		}
	}
	return NULL;
}

struct devicetreeProperty* iterateProperty(struct devicetreeConfig *config, struct devicetreeNode *node, struct devicetreeProperty *lastProperty)
{
	struct devicetreeObject *o = NULL;
	
	if (lastProperty){
		o = lastProperty->_obj.next ;
	}else{
		o = node->firstObject;
	}
	
	for(;o;o=o->next){
		if (o->type == DT_OBJECT_PROPERTY){
			return (struct devicetreeProperty*)o;
		}
	}
	return NULL;
}