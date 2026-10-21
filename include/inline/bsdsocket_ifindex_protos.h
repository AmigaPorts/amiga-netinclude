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

ULONG __if_nametoindex(__reg("a6") struct Library * , __reg("a0") const char *ifname ) = "\tjsr\t-882(a6)";
#define if_nametoindex(ifname) __if_nametoindex(SocketBase, (ifname))

char * __if_indextoname(__reg("a6") struct Library * , __reg("d0") ULONG ifindex , __reg("a0") char *ifname ) = "\tjsr\t-888(a6)";
#define if_indextoname(ifindex, ifname) __if_indextoname(SocketBase, (ifindex), (ifname))

struct if_nameindex * __if_nameindex(__reg("a6") struct Library * ) = "\tjsr\t-894(a6)";
#define if_nameindex() __if_nameindex(SocketBase)

VOID __if_freenameindex(__reg("a6") struct Library * , __reg("a0") struct if_nameindex *ptr ) = "\tjsr\t-900(a6)";
#define if_freenameindex(ptr) __if_freenameindex(SocketBase, (ptr))

#endif /* !_INLINE_BSDSOCKET_IFINDEX_H */
