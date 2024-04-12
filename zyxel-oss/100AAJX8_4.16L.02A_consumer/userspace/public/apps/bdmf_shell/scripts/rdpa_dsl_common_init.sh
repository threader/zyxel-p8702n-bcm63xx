#!/bin/bash

echo "DSL Common Init Script"

# XXX: Need to detect ports configuration
# Below lanX means rdpa_if_lanX
# For 63138 - rdpa_if_lan6 represents the SF2 Port#7
bs /bdmf/new port/index=switch,cfg={emac=emac1}
bs /bdmf/new port/index=lan0 port/index=switch
bs /bdmf/new port/index=lan1 port/index=switch
bs /bdmf/new port/index=lan2 port/index=switch
bs /bdmf/new port/index=lan3 port/index=switch
bs /bdmf/new port/index=lan4 port/index=switch
bs /bdmf/new port/index=lan5 port/index=switch
bs /bdmf/new port/index=lan6 port/index=switch

# Create DS egress_tm objects
bs /bdmf/new egress_tm/dir=ds,index=0,level=queue,mode=sp port/index=lan0
bs /bdmf/new egress_tm/dir=ds,index=1,level=queue,mode=sp port/index=lan1
bs /bdmf/new egress_tm/dir=ds,index=2,level=queue,mode=sp port/index=lan2
bs /bdmf/new egress_tm/dir=ds,index=3,level=queue,mode=sp port/index=lan3
bs /bdmf/new egress_tm/dir=ds,index=4,level=queue,mode=sp port/index=lan4
bs /bdmf/new egress_tm/dir=ds,index=5,level=queue,mode=sp port/index=lan5
bs /bdmf/new egress_tm/dir=ds,index=6,level=queue,mode=sp port/index=lan6

# Configure DS priority queues for LAN emac ports (emac ports schedulers have been already configured)
bs /bdmf/configure egress_tm/dir=ds,index=0 queue_cfg[0]={queue_id=7,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=0 queue_cfg[1]={queue_id=6,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=0 queue_cfg[2]={queue_id=5,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=0 queue_cfg[3]={queue_id=4,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=0 queue_cfg[4]={queue_id=3,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=0 queue_cfg[5]={queue_id=2,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=0 queue_cfg[6]={queue_id=1,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=0 queue_cfg[7]={queue_id=0,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes

bs /bdmf/configure egress_tm/dir=ds,index=1 queue_cfg[0]={queue_id=7,weight=0,drop_alg=dt,drop_threshold=1500,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=1 queue_cfg[1]={queue_id=6,weight=0,drop_alg=dt,drop_threshold=1500,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=1 queue_cfg[2]={queue_id=5,weight=0,drop_alg=dt,drop_threshold=1500,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=1 queue_cfg[3]={queue_id=4,weight=0,drop_alg=dt,drop_threshold=1500,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=1 queue_cfg[4]={queue_id=3,weight=0,drop_alg=dt,drop_threshold=1500,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=1 queue_cfg[5]={queue_id=2,weight=0,drop_alg=dt,drop_threshold=1500,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=1 queue_cfg[6]={queue_id=1,weight=0,drop_alg=dt,drop_threshold=1500,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=1 queue_cfg[7]={queue_id=0,weight=0,drop_alg=dt,drop_threshold=1500,red_high_threshold=0,red_low_threshold=0},enable=yes

bs /bdmf/configure egress_tm/dir=ds,index=2 queue_cfg[0]={queue_id=7,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=2 queue_cfg[1]={queue_id=6,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=2 queue_cfg[2]={queue_id=5,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=2 queue_cfg[3]={queue_id=4,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=2 queue_cfg[4]={queue_id=3,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=2 queue_cfg[5]={queue_id=2,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=2 queue_cfg[6]={queue_id=1,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=2 queue_cfg[7]={queue_id=0,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes

bs /bdmf/configure egress_tm/dir=ds,index=3 queue_cfg[0]={queue_id=7,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=3 queue_cfg[1]={queue_id=6,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=3 queue_cfg[2]={queue_id=5,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=3 queue_cfg[3]={queue_id=4,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=3 queue_cfg[4]={queue_id=3,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=3 queue_cfg[5]={queue_id=2,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=3 queue_cfg[6]={queue_id=1,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=3 queue_cfg[7]={queue_id=0,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes

bs /bdmf/configure egress_tm/dir=ds,index=4 queue_cfg[0]={queue_id=7,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=4 queue_cfg[1]={queue_id=6,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=4 queue_cfg[2]={queue_id=5,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=4 queue_cfg[3]={queue_id=4,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=4 queue_cfg[4]={queue_id=3,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=4 queue_cfg[5]={queue_id=2,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=4 queue_cfg[6]={queue_id=1,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=4 queue_cfg[7]={queue_id=0,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes

bs /bdmf/configure egress_tm/dir=ds,index=5 queue_cfg[0]={queue_id=7,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=5 queue_cfg[1]={queue_id=6,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=5 queue_cfg[2]={queue_id=5,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=5 queue_cfg[3]={queue_id=4,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=5 queue_cfg[4]={queue_id=3,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=5 queue_cfg[5]={queue_id=2,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=5 queue_cfg[6]={queue_id=1,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=5 queue_cfg[7]={queue_id=0,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes

bs /bdmf/configure egress_tm/dir=ds,index=6 queue_cfg[0]={queue_id=7,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=6 queue_cfg[1]={queue_id=6,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=6 queue_cfg[2]={queue_id=5,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=6 queue_cfg[3]={queue_id=4,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=6 queue_cfg[4]={queue_id=3,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=6 queue_cfg[5]={queue_id=2,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=6 queue_cfg[6]={queue_id=1,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes
bs /bdmf/configure egress_tm/dir=ds,index=6 queue_cfg[7]={queue_id=0,weight=0,drop_alg=dt,drop_threshold=128,red_high_threshold=0,red_low_threshold=0},enable=yes

