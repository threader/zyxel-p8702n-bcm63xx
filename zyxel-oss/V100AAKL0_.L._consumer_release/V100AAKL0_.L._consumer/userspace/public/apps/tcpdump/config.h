/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.in by autoheader.  */
/* "generated automatically" means DO NOT MAKE CHANGES TO config.h.in --
 * make them to acconfig.h and rerun autoheader */

/* Define if you enable IPv6 support */
/* #undef INET6 */

/* Define if you enable support for the libsmi. */
/* #undef LIBSMI */

/* define if you have struct __res_state_ext */
/* #undef HAVE_RES_STATE_EXT */

/* define if your struct __res_state has the nsort member */
/* #undef HAVE_NEW_RES_STATE */

/* define if you have the addrinfo function. */
#define HAVE_ADDRINFO 1

/* define if you need to include missing/addrinfoh.h. */
/* #undef NEED_ADDRINFO_H */

/* define ifyou have the h_errno variable. */
#define HAVE_H_ERRNO 1

/* define if IN6ADDRSZ is defined (XXX not used!) */
#define HAVE_IN6ADDRSZ 1

/* define if INADDRSZ is defined (XXX not used!) */
#define HAVE_INADDRSZ 1

/* define if RES_USE_INET6 is defined */
#define HAVE_RES_USE_INET6 1

/* define if you have struct sockaddr_storage */
#define HAVE_SOCKADDR_STORAGE 1

/* define if you have both getipnodebyname() and getipnodebyaddr() */
/* #undef USE_GETIPNODEBY */

/* define if you have ether_ntohost() and it works */
/* #undef USE_ETHER_NTOHOST */

/* define if libpcap has pcap_version */
/* #undef HAVE_PCAP_VERSION */

/* define if libpcap has pcap_debug */
/* #undef HAVE_PCAP_DEBUG */

/* define if libpcap has yydebug */
/* #undef HAVE_YYDEBUG */

/* define if libpcap has pcap_list_datalinks() */
/* #undef HAVE_PCAP_LIST_DATALINKS */

/* define if libpcap has pcap_set_datalink() */
/* #undef HAVE_PCAP_SET_DATALINK */

/* define if libpcap has pcap_datalink_name_to_val() */
/* #undef HAVE_PCAP_DATALINK_NAME_TO_VAL */
#define HAVE_PCAP_DATALINK_NAME_TO_VAL
/* define if libpcap has pcap_datalink_val_to_description() */
/* #undef HAVE_PCAP_DATALINK_VAL_TO_DESCRIPTION */

/* define if libpcap has pcap_dump_ftell() */
/* #undef HAVE_PCAP_DUMP_FTELL */

/* define if you have getrpcbynumber() */
#define HAVE_GETRPCBYNUMBER 1

/* AIX hack. */
/* #undef _SUN */

/* Workaround for missing 64-bit formats */
/* #undef PRId64 */
/* #undef PRIo64 */
/* #undef PRIx64 */
/* #undef PRIu64 */

/* Whether or not to include the possibly-buggy SMB printer */
#define TCPDUMP_DO_SMB 1

/* Define if you have the dnet_htoa function.  */
/* #undef HAVE_DNET_HTOA */

/* Define if you have a dnet_htoa declaration in <netdnet/dnetdb.h>.  */
/* #undef HAVE_NETDNET_DNETDB_H_DNET_HTOA */

/* define if should drop privileges by default */
/* #undef WITH_USER */

/* define if should chroot when dropping privileges */
/* #undef WITH_CHROOT */

/* Define to 1 if you have the `alarm' function. */
#define HAVE_ALARM 1

/* Define to 1 if you have the `bpf_dump' function. */
/* #undef HAVE_BPF_DUMP */

/* Define to 1 if you have the declaration of `ether_ntohost', and to 0 if you
   don't. */
/* #undef HAVE_DECL_ETHER_NTOHOST */

/* Define to 1 if you have the `ether_ntohost' function. */
/* #undef HAVE_ETHER_NTOHOST */

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* Define to 1 if you have the `getnameinfo' function. */
/* #undef HAVE_GETNAMEINFO */

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `crypto' library (-lcrypto). */
/* #undef HAVE_LIBCRYPTO */

/* Define to 1 if you have the `rpc' library (-lrpc). */
/* #undef HAVE_LIBRPC */

/* Define to 1 if you have the `smi' library (-lsmi). */
/* #undef HAVE_LIBSMI */

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the <netdnet/dnetdb.h> header file. */
/* #undef HAVE_NETDNET_DNETDB_H */

/* Define to 1 if you have the <netinet/ether.h> header file. */
/* #undef HAVE_NETINET_ETHER_H */

/* Define to 1 if you have the <netinet/if_ether.h> header file. */
#define HAVE_NETINET_IF_ETHER_H 1

/* Define to 1 if you have the <net/pfvar.h> header file. */
/* #undef HAVE_NET_PFVAR_H */

/* Define to 1 if you have the <openssl/evp.h> header file. */
#define HAVE_OPENSSL_EVP_H 1

/* if there's an os_proto.h for this platform, to use additional prototypes */
/* #undef HAVE_OS_PROTO_H */

/* Define to 1 if you have the <pcap/bluetooth.h> header file. */
/* #undef HAVE_PCAP_BLUETOOTH_H */

/* Define to 1 if you have the `pcap_breakloop' function. */
/* #undef HAVE_PCAP_BREAKLOOP */

/* Define to 1 if you have the `pcap_create' function. */
/* #undef HAVE_PCAP_CREATE */

/* Define to 1 if you have the `pcap_dump_flush' function. */
/* #undef HAVE_PCAP_DUMP_FLUSH */

/* Define to 1 if you have the `pcap_findalldevs' function. */
/* #undef HAVE_PCAP_FINDALLDEVS */

/* Define to 1 if the system has the type `pcap_if_t'. */
/* #undef HAVE_PCAP_IF_T */

/* Define to 1 if you have the `pcap_lib_version' function. */
/* #undef HAVE_PCAP_LIB_VERSION */

/* Define to 1 if you have the <pcap/usb.h> header file. */
#define HAVE_PCAP_USB_H 1

/* Define to 1 if you have the `pfopen' function. */
/* #undef HAVE_PFOPEN */

/* Define to 1 if you have the <rpc/rpcent.h> header file. */
/* #undef HAVE_RPC_RPCENT_H */

/* Define to 1 if you have the `setlinebuf' function. */
#define HAVE_SETLINEBUF 1

/* Define to 1 if you have the `sigaction' function. */
#define HAVE_SIGACTION 1

/* Define to 1 if you have the `sigset' function. */
/* #undef HAVE_SIGSET */

/* Define to 1 if you have the <smi.h> header file. */
/* #undef HAVE_SMI_H */

/* Define to 1 if you have the `snprintf' function. */
#define HAVE_SNPRINTF 1

/* if struct sockaddr has the sa_len member */
/* #undef HAVE_SOCKADDR_SA_LEN */

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strcasecmp' function. */
#define HAVE_STRCASECMP 1

/* Define to 1 if you have the `strdup' function. */
#define HAVE_STRDUP 1

/* Define to 1 if you have the `strftime' function. */
#define HAVE_STRFTIME 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strlcat' function. */
#define HAVE_STRLCAT 1

/* Define to 1 if you have the `strlcpy' function. */
#define HAVE_STRLCPY 1

/* Define to 1 if you have the `strsep' function. */
#define HAVE_STRSEP 1

/* Define to 1 if the system has the type `struct ether_addr'. */
/* #undef HAVE_STRUCT_ETHER_ADDR */

/* Define to 1 if you have the <sys/bitypes.h> header file. */
#define HAVE_SYS_BITYPES_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the `vfprintf' function. */
#define HAVE_VFPRINTF 1

/* Define to 1 if you have the `vsnprintf' function. */
#define HAVE_VSNPRINTF 1

/* define if your compiler has __attribute__ */
#define HAVE___ATTRIBUTE__ 1

/* if unaligned access fails */
#define LBL_ALIGN 1

/* Define to 1 if netinet/ether.h declares `ether_ntohost' */
/* #undef NETINET_ETHER_H_DECLARES_ETHER_NTOHOST */

/* Define to 1 if netinet/if_ether.h declares `ether_ntohost' */
/* #undef NETINET_IF_ETHER_H_DECLARES_ETHER_NTOHOST */

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME ""

/* Define to the full name and version of this package. */
#define PACKAGE_STRING ""

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME ""

/* Define to the version of this package. */
#define PACKAGE_VERSION ""

/* Define as the return type of signal handlers (`int' or `void'). */
#define RETSIGTYPE void

/* return value of signal handlers */
#define RETSIGVAL 

/* The size of `char', as computed by sizeof. */
#define SIZEOF_CHAR 1

/* The size of `int', as computed by sizeof. */
#define SIZEOF_INT 4

/* The size of `long', as computed by sizeof. */
#define SIZEOF_LONG 4

/* The size of `long long', as computed by sizeof. */
#define SIZEOF_LONG_LONG 8

/* The size of `short', as computed by sizeof. */
#define SIZEOF_SHORT 2

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define to 1 if you can safely include both <sys/time.h> and <time.h>. */
#define TIME_WITH_SYS_TIME 1

/* get BSD semantics on Irix */
/* #undef _BSD_SIGNALS */

/* needed on HP-UX */
/* #undef _HPUX_SOURCE */

/* define if your compiler allows __attribute__((format)) to be applied to
   function pointers */
#define __ATTRIBUTE___FORMAT_OK_FOR_FUNCTION_POINTERS 1

/* to handle Ultrix compilers that don't support const in prototypes */
/* #undef const */

/* Define as token for inline if inlining supported */
#define inline inline

/* Define to `short' if int16_t not defined. */
/* #undef int16_t */

/* Define to `int' if int32_t not defined. */
/* #undef int32_t */

/* Define to `long long' if int64_t not defined. */
/* #undef int64_t */

/* Define to `signed char' if int8_t not defined. */
/* #undef int8_t */

/* Define to `unsigned short' if u_int16_t not defined. */
/* #undef u_int16_t */

/* Define to `unsigned int' if u_int32_t not defined. */
/* #undef u_int32_t */

/* Define to `unsigned long long' if u_int64_t not defined. */
/* #undef u_int64_t */

/* Define to `unsigned char' if u_int8_t not defined. */
/* #undef u_int8_t */
