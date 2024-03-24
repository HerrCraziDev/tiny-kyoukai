
#ifndef __TINYKYOU_TYPES_H__
#define __TINYKYOU_TYPES_H__

#include <stdint.h>
#include "errors.h"

#define __FUCK_OFF(def) #def
#define _TOSTRING(def) __FUCK_OFF(def)

/*
-------------------------------------------------------------------------------
	Common Types
-------------------------------------------------------------------------------
*/

typedef int8_t			int8;
typedef uint8_t			uint8;
typedef int16_t			int16;
typedef uint16_t		uint16;
typedef int32_t			int32;
typedef uint32_t		uint32;
typedef int64_t			int64;
typedef uint64_t		uint64;

#endif /* __TINYKYOU_TYPES_H__ */

