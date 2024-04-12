#!/bin/bash

# We need to down eth0 prior to starting runner's data path init
ifconfig eth0 down

# Initialize bdmf shell
bdmf_shell -c init | while read a b ; do echo $b ; done > /var/bdmf_sh_id
alias bs="bdmf_shell -c `cat /var/bdmf_sh_id` -cmd "

dmesg -n 8

# Start history recording
bs /bdmf/hist/init 0x20000 on

# Create system object

rdpawantype=`/bin/pspctl dump RdpaWanType`

wantype="DSL"

if [ "$rdpawantype" = "DSL" ]
then
    echo "Detected WAN Type = DSL"
elif [ "$rdpawantype" = "GBE" ]
then
    echo "Detected WAN Type = GBE"
    wantype="GBE"
else
    echo "Could not detect WAN Type, defaulting to DSL"
    `/bin/pspctl set RdpaWanType DSL`
fi

if [ "$wantype" = "DSL" ]
then
    bs /bdmf/new system/init_cfg={wan_type=dsl,num_wan=1,num_lan=7,enabled_emac=emac0+emac1,switching_mode=none,ip_class_method=fc,runner_ext_sw={enabled=yes,emac_id=emac1,hdr_type=type0}},cfg={mtu_size=1536}
else
    bs /bdmf/new system/init_cfg={wan_type=gbe,num_wan=1,num_lan=7,enabled_emac=emac0+emac1,wan_emac=emac0,switching_mode=none,ip_class_method=fc,runner_ext_sw={enabled=yes,emac_id=emac1,hdr_type=type0}},cfg={mtu_size=1536}
    bs /bdmf/new port/index=wan0,cfg={emac=emac0}
fi

source /etc/rdpa_dsl_common_init.sh
