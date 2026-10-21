/* IPv6 wire formats: RFC 8200 and the RFC 3542 application interface.
 * Multi-octet packet fields and their masks are in network byte order.
 * The classic SDK targets big-endian m68k; no C bitfields are used.
 */
#ifndef _NETINET_IP6_H_
#define _NETINET_IP6_H_

#include <netinet/in.h>

struct ip6_hdr {
    union {
        struct ip6_hdrctl {
            __ULONG ip6_un1_flow;
            __UWORD ip6_un1_plen;
            __UBYTE ip6_un1_nxt;
            __UBYTE ip6_un1_hlim;
        } ip6_un1;
        __UBYTE ip6_un2_vfc;
    } ip6_ctlun;
    struct in6_addr ip6_src;
    struct in6_addr ip6_dst;
};
#define ip6_vfc  ip6_ctlun.ip6_un2_vfc
#define ip6_flow ip6_ctlun.ip6_un1.ip6_un1_flow
#define ip6_plen ip6_ctlun.ip6_un1.ip6_un1_plen
#define ip6_nxt  ip6_ctlun.ip6_un1.ip6_un1_nxt
#define ip6_hlim ip6_ctlun.ip6_un1.ip6_un1_hlim
#define ip6_hops ip6_hlim

#define IPV6_VERSION        0x60
#define IPV6_VERSION_MASK   0xf0
#define IPV6_FLOWINFO_MASK  0x0fffffffUL
#define IPV6_FLOWLABEL_MASK 0x000fffffUL
#define IPV6_ECN_MASK       0x00300000UL
#define IPV6_FLOWLABEL_LEN  20
#define IPV6_TRAFFIC_CLASS(p) ((ntohl((p)->ip6_flow) >> 20) & 0xff)
#define IPV6_DSCP(p)          (IPV6_TRAFFIC_CLASS(p) & 0xfc)
#define IPV6_ECN(p)           (IPV6_TRAFFIC_CLASS(p) & 0x03)

/* Extension length fields count eight-octet units, excluding the first. */
struct ip6_ext { __UBYTE ip6e_nxt, ip6e_len; };
struct ip6_hbh { __UBYTE ip6h_nxt, ip6h_len; };
struct ip6_dest { __UBYTE ip6d_nxt, ip6d_len; };
struct ip6_rthdr {
    __UBYTE ip6r_nxt, ip6r_len, ip6r_type, ip6r_segleft;
};
/* Type zero is retained for decoding old packets; RFC 5095 deprecated it. */
struct ip6_rthdr0 {
    __UBYTE ip6r0_nxt, ip6r0_len, ip6r0_type, ip6r0_segleft;
    __ULONG ip6r0_reserved;
    /* Addresses follow this fixed prefix. */
};
struct ip6_frag {
    __UBYTE ip6f_nxt, ip6f_reserved;
    __UWORD ip6f_offlg;
    __ULONG ip6f_ident;
};
#define IP6F_OFF_MASK      0xfff8
#define IP6F_RESERVED_MASK 0x0006
#define IP6F_MORE_FRAG     0x0001

/* Options have variable trailing data, excluded from these fixed prefixes. */
struct ip6_opt { __UBYTE ip6o_type, ip6o_len; };
struct ip6_opt_jumbo {
    __UBYTE ip6oj_type, ip6oj_len, ip6oj_jumbo_len[4];
};
struct ip6_opt_nsap {
    __UBYTE ip6on_type, ip6on_len, ip6on_src_nsap_len, ip6on_dst_nsap_len;
};
struct ip6_opt_tunnel {
    __UBYTE ip6ot_type, ip6ot_len, ip6ot_encap_limit;
};
struct ip6_opt_router {
    __UBYTE ip6or_type, ip6or_len, ip6or_value[2];
};
#define IP6OPT_PAD1         0x00
#define IP6OPT_PADN         0x01
#define IP6OPT_JUMBO        0xc2
#define IP6OPT_NSAP_ADDR    0xc3
#define IP6OPT_TUNNEL_LIMIT 0x04
#define IP6OPT_ROUTER_ALERT 0x05
#define IP6OPT_RTALERT      IP6OPT_ROUTER_ALERT
#define IP6OPT_EID          0x8a
#define IP6OPT_MINLEN       2
#define IP6OPT_JUMBO_LEN    6
#define IP6OPT_RTALERT_LEN  4
#define IP6OPT_RTALERT_MLD    0
#define IP6OPT_RTALERT_RSVP   1
#define IP6OPT_RTALERT_ACTNET 2
#define IP6_ALERT_MLD  0x0000
#define IP6_ALERT_RSVP 0x0001
#define IP6_ALERT_AN   0x0002
#define IP6OPT_TYPE(o)        ((o) & 0xc0)
#define IP6OPT_TYPE_SKIP      0x00
#define IP6OPT_TYPE_DISCARD   0x40
#define IP6OPT_TYPE_FORCEICMP 0x80
#define IP6OPT_TYPE_ICMP      0xc0
#define IP6OPT_MUTABLE        0x20

#define IPV6_MAXHLIM    255
#define IPV6_DEFHLIM    64
#define IPV6_DEFFRAGTTL 60000
#define IPV6_HLIMDEC    1
#define IPV6_MMTU       1280
#define IPV6_MAXPACKET  65535
#define IPV6_MAXOPTHDR  2048

#endif
