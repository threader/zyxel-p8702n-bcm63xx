The following files should be placed at the top level of Common Utility folder. The environment variable CMN_UTIL_BASE_DIR is to be exported to the path of Common utlity inside which various utilities are placed. 

1. make.rule : this file should contain the rules as to which utilities are to be built.
2. make.env : this file contain the environment variables corresponding to each utility.
3. makefile : as per the rules specified by the file make.rule, the make rules of individual utility shall be called. The default rule is to build the linux libraries whereas the individual rules for linux and Vxworks are also provided. Besides the make rule , clean rules are also provided to clean the libraries.

Note : Whenever any new utility is placed inside the common utility folder, the various entries are to be added in the above mentioned files so that the various static libraries can be created from a single location.
