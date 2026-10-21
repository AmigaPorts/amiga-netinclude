/* ICMPv6 application wire formats (RFC 3542, 4443 and 4861).
 * Packet fields and masks are in network byte order on classic m68k.
 * Variable payloads follow the fixed prefixes declared here. Applications
 * must check received lengths before accessing them.
 */
#ifndef _NETINET_ICMP6_H_
#define _NETINET_ICMP6_H_

#include <netinet/in.h>

struct icmp6_hdr {
    __UBYTE icmp6_type, icmp6_code;
    __UWORD icmp6_cksum;
    union {
        __ULONG icmp6_un_data32[1];
        __UWORD icmp6_un_data16[2];
        __UBYTE icmp6_un_data8[4];
    } icmp6_dataun;
};
#define icmp6_data32 icmp6_dataun.icmp6_un_data32
#define icmp6_data16 icmp6_dataun.icmp6_un_data16
#define icmp6_data8  icmp6_dataun.icmp6_un_data8
#define icmp6_pptr     icmp6_data32[0]
#define icmp6_mtu      icmp6_data32[0]
#define icmp6_id       icmp6_data16[0]
#define icmp6_seq      icmp6_data16[1]
#define icmp6_maxdelay icmp6_data16[0]
#define ICMPV6_PLD_MAXLEN 1232

#define ICMP6_DST_UNREACH   1
#define ICMP6_PACKET_TOO_BIG 2
#define ICMP6_TIME_EXCEEDED 3
#define ICMP6_PARAM_PROB    4
#define ICMP6_ECHO_REQUEST 128
#define ICMP6_ECHO_REPLY   129
#define MLD_LISTENER_QUERY 130
#define MLD_LISTENER_REPORT 131
#define MLD_LISTENER_DONE  132
#define MLD_LISTENER_REDUCTION MLD_LISTENER_DONE
#define ND_ROUTER_SOLICIT  133
#define ND_ROUTER_ADVERT   134
#define ND_NEIGHBOR_SOLICIT 135
#define ND_NEIGHBOR_ADVERT 136
#define ND_REDIRECT        137
#define ICMP6_ROUTER_RENUMBERING 138
#define ICMP6_NI_QUERY     139
#define ICMP6_NI_REPLY     140
#define MLDV2_LISTENER_REPORT 143
#define MLD_MTRACE_RESP    200
#define MLD_MTRACE         201
#define ICMP6_MAXTYPE      201
#define ICMP6_INFOMSG_MASK 0x80

#define ICMP6_DST_UNREACH_NOROUTE     0
#define ICMP6_DST_UNREACH_ADMIN       1
#define ICMP6_DST_UNREACH_BEYONDSCOPE 2
#define ICMP6_DST_UNREACH_NOTNEIGHBOR ICMP6_DST_UNREACH_BEYONDSCOPE
#define ICMP6_DST_UNREACH_ADDR        3
#define ICMP6_DST_UNREACH_NOPORT      4
#define ICMP6_DST_UNREACH_POLICY      5
#define ICMP6_DST_UNREACH_REJECT      6
#define ICMP6_DST_UNREACH_SRCROUTE    7
#define ICMP6_TIME_EXCEED_TRANSIT     0
#define ICMP6_TIME_EXCEED_REASSEMBLY  1
#define ICMP6_PARAMPROB_HEADER        0
#define ICMP6_PARAMPROB_NEXTHEADER    1
#define ICMP6_PARAMPROB_OPTION        2

/* Neighbor Discovery messages and options (RFC 4861). */
struct nd_router_solicit { struct icmp6_hdr nd_rs_hdr; };
#define nd_rs_type     nd_rs_hdr.icmp6_type
#define nd_rs_code     nd_rs_hdr.icmp6_code
#define nd_rs_cksum    nd_rs_hdr.icmp6_cksum
#define nd_rs_reserved nd_rs_hdr.icmp6_data32[0]

struct nd_router_advert {
    struct icmp6_hdr nd_ra_hdr;
    __ULONG nd_ra_reachable, nd_ra_retransmit;
};
#define nd_ra_type            nd_ra_hdr.icmp6_type
#define nd_ra_code            nd_ra_hdr.icmp6_code
#define nd_ra_cksum           nd_ra_hdr.icmp6_cksum
#define nd_ra_curhoplimit     nd_ra_hdr.icmp6_data8[0]
#define nd_ra_flags_reserved  nd_ra_hdr.icmp6_data8[1]
#define nd_ra_router_lifetime nd_ra_hdr.icmp6_data16[1]
#define ND_RA_FLAG_MANAGED 0x80
#define ND_RA_FLAG_OTHER   0x40
#define ND_RA_FLAG_HA      0x20
/* Router preferences from RFC 4191. */
#define ND_RA_FLAG_RTPREF_MASK   0x18
#define ND_RA_FLAG_RTPREF_HIGH   0x08
#define ND_RA_FLAG_RTPREF_MEDIUM 0x00
#define ND_RA_FLAG_RTPREF_LOW    0x18
#define ND_RA_FLAG_RTPREF_RSV    0x10

struct nd_neighbor_solicit {
    struct icmp6_hdr nd_ns_hdr;
    struct in6_addr nd_ns_target;
};
#define nd_ns_type     nd_ns_hdr.icmp6_type
#define nd_ns_code     nd_ns_hdr.icmp6_code
#define nd_ns_cksum    nd_ns_hdr.icmp6_cksum
#define nd_ns_reserved nd_ns_hdr.icmp6_data32[0]
struct nd_neighbor_advert {
    struct icmp6_hdr nd_na_hdr;
    struct in6_addr nd_na_target;
};
#define nd_na_type           nd_na_hdr.icmp6_type
#define nd_na_code           nd_na_hdr.icmp6_code
#define nd_na_cksum          nd_na_hdr.icmp6_cksum
#define nd_na_flags_reserved nd_na_hdr.icmp6_data32[0]
#define ND_NA_FLAG_ROUTER    0x80000000UL
#define ND_NA_FLAG_SOLICITED 0x40000000UL
#define ND_NA_FLAG_OVERRIDE  0x20000000UL
struct nd_redirect {
    struct icmp6_hdr nd_rd_hdr;
    struct in6_addr nd_rd_target, nd_rd_dst;
};
#define nd_rd_type     nd_rd_hdr.icmp6_type
#define nd_rd_code     nd_rd_hdr.icmp6_code
#define nd_rd_cksum    nd_rd_hdr.icmp6_cksum
#define nd_rd_reserved nd_rd_hdr.icmp6_data32[0]

/* Option lengths count eight-octet units, including the option prefix. */
struct nd_opt_hdr { __UBYTE nd_opt_type, nd_opt_len; };
#define ND_OPT_SOURCE_LINKADDR    1
#define ND_OPT_TARGET_LINKADDR    2
#define ND_OPT_PREFIX_INFORMATION 3
#define ND_OPT_REDIRECTED_HEADER  4
#define ND_OPT_MTU                5
#define ND_OPT_NONCE             14
#define ND_OPT_ROUTE_INFO        24
#define ND_OPT_RDNSS             25
#define ND_OPT_DNSSL             31
#define ND_OPT_PREF64            38
#define ND_OPT_MAX ND_OPT_PREF64
struct nd_opt_prefix_info {
    __UBYTE nd_opt_pi_type, nd_opt_pi_len;
    __UBYTE nd_opt_pi_prefix_len, nd_opt_pi_flags_reserved;
    __ULONG nd_opt_pi_valid_time, nd_opt_pi_preferred_time, nd_opt_pi_reserved2;
    struct in6_addr nd_opt_pi_prefix;
};
#define ND_OPT_PI_FLAG_ONLINK 0x80
#define ND_OPT_PI_FLAG_AUTO   0x40
struct nd_opt_rd_hdr {
    __UBYTE nd_opt_rh_type, nd_opt_rh_len;
    __UWORD nd_opt_rh_reserved1;
    __ULONG nd_opt_rh_reserved2;
};
struct nd_opt_mtu {
    __UBYTE nd_opt_mtu_type, nd_opt_mtu_len;
    __UWORD nd_opt_mtu_reserved;
    __ULONG nd_opt_mtu_mtu;
};
/* SEND, route information, DNS and PREF64: RFC 3971/4191/8106/8781. */
#define ND_OPT_NONCE_LEN 6
struct nd_opt_nonce {
    __UBYTE nd_opt_nonce_type, nd_opt_nonce_len, nd_opt_nonce[ND_OPT_NONCE_LEN];
};
struct nd_opt_route_info {
    __UBYTE nd_opt_rti_type, nd_opt_rti_len, nd_opt_rti_prefixlen, nd_opt_rti_flags;
    __ULONG nd_opt_rti_lifetime;
};
struct nd_opt_rdnss {
    __UBYTE nd_opt_rdnss_type, nd_opt_rdnss_len;
    __UWORD nd_opt_rdnss_reserved;
    __ULONG nd_opt_rdnss_lifetime;
};
struct nd_opt_dnssl {
    __UBYTE nd_opt_dnssl_type, nd_opt_dnssl_len;
    __UWORD nd_opt_dnssl_reserved;
    __ULONG nd_opt_dnssl_lifetime;
};
struct nd_opt_pref64 {
    __UBYTE nd_opt_pref64_type, nd_opt_pref64_len;
    __UWORD nd_opt_pref64_sl_plc;
    char nd_opt_prefix[12];
};

/* Multicast listener discovery (RFC 2710 and 3810). */
struct mld_hdr {
    struct icmp6_hdr mld_icmp6_hdr;
    struct in6_addr mld_addr;
};
#define mld_type     mld_icmp6_hdr.icmp6_type
#define mld_code     mld_icmp6_hdr.icmp6_code
#define mld_cksum    mld_icmp6_hdr.icmp6_cksum
#define mld_maxdelay mld_icmp6_hdr.icmp6_data16[0]
#define mld_reserved mld_icmp6_hdr.icmp6_data16[1]
#define mld_v2_reserved mld_icmp6_hdr.icmp6_data16[0]
#define mld_v2_numrecs  mld_icmp6_hdr.icmp6_data16[1]
/* Fixed prefixes only: the source and record arrays follow on the wire. */
struct mldv2_query {
    struct icmp6_hdr mld_icmp6_hdr;
    struct in6_addr mld_addr;
    __UBYTE mld_misc, mld_qqi;
    __UWORD mld_numsrc;
};
struct mldv2_report { struct icmp6_hdr mld_icmp6_hdr; };
#define mld_numrecs mld_icmp6_hdr.icmp6_data16[1]
struct mldv2_record {
    __UBYTE mr_type, mr_datalen;
    __UWORD mr_numsrc;
    struct in6_addr mr_addr;
};
#define MLD_V2_QUERY_MINLEN sizeof(struct mldv2_query)
#define MLD_MRC_EXP(x)   ((ntohs(x) >> 12) & 7)
#define MLD_MRC_MANT(x)  (ntohs(x) & 0x0fff)
#define MLD_QQIC_EXP(x)  (((x) >> 4) & 7)
#define MLD_QQIC_MANT(x) ((x) & 15)
#define MLD_QRESV(x)     (((x) >> 4) & 15)
#define MLD_SFLAG(x)     (((x) >> 3) & 1)
#define MLD_QRV(x)       ((x) & 7)
#define MLD_V2_REPORT_MAXRECS 65535
#define MLD_DO_NOTHING             0
#define MLD_MODE_IS_INCLUDE        1
#define MLD_MODE_IS_EXCLUDE        2
#define MLD_CHANGE_TO_INCLUDE_MODE 3
#define MLD_CHANGE_TO_EXCLUDE_MODE 4
#define MLD_ALLOW_NEW_SOURCES      5
#define MLD_BLOCK_OLD_SOURCES      6

/* Node Information (RFC 4620). */
struct icmp6_nodeinfo {
    struct icmp6_hdr icmp6_ni_hdr;
    __UBYTE icmp6_ni_nonce[8];
};
#define ni_type  icmp6_ni_hdr.icmp6_type
#define ni_code  icmp6_ni_hdr.icmp6_code
#define ni_cksum icmp6_ni_hdr.icmp6_cksum
#define ni_qtype icmp6_ni_hdr.icmp6_data16[0]
#define ni_flags icmp6_ni_hdr.icmp6_data16[1]
#define ICMP6_NI_SUBJ_IPV6 0
#define ICMP6_NI_SUBJ_FQDN 1
#define ICMP6_NI_SUBJ_IPV4 2
#define ICMP6_NI_SUCCESS  0
#define ICMP6_NI_REFUSED  1
#define ICMP6_NI_UNKNOWN  2
#define NI_QTYPE_NOOP     0
#define NI_QTYPE_SUPTYPES 1
#define NI_QTYPE_FQDN     2
#define NI_QTYPE_DNSNAME  NI_QTYPE_FQDN
#define NI_QTYPE_NODEADDR 3
#define NI_QTYPE_IPV4ADDR 4
#define NI_SUPTYPE_FLAG_COMPRESS 0x0001
#define NI_FQDN_FLAG_VALIDTTL    0x0001
#define NI_NODEADDR_FLAG_TRUNCATE  0x0001
#define NI_NODEADDR_FLAG_ALL       0x0002
#define NI_NODEADDR_FLAG_COMPAT    0x0004
#define NI_NODEADDR_FLAG_LINKLOCAL 0x0008
#define NI_NODEADDR_FLAG_SITELOCAL 0x0010
#define NI_NODEADDR_FLAG_GLOBAL    0x0020
#define NI_NODEADDR_FLAG_ANYCAST   0x0040
struct ni_reply_fqdn {
    __ULONG ni_fqdn_ttl;
    __UBYTE ni_fqdn_namelen, ni_fqdn_name[3];
};

/* Router Renumbering (RFC 2894). */
struct icmp6_router_renum {
    struct icmp6_hdr rr_hdr;
    __UBYTE rr_segnum, rr_flags;
    __UWORD rr_maxdelay;
    __ULONG rr_reserved;
};
#define rr_type   rr_hdr.icmp6_type
#define rr_code   rr_hdr.icmp6_code
#define rr_cksum  rr_hdr.icmp6_cksum
#define rr_seqnum rr_hdr.icmp6_data32[0]
#define ICMP6_ROUTER_RENUMBERING_COMMAND 0
#define ICMP6_ROUTER_RENUMBERING_RESULT  1
#define ICMP6_ROUTER_RENUMBERING_SEQNUM_RESET 255
#define ICMP6_RR_FLAGS_TEST       0x80
#define ICMP6_RR_FLAGS_REQRESULT  0x40
#define ICMP6_RR_FLAGS_FORCEAPPLY 0x20
#define ICMP6_RR_FLAGS_SPECSITE   0x10
#define ICMP6_RR_FLAGS_PREVDONE   0x08
struct rr_pco_match {
    __UBYTE rpm_code, rpm_len, rpm_ordinal, rpm_matchlen, rpm_minlen, rpm_maxlen;
    __UWORD rpm_reserved;
    struct in6_addr rpm_prefix;
};
#define RPM_PCO_ADD       1
#define RPM_PCO_CHANGE    2
#define RPM_PCO_SETGLOBAL 3
#define RPM_PCO_MAX       4
struct rr_pco_use {
    __UBYTE rpu_uselen, rpu_keeplen, rpu_ramask, rpu_raflags;
    __ULONG rpu_vltime, rpu_pltime, rpu_flags;
    struct in6_addr rpu_prefix;
};
#define ICMP6_RR_PCOUSE_RAFLAGS_ONLINK 0x80
#define ICMP6_RR_PCOUSE_RAFLAGS_AUTO   0x40
#define ICMP6_RR_PCOUSE_FLAGS_DECRVLTIME 0x80000000UL
#define ICMP6_RR_PCOUSE_FLAGS_DECRPLTIME 0x40000000UL
struct rr_result {
    __UWORD rrr_flags;
    __UBYTE rrr_ordinal, rrr_matchedlen;
    __ULONG rrr_ifid;
    struct in6_addr rrr_prefix;
};
#define ICMP6_RR_RESULT_FLAGS_OOB       0x0002
#define ICMP6_RR_RESULT_FLAGS_FORBIDDEN 0x0001

/* Historical BSD/AROS application spellings. */
#define ICMP6_MEMBERSHIP_QUERY MLD_LISTENER_QUERY
#define ICMP6_MEMBERSHIP_REPORT MLD_LISTENER_REPORT
#define ICMP6_MEMBERSHIP_REDUCTION MLD_LISTENER_DONE
#define MLD6_LISTENER_QUERY MLD_LISTENER_QUERY
#define MLD6_LISTENER_REPORT MLD_LISTENER_REPORT
#define MLD6_LISTENER_DONE MLD_LISTENER_DONE
#define MLD6_MTRACE_RESP MLD_MTRACE_RESP
#define MLD6_MTRACE MLD_MTRACE
#define mld6_hdr mld_hdr
#define mld6_type mld_type
#define mld6_code mld_code
#define mld6_cksum mld_cksum
#define mld6_maxdelay mld_maxdelay
#define mld6_reserved mld_reserved
#define mld6_addr mld_addr
#define ICMP6_WRUREQUEST ICMP6_NI_QUERY
#define ICMP6_WRUREPLY ICMP6_NI_REPLY
#define ICMP6_FQDN_QUERY ICMP6_NI_QUERY
#define ICMP6_FQDN_REPLY ICMP6_NI_REPLY
struct icmp6_namelookup {
    struct icmp6_hdr icmp6_nl_hdr;
    __UBYTE icmp6_nl_nonce[8];
    __LONG icmp6_nl_ttl;
};

/* Proposed BSD filter ABI at IPPROTO_ICMPV6. Availability of these
 * declarations does not establish support in a loaded socket library.
 * A set bit passes a message, matching AROS and the RFC 3542 example.
 * Per-type macro arguments must be stable expressions; types are 0..255.
 */
#define ICMP6_FILTER 18
struct icmp6_filter { __ULONG icmp6_filt[8]; };
#define ICMP6_FILTER_WILLPASS(t, p) \
    (((p)->icmp6_filt[(t) >> 5] & (1UL << ((t) & 31))) != 0)
#define ICMP6_FILTER_WILLBLOCK(t, p) (!ICMP6_FILTER_WILLPASS(t, p))
#define ICMP6_FILTER_SETPASS(t, p) \
    ((void)((p)->icmp6_filt[(t) >> 5] |= (1UL << ((t) & 31))))
#define ICMP6_FILTER_SETBLOCK(t, p) \
    ((void)((p)->icmp6_filt[(t) >> 5] &= ~(1UL << ((t) & 31))))
/* Evaluate the pointer once, with no additional C runtime dependency. */
#define ICMP6_FILTER_SETPASSALL(p) do { \
    struct icmp6_filter *__icmp6_fp = (p); \
    int __icmp6_i; \
    for (__icmp6_i = 0; __icmp6_i < 8; ++__icmp6_i) \
        __icmp6_fp->icmp6_filt[__icmp6_i] = 0xffffffffUL; \
} while (0)
#define ICMP6_FILTER_SETBLOCKALL(p) do { \
    struct icmp6_filter *__icmp6_fp = (p); \
    int __icmp6_i; \
    for (__icmp6_i = 0; __icmp6_i < 8; ++__icmp6_i) \
        __icmp6_fp->icmp6_filt[__icmp6_i] = 0; \
} while (0)

#endif
