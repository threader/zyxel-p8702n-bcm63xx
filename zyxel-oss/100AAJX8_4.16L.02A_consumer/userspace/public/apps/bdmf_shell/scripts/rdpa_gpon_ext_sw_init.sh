#!/bin/bash

# We need to down eth0 prior to starting runner's data path init
ifconfig eth0 down

# Start logger client
cat /dev/rgs_logger &

# Initialize bdmf shell
bdmf_shell -c init | while read a b ; do echo $b ; done > /var/bdmf_sh_id

# Start history recording
bs /bdmf/hist/init 0x20000 on

# Create system object
# XXX: Need to detect WAN type and other system parameters
bs /bdmf/new system/init_cfg={wan_type=gpon,emac_mode={group_mode=qsgmii,emac4_mode=rgmii,emac5_mode=sgmii},num_lan=4,enabled_emac=emac4,switching_mode=none,ip_class_method=fc,runner_ext_sw={enabled=yes,emac_id=emac4,hdr_type=type0}},cfg={mtu_size=1536}

# QoS mode configuration
bs /bdmf/configure system cfg={car_mode=true}

# XXX: Need to detect ports configuration
bs /bdmf/new port/index=switch,cfg={emac=emac4}
bs /bdmf/new port/index=lan0 port/index=switch
bs /bdmf/new port/index=lan1 port/index=switch
bs /bdmf/new port/index=lan2 port/index=switch
bs /bdmf/new port/index=lan3 port/index=switch

# Create DS egress_tm objects
bs /bdmf/new egress_tm/dir=ds,index=0,level=queue,mode=sp port/index=lan0
bs /bdmf/new egress_tm/dir=ds,index=1,level=queue,mode=sp port/index=lan1
bs /bdmf/new egress_tm/dir=ds,index=2,level=queue,mode=sp port/index=lan2
bs /bdmf/new egress_tm/dir=ds,index=3,level=queue,mode=sp port/index=lan3

# Condifure DS priority queues for LAN emac ports (emac ports schedulers have been already configured)
bs /bdmf/configure egress_tm/dir=ds,index=0 queue_cfg[0]={weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=1 queue_cfg[0]={weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=2 queue_cfg[0]={weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=3 queue_cfg[0]={weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes

# Configure CPU queue 0 for debugging purposes 
# Giving 0 (=NULL) for callback will let us to read packets from the queueu manually from shell.
bs /bdmf/configure cpu/index=host rxq_cfg[0]={rx_isr=0,sysb_type=skb,size=128,dump_data=no,isr_priv=0}

# Configure system bridge
bs /bdmf/new bridge

# Add IPTV object 
bs /bdmf/new iptv/lookup_method=group_ip_src_ip_vid

bs /bdmf/configure system cfg={inner_tpid=8100,outer_tpid=8100}


# Ingress Filters

bs /bdmf/new filter

# Filter: Ether-Type, ARP
bs /bdmf/configure filter entry[ { filter = etype_arp, ports = wan0  } ] = { enabled = yes, action = host }
bs /bdmf/configure filter entry[ { filter = etype_arp, ports = lan0  } ] = { enabled = yes, action = host }
bs /bdmf/configure filter entry[ { filter = etype_arp, ports = lan1  } ] = { enabled = yes, action = host }
bs /bdmf/configure filter entry[ { filter = etype_arp, ports = lan2  } ] = { enabled = yes, action = host }
bs /bdmf/configure filter entry[ { filter = etype_arp, ports = lan3  } ] = { enabled = yes, action = host }
bs /bdmf/configure filter entry[ { filter = etype_arp, ports = wlan0 } ] = { enabled = yes, action = host }

# Filter: Ether-Type, User-Defined, 0x888E
bs /bdmf/configure filter etype_udef[ 0 ] = 0x888E
bs /bdmf/configure filter entry[ { filter = etype_udef_0, ports = wlan0 } ] = { enabled = yes, action = host }

# Filter: Broadcast
bs /bdmf/configure filter entry[ { filter = bcast, ports = lan0  } ] = { enabled = yes, action = host }
bs /bdmf/configure filter entry[ { filter = bcast, ports = lan1  } ] = { enabled = yes, action = host }
bs /bdmf/configure filter entry[ { filter = bcast, ports = lan2  } ] = { enabled = yes, action = host }
bs /bdmf/configure filter entry[ { filter = bcast, ports = lan3  } ] = { enabled = yes, action = host }
bs /bdmf/configure filter entry[ { filter = bcast, ports = wlan0 } ] = { enabled = yes, action = host }

