//**************** start of macors *************************


//******* Macros for ipv4 or ipv6 <set> related ************ 


// settting family in sockaddr_storage

#define ASdf_mc_set_familyIntoSockaddrStorage(x,y) \
do\
{\
	(x)->ss_family = (sa_family_t) y;\
}\
while(0)


// setting Sin_addr or Sin6_addr 

#define ASdf_mc_set_SaddrIntoSockaddrStorage(x, y) \
do\
{\
	if ((x)->ss_family == AF_INET)\
	{\
		((struct sockaddr_in *)x)->sin_addr=  y;\
	}\
}\
while(0)

#define ASdf_mc_set_S6addrIntoSockaddrStorage(x, y) \
do\
{\
	if ((x)->ss_family == AF_INET6)\
	{\
		((struct sockaddr_in6 *)x)->sin6_addr=  y;\
	}\
}\
while(0)


// setting port 

#define ASdf_mc_set_portIntoSockaddrStorage(x, y) \
do\
{\
	if ((x)->ss_family == AF_INET)\
	{\
		((struct sockaddr_in *)x)->sin_port = (in_port_t) y;\
	}\
	else if((x)->ss_family == AF_INET6)\
	{\
		((struct sockaddr_in6 *)x)->sin6_port = (in_port_t) y;\
	}\
}\
while(0)


// setting s_addr or s6_addr 

#define ASdf_mc_set_inaddrIntoSockaddrStorage(x,y) \
do\
{\
	if ((x)->ss_family == AF_INET)\
	{\
		((struct sockaddr_in *)x)->sin_addr.s_addr= y;\
	}\
	else if((x)->ss_family == AF_INET6)\
	{\
		((struct sockaddr_in6 *)x)->sin6_addr.s6_addr= y;\
	}\
}\
while(0)

//************Macros for ipv4 or ipv6 <get> related********* 


// getting family from sockaddr_storage 

#define ASdf_mc_get_familyFromSockaddrStorage(x) (x)->ss_family


// marcro for getting the port

#define ASdf_mc_get_portFromSockaddrStorage(x) \
	((x)->ss_family == AF_INET) ? (((struct sockaddr_in *)x)->sin_port) \
	: (((struct sockaddr_in6 *)x)->sin6_port)

// macro for getting the Sin_addr or Sin6_addr 

#define ASdf_mc_get_addrFromSockaddrStorage(x) \
	(((x)->ss_family == AF_INET) ? (void *)(&(((struct sockaddr_in *)x)->sin_addr)) \
	: (void *)(&(((struct sockaddr_in6 *)x)->sin6_addr)))

// macro for getting the s_addr or s6_addr 

#define ASdf_mc_get_SaddrFromSockaddrStorage(x) \
	((x)->ss_family == AF_INET) ? (((struct sockaddr_in *)x)->sin_addr.s_addr) \
	: (((struct sockaddr_in6 *)x)->sin6_addr.s6_addr) 

//************* End of Macros **********************************



