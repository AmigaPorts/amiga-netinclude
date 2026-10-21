/* Compile-time layout assertions and executable CMSG/vector ABI checks.
 * The test library uses deterministic 68k callbacks, not host networking.
 */
#include "socket_abi.h"
#include "wire_runtime.h"
#include <exec/libraries.h>
#include <proto/bsdsocket_ifindex.h>

extern struct Library TestSocketBase;
struct Library *SocketBase;
struct if_nameindex TestNames[] = { { 7, "lo0" }, { 0, NULL } };
ULONG TestFreed, TestBadFree;
int CheckInterfaceNames(struct Library *base);

int main(void)
{
    union { struct cmsghdr align; unsigned char bytes[48]; } control;
    struct msghdr message;
    struct cmsghdr *first, *second;
    struct if_nameindex *names;
    char name[IF_NAMESIZE];
    struct in6_addr any = IN6ADDR_ANY_INIT;
    struct in6_addr loop = IN6ADDR_LOOPBACK_INIT;

    if (CheckSocketLayout() != 0)
        return 12;
    if (CheckWirePackets() != 0)
        return 13;
    if (!IN6_IS_ADDR_UNSPECIFIED(&any) || !IN6_IS_ADDR_LOOPBACK(&loop) ||
        in6addr_any.s6_addr[0] != 0 || in6addr_loopback.s6_addr[15] != 1)
        return 1;
    message.msg_control = NULL;
    message.msg_controllen = sizeof(control.bytes);
    if (CMSG_FIRSTHDR(&message) != NULL)
        return 2;
    message.msg_control = control.bytes;
    message.msg_controllen = sizeof(struct cmsghdr) - 1;
    if (CMSG_FIRSTHDR(&message) != NULL)
        return 3;
    message.msg_controllen = sizeof(control.bytes);
    first = CMSG_FIRSTHDR(&message);
    if ((void *)first != (void *)control.bytes ||
        CMSG_NXTHDR(&message, NULL) != first)
        return 4;
    first->cmsg_len = CMSG_LEN(1);
    second = CMSG_NXTHDR(&message, first);
    if ((void *)second != (void *)(control.bytes + 16))
        return 5;
    CMSG_DATA(first)[0] = 42;
    if (control.bytes[12] != 42)
        return 6;
    message.msg_controllen = 27;
    if (CMSG_NXTHDR(&message, first) != NULL)
        return 7;

    SocketBase = &TestSocketBase;
    names = if_nameindex();
    if (names != TestNames || if_nametoindex("lo0") != 7 ||
        if_nametoindex("unknown") != 0)
        return 8;
    if (if_indextoname(7, name) != name || name[0] != 'l' ||
        name[1] != 'o' || name[2] != '0' || name[3] != '\0' ||
        if_indextoname(0, name) != NULL)
        return 9;
    if_freenameindex(names);
    if (TestFreed != 1 || TestBadFree != 0)
        return 10;
    if (CheckInterfaceNames(SocketBase) != 0 || TestFreed != 2 ||
        TestBadFree != 0)
        return 11;
    /* A successful enumeration may contain only its terminating entry. */
    TestNames[0].if_index = 0;
    TestNames[0].if_name = NULL;
    if (CheckInterfaceNames(SocketBase) != 0 || TestFreed != 3 ||
        TestBadFree != 0)
        return 14;
    return 0;
}
