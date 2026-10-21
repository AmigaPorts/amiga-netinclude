/* Explicit opt-in to the proposed classic m68k interface-index vectors.
 * The offsets are a draft common ABI awaiting cross-stack agreement.
 * Establish that the loaded library supports this ABI before calling.
 * See docs/ipv6-sdk.txt.
 */
#ifndef PROTO_BSDSOCKET_IFINDEX_H
#define PROTO_BSDSOCKET_IFINDEX_H
#include <proto/bsdsocket.h>
#include <net/if.h>

#if defined(__amigaos4__) || defined(__PPC__) || defined(__AROS__)
#error These proposed vector bindings are for classic AmigaOS/m68k only
#endif

/* Declarations stay in net/if.h, following its __stdargs convention.
 * Merely including the ordinary proto header never enables these bindings.
 */
#if !defined(__NO_NET_API) && !defined(_NO_INLINE)
#if defined(__GNUC__)
#include <inline/bsdsocket_ifindex.h>
#elif defined(__VBCC__)
#include <inline/bsdsocket_ifindex_protos.h>
#else
#include <pragmas/bsdsocket_ifindex_pragmas.h>
#endif
#endif
#endif
