/* Automatically generated header (sfdc 1.13)! Do not edit! */
#ifndef PRAGMAS_BSDSOCKET_IFINDEX_PRAGMAS_H
#define PRAGMAS_BSDSOCKET_IFINDEX_PRAGMAS_H

/*
**   $VER: bsdsocket_ifindex_pragmas.h 1.0 $Id: bsdsocket_ifindex_lib.sfd,v 1.0 2026-10-21 $
**
**   Direct ROM interface (pragma) definitions.
**
*/

#if defined(LATTICE) || defined(__SASC) || defined(_DCC)
#ifndef __CLIB_PRAGMA_LIBCALL
#define __CLIB_PRAGMA_LIBCALL
#endif /* __CLIB_PRAGMA_LIBCALL */
#else /* __MAXON__, __STORM__ or AZTEC_C */
#ifndef __CLIB_PRAGMA_AMICALL
#define __CLIB_PRAGMA_AMICALL
#endif /* __CLIB_PRAGMA_AMICALL */
#endif /* */

#if defined(__SASC_60) || defined(__STORM__)
#ifndef __CLIB_PRAGMA_TAGCALL
#define __CLIB_PRAGMA_TAGCALL
#endif /* __CLIB_PRAGMA_TAGCALL */
#endif /* __MAXON__, __STORM__ or AZTEC_C */

#ifdef __CLIB_PRAGMA_LIBCALL
 #pragma libcall SocketBase if_nametoindex 372 801
#endif /* __CLIB_PRAGMA_LIBCALL */
#ifdef __CLIB_PRAGMA_AMICALL
 #pragma amicall(SocketBase, 0x372, if_nametoindex(a0))
#endif /* __CLIB_PRAGMA_AMICALL */
#ifdef __CLIB_PRAGMA_LIBCALL
 #pragma libcall SocketBase if_indextoname 378 8002
#endif /* __CLIB_PRAGMA_LIBCALL */
#ifdef __CLIB_PRAGMA_AMICALL
 #pragma amicall(SocketBase, 0x378, if_indextoname(d0,a0))
#endif /* __CLIB_PRAGMA_AMICALL */
#ifdef __CLIB_PRAGMA_LIBCALL
 #pragma libcall SocketBase if_nameindex 37e 00
#endif /* __CLIB_PRAGMA_LIBCALL */
#ifdef __CLIB_PRAGMA_AMICALL
 #pragma amicall(SocketBase, 0x37e, if_nameindex())
#endif /* __CLIB_PRAGMA_AMICALL */
#ifdef __CLIB_PRAGMA_LIBCALL
 #pragma libcall SocketBase if_freenameindex 384 801
#endif /* __CLIB_PRAGMA_LIBCALL */
#ifdef __CLIB_PRAGMA_AMICALL
 #pragma amicall(SocketBase, 0x384, if_freenameindex(a0))
#endif /* __CLIB_PRAGMA_AMICALL */

#endif /* PRAGMAS_BSDSOCKET_IFINDEX_PRAGMAS_H */
