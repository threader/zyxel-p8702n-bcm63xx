/*****************************************************************************
 **
 ** FILENAME:		iptkapp_cfg_read_cfg.c
 **
 ** DESCRIPTION:	This file contains the functionality to handle initial 
 **                 configuration of IPTK App
 **
 ** DATE			NAME		REFERENCE		REASON
 ** ----			----		---------		------
 **	05-DEC-07		Amit Sikka       			Initial Creation
 *****************************************************************************
 **			Copyright (c) 2007 Aricent Inc . All Rights Reserved.
 *****************************************************************************/

#include "iptkapp_macros.h"
#include "iptkapp_cfg_types.h"

/* Global Database for config parameters */
iptkapp_cfg_db_t g_iptkapp_cfg_db_t;

/******************************************************************************
 *
 * FUNCTION:        iptkapp_cfg_read_config_file
 *
 * DESCRIPTION:     This function reads the config file and fills config DB
 *                  accordingly
 *
 *****************************************************************************/
iptkapp_return_t iptkapp_cfg_read_config_file()
{
   FILE *               fileptr;
   icf_int8_t          line[100];
   icf_int8_t          *token;
   icf_int8_t          *filename = (icf_int8_t *)"iptkapp.cfg";
   icf_int8_t          cfg_filename[IPTKAPP_CONF_FILE_NAME_SIZE]={0};
   icf_int32_t         ch,counter;
   icf_int8_t          exit_flag = 0;
   icf_int8_t          token_validation[MAX_TOKENS_CONF_FILE]={0};
   icf_int8_t          token_err_string[MAX_TOKENS_CONF_FILE][50] = 
   {
       {"IPTKAPP_IP token is missing in file\0"},
       {"IPTKAPP_PORT token is missing in file\0"},
       {"IPTK_IP token is missing in file\0"},
       {"IPTK_PORT token is missing in file\0"},
       {"IPTK_NETWORK_PORT token is missing in file\0"},
       {"PROXY_IP token is missing in file\0"},
       {"PROXY_PORT token is missing in file\0"},
       {"REGISTRAR_IP token is missing in file\0"},
       {"REGISTRAR_PORT token is missing in file\0"},
       {"REGISTRAR_DURATION token is missing in file\0"},
       {"SELF_ID_URL token is missing in file\0"},
       {"AUTHENTICATION_KEY token is missing in file\0"},
       {"AUTHENTICATION_PASSWORD token is missing in file\0"}
   };


   
   iptkapp_port_strncpy(cfg_filename, filename, 
           (IPTKAPP_CONF_FILE_NAME_SIZE));

   if (IPTKAPP_NULL == (fileptr = fopen(cfg_filename, "r")))
   {
       IPTKAPP_PRINTF("\n\tIPTKAPP > Unable to open config file\n");
       return (IPTKAPP_FAILURE);
   }

   /* If the file is empty */
   if(EOF == (ch = fgetc(fileptr)))
   {
       fclose(fileptr);
       return (IPTKAPP_FAILURE);
   }

   ungetc(ch, fileptr);
   while(fgets(line,100,fileptr) != IPTKAPP_NULL)
   {
      /* skip the line starting with # */
      if('#' == line[0])
          continue;
   
      token = iptkapp_port_strtok(line, (icf_int8_t *)" \t\n");

      /* If line starts with white space, then strtok returns IPTKAPP_NULL.
       * So, in this case, read next line.
       * */
      if(IPTKAPP_NULL == token)
      {
          continue;
      }

      /* Reading IPTK_APP IP */
      if(!iptkapp_port_strcmp("IPTK_APP_IP:",(icf_int8_t *)token))
      {
          
          token_validation[IPTK_APP_IP]=1;
          token = iptkapp_port_strtok(IPTKAPP_NULL, (icf_int8_t *)" \t\n");
          if(IPTKAPP_NULL == token)
          {
              printf("\nIPTKAPP  :>   IPTK_APP_IP not present in config file\n");
              return(IPTKAPP_FAILURE);
          }
          else
          {
              sscanf(token,"%s",(icf_int8_t *)g_iptkapp_cfg_db_t.iptk_app_ip);
          }
          continue;

      }
      
      /* Reading IPTK_APP Port */
      else if(!strcmp("IPTK_APP_PORT:",(icf_int8_t *)token))
      {
          token_validation[IPTK_APP_PORT]=1;
          token = iptkapp_port_strtok(IPTKAPP_NULL, (icf_int8_t *)" \t\n");
          if(IPTKAPP_NULL == token)
          {
              printf("\nIPTKAPP  :>   IPTK_APP_Port not present in config file\n");
              return(IPTKAPP_FAILURE);
          }
          else
          {
              g_iptkapp_cfg_db_t.iptk_app_port = atoi(token);
          }
          continue;
      }
      
      /* Reading IPTK IP */
      else if(!strcmp("IPTK_IP:",(icf_int8_t *)token))
      {
          token_validation[IPTK_IP]=1;
          token = iptkapp_port_strtok(IPTKAPP_NULL, (icf_int8_t *)" \t\n");
          if(IPTKAPP_NULL == token)
          {
              printf("\nIPTKAPP  :>   IPTK_IP not present in config file\n");
              return(IPTKAPP_FAILURE);
          }
          else
          {
              sscanf(token,"%s",(icf_int8_t *)g_iptkapp_cfg_db_t.iptk_ip);
          }

          continue;
      }
      
      /* Reading IPTK Port */
      else if(!strcmp("IPTK_PORT:",(icf_int8_t *)token))
      {
          token_validation[IPTK_PORT]=1;
          token = iptkapp_port_strtok(IPTKAPP_NULL, (icf_int8_t *)" \t\n");
          if(IPTKAPP_NULL == token)
          {
              printf("\nIPTKAPP  :>   IPTK_Port not present in config file\n");
              return(IPTKAPP_FAILURE);
          }
          else
          {
              g_iptkapp_cfg_db_t.iptk_port = atoi(token);
          }

          continue;
      }

      /* Reading IPTK n/w Port */
      else if(!strcmp("IPTK_NETWORK_PORT:",(icf_int8_t *)token))
      {
          token_validation[IPTK_NETWORK_PORT]=1;
          token = iptkapp_port_strtok(IPTKAPP_NULL, (icf_int8_t *)" \t\n");
          if(IPTKAPP_NULL == token)
          {
              printf("\nIPTKAPP  :>   IPTK_NW_Port not present in config file\n");
              return(IPTKAPP_FAILURE);
          }
          else
          {
              g_iptkapp_cfg_db_t.iptk_nw_port = atoi(token);
          }

          continue;
      }
      
      /* Reading Proxy IP */
      else if(!strcmp("PROXY_IP:",(icf_int8_t *)token))
      {
          token_validation[PROXY_IP]=1;
          token = iptkapp_port_strtok(IPTKAPP_NULL, (icf_int8_t *)" \t\n");
          if(IPTKAPP_NULL == token)
          {
              printf("\nIPTKAPP  :>   Proxy IP not present in config file\n");
              return(IPTKAPP_FAILURE);
          }
          else
          {
              sscanf(token,"%s",(icf_int8_t *)g_iptkapp_cfg_db_t.proxy_ip);
          }
          continue;
      }

      /* Reading Proxy Port */
      else if(!strcmp("PROXY_PORT:",(icf_int8_t *)token))
      {
          token_validation[PROXY_PORT]=1;
          token = iptkapp_port_strtok(IPTKAPP_NULL, (icf_int8_t *)" \t\n");
          if(IPTKAPP_NULL == token)
          {
              printf("\nIPTKAPP  :>   Proxy Port not present in config file\n");
              return(IPTKAPP_FAILURE);
          }
          else
          {
              g_iptkapp_cfg_db_t.proxy_port = atoi(token);
          }
          continue;
      }

      /* Reading Registrar IP*/
      else if(!strcmp("REGISTRAR_IP:",(icf_int8_t *)token))
      {
          token_validation[REGISTRAR_IP]=1;
          token = iptkapp_port_strtok(IPTKAPP_NULL, (icf_int8_t *)" \t\n");
          if(IPTKAPP_NULL == token)
          {
              printf("\nIPTKAPP  :>   Registrar IP not present in config file\n");
              return(IPTKAPP_FAILURE);
          }
          else
          {
              sscanf(token,"%s",(icf_int8_t *)g_iptkapp_cfg_db_t.registrar_ip);
          }
          continue;
      }

      /* Reading Registrar Port */
      else if(!strcmp("REGISTRAR_PORT:",(icf_int8_t *)token))
      {
          token_validation[REGISTRAR_PORT]=1;
          token = iptkapp_port_strtok(IPTKAPP_NULL, (icf_int8_t *)" \t\n");
          if(IPTKAPP_NULL == token)
          {
              printf("\nIPTKAPP  :>   Registrar Port not present in config file\n");
              return(IPTKAPP_FAILURE);
          }
          else
          {
              g_iptkapp_cfg_db_t.registrar_port = atoi(token);
          }
          continue;
      }

      /* Reading Registrar duration */
      else if(!strcmp("REGISTER_DURATION:",(icf_int8_t *)token))
      {
          token_validation[REGISTER_DURATION]=1;
          token = iptkapp_port_strtok(IPTKAPP_NULL, (icf_int8_t *)" \t\n");
          if(IPTKAPP_NULL == token)
          {
              printf("\nIPTKAPP  :>   Registration duration not present in config file\n");
              return(IPTKAPP_FAILURE);
          }
          else
          {
              g_iptkapp_cfg_db_t.registrar_duration = atoi(token);
          }
          continue;
      }


      /* Reading SELF_ID_URL */
      else if(!strcmp("SELF_ID_URL:",(icf_int8_t *)token))
      {
          token_validation[SELF_ID_URL]=1;
          token = iptkapp_port_strtok(IPTKAPP_NULL, (icf_int8_t *)" \t\n");
          if(IPTKAPP_NULL == token)
          {
              printf("\nIPTKAPP  :>   SELF_ID_URL not present in config file\n");
              return(IPTKAPP_FAILURE);
          }
          else
          {
              sscanf(token,"%s",(icf_int8_t *)g_iptkapp_cfg_db_t.user_address);
          }
          continue;
      }

      /* Reading authentication key */

      else if(!strcmp("AUTHENTICATION_KEY:",(icf_int8_t *)token))
      {
          token_validation[AUTHENTICATION_KEY]=1;
          token = iptkapp_port_strtok(IPTKAPP_NULL, (icf_int8_t *)" \t\n");
          if(IPTKAPP_NULL == token)
          {
              printf("\nIPTKAPP  :>   SELF_ID_URL not present in config file\n");
              return(IPTKAPP_FAILURE);
          }
          else
          {
              sscanf(token,"%s",(icf_int8_t *)g_iptkapp_cfg_db_t.authentication_key);
          }
          continue;
      }

      /* Reading authentication Password */

      else if(!strcmp("AUTHENTICATION_PASSWORD:",(icf_int8_t *)token))
      {
          token_validation[AUTHENTICATION_PASSWORD]=1;
          token = iptkapp_port_strtok(IPTKAPP_NULL, (icf_int8_t *)" \t\n");
          if(IPTKAPP_NULL == token)
          {
              printf("\nIPTKAPP  :>   SELF_ID_URL not present in config file\n");
              return(IPTKAPP_FAILURE);
          }
          else
          {
              sscanf(token,"%s",(icf_int8_t *)g_iptkapp_cfg_db_t.authentication_password);
          }
          continue;
      }
      token = iptkapp_port_strtok(IPTKAPP_NULL, (icf_int8_t *)" \t\n");
   }

   fclose(fileptr);
   for(counter=0;counter<MAX_TOKENS_CONF_FILE;counter++)
   {
       if (0 == token_validation[counter])
       {
           printf("\n\tIPTKAPP > %s",token_err_string[counter]);
           exit_flag=1;
        }
   }

   if (1==exit_flag)
       return IPTKAPP_FAILURE;
   return(IPTKAPP_SUCCESS);
}


/******************************************************************************
 *
 * FUNCTION:        iptkapp_cfg_display_cfg_db
 *
 * DESCRIPTION:     This function displays the config DB structure
 *
 *****************************************************************************/

void iptkapp_cfg_display_cfg_db()
{
    printf("\n\tIPTKAPP_IP is %s\n",g_iptkapp_cfg_db_t.iptk_app_ip);
    printf("\n\tIPTKAPP_PORT is %d\n",g_iptkapp_cfg_db_t.iptk_app_port);

    printf("\n\tIPTK_IP is %s\n",g_iptkapp_cfg_db_t.iptk_ip);
    printf("\n\tIPTK_PORT is %d\n",g_iptkapp_cfg_db_t.iptk_port);

    printf("\n\tIPTK_PROXY_IP is %s\n",g_iptkapp_cfg_db_t.proxy_ip);
    printf("\n\tIPTK_PROXY_PORT is %d\n",g_iptkapp_cfg_db_t.proxy_port);

    printf("\n\tIPTK_REGISTRAR_IP is %s\n",g_iptkapp_cfg_db_t.registrar_ip);
    printf("\n\tIPTK_REGISTRAR_PORT is %d\n",g_iptkapp_cfg_db_t.registrar_port);
    printf("\n\tIPTK_REGISTRAR_Duration is %d\n",g_iptkapp_cfg_db_t.registrar_duration);

    printf("\n\tIPTK_SELF_ID_URL is %s\n",g_iptkapp_cfg_db_t.user_address);
}

