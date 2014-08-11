#pragma once

#ifdef JPEG_CJPEG_DJPEG

	#define BMP_SUPPORTED
	#undef GIF_SUPPORTED
	#undef PPM_SUPPORTED
	#undef RLE_SUPPORTED
	#undef TARGA_SUPPORTED

#endif /* JPEG_CJPEG_DJPEG */


#ifdef _MSC_VER

	#define HAVE_PROTOTYPES
	#define HAVE_UNSIGNED_CHAR
	#define HAVE_UNSIGNED_SHORT
	/* #define void char */
	/* #define const */
	#undef CHAR_IS_UNSIGNED
	#define HAVE_STDDEF_H
	#define HAVE_STDLIB_H
	#undef NEED_BSD_STRINGS
	#undef NEED_SYS_TYPES_H
	#undef NEED_FAR_POINTERS	/* we presume a 32-bit flat memory model */
	#undef NEED_SHORT_EXTERNAL_NAMES
	#undef INCOMPLETE_TYPES_BROKEN

	/* Define "boolean" as unsigned char, not enum, per Windows custom */
	#ifndef __RPCNDR_H__		/* don't conflict if rpcndr.h already read */
	typedef unsigned char boolean;
	#endif
	#define HAVE_BOOLEAN		/* prevent jmorecfg.h from redefining it */


	#ifdef JPEG_INTERNALS

		#undef RIGHT_SHIFT_IS_UNSIGNED

	#endif /* JPEG_INTERNALS */

	#ifdef JPEG_CJPEG_DJPEG

		#define TWO_FILE_COMMANDLINE	/* optional */
		#define USE_SETMODE		/* Microsoft has setmode() */
		#undef NEED_SIGNAL_CATCHER
		#undef DONT_USE_B_MODE
		#undef PROGRESS_REPORT		/* optional */

	#endif /* JPEG_CJPEG_DJPEG */

#endif

#ifdef __GNUG__

	#define HAVE_PROTOTYPES 
	#define HAVE_UNSIGNED_CHAR 
	#define HAVE_UNSIGNED_SHORT 
	#undef CHAR_IS_UNSIGNED
	#define HAVE_STDDEF_H 
	#define HAVE_STDLIB_H 
	#undef NEED_BSD_STRINGS
	#undef NEED_SYS_TYPES_H
	#undef NEED_FAR_POINTERS
	#undef NEED_SHORT_EXTERNAL_NAMES
	#undef INCOMPLETE_TYPES_BROKEN

	typedef unsigned char boolean;
	#define HAVE_BOOLEAN

	#ifdef JPEG_INTERNALS
		#undef RIGHT_SHIFT_IS_UNSIGNED
	#endif /* JPEG_INTERNALS */

	#ifdef JPEG_CJPEG_DJPEG

		#undef TWO_FILE_COMMANDLINE
		#undef NEED_SIGNAL_CATCHER
		#undef DONT_USE_B_MODE
		#undef PROGRESS_REPORT

	#endif /* JPEG_CJPEG_DJPEG */

#endif