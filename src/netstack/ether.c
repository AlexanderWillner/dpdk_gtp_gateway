/**
 * ether.c
 *  reference: https://github.com/vipinpv85/GTP_PKT_DECODE
 */
#include "ether.h"

#include <rte_common.h>
#include <string.h>

#include "arp.h"
#include "stdio.h"

struct Interface *InterfaceList = NULL;

uint32_t GetIntAddFromChar(unsigned char *address, uint8_t order) {
    uint32_t ip_add = 0;
    int i;
    printf("Converting address for \n");
    for (i = 0; i < 4; i++) {
        printf("%d ", address[i]);
    }
    for (i = 0; i < 4; i++) {
        ip_add = ip_add << 8;
        if (order == 1) {
            ip_add = ip_add | address[3 - i];
        }
        if (order == 0) {
            ip_add = ip_add | address[i];
        }
    }
    return ip_add;
}

void SetInterfaceHW(uint8_t *MacAddr, uint8_t interface) {
    printf("setting interface %u\n", interface);
    if (interface < MAX_INTERFACES) {
        memcpy(InterfaceHwAddr[interface], MacAddr, HW_ADDRESS_LEN);
    } else {
        printf("ERROR :: interfcae number more than max.\n");
    }
}

void InitInterface(struct Interface *IfList[], unsigned int Count) {
    struct Interface *ptr = NULL;
    unsigned int i = 0;
    for (i = 0; i < Count; i++) {
        ptr = malloc(sizeof(struct Interface));
        memcpy(ptr, IfList[i], sizeof(struct Interface));
        ptr->next = NULL;
        if (i == 0) {
            InterfaceList = ptr;
        } else {
            InterfaceList->next = ptr;
        }
        uint32_t Ipv4Addr = GetIntAddFromChar(ptr->ip, 0);
        //  ptr->ip[0] | ptr->ip[1] << 8 | ptr->ip[2] << 16 | ptr->ip[3] << 24 ;
        //printf("assembled mac address = %x\n", Ipv4Addr);
        add_mac(Ipv4Addr, ptr->hw_addr);
    }
}

void AddInterface(struct Interface *Iface) {
    struct Interface *ptr = NULL;
    ptr = malloc(sizeof(struct Interface));
    memcpy(ptr, Iface, sizeof(struct Interface));
    ptr->next = NULL;
    if (InterfaceList == NULL) {
        InterfaceList = ptr;
    } else {
        InterfaceList->next = ptr;
    }
    uint32_t Ipv4Addr = GetIntAddFromChar(ptr->ip, 0);
    //  ptr->ip[0] | ptr->ip[1] << 8 | ptr->ip[2] << 16 | ptr->ip[3] << 24 ;
    //printf("assembled mac address = %x\n", Ipv4Addr);
    add_mac(Ipv4Addr, ptr->hw_addr);
}

uint8_t GetInterfaceMac(uint8_t iface_num, uint8_t *mac) {
    struct Interface *temp = NULL;
    temp = InterfaceList;

    while (temp && (temp->iface_num != iface_num)) {
        temp = temp->next;
    }

    if (temp) {
        memcpy(mac, temp->hw_addr, 6);
        return 1;
    }
    return 0;
}
