/* Automatically generated header (sfdc 1.13)! Do not edit! */

#ifndef _INLINE_BSDSOCKET_IFINDEX_H
#define _INLINE_BSDSOCKET_IFINDEX_H

#ifndef _SFDC_VARARG_DEFINED
#define _SFDC_VARARG_DEFINED
#ifdef __HAVE_IPTR_ATTR__
typedef APTR _sfdc_vararg __attribute__((iptr));
#else
typedef ULONG _sfdc_vararg;
#endif /* __HAVE_IPTR_ATTR__ */
#endif /* _SFDC_VARARG_DEFINED */

#ifndef __INLINE_MACROS_H
#include <inline/macros.h>
#endif /* !__INLINE_MACROS_H */

#ifndef BSDSOCKET_IFINDEX_BASE_NAME
#define BSDSOCKET_IFINDEX_BASE_NAME SocketBase
#endif /* !BSDSOCKET_IFINDEX_BASE_NAME */

#define if_nametoindex(___ifname) \
      LP1(0x372, ULONG, if_nametoindex , const char *, ___ifname, a0,\
      , BSDSOCKET_IFINDEX_BASE_NAME)

#define if_indextoname(___ifindex, ___ifname) \
      LP2(0x378, char *, if_indextoname , ULONG, ___ifindex, d0, char *, ___ifname, a0,\
      , BSDSOCKET_IFINDEX_BASE_NAME)

#define if_nameindex() \
      LP0(0x37e, struct if_nameindex *, if_nameindex ,\
      , BSDSOCKET_IFINDEX_BASE_NAME)

#define if_freenameindex(___ptr) \
      LP1NR(0x384, if_freenameindex , struct if_nameindex *, ___ptr, a0,\
      , BSDSOCKET_IFINDEX_BASE_NAME)

#endif /* !_INLINE_BSDSOCKET_IFINDEX_H */
