/* Native byte-for-byte packet checks and all 256 ICMPv6 filter types. */
#ifndef TEST_WIRE_RUNTIME_H
#define TEST_WIRE_RUNTIME_H
#include <string.h>
#define WIRE_REQUIRE(c) do { if (!(c)) return __LINE__; } while (0)
static int CheckWirePackets(void)
{
    struct ip6_hdr ip;
    struct icmp6_hdr echo;
    struct nd_router_advert ra;
    struct ip6_frag frag;
    struct icmp6_filter filter;
    struct icmp6_filter *fp;
    const unsigned char *bytes;
    int i, j;
    static const unsigned char expected_ip[40] = {
        0x6a,0xb1,0x23,0x45, 0x12,0x34,0x3a,0x40,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        0xff,2,0,0,0,0,0,0,0,0,0,0,0,0,0,1
    };
    static const unsigned char expected_echo[8] = {
        0x80,0,0x12,0x34,0xab,0xcd,1,2
    };
    static const unsigned char expected_ra[16] = {
        0x86,0,0x12,0x34,0x40,0xc8,0,0x1e,
        0,0,3,0xe8,0,0,7,0xd0
    };
    static const unsigned char expected_frag[8] = {
        0x11,0,0,0x11,0x12,0x34,0x56,0x78
    };

    memset(&ip, 0, sizeof(ip));
    ip.ip6_flow = htonl(0x6ab12345UL);
    ip.ip6_plen = htons(0x1234);
    ip.ip6_nxt = IPPROTO_ICMPV6;
    ip.ip6_hops = 64;
    ip.ip6_src.s6_addr[15] = 1;
    ip.ip6_dst.s6_addr[0] = 0xff;
    ip.ip6_dst.s6_addr[1] = 2;
    ip.ip6_dst.s6_addr[15] = 1;
    WIRE_REQUIRE(memcmp(&ip, expected_ip, sizeof(ip)) == 0);
    WIRE_REQUIRE(ip.ip6_vfc == 0x6a && IPV6_TRAFFIC_CLASS(&ip) == 0xab);
    WIRE_REQUIRE(IPV6_DSCP(&ip) == 0xa8 && IPV6_ECN(&ip) == 3);
    WIRE_REQUIRE((ip.ip6_flow & IPV6_FLOWLABEL_MASK) == htonl(0x12345UL));

    echo.icmp6_type = ICMP6_ECHO_REQUEST;
    echo.icmp6_code = 0;
    echo.icmp6_cksum = htons(0x1234);
    echo.icmp6_id = htons(0xabcd);
    echo.icmp6_seq = htons(0x0102);
    WIRE_REQUIRE(memcmp(&echo, expected_echo, sizeof(echo)) == 0);
    echo.icmp6_mtu = htonl(1280);
    bytes = (const unsigned char *)&echo;
    WIRE_REQUIRE(bytes[4] == 0 && bytes[5] == 0 && bytes[6] == 5 && bytes[7] == 0);
    WIRE_REQUIRE(echo.icmp6_pptr == htonl(1280));

    /* Decode from independent wire bytes, then check all public aliases. */
    memcpy(&ra, expected_ra, sizeof(ra));
    WIRE_REQUIRE(ra.nd_ra_type == ND_ROUTER_ADVERT && ra.nd_ra_code == 0);
    WIRE_REQUIRE(ntohs(ra.nd_ra_cksum) == 0x1234 && ra.nd_ra_curhoplimit == 64);
    WIRE_REQUIRE(ra.nd_ra_flags_reserved == (ND_RA_FLAG_MANAGED | ND_RA_FLAG_OTHER |
                 ND_RA_FLAG_RTPREF_HIGH));
    WIRE_REQUIRE(ntohs(ra.nd_ra_router_lifetime) == 30);
    WIRE_REQUIRE(ntohl(ra.nd_ra_reachable) == 1000 && ntohl(ra.nd_ra_retransmit) == 2000);
    frag.ip6f_nxt = IPPROTO_UDP;
    frag.ip6f_reserved = 0;
    frag.ip6f_offlg = htons(0x10) | IP6F_MORE_FRAG;
    frag.ip6f_ident = htonl(0x12345678UL);
    WIRE_REQUIRE(memcmp(&frag, expected_frag, sizeof(frag)) == 0);
    WIRE_REQUIRE((frag.ip6f_offlg & IP6F_OFF_MASK) == htons(0x10));
    WIRE_REQUIRE((frag.ip6f_offlg & IP6F_RESERVED_MASK) == 0);
    WIRE_REQUIRE(IP6OPT_TYPE(IP6OPT_JUMBO) == IP6OPT_TYPE_ICMP);

    /* Each bit, including bit 31 of every word, must change independently. */
    for (i = 0; i < 256; ++i) {
        ICMP6_FILTER_SETBLOCKALL(&filter);
        ICMP6_FILTER_SETPASS(i, &filter);
        WIRE_REQUIRE(filter.icmp6_filt[i >> 5] == (1UL << (i & 31)));
        for (j = 0; j < 256; ++j) {
            WIRE_REQUIRE(ICMP6_FILTER_WILLPASS(j, &filter) == (i == j));
            WIRE_REQUIRE(ICMP6_FILTER_WILLBLOCK(j, &filter) == (i != j));
        }
        ICMP6_FILTER_SETPASSALL(&filter);
        ICMP6_FILTER_SETBLOCK(i, &filter);
        WIRE_REQUIRE(filter.icmp6_filt[i >> 5] == ~(1UL << (i & 31)));
        for (j = 0; j < 256; ++j)
            WIRE_REQUIRE(ICMP6_FILTER_WILLPASS(j, &filter) == (i != j));
    }
    fp = &filter;
    ICMP6_FILTER_SETPASSALL(fp++);
    WIRE_REQUIRE(fp == &filter + 1);
    fp = &filter;
    ICMP6_FILTER_SETBLOCKALL(fp++);
    WIRE_REQUIRE(fp == &filter + 1);
    return 0;
}
#undef WIRE_REQUIRE
#endif
