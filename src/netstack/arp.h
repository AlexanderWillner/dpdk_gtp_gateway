/**
 * arp.h - arp data structure
 *  reference: https://github.com/vipinpv85/GTP_PKT_DECODE
 */
#ifndef __ARP_H_
#define __ARP_H_

#include <rte_common.h>
#include <rte_ether.h>

#include "logger.h"

typedef enum {
    ARP_REQ = 1,
    ARP_REPLY,
    RARP_REQ,
    RARP_REPLY,
} arp_type;

#define HW_TYPE_ETHERNET 1
#define SW_TYPE_IPV4 0x0800
#define HW_LEN_ETHER 6
#define PR_LEN_IPV4 4
//http://www.tcpipguide.com/free/t_ARPMessageFormat.htm // for arp format.
struct arp {
    uint16_t hw_type;
    uint16_t pr_type;
    uint8_t hw_len;
    uint8_t pr_len;
    uint16_t opcode;
    unsigned char src_hw_add[6];
    unsigned char src_pr_add[4];
    unsigned char dst_hw_add[6];
    unsigned char dst_pr_add[4];
} __packed__;

typedef enum {
    FREE = 0,
    PENDING,
    RESOLVED,
} arp_state;

struct arp_map {
    uint32_t ipv4_addr;
    unsigned char mac_addr[6];
    struct arp_map *next;
};

struct rte_mbuf_queue {
    struct rte_mbuf *m;
    struct rte_mbuf_queue *next;
};

struct arp_entry {
    // unsigned char pr_addr[4];
    unsigned char hw_addr[RTE_ETHER_ADDR_LEN];
    uint16_t ttl;
    uint8_t tries;
    arp_state state;
    struct rte_mbuf_queue *queue;
};

int arp_in(struct rte_mbuf *mbuf);

int get_mac(uint32_t ipv4_addr, unsigned char *mac_addr);
int add_mac(uint32_t ipv4_addr, unsigned char *mac_addr);
int get_arp_table(char *buffer, int len);

int send_arp_request(unsigned char *src_pr_add, unsigned char *dst_pr_add);
int send_arp_reply(unsigned char *src_hw_addr, unsigned char *src_pr_add, unsigned char *dst_pr_add);
int send_arp(struct arp *arp_pkt, uint8_t port);

void print_ipv4(uint32_t ip_addr, TraceLevel trace_level);
int print_ipv4_in_buf(uint32_t ip_add, char *buffer);
void print_arp_table(TraceLevel trace_level);
void print_mac(unsigned char *mac_addr, TraceLevel trace_level);

#endif /* __ARP_H_ */
