
#ifndef __TINYKYOU_ERRORS_H__
#define __TINYKYOU_ERRORS_H__

/*
-------------------------------------------------------------------------------
	Common Errors
-------------------------------------------------------------------------------
*/

typedef enum {
	NO_ERR						= 0x0000,	/* No Errors					*/
	ERR_UNKNOWN					= 0xFFFF,	/* Unknown error 				*/

	ERR_NOT_IMPLEMENTED			= 0xFF01,	/* Not implemented				*/
	ERR_BAD_PARAMS				= 0xFF02,	/* Bad param (value)			*/
	ERR_USAGE					= 0xFF03,	/* Incorrect usage				*/
	ERR_SKILL_ISSUE				= 0xDEAD,	/* Unspecified internal error	*/

} e_ret;

#endif /* __TINYKYOU_ERRORS_H__ */