#ifndef __COMMANDS_H__
#define __COMMANDS_H__

/*____________________________________________________________________________*/
/*	constant                                                                                                                     */
/*____________________________________________________________________________*/
#define	NARG		40	/* Max number of args to commands	*/

/*____________________________________________________________________________*/
/*	type definition                                                                                                             */
/*____________________________________________________________________________*/

typedef	struct cmds_s {
	char	*name;		/* Name of command			*/
	int	(*func)(int argc,char *argv[]);
				/* Function to execute command		*/
	int	flags;		/* ci flags (see flag definition)	*/
	int	argcmin;	/* Minimum number of args		*/
	char *help;		/* Message to print when show vaild commands */
	char	*argc_errmsg;	/* Message to print if insufficient args */
} cmds_t;

/*-----	flags definition -----------------------------------------------*/
#define	CAT1		0x02	/* catagory 1 - CI commands for everyone*/
#define	CAT2		0x01	/* catagory 2 - CI cmds for internal use*/
#define	CAT3		0x04	/* catagory 3 - for OEM only		*/
#define	CAT4		0x08	/* catagory 4 - for protected CI cmds */
#define SUBCMD		0x10	/* commands contains subcommands	*/

/*____________________________________________________________________________*/
/*	functions                                                                                                                     */
/*____________________________________________________________________________*/
int cmdparse(cmds_t cmds[], char *line);
int subcmd(const cmds_t tab[], int argc, char *argv[]);
void doZyims(char *cmdLine);

#endif

