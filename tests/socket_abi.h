#ifndef TEST_SOCKET_ABI_H
#define TEST_SOCKET_ABI_H
#ifndef TEST_REVERSE_INCLUDE
#define TEST_REVERSE_INCLUDE 0
#endif
#include <exec/types.h>
#include <stddef.h>
#if TEST_REVERSE_INCLUDE
#include <net/if_dl.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <net/if.h>
#include <net/if_dl.h>
#endif
struct align_sockaddr_storage { char c; struct sockaddr_storage value; };
struct align_sockaddr_dl { char c; struct sockaddr_dl value; };
struct align_sockaddr_in { char c; struct sockaddr_in value; };
struct align_sockaddr_in6 { char c; struct sockaddr_in6 value; };

/* SAS/C's offsetof macro is not an integer constant expression, so the
 * portable executable checks these values at run time. The GCC matrix also
 * checks them at compile time using its builtin offsetof/alignment support.
 */
#define OFF(t, f) offsetof(struct t, f)
#define CHECK(c) do { if (!(c)) return __LINE__; } while (0)
static int CheckSocketLayout(void)
{
#ifndef SIN6_LEN
#error Missing BSD IPv6 ABI
#endif
CHECK(AF_INET6 == 28 && AF_INET6 != AF_IPX && AF_MAX > AF_INET6);
CHECK(PF_INET6 == AF_INET6);
#ifdef IP_PKTINFO
#error IPv4 packet-info extension leaked into common headers
#endif
CHECK(IPV6_V6ONLY == 27 && IPV6_CHECKSUM == 26);
CHECK(IPV6_TCLASS == 36 && IPV6_RECVPKTINFO == 61);
#ifdef AI_ADDRCONFIG
#error Unsupported resolver flag leaked into common headers
#endif
CHECK(AI_MASK == (AI_PASSIVE | AI_CANONNAME | AI_NUMERICHOST | AI_NUMERICSERV));
CHECK(AI_NUMERICSERV == 16 && EAI_NONAME == -2);
CHECK(sizeof(struct in6_pktinfo) == 20);

CHECK(sizeof(struct if_nameindex) == 8 && IF_NAMESIZE == 16);

CHECK(sizeof(struct sockaddr_dl) <= sizeof(struct sockaddr_storage));
CHECK(offsetof(struct align_sockaddr_storage, value) >= offsetof(struct align_sockaddr_dl, value));
CHECK(CMSG_SPACE(1) == 16 && CMSG_LEN(1) == 13);
CHECK(sizeof(struct sockaddr_in) == 16);
CHECK(OFF(sockaddr_in, sin_len) == 0);
CHECK(OFF(sockaddr_in, sin_family) == 1);
CHECK(sizeof(struct sockaddr_in6) == 28);
CHECK(OFF(sockaddr_in6, sin6_len) == 0);
CHECK(OFF(sockaddr_in6, sin6_family) == OFF(sockaddr, sa_family));
CHECK(OFF(sockaddr_in6, sin6_port) == 2);
CHECK(OFF(sockaddr_in6, sin6_flowinfo) == 4);
CHECK(OFF(sockaddr_in6, sin6_addr) == 8);
CHECK(OFF(sockaddr_in6, sin6_scope_id) == 24);
CHECK(sizeof(struct sockaddr_storage) == 128);
CHECK(OFF(sockaddr_storage, ss_family) == OFF(sockaddr, sa_family));
CHECK(offsetof(struct align_sockaddr_storage, value) >= offsetof(struct align_sockaddr_in, value));
CHECK(offsetof(struct align_sockaddr_storage, value) >= offsetof(struct align_sockaddr_in6, value));

CHECK(sizeof(int) == 4 && sizeof(ssize_t) == 4 && (ssize_t)-1 < 0);
CHECK(sizeof(in_addr_t) == 4 && sizeof(socklen_t) == 4);
CHECK(sizeof(struct cmsghdr) == 12);
    return 0;
}
#undef CHECK
#undef OFF
#endif
