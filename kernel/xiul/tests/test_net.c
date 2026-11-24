#include <assert.h>
#include <stdio.h>
#include "xiul/net.h"
static unsigned int received, transmitted;
static xiul_status_t input(xiul_netif_t *n, xiul_net_packet_t *p)
{ (void)n; ++received; return p->protocol == 0x0800 ? XIUL_OK : XIUL_E_UNSUPPORTED; }
static xiul_status_t transmit(xiul_netif_t *n, xiul_net_packet_t *p)
{ (void)n; (void)p; ++transmitted; return XIUL_OK; }
int main(void)
{
    char bytes[64];
    xiul_net_packet_t packet = {bytes, sizeof(bytes), 0x0800, 0};
    xiul_netif_t nic = {"eth0", 1500, 0, transmit};
    xiul_net_test_reset();
    assert(xiul_netif_register(&nic) == XIUL_OK);
    assert(xiul_netif_register(&nic) == XIUL_E_DUPLICATE);
    assert(xiul_net_set_input_handler(input) == XIUL_OK);
    assert(xiul_net_receive(&nic, &packet) == XIUL_OK && received == 1);
    assert(xiul_net_transmit(&nic, &packet) == XIUL_OK && transmitted == 1);
    packet.length = 1501;
    assert(xiul_net_transmit(&nic, &packet) == XIUL_E_INVALID);
    assert(xiul_netif_unregister(&nic) == XIUL_OK);
    assert(xiul_net_receive(&nic, &packet) == XIUL_E_INVALID);
    xiul_net_test_reset();
    puts("XIUL network interface tests: PASS");
    return 0;
}
