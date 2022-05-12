 var cgipath = "/cgi-bin/zysh-cgi?offset=";
var gl = top.frames[1];

function txcli(cmd , backhtml, cmdcnt, fail_page, success_action, waitpage){
	var fr=4, frs=3, sda='', dclicnt;
	var tempcmd = new Array();
	var postmethod = true;
	
	if ( (fail_page == undefined) || (fail_page == "" ))  fail_page = backhtml;
	top.frames[1].fail_page = fail_page;
	top.frames[1].success_page = backhtml;
	top.frames[1].cmdcount = cmdcnt;
	top.frames[1].saction = success_action;
	/*
	if ( waitpage == undefined ){
		if ( top.frames[2].frames[1].location.pathname.substr(1) != "waitdata.html" ) {
			top.frames[2].frames[1].location = "waitdata.html";
		}
	}else{
		if ( top.frames[2].frames[1].location.pathname.substr(1) != waitpage) {
			top.frames[2].frames[1].location = waitpage;
		}
	}
	*/	
	if ( cmd == "" ) {
		displayStr("CLI command is empty.");
		return ;
	}
	 gl.TempCLI = cmd.split(/(&cmd=)/);
	//alert(gl.TempCLI+'\n'+gl.TempCLI.length);
		
	/* framse 3 */
	if(postmethod){
		top.frames[frs].location = "dummy.html";
		top.frames[frs].document.open("text\/html");
		top.frames[frs].document.writeln(' <html>');
		top.frames[frs].document.writeln(' <head>');
		top.frames[frs].document.writeln(' <meta http-equiv="Pragma" content="no-cache">');
		top.frames[frs].document.writeln(' <script language=\"JavaScript\">');
		top.frames[frs].document.writeln(' 	var data_ready = 0;');
		top.frames[frs].document.writeln(' function AutoSubmit(){');
		top.frames[frs].document.writeln(' 	document.postCli.submit();');
		top.frames[frs].document.writeln(' }');
		top.frames[frs].document.writeln(' <\/script>');
		top.frames[frs].document.writeln('<\/head>');
		top.frames[frs].document.writeln('<body onload="AutoSubmit()">');
		top.frames[frs].document.writeln('<form name="postCli" action="/cgi-bin/zysh-cgi" enctype="application/x-www-form-urlencoded" method="post" autocomplete="on" >');
		if(top.frames[1].nowriteflag){//if true -> no write cli to flash
			top.frames[frs].document.writeln(" <input type='text' name='write'  value='0'> ");
			top.frames[1].nowriteflag = false;// write =1;
		}
		for(var i=0; i<gl.TempCLI.length; i++){
			if((gl.TempCLI[i] != '&cmd=')&&(gl.TempCLI[i].length != 0)){// for Netscape Browser
				top.frames[frs].document.writeln(" <input type='text' name='cmd'  value='"+gl.TempCLI[i]+"'> ");
			}
		}
		top.frames[frs].document.writeln(' <\/form>');
		top.frames[frs].document.writeln('<\/body>');
		top.frames[frs].document.writeln(' <\/html>');	
		top.frames[frs].document.close();
		//top.frames[frs].document.location.reload();
	}
	/* framse 4 */	
	top.frames[fr].window.location.href = 'getdata.html';
}

function buincli(cmd , backhtml, cmdcnt, fail_page){
	var fr=4, frs=3, sda='', dclicnt;
	var tempcmd = new Array();
	var postmethod = true;
	
	if ( (fail_page == undefined) || (fail_page == "" ))  fail_page = backhtml;
	top.frames[1].fail_page = fail_page;
	top.frames[1].success_page = backhtml;
	top.frames[1].cmdcount = cmdcnt;
	
	if ( cmd == "" ) {
		displayStr("CLI command is empty.");
		return ;
	}
	 gl.TempCLI = cmd.split(/(&cmd=)/);
	//alert(gl.TempCLI+'\n'+gl.TempCLI.length);
		
	/* framse 3 */
	if(postmethod){
		top.frames[frs].document.open("text\/html");
		top.frames[frs].document.writeln(' <html>');
		top.frames[frs].document.writeln(' <head>');
		top.frames[frs].document.writeln(' <meta http-equiv="Pragma" content="no-cache">');
		top.frames[frs].document.writeln(' <script language=\"JavaScript\">');
		top.frames[frs].document.writeln(' 	var data_ready = 0;');
		top.frames[frs].document.writeln(' function AutoSubmit(){');
		top.frames[frs].document.writeln(' 	document.postCli.submit();');
		top.frames[frs].document.writeln(' }');
		top.frames[frs].document.writeln(' <\/script>');
		top.frames[frs].document.writeln('<\/head>');
		top.frames[frs].document.writeln('<body onload="AutoSubmit()">');
		top.frames[frs].document.writeln('<form name="postCli" action="/cgi-bin/zysh-cgi" enctype="application/x-www-form-urlencoded" method="post" autocomplete="on" >');
		if(top.frames[1].nowriteflag){//if true -> no write cli to flash
			top.frames[frs].document.writeln(" <input type='text' name='write'  value='0'> ");
			top.frames[1].nowriteflag = false;// write =1;
		}
		for(var i=0; i<gl.TempCLI.length; i++){
			if((gl.TempCLI[i] != '&cmd=')&&(gl.TempCLI[i].length != 0)){// for Netscape Browser
				top.frames[frs].document.writeln(" <input type='text' name='cmd'  value='"+gl.TempCLI[i]+"'> ");
			}
		}
		top.frames[frs].document.writeln(' <\/form>');
		top.frames[frs].document.writeln('<\/body>');
		top.frames[frs].document.writeln(' <\/html>');	
		top.frames[frs].document.close();
	}
	/* framse 4 */	
//	top.frames[fr].window.location.href = 'getdata.html';
}
function txLangcli(cmd, cmdcnt){
	var fr=4, frs=3, sda='', dclicnt;
	
	if ( cmd == "" ) {
		displayStr("CLI command is empty.");
		return ;
	}
	 gl.TempCLI = cmd.split(/(&cmd=)/);
	//alert(gl.TempCLI+'\n'+gl.TempCLI.length);
		
	/* framse 3 */
	top.frames[frs].location = "dummy.html";
	top.frames[frs].document.open("text\/html");
	top.frames[frs].document.writeln(' <html>');
	top.frames[frs].document.writeln(' <head>');
	top.frames[frs].document.writeln(' <meta http-equiv="Pragma" content="no-cache">');
	top.frames[frs].document.writeln(' <script language=\"JavaScript\">');
	top.frames[frs].document.writeln(' 	var data_ready = 0;');
	top.frames[frs].document.writeln(' function AutoSubmit(){');
	top.frames[frs].document.writeln(' 	document.postCli.submit();');
	top.frames[frs].document.writeln(' }');
	top.frames[frs].document.writeln(' <\/script>');
	top.frames[frs].document.writeln('<\/head>');
	top.frames[frs].document.writeln('<body onload="AutoSubmit()">');
	top.frames[frs].document.writeln('<form name="postCli" action="/cgi-bin/zysh-cgi" enctype="application/x-www-form-urlencoded" method="post" autocomplete="on" >');
	if(top.frames[1].nowriteflag){//if true -> no write cli to flash
		top.frames[frs].document.writeln(" <input type='text' name='write'  value='0'> ");
		top.frames[1].nowriteflag = false;// write =1;
	}
	for(var i=0; i<gl.TempCLI.length; i++){
		if((gl.TempCLI[i] != '&cmd=')&&(gl.TempCLI[i].length != 0)){// for Netscape Browser
			top.frames[frs].document.writeln(" <input type='text' name='cmd'  value='"+gl.TempCLI[i]+"'> ");
		}
	}
	top.frames[frs].document.writeln(' <\/form>');
	top.frames[frs].document.writeln('<\/body>');
	top.frames[frs].document.writeln(' <\/html>');	
	top.frames[frs].document.close();
	//top.frames[frs].document.location.reload();
	/* framse 4 */	
	top.frames[fr].window.location.href = 'getdata.html';
}

function doLoad(){
	/* Total timeout */
	clrtimeout = setTimeout("window.location.href=gl.success_page; ", 80 * 1000);
	clrinterval = setInterval("detectServer();", 100);
}

function detectServer(){
	 if(gl.zyshdcnt != 0){/* data get finish or error detect*/	 	
		clearInterval(clrinterval);/*stop interval */
		chkData();
	}
}

function chkData(){
	for (var i=0; i<gl.cmdcount; i++){
		if(( gl.zyshErrorNo[i] != 0 ) && ( gl.zyshErrorNo[i]!=undefined )){
			gl.zyshdcnt  = 1;
			displayStr('Error: ['+gl.zyshErrorNo[i]+'], '+gl.zyshErrorMsg[i]);	
			//window.status =' CLI['+i+']: '+gl.TempCLI[i];		
			if(gl.isreload){
				gl.zyshdcnt  = -1;
				window.location.href=gl.fail_page;
			}
			break;
		}else{
			gl.zyshdcnt = 1;	
			if(gl.isreload) {
				window.location.href = gl.success_page;
			}
		}
	}
}    


function rptcli(cmd , backhtml, cmdcnt ,isreload ,fail_page, success_action){ // For report collection period button
	var fr=4, frs=3, sda='', dclicnt;
	var tempcmd = new Array();
	var postmethod = true;
	
	if ( (fail_page == undefined) || (fail_page == "" ))  fail_page = backhtml;
	
	if ( (fail_page == undefined) || (fail_page == "" ))  fail_page = backhtml;
	top.frames[1].fail_page = fail_page;
	top.frames[1].success_page = backhtml;
	top.frames[1].cmdcount = cmdcnt;
	top.frames[1].isreload = isreload;
	
	doLoad();
	if ( cmd == "" ) {
		displayStr("CLI command is empty.");
		return ;
	}
	 gl.TempCLI = cmd.split(/(&cmd=)/);

	/* framse 3 */
	if(postmethod){
		top.frames[frs].document.open("text\/html");
		top.frames[frs].document.writeln(' <html>');
		top.frames[frs].document.writeln(' <head>');
		top.frames[frs].document.writeln(' <meta http-equiv="Pragma" content="no-cache">');
		top.frames[frs].document.writeln(' <script language=\"JavaScript\">');
		top.frames[frs].document.writeln(' 	var data_ready = 0;');
		top.frames[frs].document.writeln(' function AutoSubmit(){');
		top.frames[frs].document.writeln(' 	document.postCli.submit();');
		top.frames[frs].document.writeln(' }');
		top.frames[frs].document.writeln(' <\/script>');
		top.frames[frs].document.writeln('<\/head>');
		top.frames[frs].document.writeln('<body onload="AutoSubmit()">');
		top.frames[frs].document.writeln('<form name="postCli" action="/cgi-bin/zysh-cgi" enctype="application/x-www-form-urlencoded" method="post" autocomplete="on" >');
		if(top.frames[1].nowriteflag){//if true -> no write cli to flash
			top.frames[frs].document.writeln(" <input type='text' name='write'  value='0'> ");
			top.frames[1].nowriteflag = false;// write =1;
		}
		for(var i=0; i<gl.TempCLI.length; i++){
			if(gl.TempCLI[i] != '&cmd='){// for Netscape Browser
				top.frames[frs].document.writeln(" <input type='text' name='cmd'  value='"+gl.TempCLI[i]+"'> ");
			}
		}
		top.frames[frs].document.writeln(' <\/form>');
		top.frames[frs].document.writeln('<\/body>');
		top.frames[frs].document.writeln(' <\/html>');	
		top.frames[frs].document.close();
	}
	
	/* framse 4 */	
	top.frames[fr].window.location.href = 'getdata.html';
}

/********************************************************************************************************/
function if_difference( arg1, arg2){
	if ( (arg1 == "" || arg1 == undefined ) && (arg2 == "" || arg2 == undefined ) )
		return false;
	if ( arg1 == arg2 )
		return false;
	return true;
}

function convert_to_bolean(type, str){
	if ( type == 0 ) {		// type:0 => on/off -> true/false
		if ( ( str == "on" ) && ( str!=undefined )) 
			return true;
		else
			return false;
	}

	if ( type == 1 ) {		// type:1 => up/down -> true/false
		if ( ( str == "up" ) && ( str!=undefined )) 
			return true;
		else
			return false;
	}

	if ( type == 2 ) {		// type:2 => no/yes-> true/false
		if ( ( str == "no" ) && ( str!=undefined )) 
			return false;
		else
			return true;
	}

	
}


function option_field(prefix, value){
	if (value == "") 
		return "";
	else
		return " " + prefix + " " + value + " ";
}

function compose_CLI( cmd, cmd1){
	if ( ( cmd != "") && (cmd != undefined) ) 
		cmd = cmd + "&cmd=" + cmd1;
	else
		cmd = cmd1;
	return cmd;
}

function link_arg(arg){
	if (arg == undefined) arg="";
	return " " + arg + " ";
}

function if_undefined(str){
	if ( str != undefined ) return str;
	return "";
}

function Get_CLI ( feature, cli_no, arg){
	//   feature:string,  cli_no:number,  arg:array
	var cmd = "";
	switch(feature){
		case "antivirus":
		case "avedit":
		case "antisetting":
			if( cli_no == 1 ) cmd = "show anti-virus activation"+"&cmd=show zone"+"&cmd=show anti-virus rule";
			if( cli_no == 2 ) cmd =  link_arg(arg[0])+"&cmd=show anti-virus activation"+"&cmd=show zone"+"&cmd=show anti-virus rule";
			if( cli_no == 3 ) cmd = "show anti-virus rule"+ link_arg(arg[0]);
			if( cli_no == 4 ) cmd = "show zone";
			if( cli_no == 5 ) cmd = "anti-virus rule"+link_arg(arg[0])+link_arg(arg[1])+"&cmd=exit"+
                        			"&cmd=show anti-virus activation"+"&cmd=show zone"+"&cmd=show anti-virus rule";
			if( cli_no == 6 ) cmd = "anti-virus rule delete"+link_arg(arg[0])+"&cmd=show anti-virus activation"+"&cmd=show zone"+"&cmd=show anti-virus rule";
			// Anti-Virus Setting
			if( cli_no == 7 ) cmd = "show anti-virus white-list status"+"&cmd=show anti-virus white-list"+"&cmd=show anti-virus black-list status"+"&cmd=show anti-virus black-list";
			if( cli_no == 8 ) cmd = link_arg(arg[0])+"&cmd=show anti-virus file-list status"+"&cmd=show anti-virus file-list"+
                           		"&cmd=show anti-virus protocol http"+"&cmd=show anti-virus protocol ftp"+"&cmd=show anti-virus protocol smtp"+
                           		"&cmd=show anti-virus protocol pop3";
			if( cli_no == 9 ) cmd = "show service-register status av";
			break;
		case "antiupdate":
			if ( cli_no == 1 ) cmd = "show anti-virus signatures status&cmd=show anti-virus update";
			if ( cli_no == 2 ) cmd = arg[0]+"&cmd=show anti-virus signatures status&cmd=show anti-virus update&cmd=show service-register status av";
			break;
		case "bridge":
		case "bridgeedit":
			if( cli_no == 1 ) cmd = "show interface bridge&cmd=show interface virtual bridge";
			if( cli_no == 2 ) cmd =  "no interface"+link_arg(arg[0])+"&cmd=show interface bridge"+
								"&cmd=show interface virtual bridge";
			if( cli_no == 3 ) cmd =  "show bridge available member"+"&cmd=show ip dhcp pool"+
								"&cmd=show interface"+link_arg(arg[0])+"&cmd=show ip dhcp server status"+
								"&cmd=show dhcp-relay"+"&cmd=show dhcp status"+link_arg(arg[0]);
			if( cli_no == 4 ) cmd = link_arg(arg[0])+"&cmd=show interface bridge"+
								"&cmd=show interface virtual bridge";
			break;
		case "portgp":
			if( cli_no == 1 ) cmd = "show port-grouping";
			if( cli_no == 2 ) cmd =  link_arg(arg[0])+"&cmd=show port-grouping";
			break;
		case "idpgl":
		case "idppf":
		case "idpcs":
		case "idppfedit":
		case "idpquery":
		case 'idpfedat':
		case 'idpfedpa':
		case 'idpcs':
		case 'idpcsedit':
			/* IDP Global */
			if( cli_no == 1 ) cmd = "show idp signature rules"+"&cmd=show idp signature profiles";
			
			/* IDP anomaly */
			if( cli_no == 2 ) cmd = "show idp anomaly rules"+"&cmd=show idp anomaly profiles";
	
			/* IDP Profile */
			if( cli_no == 3 ) cmd = "show idp signature profiles"+"&cmd=show idp signature base profile";
			if( cli_no == 4 ) cmd = "no idp signature"+link_arg(arg[0])+"&cmd=show idp signature profiles"+"&cmd=show idp signature base profile";
	
			/* IDP Profile Packet Inspection */
			if( cli_no == 5 ) cmd = "show idp signature"+link_arg(arg[0])+"signature all details";//edit
			if( cli_no == 6 ) cmd = "idp signature"+link_arg(arg[0])+"base"+link_arg(arg[1])+"&cmd=exit"+"&cmd=show idp signature"+link_arg(arg[0])+"signature all details";	//new add
			if( cli_no == 7 ) cmd = "idp"+link_arg(arg[0])+link_arg(arg[1])+"&cmd=exit"+"&cmd=show idp signature profiles"+"&cmd=show idp signature base profile";//submit
			if( cli_no == 8 ) cmd = "no idp signature"+link_arg(arg[0])+link_arg(arg[1]);//exit and delete new record
	
			/* IDP Query */
			if( cli_no == 9 ) cmd = "show idp"+link_arg(arg[2])+"name"+link_arg(arg[3])+"sid"+link_arg(arg[4])+
			                        "severity"+link_arg(arg[5])+"platform"+link_arg(arg[6])+"policytype"+link_arg(arg[7])+
			                        "service"+link_arg(arg[8])+"activate"+link_arg(arg[9])+"log"+link_arg(arg[10])+"action"+link_arg(arg[11]);
	
			/* IDP Traffic Anomaly */
			if( cli_no == 10 ) cmd = "show idp anomaly"+link_arg(arg[0])+"scan-detection all details"+
			                    "&cmd=show idp anomaly"+link_arg(arg[0])+"flood-detection all details"+
			                    "&cmd=show idp anomaly"+link_arg(arg[0])+"scan-detection "+
			                    "&cmd=show idp anomaly"+link_arg(arg[0])+"flood-detection ";
			
			/* IDP Protocol Anomaly */
			if( cli_no == 12 ) cmd = "show idp anomaly"+link_arg(arg[0])+"http-inspection all details"+
			                    "&cmd=show idp anomaly"+link_arg(arg[0])+"tcp-decoder all details"+
			                    "&cmd=show idp anomaly"+link_arg(arg[0])+"udp-decoder all details"+
			                    "&cmd=show idp anomaly"+link_arg(arg[0])+"icmp-decoder all details";
		
			/* IDP Custom Signature */
			if( cli_no == 14 ) cmd = "show idp signatures custom-signature all details"+"&cmd=show users current";
			if( cli_no == 15 ) cmd = "no idp customize signature"+link_arg(arg[0])+
			                         "&cmd=show idp signatures custom-signature all details"+"&cmd=show users current";
			if( cli_no == 16 ) cmd = "show idp signatures custom-signature"+link_arg(arg[0])+"details"+
			                    "&cmd=show idp signatures custom-signature "+link_arg(arg[0])+"contents"+
			                    "&cmd=show idp signatures custom-signature "+link_arg(arg[0])+"non-contents";
								
			if( cli_no == 17 ) cmd= arg[0]+link_arg(arg[1])+"&cmd=show idp signatures custom-signature all details"+"&cmd=show users current";
			if( cli_no == 18 ) cmd = "idp"+link_arg(arg[0])+link_arg(arg[1])+"&cmd=exit"+
			                         "&cmd=show idp signature"+link_arg(arg[2])+"signature all details";//save
			if( cli_no == 19) cmd = "idp"+link_arg(arg[0])+link_arg(arg[1])+"&cmd=exit"+
			                        "&cmd=show idp"+link_arg(arg[2])+"http-inspection all details"+
			                        "&cmd=show idp"+link_arg(arg[2])+"tcp-decoder all details"+
			                        "&cmd=show idp"+link_arg(arg[2])+"udp-decoder all details"+
			                        "&cmd=show idp"+link_arg(arg[2])+"icmp-decoder all details"//save
			if( cli_no == 20 ) cmd = "idp"+link_arg(arg[0])+link_arg(arg[1])+"&cmd=exit"+
			                         "&cmd=show idp"+link_arg(arg[2])+"scan-detection all details"+
			                         "&cmd=show idp"+link_arg(arg[2])+"flood-detection all details"+
			                         "&cmd=show idp"+link_arg(arg[2])+"scan-detection "+
			                         "&cmd=show idp"+link_arg(arg[2])+"flood-detection ";//save
			if( cli_no == 21 ) cmd = "idp"+link_arg(arg[0])+link_arg(arg[1])+"&cmd=exit"+//save
			                         "&cmd=idp"+link_arg(arg[2])+"name"+link_arg(arg[3])+"sid"+link_arg(arg[4])+
			                         "severity"+link_arg(arg[5])+"platform"+link_arg(arg[6])+"policytype"+link_arg(arg[7])+
			                         "service"+link_arg(arg[8])+"activate"+link_arg(arg[9])+"log"+link_arg(arg[10])+"action"+link_arg(arg[11]);
			if( cli_no == 22 ) cmd = "idp"+link_arg(arg[0])+"&cmd=exit"+"&cmd=show idp profiles";
			
			/* IDP Custom Query */
			if( cli_no == 23 ) cmd = arg[0];
			if( cli_no == 24 ) cmd = "show service-register status idp"; 
			if( cli_no == 25 ) cmd = "show idp signature activation";
			break;
		case "index":
			if( cli_no == 1 ) cmd = "reboot";
			break;
		case "vlan" :
			if( cli_no == 1 ) cmd = "show interface vlan&cmd=show interface virtual vlan";
			if( cli_no == 2 ) cmd = "show interface"+link_arg(arg[0]);
			if( cli_no == 3 ) cmd = "interface"+link_arg(arg[0]);
			if( cli_no == 4 ) cmd = link_arg(arg[0])+"port"+link_arg(arg[1]);
			if( cli_no == 5 ) cmd = "vlan-id"+link_arg(arg[0]);
			if( cli_no == 6 ) cmd = link_arg(arg[0])+"description"+link_arg(arg[1]);
			if( cli_no == 7 ) cmd = "ip address dhcp"+link_arg(arg[0]);
			if( cli_no == 8 ) cmd = "ip address"+link_arg(arg[0])+link_arg(arg[1]);
			if( cli_no == 9 ) cmd = "ip gateway"+link_arg(arg[0])+"metric"+link_arg(arg[1]);
			if( cli_no == 10 ) cmd = "upstream"+link_arg(arg[0]);
			if( cli_no == 11 ) cmd = "downstream"+link_arg(arg[0]);
			if( cli_no == 12 ) cmd = "mtu"+link_arg(arg[0]);
			if( cli_no == 15 ) cmd = "no interface"+link_arg(arg[0]);
			if( cli_no == 16 ) cmd = link_arg(arg[0])+"shutdown";
			if( cli_no == 20 ) cmd = "exit";
			if( cli_no == 21 ) cmd = "show dhcp status"+link_arg(arg[0])+"&cmd=show dhcp-relay";
			if( cli_no == 22 ) cmd = "show ip dhcp pool&cmd=show ip dhcp server status";
			if( cli_no == 24 ) cmd = link_arg(arg[0])+"ip gateway";
			break;
		case "ddns" :
			if( cli_no == 1 ) cmd = "show ddns";
			if( cli_no == 2 ) cmd = "no ip ddns profile"+link_arg(arg[0]);
			if( cli_no == 3 ) cmd = "ip ddns profile"+link_arg(arg[0]);
			if( cli_no == 4 ) cmd = link_arg(arg[0])+"username"+link_arg(arg[1]);
			if( cli_no == 5 ) cmd = "service-type"+link_arg(arg[0]);
			if( cli_no == 6 ) cmd = "custom"+link_arg(arg[0]);
			if( cli_no == 7 ) cmd = link_arg(arg[0])+"host"+link_arg(arg[1]);
			if( cli_no == 8 ) cmd = "ip-select"+link_arg(arg[0]);
			if( cli_no == 9 ) cmd = link_arg(arg[0])+"mx"+link_arg(arg[1]);
			if( cli_no == 10 )cmd = link_arg(arg[0])+"backmx";
			if( cli_no == 11 )cmd = link_arg(arg[0])+"wildcard";
			if( cli_no == 12 ) cmd = "exit";
			if( cli_no == 13 ) cmd = link_arg(arg[0])+"wan-iface"+link_arg(arg[1]);
			if( cli_no == 14 ) cmd = link_arg(arg[0])+"ha-iface"+link_arg(arg[1]);
			break;
		case "session" :
			if( cli_no == 1 ) cmd = "show conn begin"+link_arg(arg[0])+"end"+link_arg(arg[1]);
			if( cli_no == 2 ) cmd = "show conn"+link_arg(arg[0]);
			if( cli_no == 3 ) cmd = "show conn service"+link_arg(arg[0]);
			if( cli_no == 4 ) cmd = "show conn user"+link_arg(arg[0]);
			if( cli_no == 5 ) cmd = "show conn status";
			break;
		case "report" :
			if( cli_no == 1 ) cmd = "show report"+link_arg(arg[0])+link_arg(arg[1])+"&cmd=show report status";
			if( cli_no == 2 ) cmd = link_arg(arg[0])+"report";
			if( cli_no == 3 ) cmd = "clear report"+link_arg(arg[0]);
			if( cli_no == 4 ) cmd = "show interface ethernet&cmd=show interface vlan&cmd=show interface bridge&cmd=show interface ppp";
			break;
		case "viewlogs" :
			if( cli_no == 1 ) cmd = "show logging _category";
			if( cli_no == 2 ) cmd = "show logging entries";
			if( cli_no == 3 ) cmd = "show logging entries category"+link_arg(arg[0]);
			if( cli_no == 4 ) cmd = "show logging debug entries";  // FIXME
			if( cli_no == 5 ) cmd = "logging mail sending-now";
			if( cli_no == 6 ) cmd = "clear logging debug buffer&cmd=clear logging system-log buffer";
			break;
		case "logsetting" :
			if( cli_no == 1 ) cmd = "show logging status system-log&cmd=show logging status mail&cmd=show logging status syslog";
			/*--- logging systemlog -----*/
			if( cli_no == 2 ) cmd = link_arg(arg[1])+"logging system-log category"+link_arg(arg[0])+link_arg(arg[2]);
			if( cli_no == 3 ) cmd = link_arg(arg[1])+"logging system-log suppression";
			if( cli_no == 4 ) cmd = link_arg(arg[1])+"logging system-log suppression interval"+link_arg(arg[0]);
			/*---- logging mail ----*/
			if( cli_no == 10 ) cmd = link_arg(arg[1])+"logging mail"+link_arg(arg[0])+"address"+link_arg(arg[2]);
			if( cli_no == 11 ) cmd = link_arg(arg[1])+"logging mail"+link_arg(arg[0]);
			if( cli_no == 12 ) cmd = link_arg(arg[1])+"logging mail"+link_arg(arg[0])+"subject"+link_arg(arg[2]);
			if( cli_no == 13 ) cmd = link_arg(arg[1])+"logging mail"+link_arg(arg[0])+"send-log-to"+link_arg(arg[2]);
			if( cli_no == 14 ) cmd = link_arg(arg[1])+"logging mail"+link_arg(arg[0])+"send-alerts-to"+link_arg(arg[2]);
			if( cli_no == 15 ) cmd = "logging mail"+link_arg(arg[0])+"schedule"+link_arg(arg[1])+link_arg(arg[2]);
			if( cli_no == 16 ) cmd = link_arg(arg[1])+"logging mail"+link_arg(arg[0])+"authentication username"+link_arg(arg[2])+link_arg(arg[3]);
			if( cli_no == 17 ) cmd = link_arg(arg[1])+"logging mail"+link_arg(arg[0])+"authentication";
			if( cli_no == 18 ) cmd = link_arg(arg[1])+"logging mail"+link_arg(arg[0])+"category"+link_arg(arg[2])+"level"+link_arg(arg[3]);			
			if( cli_no == 19 ) cmd = link_arg(arg[1])+"logging mail"+link_arg(arg[0])+"from"+link_arg(arg[2]);
			/*--- logging remote server ---*/
			if( cli_no == 20 ) cmd = link_arg(arg[2])+"logging syslog"+link_arg(arg[0])+"address"+link_arg(arg[1]);
			if( cli_no == 21 ) cmd = link_arg(arg[1])+"logging syslog"+link_arg(arg[0]);
			if( cli_no == 22 ) cmd = "logging syslog"+link_arg(arg[0])+"facility"+link_arg(arg[1]);
			if( cli_no == 23 ) cmd = link_arg(arg[1])+"logging syslog"+link_arg(arg[0])+"category"+link_arg(arg[2])+link_arg(arg[3]);
			break;
		case "voipalg" :
			if( cli_no == 1 ) cmd = "show alg sip&cmd=show alg h323&cmd=show alg ftp";
			if( cli_no == 2 ) cmd = link_arg(arg[0])+"alg sip&cmd=alg sip signal-port"+link_arg(arg[1])+"&cmd=alg sip media-timeout"+link_arg(arg[2])+"&cmd=alg sip signal-timeout"+link_arg(arg[3]);
			if( cli_no == 3 ) cmd = link_arg(arg[0])+"alg sip signal-extra-port"+link_arg(arg[1]);
			if( cli_no == 4 ) cmd = link_arg(arg[0])+"alg h323";
			break;
		case "httpred" :
			if( cli_no == 1 ) cmd = "show ip http-redirect";
			if( cli_no == 2 ) cmd = "ip http-redirect"+link_arg(arg[0])+"interface"+link_arg(arg[1])+
						"redirect-to"+link_arg(arg[2])+link_arg(arg[3]);
			if( cli_no == 3 ) cmd = "no ip http-redirect"+link_arg(arg[0]);
			if( cli_no == 4 ) cmd = "ip http-redirect"+link_arg(arg[0])+link_arg(arg[1])+"&cmd=show ip http-redirect";
			break;
		case "regist":
			if( cli_no == 1 ) cmd = "show device-register status&cmd=show service-register status all&cmd=show service-register status _remaining-day";
			if( cli_no == 2 ) cmd = "device-register checkuser"+link_arg(arg[0]); 
			if( cli_no == 3 ) cmd = "device-register username"+link_arg(arg[0])+"password"+
						link_arg(arg[1])+"e-mail"+link_arg(arg[2])+"country-code"+link_arg(arg[3]);
			if( cli_no == 4 ) cmd = "service-register service-type trial service"+link_arg(arg[0]);
			if( cli_no == 5 ) cmd = "device-register username"+link_arg(arg[0])+"password"+link_arg(arg[1]);
			if( cli_no == 10 ) cmd = "show device-register status&cmd=show service-register status all"; //for wizard
			break;
		case "registmgn":
			if( cli_no == 1 ) cmd = "show device-register status&cmd=show service-register status all&cmd=show service-register status _remaining-day";
			if( cli_no == 2 ) cmd = "service-register service-type standard license-key"+link_arg(arg[0]);
			if( cli_no == 3 ) cmd = "service-register checkexpire"; 
			if( cli_no == 10 )cmd = "show clock date";	
			break;
		case "aplother":
		case "aplotheredit":			
		case "aplpatrol":
		case "aplpatroledit":
		case "appIM":
		case "appP2P":
		case "appStream":
			if( cli_no == 1 ) cmd = "show app general"+"&cmd=show app im"+"&cmd=show app p2p"+"&cmd=show app stream"+
			                   "&cmd=show app config"+"&cmd=show app im support action";
			if( cli_no == 2 ) cmd = "show app"+link_arg(arg[0])+"defaultport"+"&cmd=show app"+link_arg(arg[0])+"rule all"+"&cmd=show zone"+"&cmd=show app all defaultport";
			if( cli_no == 3 ) cmd = arg[0]
			                        +"&cmd=show app general"+"&cmd=show app im"+"&cmd=show app p2p"
			                        +"&cmd=show app stream"+"&cmd=show app config";
			if( cli_no == 4) cmd = 	"show app other rule all"+"&cmd=show zone"+
			                   "&cmd=show schedule-object"+"&cmd=show username"+	"&cmd=show groupname"+"&cmd=show address-object"+
			                   "&cmd=show object-group address";
			if( cli_no == 5) cmd = "app other"+link_arg(arg[0])+"&cmd="+link_arg(arg[1])+"&cmd="+link_arg(arg[2])+
				"&cmd=show app other config"+"&cmd=show app other rule";
			if( cli_no == 6) cmd = "no app other"+link_arg(arg[0])+"&cmd=show app other config"+"&cmd=show app other rule";
			if( cli_no == 7) cmd = "app other move"+link_arg(arg[0])+"to"+link_arg(arg[1])+"&cmd=show app other config"+"&cmd=show app other rule";
			if( cli_no == 8) cmd = "show schedule-object"+"&cmd=show username"+"&cmd=show groupname"+
				"&cmd=show address-object&cmd=show object-group address";
			if( cli_no == 9) cmd = "app other"+link_arg(arg[0])+link_arg(arg[1])+"&cmd=show app other config"+"&cmd=show app other rule";
			if( cli_no == 10) cmd = link_arg(arg[0])+link_arg(arg[1])+link_arg(arg[2])+link_arg(arg[3])+link_arg(arg[4])+
				"&cmd=show app general"+"&cmd=show app im"+"&cmd=show app p2p"+
				"&cmd=show app stream"+"&cmd=show app config";//+"&cmd=show app usage";
			if( cli_no == 11 ) cmd = link_arg(arg[0])+link_arg(arg[1])+link_arg(arg[2])+link_arg(arg[3])+link_arg(arg[4])+
				"&cmd=show app im";//appIM.html			
			if( cli_no == 12 ) cmd = "show app im"+"&cmd=show app im support action"; //appIM.html
			if( cli_no == 13 ) cmd = link_arg(arg[0])+link_arg(arg[1])+link_arg(arg[2])+link_arg(arg[3])+link_arg(arg[4])+
				"&cmd=show app p2p";//appP2P.html			
			if( cli_no == 14 ) cmd = "show app p2p"; //appP2P.html
			if( cli_no == 15 ) cmd = link_arg(arg[0])+link_arg(arg[1])+link_arg(arg[2])+link_arg(arg[3])+link_arg(arg[4])+
				"&cmd=show app stream";//appStream.html			
			if( cli_no == 16 ) cmd = "show app stream"; //appStream.html
			if( cli_no == 17 ) cmd = "show app general"+"&cmd=show app other rule all"+"&cmd=show zone"+ "&cmd=show schedule-object"+
				"&cmd=show username"+"&cmd=show groupname"+"&cmd=show address-object"+
				"&cmd=show object-group address";//aplother.html
			if( cli_no == 18 ) cmd = "show app general"+"&cmd=show app im"+"&cmd=show app p2p"+"&cmd=show app stream"+
				"&cmd=show app config"+"&cmd=show app im support action";
			if ( cli_no == 19 ) cmd = "show address-object&cmd=show object-group address"+
					"&cmd=show schedule-object";	
				break;
		case "firewall":
		case "firewalledit":
		case "fwglobal":
			if( cli_no == 1 ) cmd = "show zone"+"&cmd=show firewall";
			if( cli_no == 2 ) cmd = "firewall delete"+link_arg(arg[0])+"&cmd=show zone"+"&cmd=show firewall";
			if( cli_no == 3 ) cmd = "show schedule-object"+"&cmd=show username"+"&cmd=show groupname"+
				"&cmd=show address-object"+"&cmd=show service-object"+"&cmd=show firewall"+link_arg(arg[0]);
			if( cli_no == 4 ) cmd = "firewall"+link_arg(arg[0])+link_arg(arg[1])+link_arg(arg[2])+
				link_arg(arg[3])+link_arg(arg[4])+link_arg(arg[5])+link_arg(arg[6])+
				link_arg(arg[7])+link_arg(arg[8])+link_arg(arg[9])+link_arg(arg[10])+
				"&cmd=exit"+"&cmd=show zone"+"&cmd=show firewall";	
			if( cli_no == 5 ) cmd = link_arg(arg[0])+"&cmd=show zone"+"&cmd=show firewall";	
			if( cli_no == 6 ) cmd = "show schedule-object"+"&cmd=show username"+"&cmd=show groupname"+
				"&cmd=show address-object"+"&cmd=show object-group address"+"&cmd=show service-object"+"&cmd=show object-group service"+"&cmd=show users default-setting";
			if( cli_no == 7 ) cmd ="show schedule-object"+"&cmd=show username"+"&cmd=show groupname"+
		                "&cmd=show address-object"+"&cmd=show object-group address"+"&cmd=show service-object"+"&cmd=show object-group service"+"&cmd=show users default-setting"+"&cmd=show firewall"+link_arg(arg[0])+link_arg(arg[1])+link_arg(arg[2]);
			if( cli_no == 8 ) cmd = "firewall"+link_arg(arg[0])+link_arg(arg[1])+"delete"+link_arg(arg[2])+"&cmd=show zone"+"&cmd=show firewall";
			if( cli_no == 9 ) cmd = "firewall"+link_arg(arg[0])+link_arg(arg[1])+"move"+link_arg(arg[2])+"to"+link_arg(arg[3])+
							"&cmd=show zone"+"&cmd=show firewall";
			if( cli_no == 10 ) cmd = "firewall move"+link_arg(arg[0])+"to"+link_arg(arg[1])+"&cmd=show zone"+"&cmd=show firewall";
			if( cli_no == 11 ) cmd = "show firewall status";
			if( cli_no == 12 ) cmd = "show connlimit max-per-host";
			/* for pop up window */
			if ( cli_no == 13 ) cmd = "show address-object&cmd=show object-group address"+
					"&cmd=show service-object&cmd=show object-group service"+
					"&cmd=show schedule-object";	
			break;
		case "home":
			if( cli_no == 1 ) cmd = "show fqdn"+"&cmd=show version"+"&cmd=show system uptime"+
				"&cmd=show interface summary all"+"&cmd=show zone binding-iface"+"&cmd=show service-register status all"+
				"&cmd=show users all"+"&cmd=show mac"+"&cmd=show cpu status"+"&cmd=show mem status"+
				"&cmd=show idp signatures version"+"&cmd=show idp signatures date"+"&cmd=show idp signatures number"+
				"&cmd=show idp signatures custom-signature number"+"&cmd=show clock time&cmd=show clock date"+
				"&cmd=show serial-number"+"&cmd=show device-ha status"+"&cmd=show anti-virus signatures status"+"&cmd=show disk"+"&cmd=show anti-virus update status"+
				"&cmd=show conn status"+
				"&cmd=show users current"+"&cmd=show idp signature update status"+"&cmd=show idp statistics ranking signature-name&cmd=show anti-virus statistics ranking virus-name"+
				"&cmd=show language setting"+"&cmd=show language all";
			if( cli_no == 2 ) cmd = "show ip dhcp binding";
			break;

	/*=======User/Group==================*/
		case "users" :
			if( cli_no == 1) cmd =	"show username&cmd=show users default-setting&cmd=show users simultaneous-logon-settings&cmd=show users retry-settings"+
			 	"&cmd=show users update-lease-settings&cmd=show users idle-detection-settings";
			if( cli_no == 2 ) cmd = "username"+link_arg(arg[0])+"password"+link_arg(arg[1])+"user-type"+link_arg(arg[2]);	
			if( cli_no == 3 ) cmd = "username"+link_arg(arg[0])+"nopassword user-type"+link_arg(arg[1]);	
			if( cli_no == 4 ) cmd = "username"+link_arg(arg[0])+"user-type"+link_arg(arg[1]);			
			if( cli_no == 5 ) cmd = "username"+link_arg(arg[0])+link_arg(arg[1])+"description"+link_arg(arg[2]);
			if( cli_no == 6 ) cmd = "username"+link_arg(arg[0])+"logon-lease-time"+link_arg(arg[1])+"&cmd=username"+link_arg(arg[0])+"logon-re-auth-time"+link_arg(arg[2]);
			if( cli_no == 7 ) cmd = "no username"+link_arg(arg[0]);	  
			/*-------- Remote setting ---------*/
			if( cli_no == 10 ) cmd = "users default-setting user-type"+link_arg(arg[0]);
			if( cli_no == 11 ) cmd = "users default-setting logon-lease-time"+link_arg(arg[0]);
			if( cli_no == 12 ) cmd = "users default-setting logon-re-auth-time"+link_arg(arg[0]);
			if( cli_no == 13 ) cmd = link_arg(arg[0])+"users simultaneous-logon administration enforce";
			//if( cli_no == 13 ) cmd = link_arg(arg[0])+"users logon-count management enforce";
			if( cli_no == 14 ) cmd = link_arg(arg[0])+"users simultaneous-logon access enforce";
			if( cli_no == 15 ) cmd = "users simultaneous-logon administration limit"+link_arg(arg[0]);
			//if( cli_no == 15 ) cmd = "users logon-count management limit"+link_arg(arg[0]);
			if( cli_no == 16 ) cmd = "users simultaneous-logon access limit"+link_arg(arg[0]);
			if( cli_no == 17 ) cmd = link_arg(arg[0])+"users retry-limit";
			if( cli_no == 18 ) cmd = "users retry-count"+link_arg(arg[0]);
			if( cli_no == 19 ) cmd = "users lockout-period"+link_arg(arg[0]);						
						
			/*--------- For user edit page ------*/
			if( cli_no == 20 ) cmd = "show username"+link_arg(arg[0]);	
			if( cli_no == 21 ) cmd = "show users current"
			break;
		case "grps" :
			if( cli_no == 1) cmd = "show groupname"+link_arg(arg[0]);
			if( cli_no == 2) cmd = "no groupname"+link_arg(arg[0]);
			if( cli_no == 3) cmd = "groupname"+link_arg(arg[0]);
			if( cli_no == 4) cmd = link_arg(arg[0])+"description"+link_arg(arg[1]);
			if( cli_no == 5) cmd = link_arg(arg[0])+"user"+link_arg(arg[1]);
			if( cli_no == 6) cmd = link_arg(arg[0])+"groupname"+link_arg(arg[1]);
			if( cli_no == 10) cmd = "exit";
			break;
		
	/*========= System ==============*/		   
		case "dhcp":
	/*   summary:   //FIXME: static DHCP
		 	1.show ip dhcp pool
	 	 	2.no ip dhcp pool POOL_NAME |show ip dhcp pool
	 	 edit
	 	 	3.ip dhcp pool POOL_NAME | network IP SUBNET|default-router IP|cmd=dns_server IP1 IP2 IP3
 	 */
			if( cli_no == 1 ) cmd = "show ip dhcp pool";
			if( cli_no == 2 ) cmd = "no ip dhcp pool"+link_arg(arg[0])+"&cmd=show ip dhcp pool";
			if( cli_no == 3 ) cmd = "ip dhcp pool"+link_arg(arg[0])+"&cmd=network"+link_arg(arg[1])+
							       link_arg(arg[2])+"&cmd=default-router"+link_arg(arg[3])+arg[4]+
							        link_arg(arg[5])+link_arg(arg[6])+link_arg(arg[7])+"&cmd=lease"+link_arg(arg[8])+
								"&cmd=exit&cmd=show ip dhcp pool";
			if( cli_no == 4 ) cmd = "ip dhcp pool"+link_arg(arg[0])+"&cmd=host"+link_arg(arg[1])+"&cmd=hardware-address"+
						link_arg(arg[2])+"&cmd=exit&cmd=show ip dhcp pool";
			if( cli_no == 5 ) cmd = link_arg(arg[0])+"service dhcp";
			if( cli_no == 6 ) cmd = "show ip dhcp server status";
			break;
		case "dns":
			if( cli_no == 1 ) cmd = "show ip dns server database";
			if( cli_no == 2 ) cmd = "no ip dns server a-record"+link_arg(arg[0]);
			if( cli_no == 3 ) cmd = "ip dns server a-record"+link_arg(arg[0])+link_arg(arg[1]);
			if( cli_no == 4 ) cmd = "no ip dns server zone-forwarder"+link_arg(arg[0]); 
			if( cli_no == 5 ) cmd = "ip dns server zone-forwarder"+link_arg(arg[0])+link_arg(arg[1])+link_arg(arg[2]);
			if( cli_no == 6 ) cmd = "no ip dns server mx-record"+link_arg(arg[0])+link_arg(arg[1]);
			if( cli_no == 7 ) cmd = "ip dns server mx-record"+link_arg(arg[0])+link_arg(arg[1]);
			if( cli_no == 8 ) cmd = link_arg(arg[0])+"ip dns server";
			if( cli_no == 9 ) cmd = "show ip dns server status";
			if( cli_no == 10 ) cmd = "show ip name-server";
			if( cli_no == 11 ) cmd = "ip name-server"+link_arg(arg[0]);
			if( cli_no == 12 ) cmd = "no ip name-server"+link_arg(arg[0]);
			if( cli_no == 13 ) cmd = "ip dns server"+link_arg(arg[0])+link_arg(arg[1])+"access-group"+link_arg(arg[2])+"zone"+link_arg(arg[3])+"action"+link_arg(arg[4]);
			if( cli_no == 14 ) cmd = "no ip dns server rule"+link_arg(arg[0]);
			if( cli_no == 15 ) cmd = "ip dns server rule move"+link_arg(arg[0])+"to"+link_arg(arg[1]);
			if( cli_no == 16 ) cmd = "show zone";
			if( cli_no == 17 ) cmd = "ip dns server zone-forwarder move"+link_arg(arg[0])+"to"+link_arg(arg[1]);
			if( cli_no == 18 ) cmd = "show address-object&cmd=show object-group address";
			break;	
		
		case "buinsrv": // http & http secure
			if( cli_no == 1 ) cmd = "show aaa authentication&cmd=show ca category local&cmd=show zone&cmd=show ip http server secure status&cmd=show ip http server status";
			if( cli_no == 2 ) cmd = link_arg(arg[0])+"ip http server";
			if( cli_no == 3 ) cmd = link_arg(arg[0])+"ip http secure-server";
			if( cli_no == 4 ) cmd = "ip http port"+link_arg(arg[0]);
			if( cli_no == 5 ) cmd = "ip http authentication"+link_arg(arg[0]);
			if( cli_no == 6 ) cmd = "ip http secure-port"+link_arg(arg[0]);
			if( cli_no == 7 ) cmd = "ip http secure-server authentication"+link_arg(arg[0]);
			if( cli_no == 8 ) cmd = link_arg(arg[0])+"ip http secure-server auth-client";
			if( cli_no == 9 ) cmd = link_arg(arg[0])+"ip http secure-server force-redirect";
			if( cli_no == 10) cmd = "ip http secure-server cert"+link_arg(arg[0]);
			if( cli_no == 20 ) cmd = "ip http server table admin"+link_arg(arg[0])+link_arg(arg[1])+"access-group"+link_arg(arg[2])+"zone"+link_arg(arg[3])+"action"+link_arg(arg[4]);			
			if( cli_no == 21 ) cmd = "ip http secure-server table admin"+link_arg(arg[0])+link_arg(arg[1])+"access-group"+link_arg(arg[2])+"zone"+link_arg(arg[3])+"action"+link_arg(arg[4]);
			if( cli_no == 22 ) cmd = "ip http server table user"+link_arg(arg[0])+link_arg(arg[1])+"access-group"+link_arg(arg[2])+"zone"+link_arg(arg[3])+"action"+link_arg(arg[4]);			
			if( cli_no == 23 ) cmd = "ip http secure-server table user"+link_arg(arg[0])+link_arg(arg[1])+"access-group"+link_arg(arg[2])+"zone"+link_arg(arg[3])+"action"+link_arg(arg[4]);
			//TODO : {admin|user} rule move
			if( cli_no == 24 ) cmd = "no ip http server table"+link_arg(arg[0])+"rule"+link_arg(arg[1]);
			if( cli_no == 25 ) cmd = "ip http server table"+link_arg(arg[0])+"rule move"+link_arg(arg[1])+"to"+link_arg(arg[2]);
			if( cli_no == 26 ) cmd = "no ip http secure-server table"+link_arg(arg[0])+"rule"+link_arg(arg[1]);
			if( cli_no == 27 ) cmd = "ip http secure-server table"+link_arg(arg[0])+"rule move"+link_arg(arg[1])+"to"+link_arg(arg[2]);
			break; 
		case "ssh":
			if( cli_no == 1 ) cmd = "show ca category local&cmd=show zone&cmd=show ip ssh server status";
			if( cli_no == 2 ) cmd = link_arg(arg[0])+"ip ssh server";
			if( cli_no == 3 ) cmd = "ip ssh server port"+link_arg(arg[0]);
			if( cli_no == 4 ) cmd = "ip ssh server cert"+link_arg(arg[0]); 
			if( cli_no == 5 ) cmd = "ip ssh server"+link_arg(arg[0])+link_arg(arg[1])+"access-group"+link_arg(arg[2])+"zone"+link_arg(arg[3])+"action"+link_arg(arg[4]);
			if( cli_no == 6 ) cmd = link_arg(arg[0])+"ip ssh server v1";
			if( cli_no == 7 ) cmd = "no ip ssh server rule"+link_arg(arg[0]);
			if( cli_no == 8 ) cmd = "ip ssh server rule move"+link_arg(arg[0])+"to"+link_arg(arg[1]);
			break; 
				
		case "telnet":
			if( cli_no == 1 ) cmd = "show zone&cmd=show ip telnet server status";
			if( cli_no == 2 ) cmd = link_arg(arg[0])+"ip telnet server";
			if( cli_no == 3 ) cmd = "ip telnet server port"+link_arg(arg[0]);
			if( cli_no == 4 ) cmd = "ip telnet server"+link_arg(arg[0])+link_arg(arg[1])+"access-group"+link_arg(arg[2])+"zone"+link_arg(arg[3])+"action"+link_arg(arg[4]);
			if( cli_no == 5 ) cmd = "no ip telnet server rule"+link_arg(arg[0]);
			if( cli_no == 6 ) cmd = "ip telnet server rule move"+link_arg(arg[0])+"to"+link_arg(arg[1]);
			break;	
		case "ftp":
			if( cli_no == 1 ) cmd = "show ca category local&cmd=show zone&cmd=show ip ftp server status";
			if( cli_no == 2 ) cmd = link_arg(arg[0])+"ip ftp server";
			if( cli_no == 3 ) cmd = "ip ftp server port"+link_arg(arg[0]);
			if( cli_no == 4 ) cmd = "ip ftp server"+link_arg(arg[0])+link_arg(arg[1])+"access-group"+link_arg(arg[2])+"zone"+link_arg(arg[3])+"action"+link_arg(arg[4]);
			if( cli_no == 5 ) cmd = link_arg(arg[0])+"ip ftp server tls-required";
			if( cli_no == 6 ) cmd = "ip ftp server cert"+link_arg(arg[0]);
			if( cli_no == 7 ) cmd = "no ip ftp server rule"+link_arg(arg[0]);
			if( cli_no == 8 ) cmd = "ip ftp server rule move"+link_arg(arg[0])+"to"+link_arg(arg[1]);
			break; 
		case "snmp":
			if( cli_no == 1 ) cmd = "show zone&cmd=show snmp status";
			if( cli_no == 2 ) cmd = link_arg(arg[0])+"snmp-server";
			if( cli_no == 3 ) cmd = "snmp-server port"+link_arg(arg[0]);
			if( cli_no == 4 ) cmd = link_arg(arg[0])+"snmp-server community"+link_arg(arg[1])+"ro";
			if( cli_no == 5 ) cmd = link_arg(arg[0])+"snmp-server community"+link_arg(arg[1])+"rw";
			if( cli_no == 6 ) cmd = link_arg(arg[0])+"snmp-server host"+link_arg(arg[1])+link_arg(arg[2]);
			if( cli_no == 7 ) cmd = "snmp-server"+link_arg(arg[0])+link_arg(arg[1])+"access-group"+link_arg(arg[2])+"zone"+link_arg(arg[3])+"action"+link_arg(arg[4]);			
			if( cli_no == 8 ) cmd = "no snmp-server rule"+link_arg(arg[0]);
			if( cli_no == 9 ) cmd = "snmp-server rule move"+link_arg(arg[0])+"to"+link_arg(arg[1]);
			break; 
			
		case "icmp":
			if( cli_no == 1 ) cmd = "show zone&cmd=show address-object&cmd=show object-group address&cmd=show ip icmp-filter status";
			if( cli_no == 2 ) cmd = link_arg(arg[0])+"ip icmp-filter activate";
			if( cli_no == 3 ) cmd = "ip icmp-filter"+link_arg(arg[0])+link_arg(arg[1])+"access-group"+link_arg(arg[2])+"zone"+link_arg(arg[3])+"icmp-type"+link_arg(arg[4])+"action"+link_arg(arg[5]);			
			if( cli_no == 4 ) cmd = "no ip icmp-filter rule"+link_arg(arg[0]);
			if( cli_no == 5 ) cmd = "ip icmp-filter rule move"+link_arg(arg[0])+"to"+link_arg(arg[1]);
			break;

		case "virsvr":
			/* show Table */
			if ( cli_no == 1 ) cmd = "show ip virtual-server";
			/*delete & show*/
			if ( cli_no == 2 ) cmd = "no ip virtual-server"+link_arg(arg[0])+"&cmd=show ip virtual-server";
			/* deactivate virtual server*/
			if ( cli_no == 3 ) cmd = "ip virtual-server deactivate"+link_arg(arg[0])+"&cmd=show ip virtual-server";
			/* activate virtual server*/
			if ( cli_no == 4 ) cmd = "ip virtual-server activate"+link_arg(arg[0])+"&cmd=show ip virtual-server";
			
			/* edit virtual server, mapping type is "any"*/
			if ( cli_no == 5 ) cmd = "ip virtual-server rename"+link_arg(arg[10])+link_arg(arg[0])+
				"&cmd=ip virtual-server"+link_arg(arg[0])+"interface"+link_arg(arg[1])+"original-ip"+link_arg(arg[2])+
				"map-to"+link_arg(arg[3])+"map-type"+link_arg(arg[4])+	"&cmd=show ip virtual-server";
			
			/* edit virtual server, mapping type is "port"*/
			if ( cli_no == 6 ) cmd =  "ip virtual-server rename"+link_arg(arg[10])+link_arg(arg[0])+
				"&cmd=ip virtual-server"+link_arg(arg[0])+"interface"+link_arg(arg[1])+"original-ip"+link_arg(arg[2])+
				"map-to"+link_arg(arg[3])+"map-type"+link_arg(arg[4])+"protocol"+link_arg(arg[5])+
				"original-port"+link_arg(arg[6])+"mapped-port"+link_arg(arg[8])+"&cmd=show ip virtual-server";
			
			/* edit virtual server, mapping type is "ports"*/
			if ( cli_no == 7 ) cmd =  "ip virtual-server rename"+link_arg(arg[10])+link_arg(arg[0])+
				"&cmd=ip virtual-server"+link_arg(arg[0])+"interface"+link_arg(arg[1])+"original-ip"+link_arg(arg[2])+
				"map-to"+link_arg(arg[3])+"map-type"+link_arg(arg[4])+"protocol"+link_arg(arg[5])+
				"original-port-begin"+link_arg(arg[6])+"original-port-end"+link_arg(arg[7])+"mapped-port-begin"+link_arg(arg[8])+
				//"mapped-port-end"+link_arg(arg[9])+"&cmd=show ip virtual-server";
				"&cmd=show ip virtual-server";

			/* add virtual server, mapping type is "any"*/
			if ( cli_no == 8 ) cmd = "ip virtual-server"+link_arg(arg[0])+"interface"+link_arg(arg[1])+"original-ip"+link_arg(arg[2])+
				"map-to"+link_arg(arg[3])+"map-type"+link_arg(arg[4])+	"&cmd=show ip virtual-server";
			
			/* add virtual server, mapping type is "port"*/
			if ( cli_no == 9 ) cmd = "ip virtual-server"+link_arg(arg[0])+"interface"+link_arg(arg[1])+"original-ip"+link_arg(arg[2])+
				"map-to"+link_arg(arg[3])+"map-type"+link_arg(arg[4])+"protocol"+link_arg(arg[5])+
				"original-port"+link_arg(arg[6])+"mapped-port"+link_arg(arg[8])+"&cmd=show ip virtual-server";
			
			/* add virtual server, mapping type is "ports"*/
			if ( cli_no == 10 ) cmd = "ip virtual-server"+link_arg(arg[0])+"interface"+link_arg(arg[1])+"original-ip"+link_arg(arg[2])+
				"map-to"+link_arg(arg[3])+"map-type"+link_arg(arg[4])+"protocol"+link_arg(arg[5])+
				"original-port-begin"+link_arg(arg[6])+"original-port-end"+link_arg(arg[7])+"mapped-port-begin"+link_arg(arg[8])+
				//"mapped-port-end"+link_arg(arg[9])+"&cmd=show ip virtual-server";
				"&cmd=show ip virtual-server";
			break;

		case "zone":
		    /*1. show zone PROFILE
			2. [no] zone PROFILE
			3. block
			4. interface INTERFACE
			5. crypto NAME			
			6. show zone no-binding 		*/
			if ( cli_no == 1 ) cmd = "show zone" + link_arg(arg[0]) ;
			if ( cli_no == 2 ) cmd = link_arg(arg[0]) + "zone" + link_arg(arg[1]) ;
			if ( cli_no == 3 ) cmd = link_arg(arg[0]) +"block" ;
			if ( cli_no == 4 ) cmd = "interface" + link_arg(arg[0]) ;
			if ( cli_no == 5 ) cmd = "crypto" + link_arg(arg[0]) ;
			if ( cli_no == 6 ) cmd = "show zone none-binding";
			if ( cli_no == 7 ) cmd = "no interface" + link_arg(arg[0]);
			if ( cli_no == 8 ) cmd = "no crypto" + link_arg(arg[0]);
			/* USg 300*/
			if ( cli_no == 9 ) cmd = "no sslvpn" + link_arg(arg[0]);
			if ( cli_no == 10 ) cmd = "sslvpn" + link_arg(arg[0]) ;
			break;
			
		case "routing-protocol":
			/*** rip.html 
			1. show rip global
			2. no authentication
			3. authentication mode {md5|text} 
			4. authentication string AUTHKEY
			5. authentication key-id <1..255> key-string AUTHKEY
			6. [no] redistribute {static|ospf}
			7. redistribute {static|ospf} metric <0..16>	*/
			if ( cli_no == 1 ) cmd = "show rip global";
			if ( cli_no == 2 ) cmd = "no authentication";
			if ( cli_no == 3 ) cmd = "authentication mode" + link_arg(arg[0]) ;
			if ( cli_no == 4 ) cmd = "authentication string" + link_arg(arg[0]) ;
			if ( cli_no == 5 ) cmd = "authentication key" + link_arg(arg[0]) + "key-string" + link_arg(arg[1]);
			if ( cli_no == 6 ) cmd = link_arg(arg[0]) + "redistribute" + link_arg(arg[1]);
			if ( cli_no == 7 ) cmd = "redistribute" + link_arg(arg[0]) + "metric" + link_arg(arg[1]);

			/*** ospf.html
			10. show ospf global
			11. [no] router-id IPv4
			12. redistribute {static|rip} metric-type <1..2> metric <0..16777214>*/
			if ( cli_no == 10 ) cmd = "show ospf global";
			if ( cli_no == 11 ) cmd = link_arg(arg[0]) + "router-id" + link_arg(arg[1]);
			if ( cli_no == 12 ) cmd = "redistribute" + link_arg(arg[0]) + "metric-type" + link_arg(arg[1]) + "metric" + link_arg(arg[2]);
			if ( cli_no == 13 ) cmd = "show ospf area";

			/*** ospfedit.html
			20. [no] area IPv4 [stub|nssa]
			-- 21. no area IPv4 authentication 
			22. area IPv4 authentication [message-digest]				-- "area IPv4 authentication"
 is simple 
			23. area IPv4 authentication authentication-key AUTHKEY	-- Simple Authentication Key
			24. area IPv4 authentication message-digest-key <1..255> md5 AUTHKEY	-- MD5 Authentication ID & Key*/
			if ( cli_no == 20 ) cmd = link_arg(arg[0]) + "area"+ link_arg(arg[1]) + link_arg(arg[2]);
			//if ( cli_no == 21 ) cmd = "no area "+link_arg(arg[0])+"authentication";
			if ( cli_no == 22 ) cmd = "area "+link_arg(arg[0])+"authentication"+ link_arg(arg[1]);
			if ( cli_no == 23 ) cmd = "area "+link_arg(arg[0])+"authentication authentication-key"+ link_arg(arg[1]);
			if ( cli_no == 24 ) cmd = "area "+link_arg(arg[0])+"authentication message-digest-key"+ link_arg(arg[1]) + "md5" + link_arg(arg[2]);
/*			30. area IPv4 virtual-link IPv4
			31. area IPv4 virtual-link IPv4 authentication [same-as-area]; */
			if ( cli_no == 30 ) cmd = "area "+link_arg(arg[0])+"virtual-link"+ link_arg(arg[1]);
			if ( cli_no == 31 ) cmd = "area "+link_arg(arg[0])+"virtual-link"+ link_arg(arg[1]) +"authentication" + link_arg(arg[2]) ;
			if ( cli_no == 32 ) cmd = "no area "+link_arg(arg[0])+"virtual-link"+ link_arg(arg[1]);
			if ( cli_no == 33 ) cmd = "area "+link_arg(arg[0])+"virtual-link"+ link_arg(arg[1]) +"authentication" + link_arg(arg[2]) ;
			if ( cli_no == 34 ) cmd = "area "+link_arg(arg[0])+"virtual-link"+ link_arg(arg[1]) +"authentication authentication-key" + link_arg(arg[2]) ;
			if ( cli_no == 35 ) cmd = "area "+link_arg(arg[0])+"virtual-link"+ link_arg(arg[1]) +"authentication message-digest-key" + link_arg(arg[2])+ "md5" + link_arg(arg[3]); ;

		/****** Interface ****** 
			50. [no] network ETH_IFACE
			51. [no] passive-interface ETH_IFACE
			52. [no] outonly-interface ETH_IFACE
			53. [no] network ETH_IFACE area IPv4
			54. [no] ip ospf cost <1..65535>
			55. no ip rip enable								*/
			if ( cli_no == 50 ) cmd = arg[0] + " network " + arg[1];
			if ( cli_no == 51 ) cmd = arg[0] + " passive-interface " + arg[1];
			if ( cli_no == 52 ) cmd = arg[0] + " outonly-interface " + arg[1];
			if ( cli_no == 53 ) cmd = link_arg(arg[0]) + "network" + link_arg(arg[1]) +"area" + link_arg(arg[2]);
			if ( cli_no == 54 ) cmd = link_arg(arg[0]) + "ip ospf cost" + link_arg(arg[1]) ;
			if ( cli_no == 55 ) cmd = "no ip rip enable";
/*			56. show ospf area
			57. [no] network ETH_IFACE area IPv4
			58. [no] ip ospf cost <1..65535>
			59. ip ospf enable  
			60. [no] ip ospf authentication [message-digest | same-as-area]		
			61. ip ospf authentication-key KEY
			62. ip ospf message-digest-key ID md5 KEY			
			63. show ospf area 11.11.11.11 virtual-link 	
			
			64. ip rip send version VERSION 
			65. ip rip receive version VERSION 
			66. ip rip v2-broadcast					*/
			if ( cli_no == 56 ) cmd = "show ospf area";
			if ( cli_no == 57 ) cmd = link_arg(arg[0]) + "network" + link_arg(arg[1]) + "area" + link_arg(arg[2]) ;
			if ( cli_no == 58 ) cmd = link_arg(arg[0]) + "ip ospf cost" + link_arg(arg[1]) ;
			if ( cli_no == 59 ) cmd = "ip ospf enable";
			if ( cli_no == 60 ) cmd = link_arg(arg[0]) +"ip ospf authentication" + link_arg(arg[1]) ;
			if ( cli_no == 61 ) cmd = "ip ospf authentication-key" + link_arg(arg[0]) ;
			if ( cli_no == 62 ) cmd = "ip ospf message-digest-key" + link_arg(arg[0]) + "md5" + link_arg(arg[1]) ;
			if ( cli_no == 63 ) cmd = "show ospf area" + link_arg(arg[0]) + "virtual-link";

			if ( cli_no == 64 ) cmd = "ip rip send version" + link_arg(arg[0]);
			if ( cli_no == 65 ) cmd = "ip rip receive version" + link_arg(arg[0]);
			if ( cli_no == 66 ) cmd = arg[0] + " ip rip v2-broadcast";
			if ( cli_no == 67 ) cmd = link_arg(arg[0]) + "ip ospf priority" + link_arg(arg[1]) ;


/******** Interface ********
show ospf area
[no] network ETH_IFACE area IPv4
[no] ip ospf cost <1..65535>
passive interface 		=> router ospf + passive-interface ETH_IFACE + exit  
No passive interface 	=> router ospf + no passive-interface ETH_IFACE + exit  
ip ospf authentication [message-digest | same-as-area]
******** Interface ********/
			break;

		case "staticroute":
		case "stroutedit":
			/* show CLIs */
			if ( cli_no == 1 ) cmd = "show ip route-settings";
			/*delete & show*/
			if ( cli_no == 2 ) cmd = "no ip route"+link_arg(arg[0])+link_arg(arg[1])+link_arg(arg[2])+"&cmd=show ip route-settings";
			/* Edit ip route , delete orginal route then append new ip route*/
			if ( cli_no == 3 ) cmd = "ip route replace"+link_arg(arg[0])+link_arg(arg[1])+link_arg(arg[2])+link_arg(arg[10])+
								"with"+link_arg(arg[3])+link_arg(arg[4])+link_arg(arg[5])+link_arg(arg[6])+
								"&cmd=show ip route-settings";
			/* add new ip route*/
			if ( cli_no == 4 ) cmd = "ip route"+link_arg(arg[3])+link_arg(arg[4])+link_arg(arg[5])+link_arg(arg[6])+
								"&cmd=show ip route-settings";
			if ( cli_no == 5 ) cmd = "show interface bridge &cmd=show interface ethernet &cmd=show interface ppp &cmd=show interface vlan";
			break;

		case "interface-group":
/*			1. show interface-group {PROFILE}
			2. interface-group PROFILE
			3. mode MODE
			4. algorithm ALGORITHM
			5. interface NUM INTERFACE {passive | weight WEIGHT | limit LIMIT}
			6. no interface-group PROFILE											*/		
			if ( cli_no == 1 ) cmd = "show interface-group" + link_arg(arg[0]);
			if ( cli_no == 2 ) cmd = "interface-group" + link_arg(arg[0]);
			if ( cli_no == 3 ) cmd = "mode" + link_arg(arg[0]);
			if ( cli_no == 4 ) cmd = "algorithm" + link_arg(arg[0]);
			if ( cli_no == 5 ) cmd = link_arg(arg[3])+"interface" + link_arg(arg[0]) + link_arg(arg[1]) + link_arg(arg[2]) ;
			if ( cli_no == 6 ) cmd = "no interface-group" + link_arg(arg[0]);
			break;			

		case "hostname":
			if ( cli_no == 1 ) cmd = "show fqdn";
			if ( cli_no == 2 ) cmd = "hostname "+arg[0];
			if ( cli_no == 3 ) cmd = "domainname "+arg[0];
			if ( cli_no == 4 ) cmd = "no hostname";
			if ( cli_no == 5 ) cmd = "no domainname";
			break;
		case "baudrate":
			if(cli_no == 1) cmd = "show console";
			if(cli_no == 2) cmd = "console baud "+arg[0];
                             break;			
		case "myclock" : 
			if( cli_no == 1 ) cmd = "show clock status&cmd=show ntp server&cmd=show clock time&cmd=show clock date"; 
			if( cli_no == 2 ) cmd = "ntp server"+link_arg(arg[0]); 
			if( cli_no == 3 ) cmd = "no ntp server"; 
			if( cli_no == 4 ) cmd = "clock time-zone"+link_arg(arg[0]); 
			if( cli_no == 5 ) cmd = "clock daylight-saving"; 
			if( cli_no == 6 ) cmd = "no clock daylight-saving"; 
			if( cli_no == 7 ) cmd = "clock date "+arg[0]+"-"+arg[1]+"-"+arg[2]+" time "+arg[3]+":"+arg[4]+":"+arg[5]; 
			if( cli_no == 8 ) cmd = "clock saving-interval begin "+arg[0]+" "+arg[1]+" "+arg[2]+" "+arg[3]+":"+arg[4]+ 
			                        " end "+arg[5]+ " "+arg[6]+" "+arg[7]+" "+arg[8]+":"+arg[9]+" "+arg[10];
			break;
		case "account":
			if ( cli_no == 1 ) cmd = "show account" + link_arg(arg[0]) + link_arg(arg[1]) ;;
			if ( cli_no == 2 ) cmd = link_arg(arg[0]) + "account " + link_arg(arg[1]) + link_arg(arg[2]) ;
			if ( cli_no == 3 ) cmd = link_arg(arg[0]) + "user" + link_arg(arg[1]) ;
			if ( cli_no == 4 ) cmd = link_arg(arg[0]) + "password" + link_arg(arg[1]) ;
			if ( cli_no == 5 ) cmd = link_arg(arg[0]) + "authentication" + link_arg(arg[1]) ;

			if ( cli_no == 6 ) cmd = link_arg(arg[0]) + "service-name" + link_arg(arg[1]) ;
			if ( cli_no == 7 ) cmd = link_arg(arg[0]) + "compression" + link_arg(arg[1]) ;
			if ( cli_no == 8 ) cmd = link_arg(arg[0]) + "idle" + link_arg(arg[1]) ;
			if ( cli_no == 9 ) cmd = link_arg(arg[0]) + "period" + link_arg(arg[1]) + "allocated-budget" + link_arg(arg[2]);

			if ( cli_no == 10 ) cmd = link_arg(arg[0]) + "server" + link_arg(arg[1]) ;
			if ( cli_no == 11 ) cmd = link_arg(arg[0]) + "encryption" + link_arg(arg[1]) ;
			if ( cli_no == 12 ) cmd = link_arg(arg[0]) + "connection-id" + link_arg(arg[1]) ;

			break;

		case "policyroute":
		case "proutedit":
			/* show table */
			if ( cli_no == 1 ) cmd = "show policy-route";
			if ( cli_no == 2 ) cmd = "show policy-route";
			/* table top add button */
			if ( cli_no == 3 ) cmd = "policy insert 1 &cmd=exit&cmd=show policy-route";
			/* table add buttom*/
			if ( cli_no == 4 ) cmd = "policy insert "+arg[0]+"&cmd=exit&cmd=show policy-route";
			/* disable rule */
			if ( cli_no == 5 ) cmd = arg[0]+"&cmd=show policy-route";
			/* enable rule */
			//if ( cli_no == 6 ) cmd = "policy "+arg[0]+"&cmd=no deactivate&cmd=exit&cmd=show policy-route";
			/* delete button */
			if ( cli_no == 7 ) cmd = "policy delete "+arg[0]+"&cmd=show policy-route";
			/* move to button */
			if ( cli_no == 8 ) cmd = "policy move "+arg[0]+" to "+arg[1]+"&cmd=show policy-route";
			/* Policy Route edit Page*/
			if ( cli_no == 9 ) cmd = "show username&cmd=show groupname"+ 
					"&cmd=show interface ethernet&cmd=show interface bridge"+
					"&cmd=show interface ppp&cmd=show interface vlan&cmd=show interface-group"+
					"&cmd=show address-object&cmd=show object-group address"+
					"&cmd=show service-object&cmd=show object-group service"+
					"&cmd=show crypto map" +"&cmd=show schedule-object"+
					"&cmd=show interface virtual ethernet"+"&cmd=show sslvpn policy"+
					"&cmd=show users default-setting"+
					"&cmd= show policy-route"+link_arg(arg[0])+"trigger"+"&cmd=show policy-route"+link_arg(arg[0]);
			/* edit page usge*/
			if ( cli_no == 10 ) cmd = link_arg(arg[0])+"&cmd=exit"+"&cmd=show policy-route";
							
			if ( cli_no == 11 ) cmd = "show username &cmd=show groupname"+ 
					"&cmd=show interface ethernet&cmd=show interface bridge"+
					"&cmd=show interface ppp&cmd=show interface vlan&cmd=show interface-group"+
					"&cmd=show address-object&cmd=show object-group address"+
					"&cmd=show service-object&cmd=show object-group service"+
					"&cmd=show crypto map"+"&cmd=show schedule-object"+
					"&cmd=show interface virtual ethernet"+"&cmd=show sslvpn policy"+
					"&cmd=show users default-setting";
			/* for pop up window */
			if ( cli_no == 12 ) cmd = "show interface summary all"+
					"&cmd=show address-object&cmd=show object-group address"+
					"&cmd=show service-object&cmd=show object-group service"+
					"&cmd=show schedule-object";					
			break;
		case "interface":
			if ( cli_no == 1 ) cmd = "show interface ethernet";
			if ( cli_no == 2 ) cmd = "show interface" + link_arg(arg[0]);
			if ( cli_no == 3 ) cmd = "interface" + link_arg(arg[0]);
			if ( cli_no == 4 ) cmd = link_arg(arg[0]) + 'description' + link_arg(arg[1]);

			if ( cli_no == 5 ) cmd = 'mtu' + link_arg(arg[0]);
			if ( cli_no == 6 ) cmd = 'upstream' + link_arg(arg[0]);
			if ( cli_no == 7 ) cmd = 'downstream' + link_arg(arg[0]);

			if ( cli_no ==  8) cmd = link_arg(arg[0]) + 'ip address dhcp';
			if ( cli_no ==  9) cmd = link_arg(arg[0]) + 'ip address dhcp-pool';
			if ( cli_no == 10) cmd = link_arg(arg[0]) + 'ip address helper-address' + link_arg(arg[1]);
			if ( cli_no == 11) cmd = link_arg(arg[0]) + 'ip address rip';
			if ( cli_no == 13) cmd = link_arg(arg[0]) + 'ip address ospf';
			
			if ( cli_no == 14) cmd = "ip address" + link_arg(arg[0]) + link_arg(arg[1]);
			if ( cli_no == 15) cmd = "ip address dhcp";
			if ( cli_no == 16) cmd = 'ip gateway'+ link_arg(arg[0])+"metric"+link_arg(arg[1]);

			if ( cli_no == 17) cmd = "show ospf interface"+ link_arg(arg[0]);
			if ( cli_no == 18) cmd = "show rip interface"+ link_arg(arg[0]);

			if ( cli_no == 19) cmd = "show ip dhcp pool";
			if ( cli_no == 20) cmd = "show ip dhcp server status";
			if ( cli_no == 21) cmd = link_arg(arg[0]) + "ip dhcp-pool" + link_arg(arg[1]);
			if ( cli_no == 22) cmd = link_arg(arg[0]) + "ip helper-address"+ link_arg(arg[1]);
			if ( cli_no == 23) cmd = "show dhcp-relay";

			if ( cli_no == 25) cmd = link_arg(arg[0])+"service dhcp";//remove all dhcp pool
			
			if ( cli_no == 30) cmd =  link_arg(arg[0]) +"phone-number" + link_arg(arg[1]);
			if ( cli_no == 31) cmd =  "dialing-type" + link_arg(arg[0]);
			if ( cli_no == 32) cmd =  link_arg(arg[0]) +"port-speed" + link_arg(arg[1]);
			if ( cli_no == 33) cmd =  link_arg(arg[0]) +"initial-string" + link_arg(arg[1]);
			if ( cli_no == 34) cmd =  link_arg(arg[0]) +"dial-timeout" + link_arg(arg[1]);
			if ( cli_no == 35) cmd =  link_arg(arg[0]) +"retry-count" + link_arg(arg[1]);
			if ( cli_no == 36) cmd =  link_arg(arg[0]) +"retry-interval" + link_arg(arg[1]);
			if ( cli_no == 37) cmd =  link_arg(arg[0]) +"username" + link_arg(arg[1]);
			if ( cli_no == 38) cmd =  link_arg(arg[0]) +"password" + link_arg(arg[1]);
			if ( cli_no == 39) cmd =  link_arg(arg[0]) +"authentication" + link_arg(arg[1]);

			if ( cli_no == 40) cmd = "ip address dhcp metric" + link_arg(arg[0]);
			if ( cli_no == 41) cmd = "ip gateway"+ link_arg(arg[0]) + "metric" + link_arg(arg[1]);
			if ( cli_no == 42) cmd = "show dhcp status"+link_arg(arg[0]);
			if ( cli_no == 43) cmd = link_arg(arg[0])+"ip gateway";
			if ( cli_no == 50) cmd = link_arg(arg[0])+"idle"+link_arg(arg[1]);
			break;

		case "interface-ppp":

/*			-- PPPoE
			1. show interface ppp
			2. [no] account [PROFILE]
			3. [no] bind [ETH_FACE]
			4. [no] local-address [ADDR]
			5. [no] connectivity {nail-up | dial-on-demand}		
			6. interface no [PROFILE]		*/

			if ( cli_no == 1 ) cmd = "show interface ppp";
			if ( cli_no == 2 ) cmd = "account" + link_arg(arg[0]);
			if ( cli_no == 3 ) cmd = "bind" + link_arg(arg[0]);
			if ( cli_no == 4 ) cmd = link_arg(arg[0]) + "local-address" + link_arg(arg[1]);
			if ( cli_no == 5 ) cmd = "connectivity" + link_arg(arg[0]);
		//if ( cli_no == 6 ) cmd = link_arg(arg[0]) + "schedule" + link_arg(arg[1]);

			break;

		case "authentication":
/*			1. show aaa authentication [default | LIST_NAME]
			2. [no] aaa authentication {default | LIST_NAME } METHOD1 [METHOD2] [METHOD3]		
			3. clear aaa authentication [default | LIST_NAME]	*/
			if ( cli_no == 1 ) cmd = "show aaa authentication" + link_arg(arg[0]);
			if ( cli_no == 2 ) cmd = arg[0]+ " aaa authentication" + link_arg(arg[1]) + link_arg(arg[2]) + link_arg(arg[3]) + link_arg(arg[4]) + link_arg(arg[5]);
			if ( cli_no == 3 ) cmd = "clear aaa authentication" + link_arg(arg[0]);
			break;

		case "AAA":
/*			1. show ldap-server
			2. [no] ldap-server host LDAP_SERVER
			3. [no] ldap-server ssl
			4. [no] ldap-server port PORT_NUMBER
			5. [no] ldap-server password PASSWORD
			6. [no] ldap-server basedn QUOTED_STRING
			7. [no] ldap-server binddn QUOTED_STRING
			8. [no] ldap-server search-time-limit SECONDS 	*/
			if ( cli_no == 1 ) cmd = "show ldap-server" ;
			if ( cli_no == 2 ) cmd = link_arg(arg[0])+"ldap-server host"+link_arg(arg[1]);
			if ( cli_no == 3 ) cmd = link_arg(arg[0])+"ldap-server ssl";
			if ( cli_no == 4 ) cmd = link_arg(arg[0])+"ldap-server port"+link_arg(arg[1]);
			if ( cli_no == 5 ) cmd = link_arg(arg[0])+"ldap-server password"+link_arg(arg[1]);
			if ( cli_no == 6 ) cmd = link_arg(arg[0])+"ldap-server basedn"+link_arg(arg[1]);
			if ( cli_no == 7 ) cmd = link_arg(arg[0])+"ldap-server binddn"+link_arg(arg[1]);
			if ( cli_no == 8 ) cmd = link_arg(arg[0])+"ldap-server search-time-limit"+link_arg(arg[1]);
			if ( cli_no == 9 ) cmd = link_arg(arg[0])+"ldap-server cn-identifier"+link_arg(arg[1]);
/*			11. show radius-server
			12. [no] radius-server host RADIUS_SERVER [auth-port AUTH_PORT] [acct-port ACCT_PORT]
			13. [no] radius-server key QUOTED_STRING
			14. [no] radius-server timeout SECONDS			*/
			if ( cli_no == 11 ) cmd = "show radius-server" ;
		//	if ( cli_no == 12 ) cmd = link_arg(arg[0])+"radius-server host"+ link_arg(arg[1]) + option_field("auth-port", arg[2]) + option_field("acct-port", arg[3]);
			if ( cli_no == 12 ) cmd = link_arg(arg[0])+"radius-server host"+ link_arg(arg[1]) + option_field("auth-port", arg[2]);
			if ( cli_no == 13 ) cmd = link_arg(arg[0])+"radius-server key"+ link_arg(arg[1]);
			if ( cli_no == 14 ) cmd = link_arg(arg[0])+"radius-server timeout"+ link_arg(arg[1]) ;
/*			21. show aaa group server ldap [GROUP_NAME]
			22. clear aaa group server ldap [GROUP_NAME]
			23. [no] aaa group server ldap GROUP_NAME	// create aaa group and enter subcommand mode
				24. [no] server host LDAP_SERVER
				25. [no] server ssl
				26. [no] server port PORT_NUMBER
				27. [no] server password PASSWORD
				28. [no] server basedn QUOTED_STRING
				29. [no] server binddn QUOTED_STRING
				30. [no] server search-time-limit SECONDS*/
			if ( cli_no == 20 ) cmd = link_arg(arg[0])+"server cn-identifier" + link_arg(arg[1]) ;
			if ( cli_no == 21 ) cmd = "show aaa group server ldap" + link_arg(arg[0]) ;
			if ( cli_no == 22 ) cmd = "clear aaa group server ldap" + link_arg(arg[0]) ;
			if ( cli_no == 23 ) cmd = link_arg(arg[0])+"aaa group server ldap" + link_arg(arg[1]) ;
			if ( cli_no == 24 ) cmd = link_arg(arg[0])+"server host" + link_arg(arg[1]) ;
			if ( cli_no == 25 ) cmd = link_arg(arg[0])+"server ssl";
			if ( cli_no == 26 ) cmd = link_arg(arg[0])+"server port" + link_arg(arg[1]) ;
			if ( cli_no == 27 ) cmd = link_arg(arg[0])+"server password" + link_arg(arg[1]) ;
			if ( cli_no == 28 ) cmd = link_arg(arg[0])+"server basedn" + link_arg(arg[1]);
			if ( cli_no == 29 ) cmd = link_arg(arg[0])+"server binddn" + link_arg(arg[1]);
			if ( cli_no == 30 ) cmd = link_arg(arg[0])+"server search-time-limit" + link_arg(arg[1]) ;
/*			31. show aaa group server radius [GROUP_NAME]
			32. clear aaa group server radius [GROUP_NAME]
			33. [no] aaa group server radius GROUP_NAME
				34. [no] server host RADIUS_SERVER [auth-port AUTH_PORT] [acct-port ACCT_PORT]
				35. [no] server key QUOTED_STRING
				36. [no] server timeout SECONDS				*/
			if ( cli_no == 31 ) cmd = "show aaa group server radius" + link_arg(arg[0]) ;
			if ( cli_no == 32 ) cmd = "clear aaa group server radius" + link_arg(arg[0]) ;
			if ( cli_no == 33 ) cmd = link_arg(arg[0])+"aaa group server radius" + link_arg(arg[1]) ;
			if ( cli_no == 34 ) cmd = link_arg(arg[0])+"server host" + link_arg(arg[1]) + option_field("auth-port", arg[2]);
			if ( cli_no == 35 ) cmd = link_arg(arg[0])+"server key " + link_arg(arg[1]);
			if ( cli_no == 36 ) cmd = link_arg(arg[0])+"server timeout" + link_arg(arg[1]);
			break;


		case "user":
/*			1. show username [username]
			2. username STR_NAME { nopassword | password IN_PASSWORD_STR}
			3. username STR_NAME [no] logon-idle-timeout IDLE_TIMEOUT
			4. username STR_NAME [no] access PRIVILEGE visibility PRIVILEGE
			5. username STR_NAME [no] description DESCRIPTION*/
			if ( cli_no == 1 ) cmd = "show username" + link_arg(arg[0]);
			if ( cli_no == 2 ) cmd = "username"+link_arg(arg[0])+link_arg(arg[1])+"logon-idle-timeout"+link_arg(arg[2]);
			if ( cli_no == 3 ) cmd = "username"+link_arg(arg[0])+link_arg(arg[1])+"logon-idle-timeout"+link_arg(arg[2]);
			if ( cli_no == 4 ) cmd = "username"+link_arg(arg[0])+link_arg(arg[1])+"access"+link_arg(arg[2])+"visibility"+link_arg(arg[2]);
			if ( cli_no == 5 ) cmd = "username"+link_arg(arg[0])+link_arg(arg[1])+"description"+link_arg(arg[2]);
/*			6. show groupname [GROUP_NAME]
			7. [no] groupname GROUP_NAME
			8. [no] user USER_NAME
			9. [no] groupname GROUP_NAME
		     10. [no] description DESCRIPTION		*/
			if ( cli_no == 6 ) cmd = "show groupname" + link_arg(arg[0]);
			if ( cli_no == 7 ) cmd = link_arg(arg[0])+"groupname" + link_arg(arg[1]);
			if ( cli_no == 8 ) cmd = link_arg(arg[0])+"user" + link_arg(arg[1]);
			if ( cli_no == 9 ) cmd = link_arg(arg[0])+"groupname" + link_arg(arg[1]);
			if ( cli_no == 10 ) cmd = link_arg(arg[0])+"description" + link_arg(arg[1]);
			break;

		case "content-filter":
			if ( cli_no == 1 ) cmd = "show content-filter policy";
			if ( cli_no == 2 ) cmd = arg[0] + ' content-filter block message '+arg[1];
			if ( cli_no == 3 ) cmd = arg[0] + " content-filter block redirect"+link_arg(arg[1]);

			if ( cli_no == 5 ) cmd = "content-filter policy" +link_arg(arg[0]) +link_arg(arg[1]) +link_arg(arg[2]) +link_arg(arg[3]) +link_arg(arg[4]) ;
			if ( cli_no == 7 ) cmd = "content-filter policy insert" +link_arg(arg[0]) +link_arg(arg[1]) +link_arg(arg[2]) +link_arg(arg[3]) +link_arg(arg[4]) ;
			if ( cli_no == 8 ) cmd = "content-filter policy move" +link_arg(arg[0]) +"to"+ link_arg(arg[1]);
			if ( cli_no == 9 ) cmd = "content-filter policy" +link_arg(arg[0]) + link_arg(arg[1]);
			if ( cli_no == 10) cmd = "no content-filter policy" +link_arg(arg[0]);
			if ( cli_no == 11) cmd = link_arg(arg[0])+ "content-filter activate";
			if ( cli_no == 12) cmd = link_arg(arg[0])+ "content-filter default block";
			if ( cli_no == 20 ) cmd = "show content-filter profile" + link_arg(arg[0]) ;  	
			if ( cli_no == 21 ) cmd =  link_arg(arg[0])+ "content-filter profile" + link_arg(arg[1]) ; 
			if ( cli_no == 22 ) cmd = link_arg(arg[0])+"content-filter profile"+link_arg(arg[1])+"url url-server";
			if ( cli_no == 23 ) cmd = link_arg(arg[0])+"content-filter profile"+link_arg(arg[1])+"url match"+link_arg(arg[2]);  	
			if ( cli_no == 24 ) cmd = link_arg(arg[0])+"content-filter profile"+link_arg(arg[1])+"url unrate"+link_arg(arg[2]);  	
			if ( cli_no == 25 ) cmd = link_arg(arg[0])+"content-filter profile"+link_arg(arg[1])+"url offline"+link_arg(arg[2]);  	
			if ( cli_no == 26 ) cmd = link_arg(arg[0])+"content-filter profile"+link_arg(arg[1])+"url category"+link_arg(arg[2]);  	
			if ( cli_no == 27 ) cmd = "content-filter profile rename"+link_arg(arg[0])+link_arg(arg[1]); 
			if ( cli_no == 28 ) cmd = "content-filter service-timeout"+link_arg(arg[0]);
			if ( cli_no == 30) cmd = link_arg(arg[0])+"content-filter profile "+link_arg(arg[1])+"custom";
			if ( cli_no == 31) cmd = link_arg(arg[0])+"content-filter profile "+link_arg(arg[1])+"custom trust-only";
			if ( cli_no == 32) cmd = link_arg(arg[0])+"content-filter profile "+link_arg(arg[1])+"custom trust-allow-features";
			if ( cli_no == 33) cmd = link_arg(arg[0])+"content-filter profile "+link_arg(arg[1])+"custom activex";
			if ( cli_no == 34) cmd = link_arg(arg[0])+"content-filter profile "+link_arg(arg[1])+"custom java";
			if ( cli_no == 35) cmd = link_arg(arg[0])+"content-filter profile "+link_arg(arg[1])+"custom cookie";
			if ( cli_no == 36) cmd = link_arg(arg[0])+"content-filter profile "+link_arg(arg[1])+"custom proxy";
			if ( cli_no == 37) cmd = link_arg(arg[0])+"content-filter profile "+link_arg(arg[1])+"custom trust"+link_arg(arg[2]);
			if ( cli_no == 38) cmd = link_arg(arg[0])+"content-filter profile "+link_arg(arg[1])+"custom forbid"+link_arg(arg[2]);
			if ( cli_no == 39) cmd = link_arg(arg[0])+"content-filter profile "+link_arg(arg[1])+'custom keyword'+link_arg(arg[2]); 
			if ( cli_no == 40) cmd = "content-filter url-server test"+link_arg(arg[0]);
			if ( cli_no == 41) cmd = "content-filter url-cache test"+link_arg(arg[0]);
			if ( cli_no == 50) cmd = "show content-filter url-cache";
			if ( cli_no == 51) cmd = "show content-filter settings";
			if ( cli_no == 52) cmd = "content-filter cache-timeout"+link_arg(arg[0]);
			if ( cli_no == 53) cmd = link_arg(arg[0])+"content-filter license"+link_arg(arg[1]);
			if ( cli_no == 54) cmd = link_arg(arg[0])+"content-filter service-timeout"+link_arg(arg[1]);
			if ( cli_no == 55) cmd = "clear content-filter url-cache";
			if ( cli_no == 56) cmd = "clear content-filter url-cache"+link_arg(arg[0]);
			
			break;
			
		case "service":
			if ( cli_no == 1 ) cmd = "show service-object";
			if ( cli_no == 2 ) cmd = "show object-group service" + link_arg(arg[0]) ;		

			if ( cli_no == 10 ) cmd = "service-object" +link_arg(arg[0]) + link_arg(arg[1]) + link_arg(arg[2]) + link_arg(arg[3]) + link_arg(arg[4]);
			if ( cli_no == 11 ) cmd = "object-group service" +link_arg(arg[0]);
			
			if ( cli_no == 20 ) cmd = "service-object" + link_arg(arg[0]) + link_arg(arg[1]);
			if ( cli_no == 21 ) cmd = "service-object rename" + link_arg(arg[0]) + link_arg(arg[1]);
			if ( cli_no == 22 ) cmd = "object-group service rename" + link_arg(arg[0]) + link_arg(arg[1]);
			
			if ( cli_no == 30 ) cmd = "no service-object" +link_arg(arg[0]); 
			if ( cli_no == 31 ) cmd = "no object-group service" +link_arg(arg[0]); 
			
			if ( cli_no == 40 ) cmd = 'description' + link_arg(arg[0]); 
			if ( cli_no == 41 ) cmd = "service-object" + link_arg(arg[0]);
			if ( cli_no == 42 ) cmd = "no service-object" + link_arg(arg[0]); 
			if ( cli_no == 43 ) cmd = "no object-group" + link_arg(arg[0]); 
			if ( cli_no == 44 ) cmd = "object-group" + link_arg(arg[0]);
			
			break;

		case "schedule":
			if ( cli_no == 1 ) cmd = "show schedule-object";
			if ( cli_no == 2 ) cmd = "show object-group schedule" + link_arg(arg[0]) ;		

			
			// ********** Generate CLIs ********** 
			// schedule-object NAME hh:mm hh:mm		 schedule-object SC1 11:00 23:00
			if ( cli_no == 10 ) cmd = "schedule-object" +link_arg(arg[0]) + link_arg(arg[1]) + link_arg(arg[2]);
			// schedule-object NAME yyyymmdd hh:mm yyyymmdd hh:mm 
			if ( cli_no == 11 ) cmd = "schedule-object" +link_arg(arg[0]) + link_arg(arg[1]) + link_arg(arg[2]) + link_arg(arg[3]) + link_arg(arg[4]) ;


			// ********** Update CLIs ********** 
			// schedule-object  NAME {IPv4|IPv4 schedule range|IPv4 subnet in CIDR format}  // Replace schedule-object entity
			// schedule-object  rename OLD_NAME NEW_NAME 
			if ( cli_no == 20 ) cmd = "schedule-object" + link_arg(arg[0]) + link_arg(arg[1]);
			if ( cli_no == 21 ) cmd = "schedule-object rename" + link_arg(arg[0]) + link_arg(arg[1]);
			
			// ********** Delete CLIs ********** 
			// no schedule-object NAME
			if ( cli_no == 30 ) cmd = "no schedule-object" +link_arg(arg[0]); 
			if ( cli_no == 31 ) cmd = "no object-group schedule" +link_arg(arg[0]); 


			// ********** SubCommand CLIs ********** 
			if ( cli_no == 40 ) cmd = "description" + link_arg(arg[0]) ;  	// description NAME
			if ( cli_no == 41 ) cmd = "schedule-object" + link_arg(arg[0]) ;  	// schedule-object NAME // add schedule-object into group
			if ( cli_no == 42 ) cmd = "no schedule-object" + link_arg(arg[0]) ;  	// no schedule-object NAME // delete schedule-object from group
			if ( cli_no == 43 ) cmd = "no object-group" + link_arg(arg[0]) ;  	// no object-group NAME // delete object-group from group
			if ( cli_no == 44 ) cmd = "object-group" + link_arg(arg[0]) ;  	// object-group NAME // add object-group into group
			
			break;

		case "address":
			// ********** Show CLIs **********
			if ( cli_no == 1 ) cmd = "show address-object";
			if ( cli_no == 2 ) cmd = "show object-group address" + link_arg(arg[0]) ;		// show object-group address [NAME]


			// ********** Generate CLIs ********** 
			// address-object NAME {IPv4|IPv4 address range|IPv4 subnet in CIDR format}
			if ( cli_no == 10 ) cmd = "address-object" +link_arg(arg[0]) + link_arg(arg[1]) ;
			if ( cli_no == 11 ) cmd = "object-group address" +link_arg(arg[0]);
			if ( cli_no == 21 ) cmd = "address-object rename" + link_arg(arg[0]) + link_arg(arg[1]);
			if ( cli_no == 22 ) cmd = "object-group address rename" + link_arg(arg[0]) + link_arg(arg[1]);
		
			if ( cli_no == 30 ) cmd = "no address-object" +link_arg(arg[0]); 
			if ( cli_no == 31 ) cmd = "no object-group address" +link_arg(arg[0]); 

			if ( cli_no == 40 ) cmd = 'description'+link_arg(arg[0]);  	// description NAME
			if ( cli_no == 41 ) cmd = "address-object" + link_arg(arg[0]) ;  	// address-object NAME // add address-object into group
			if ( cli_no == 42 ) cmd = "no address-object" + link_arg(arg[0]) ;  	// no address-object NAME // delete address-object from group
			if ( cli_no == 43 ) cmd = "no object-group" + link_arg(arg[0]) ;  	// no object-group NAME // delete object-group from group
			if ( cli_no == 44 ) cmd = "object-group" + link_arg(arg[0]) ;  	// object-group NAME // add object-group into group			
			break;
			
		case "certificate": 
			// ********** Show CLIs ********** 
			// show ca category {local|remote} 
			if ( cli_no == 1 ) cmd = "show ca category" +link_arg(arg[0]); 
			// show ca category {local|remote} [name NAME format {text|pem}] 
			if ( cli_no == 2 ) cmd = "show ca category" +link_arg(arg[0]) + "name" +link_arg(arg[1]) +"format" +link_arg(arg[2]); 
			// show ca category {local|remote} [name NAME] certpath 
			if ( cli_no== 3 ) cmd = "show ca category" +link_arg(arg[0]) + "name" +link_arg(arg[1]) +"certpath"; 
			// show ca spaceusage  -- show space used by certificates (Using Space / Total Space * 100%) 
			if ( cli_no == 4 ) cmd = "show ca spaceusage" ; 
			// show ca validation name NAME 
			if ( cli_no == 5 ) cmd = "show ca validation name" +link_arg(arg[0]); 


			// ********** Generate CLIs ********** 
			// ca generate x509 name NAME cn-type {ip cn IPv4|fqdn cn FQDN|mail cn EMAIL} [ou NAME] [o NAME] [c NAME] key-type {rsa|dsa} key-len KEYLEN 
			// ca generate pkcs10 name NAME cn-type {ip cn IPv4|fqdn cn FQDN|mail cn EMAIL} [ou NAME] [o NAME] [c NAME] key-type {rsa|dsa} key-len KEYLEN 
			if ( cli_no == 10 ) cmd = "ca generate" +link_arg(arg[0])+ "name" +link_arg(arg[1])+ "cn-type" +link_arg(arg[2])+ "cn" +link_arg(arg[3])+ option_field("ou",arg[4])+ option_field("o",arg[5])+option_field("c",arg[6])+ "key-type"+ link_arg(arg[7])+ "key-len" +link_arg(arg[8]); 
			// ca enroll cmp name NAME cn-type {ip cn IPv4|fqdn cn FQDN|mail cn EMAIL} [ou NAME] [o NAME] [c NAME] key-type {rsa|dsa} key-len KEYLEN num <0..99999999> password PASSWORD ca NAME url URL; 
			if ( cli_no == 11 ) cmd = "ca enroll" +link_arg(arg[0])+ "name" +link_arg(arg[1])+ "cn-type" +link_arg(arg[2])+ "cn" +link_arg(arg[3])+ option_field("ou",arg[4])+ option_field("o",arg[5])+option_field("c",arg[6])+ "key-type"+ link_arg(arg[7])+ "key-len" +link_arg(arg[8])+"num"+link_arg(arg[9])+ "password"+link_arg(arg[10])+"ca"+link_arg(arg[11])+"url"+link_arg(arg[12]); 
			// ca enroll scep name NAME cn-type {ip cn IPv4|fqdn cn FQDN|mail cn EMAIL} [ou NAME] [o NAME] [c NAME] key-type {rsa|dsa} key-len KEYLEN password PASSWORD ca NAME url URL 
			if ( cli_no == 12 ) cmd = "ca enroll" +link_arg(arg[0])+ "name" +link_arg(arg[1])+ "cn-type" +link_arg(arg[2])+ "cn" +link_arg(arg[3])+ option_field("ou",arg[4])+ option_field("o",arg[5])+option_field("c",arg[6])+ "key-type"+ link_arg(arg[7])+ "key-len" +link_arg(arg[8])+ "password"+link_arg(arg[10])+"ca"+link_arg(arg[11])+"url"+link_arg(arg[12]); 


			// ********** Update CLIs ********** 
			// 20. ca rename category {local|remote} OLD_NAME NEW_NAME    /*rename certificate*/ 
			if ( cli_no == 20 ) cmd = "ca rename category" +link_arg(arg[0])+ link_arg(arg[1])+ link_arg(arg[2]); 


			// ********** Delete CLIs ********** 
			// no ca category {local|remote} NAME /* remove certificate */ 
			if ( cli_no == 30 ) cmd = "no ca category "+link_arg(arg[0])+ link_arg(arg[1]); 
			// no ca validation NAME  /* remove ca validation (back to default value) */ 
			if ( cli_no == 31 ) cmd = "no ca validation "+link_arg(arg[0]); 


			// ********** SubCommand CLIs ********** 
			// 40. ca validation NAME  -- sub-config for ca validation , for Trusted Certificates, this command will enter sub-config mode 
			if ( cli_no == 40 ) cmd = "ca validation" +link_arg(arg[0]); 
			// 41. ca validation NAME cdp {activate|deactivate} 
			if ( cli_no == 41 ) cmd = "cdp" + link_arg(arg[1]) ; 
			// 42. ca validation NAME ocsp {activate|deactivate} 
			if ( cli_no == 42 ) cmd = "ocsp" +link_arg(arg[1]); 
			// 43. ca validation NAME ocsp url URL [id NAME password PASSWORD] [deactivate] 
			if ( cli_no == 43 ) cmd = "ocsp url" +link_arg(arg[1])+ option_field("id",arg[2]) + option_field("password",arg[3]) + link_arg(arg[4]);
			// 44. ca validation NAME ldap {activate|deactivate} 
			if ( cli_no == 44 ) cmd = "ldap" +link_arg(arg[1]); 
			// 45. ca validation NAME ldap ip {IPv4|FQDN} port <1..65535> [id NAME password PASSWORD] [deactivate] 
			if ( cli_no == 45 ) cmd = "ldap ip" +link_arg(arg[1])+ "port" +link_arg(arg[2])+ option_field("id",arg[3]) + option_field("password",arg[4]) + link_arg(arg[5]);

			// ********** Import CLIs ********** 
			// ca import category {local|remote} name NAME [password PASSWORD]   /* currently, default upload path would be /tmp/, you should put cert here import a certificate (either My Certificates or Trusted Certificates) 
			

			break; 

		case "VPN":
			//	VPN Connection commands
			if ( cli_no == 1 ) cmd = "show crypto map" + link_arg(arg[0]) ;
			if ( cli_no == 2 ) cmd = link_arg(arg[0]) + "crypto map" + link_arg(arg[1])  ;
			if ( cli_no == 30) cmd = "show sa monitor";

			// VPN Gateway
			if ( cli_no == 51 ) cmd = "show isakmp policy" + link_arg(arg[0]) ;
			if ( cli_no == 52 ) cmd = link_arg(arg[0]) + "isakmp policy" + link_arg(arg[1]) ;

/*	VPN Connection commands
#define ISAKMP_ALGO     des-md5 |des-sha|       \
	                        3des-md5|3des-sha|      \
	                        aes128-md5|aes128-sha|  \
	                        aes192-md5|aes192-sha|  \
	                        aes256-md5|aes256-sha

#define CRYPTO_ALGO_ESP esp-null-md5|esp-null-sha|      \
                        esp-des-md5|esp-des-sha|        \
                        esp-3des-md5|esp-3des-sha|      \
                        esp-aes128-md5|esp-aes128-sha|  \
                        esp-aes192-md5|esp-aes192-sha|  \
                        esp-aes256-md5|esp-aes256-sha
                        
#define CRYPTO_ALGO_AH  ah-md5|ah-sha

1. crypto map show [NAME]
2. [no] crypto map NAME	// enter crypto map sub-command mode

// VPN Bind Gateway
 	crypto map NAME {ipsec-isakmp NAME|ipsec-manual}

	crypto map NAME encapsulation {tunnel|transport}

// Algorithm 
	crypto map NAME transform-set {CRYPTO_ALGO_ESP} [(CRYPTO_ALGO_ESP) [(CRYPTO_ALGO_ESP)]]	
	crypto map NAME transform-set	{CRYPTO_ALGO_AH} [(CRYPTO_ALGO_AH) [(CRYPTO_ALGO_AH)]]

// Algorithm - Manual Key - Set Key
	crypto map NAME set session-key {ah <256 - 4095> AUTHKEY |esp <256 - 4095> cipher ENCKEY authenticator AUTHKEY }
										SPI 
// IKE
	crypto map NAME set security-association lifetime seconds <180 - 3000000>
	crypto map NAME set pfs {group1|group2|group5}

// Policy
	crypto map NAME local-policy PROFILE
	crypto map NAME remote-policy PROFILE

// Property
	crypto map NAME local-ip {IPv4|INTERFACE}
	crypto map NAME peer-ip IPv4

	crypto map NAME nail-up
	crypto map NAME replay-detection
	crypto map NAME netbios-broadcast

// Inbound/Outbound traffic NAT ?? no ready

Gateway:		
	1. isakmp policy show [NAME] 	// show isakmp policy summary / detail
	2. [no] isakmp policy NAME 		//enter isakmp policy sub-command mode					

	// Phase 1
	// mode {main|aggressive}
	// Proposal  
	// transform-set {ISAKMP_ALGO} [(ISAKMP_ALGO) [(ISAKMP_ALGO)]]
	// lifetime <180 - 3000000>
	// ??? {group1|group2|group5}
	// natt
	// dpd [interval <30..3000> threshold <1..10>]
	
	// Property
	// local-ip {IPv4|FQDN|INTERFACE}	
	// peer-ip {IPv4|FQDN} [{IPv4|FQDN}]
	
	// Authentication Method
	// authentication {pre-share|rsa-sig}
	// keystring PRESHREKEY
	// certificate NAME
	// local-id type {ip IPv4|fqdn FQDN|mail EMAIL}
	// peer-id type {any|ip IPv4|fqdn FQDN|mail EMAIL}
	
	// Extended Authentication 
	// xauth type {server|client name NAME password PASSWORD}		*/

			break;

		case "AS":
			/****** Summary ******/
			// 1. anti-spam {activate | deactivate}
			// 2. anti-spam smtp-notification  {activate | deactivate}
			// 3. anti-spam smtp-notification message [SMTP-NOTIFICATION]
			if ( cli_no == 1 ) cmd = "anti-spam" + link_arg(arg[0]) ;
			if ( cli_no == 2 ) cmd = "anti-spam smtp-notification" + link_arg(arg[0]) ;
			if ( cli_no == 3 ) cmd = "anti-spam smtp-notification message" + link_arg(arg[0]) ;

			// 4. anti-spam zone IN_ZONE_PROFILE OUT_ZONE_PROFILE [activate|deactivate]
			// 4. anti-spam zone IN_ZONE_PROFILE OUT_ZONE_PROFILE
			// 6. spam-score <0..100>
			// 7. spam-log [activate|deactivate]
			// 0. {smtp | pop3 | imap4} {activate|deactivate}
			// 0. smtp action {pass | drop | mark}
			// 0. {pop3 | imap4} action {pass | mark}
			// 0. {smtp | pop3 | imap4} mark [SPAM-MARK]
			//13. show anti-spam zone [IN_ZONE_PROFILE OUT_ZONE_PROFILE]
			if ( cli_no == 4 ) cmd = "anti-spam zone" + link_arg(arg[0]);
			if ( cli_no == 6 ) cmd = "spam-score" + link_arg(arg[0]) ;
			if ( cli_no == 7 ) cmd = link_arg(arg[0]) ;
			if ( cli_no == 13 ) cmd = "show anti-spam zone" + link_arg(arg[0]);

			/****** White / Black List ******/
			// 20. anti-spam {white-list | black-list} {activate | deactivate}
			// 00. anti-spam {white-list | black-list} 
			// 00. { append | insert RULENUM | RULENUM } 
			// 00. { ip IPv4 MASK | e-mail E-MAIL |mail-header MAIL-HEADER MAIL-HEADER-VALUE }
			// 00. { move OLD_RULENUM to NEW_RULENUM |delete RULENUM |activate RULENUM |deactivate RULENUM }
			if ( cli_no == 20 ) cmd = "anti-spam" + link_arg(arg[0]) + link_arg(arg[1]);

			/****** RBL / ORDBL ******/
			// 30. anti-spam rbl max-query <1..10> {activate | deactivate}
			// 31. anti-spam rbl-list { append | insert RULENUM | RULENUM } server-address { RBL-SERVER-ADDRESS|IPv4 } query-postfix RBL-QUERY_POSTFIX [deactivate]
			// 31. anti-spam rbl-list { move OLD_RULENUM to NEW_RULENUM |delete RULENUM |activate RULENUM |deactivate RULENUM }
			// 33. show anti-spam { white-list | black-list | rbl-list } [RULENUM]
			// 33. show anti-spam { white-list | black-list | rbl-list } status
			// 33. show anti-spam max-rbl-query
			if ( cli_no == 30 ) cmd = "anti-spam rbl max-query" + link_arg(arg[0]) + link_arg(arg[1]);
			if ( cli_no == 31 ) cmd = "anti-spam rbl-list" + link_arg(arg[0]);
			if ( cli_no == 33 ) cmd = "show anti-spam" + link_arg(arg[0]);

			/****** Update SPAM Database ******/
			// 40. show anti-spam status
			// 41. show anti-spam smtp-notification
			if ( cli_no == 40 ) cmd = "show anti-spam status";
			if ( cli_no == 41 ) cmd = "show anti-spam smtp-notification";
			if ( cli_no == 42 ) cmd = "no anti-spam zone" + link_arg(arg[0]) + link_arg(arg[1]);
			if ( cli_no == 43 ) cmd = "anti-spam update database";
			if ( cli_no == 44 ) cmd = "show anti-spam database version"+"&cmd=show anti-spam update"+"&cmd=show service-register status as";
			break;
		case "configfile":
			if ( cli_no == 1 ) cmd = "dir /conf";
			if ( cli_no == 2 ) cmd = "apply /conf/" + arg[0];
			if ( cli_no == 3 ) cmd = "rename /conf/" + arg[0] + " /conf/" + arg[1];
			if ( cli_no == 4 ) cmd = "delete /conf/" + arg[0];
			if ( cli_no == 5 ) cmd = "copy /conf/" + arg[0] + " /conf/" + arg[1];
/*
# delete /conf/FILENAME.conf
# rename /conf/OLD.conf /conf/NEW.conf
# copy /conf/FOO.conf /conf/BAR.conf
# apply /script/FOO.zysh
*/
			break;
		case "shellscript":
			if ( cli_no == 1 ) cmd = "dir /script";
			if ( cli_no == 2 ) cmd = "exit&cmd=run /script/" + arg[0];
			if ( cli_no == 3 ) cmd = "rename /script/" + arg[0] + " /script/" + arg[1];
			if ( cli_no == 4 ) cmd = "delete /script/" + arg[0];
			if ( cli_no == 5 ) cmd = "copy /script/" + arg[0] + " /script/" + arg[1];
/*
# delete /zysh/FILENAME.zysh
# rename /zysh/OLD.zysh /zysh/NEW.zysh
# copy /zysh/FOO.zysh /zysh/BAR.zysh
# apply /script/FOO.zysh
*/
			break;
		case "fwfile":
			if ( cli_no == 1 ) cmd = "show version";
			break;
		case "idpfile":
			if ( cli_no == 1 ) cmd = "show idp signatures version"+"&cmd=show idp signatures date"+"&cmd=show idp signature update"+
									"&cmd=show idp signatures number"+"&cmd=show service-register status idp";
			if ( cli_no == 2 ) cmd = "idp signature update signatures&cmd=show idp signatures version&cmd=show idp signatures date&cmd=show idp signature update"+
									"&cmd=show idp signatures number"+"&cmd=show service-register status idp";
			if ( cli_no == 3 ) cmd = arg[0]+"&cmd=show idp signatures version"+"&cmd=show idp signatures date"+
			                 "&cmd=show idp signature update"+"&cmd=show idp signatures number"+"&cmd=show service-register status idp";			
			if ( cli_no == 8 ) cmd = "no idp signature"+link_arg(arg[0])+link_arg(arg[1]);//exit and delete new record								
			break;
		case "DeviceHA":
		case "vrrpgrp":	
		/*[no] device-ha vrrp-group NAME
		[no] vrid <1..254>
		[no] interface ETH_IFACE
		[no] role {master|backup}
		[no] priority <1..254> (default 100)
		
		[no] preempt
		[no] manage-ip IPv4_CIDR
		[no] authentication {string KEYSTRING| ah-md5 KEYSTRING }
		[no] description DESCRIPTION
		exit

		show vrrp
		*/
			if ( cli_no == 1 ) cmd = arg[0] + " device-ha vrrp-group " + arg[1];
			if ( cli_no == 2 ) cmd = arg[0] + " vrid " + arg[1];
			if ( cli_no == 3 ) cmd = arg[0] + " interface " + arg[1];
			if ( cli_no == 4 ) cmd = arg[0] + " role " + arg[1];
			if ( cli_no == 5 ) cmd = arg[0] + " priority " + arg[1];
			
			if ( cli_no == 6 ) cmd = arg[0] + " preempt";
			if ( cli_no == 7 ) cmd = arg[0] + " manage-ip " + arg[1]+ " "+ arg[2];
			if ( cli_no == 8 ) cmd = arg[0] + " authentication " + arg[1]+ " "+ arg[2];
			if ( cli_no == 9 ) cmd = arg[0] + ' description' + link_arg(arg[1]);
			if ( cli_no == 10 ) cmd = "show device-ha vrrp-group";
			if ( cli_no == 11 ) cmd = "show device-ha status";
			if ( cli_no == 12 ) cmd = arg[0]+"&cmd=show device-ha vrrp-group"+"&cmd=show device-ha status";
			if ( cli_no == 13 ) cmd = "&cmd=show interface ethernet&cmd=show interface vlan";
			break;
		case "usrset":
		case "usrsetedit":
			if ( cli_no == 1 ) cmd = "show users default-setting&cmd=show users simultaneous-logon-settings&cmd=show users retry-settings"+
			 	"&cmd=show users update-lease-settings&cmd=show users idle-detection-settings&cmd=show force-auth policy all";
			if ( cli_no == 2 ) cmd = "force-auth policy"+link_arg(arg[0])+link_arg(arg[1]);
			if ( cli_no == 3 ) cmd = "show address-object&cmd=show object-group address&cmd=show schedule-object";
			if ( cli_no == 4 ) cmd = "force-auth policy"+link_arg(arg[0])+"move"+link_arg(arg[1])+"to"+link_arg(arg[2])
			break;
		case "contra":
		case "contraedit":
			if ( cli_no == 1 ) cmd = "show vpn-concentrator";
			if ( cli_no == 2 ) cmd = "no vpn-concentrator "+arg[0];
			if ( cli_no == 3 ) cmd = "show zone&cmd=show address-object&cmd=show object-group address&cmd=show schedule-object";
			/* contraedit */
			if ( cli_no == 4 ) cmd = arg[0] + " vpn-concentrator " + arg[1];
			if ( cli_no == 5 ) cmd = arg[0] + " crypto " + arg[1];
			break;
		case "l2tps":
			if ( cli_no == 1 ) cmd = "show l2tp-over-ipsec&cmd=show crypto map&cmd=show address-object&cmd=show aaa authentication"+
					"&cmd=show username&cmd=show groupname&cmd=show interface all";
			break;
			/* ZyWALL USG 300 */
		case "sslvpn": 
			if ( cli_no == 1 ) cmd = "show sslvpn policy";
			if ( cli_no == 2 ) cmd = "no sslvpn policy"+link_arg(arg[0]);
			if ( cli_no == 3 ) cmd = "sslvpn policy move"+link_arg(arg[0])+"to"+link_arg(arg[1]);
			if ( cli_no == 4 ) cmd = "show sslvpn application";
			break;
		case "application": 
			if ( cli_no == 1 ) cmd = "show sslvpn application";
			if ( cli_no == 2 ) cmd = "no sslvpn application"+link_arg(arg[0]);
			break;
		case "sslvpnmonitor":
			if ( cli_no == 1 ) cmd = "show sslvpn monitor";
			if ( cli_no == 2 ) cmd = "sslvpn no connection _vpnid"+link_arg(arg[0]);//dis-connect
			break;
		case "sslvpnglobal":
			if ( cli_no == 1 ) cmd = "show sslvpn network-extension local-ip&cmd=show sslvpn login message"+
					"&cmd=show sslvpn logout message";
			if ( cli_no == 2 ) cmd = "sslvpn network-extension local-ip"+link_arg(arg[0])+"&cmd=sslvpn login message"+link_arg(arg[1])
					+"&cmd=sslvpn logout message"+link_arg(arg[2]);
			break;
	}
	if (cmd == "") alert("Can't get CLI command! #" + cli_no);
	//alert("CLI No:"+cli_no+" / "+cmd);
	return cmd;
}

function vpnsa_fetch_1(start_index){
	var sourcePtr = top.frames[4]['result_'+start_index]; 
	var sourcecnt = sourcePtr.length; 

	var theDataPtr = gl.vpnsaList; // get data pointer	
	theDataPtr.dataSet.splice(0,theDataPtr.dataCount());

	for( var j=0; j<sourcecnt; j++){
		var start = sourcePtr[j]._In.search(/\(/);
		var start2 = sourcePtr[j]._Out.search(/\(/);
		var end = sourcePtr[j]._In.search(/\s/);
		var end2 = sourcePtr[j]._Out.search(/\s/);
		var iB = sourcePtr[j]._In.slice(start+1, end);
		var oB = sourcePtr[j]._Out.slice(start2+1, end2);
		
		theDataPtr.addNewRecord(
			name=sourcePtr[j]._Name,
			encap=sourcePtr[j]._Encapsulation,
			policy=sourcePtr[j]._Policy, 
			algor=sourcePtr[j]._Algorithm, 
			cont=0,
			uptime=sourcePtr[j]._UpTime,
			timeout=sourcePtr[j]._Timeout,
			spi = sourcePtr[j]._SPI,
			iB,
			oB);
	}
}
		

function vpngedit_fetch_1(start_index, index){
	var sourcePtr = top.frames[4]['result_'+start_index]; 
	//var sourcecnt = sourcePtr.length; 
	var thegyPtr = gl.vpngList.dataSet[index].edgy;
	thegyPtr.dataSet.splice(0,thegyPtr.dataCount());
	thegyPtr.addNewRecord(
						negmode = sourcePtr[0]._ISAKMP_policy[0]._negotiation_mode,
						satime	=if_undefined(sourcePtr[0]._ISAKMP_policy[0]._SA_lifetime), 
						nattral	=convert_to_bolean(2, sourcePtr[0]._ISAKMP_policy[0]._NAT_traversal),
						dpd		=convert_to_bolean(2,sourcePtr[0]._ISAKMP_policy[0]._dead_peer_detection), 
						myaddr	=( sourcePtr[0]._ISAKMP_policy[0]._my_address._type ==  "interface" ? 0 : 1 ), 
						inter	=( sourcePtr[0]._ISAKMP_policy[0]._my_address._type ==  "interface" ? sourcePtr[0]._ISAKMP_policy[0]._my_address.__name : "" ),
						interdes	="", 
						dname	=( sourcePtr[0]._ISAKMP_policy[0]._my_address._type ==  "interface" ? "" : sourcePtr[0]._ISAKMP_policy[0]._my_address.__name ),  
						gateway1	=if_undefined(sourcePtr[0]._ISAKMP_policy[0]._secure_gateway_address[0]._address), 
						gateway2	=if_undefined(sourcePtr[0]._ISAKMP_policy[0]._secure_gateway_address[1]._address), 
						authmeth	=( sourcePtr[0]._ISAKMP_policy[0]._authentication_method ==  "pre-share" ? 0 : 1 ) , 
						psk		=if_undefined(sourcePtr[0]._ISAKMP_policy[0]._pre_shared_key), 
						cert		=if_undefined(sourcePtr[0]._ISAKMP_policy[0]._certificate),
						lidtype	=if_undefined(sourcePtr[0]._ISAKMP_policy[0]._local_ID._type ), 
						lidcontent	=if_undefined(sourcePtr[0]._ISAKMP_policy[0]._local_ID.__name), 
						pidtype	=if_undefined(sourcePtr[0]._ISAKMP_policy[0]._peer_ID._type),
						pidcontent=if_undefined(sourcePtr[0]._ISAKMP_policy[0]._peer_ID.__name),
						enextauth=if_undefined(sourcePtr[0]._ISAKMP_policy[0]._X_Auth.__name), 
						extauth	=( sourcePtr[0]._ISAKMP_policy[0]._X_Auth._type == "client" ? 1 : 0 ), 
						eun		=if_undefined(sourcePtr[0]._ISAKMP_policy[0]._X_Auth._username), 
						epw		=if_undefined(sourcePtr[0]._ISAKMP_policy[0]._X_Auth._password),
						proposal_tmp	= " ",
						pfs 		=if_undefined(sourcePtr[0]._ISAKMP_policy[0]._key_group),
						server_name=if_undefined(sourcePtr[0]._ISAKMP_policy[0]._X_Auth._method)
						);
	/* Has pointer*/
	var i=1;
	while( sourcePtr[0]._ISAKMP_policy[0]._proposal[i-1] != undefined ){
	if( sourcePtr[0]._ISAKMP_policy[0]._proposal[i-1]._authentication != "" ){
		thegyPtr.dataSet[0].proposal.addNewRecord( sourcePtr[0]._ISAKMP_policy[0]._proposal[i-1]._encryption, sourcePtr[0]._ISAKMP_policy[0]._proposal[i-1]._authentication);
		thegyPtr.dataSet[0].proposal_tmp = if_undefined(thegyPtr.dataSet[0].proposal_tmp) + sourcePtr[0]._ISAKMP_policy[0]._proposal[i-1]._encryption +"-"+ sourcePtr[0]._ISAKMP_policy[0]._proposal[i-1]._authentication +" ";
	}
	i++;
	}
	/* show interface all*/
	var sourcePtr = top.frames[4]['result_'+(start_index+1)]; 
	var sourcecnt = sourcePtr.length; 
	var theDataPtr = gl.globalList;
	theDataPtr.dataSet.splice(0, theDataPtr.dataCount());//clear data base
	for( var j=0; j<sourcecnt; j++){
		theDataPtr.addNewRecord(
			mb1=sourcePtr[j]._Name,
			mb2=sourcePtr[j]._Status,
			mb3=sourcePtr[j]._IP_Address, 
			mb4=sourcePtr[j]._Mask, 
			mb5 = sourcePtr[j]._IP_Assignment
			);
	}
}

function vpng_fetch_1(start_index){
	var sourcecnt = 0;
	var sourcePtr = top.frames[4]['result_'+start_index]; 
	var theDataPtr = gl.vpngList; // get data pointer	
	theDataPtr.dataSet.splice(0,theDataPtr.dataCount());
	if (sourcePtr[0]._ISAKMP_policy != undefined) { 
		sourcecnt = sourcePtr[0]._ISAKMP_policy.length; 
	} else {
		sourcecnt = 0;
	}
	for (var k=0; k<sourcecnt; k++){
		var str = (if_undefined(sourcePtr[0]._ISAKMP_policy[k]._secure_gateway_address[1]._address) == "")? "":", "+if_undefined(sourcePtr[0]._ISAKMP_policy[k]._secure_gateway_address[1]._address);
		
		var vstr='';	
		if(if_undefined(sourcePtr[0]._ISAKMP_policy[k]._VPN_connection)!=''){
			vstr += if_undefined(sourcePtr[0]._ISAKMP_policy[k]._VPN_connection);
			for(var jj=2; jj<1024; jj++){
				if(if_undefined(sourcePtr[0]._ISAKMP_policy[k]['_VPN_connection_'+jj])!=''){
					vstr += ", "+(if_undefined(sourcePtr[0]._ISAKMP_policy[k]['_VPN_connection_'+jj]));
				}else{
					break;
				}
			}
		}
		theDataPtr.addNewRecord(name	=sourcePtr[0]._ISAKMP_policy[k].__name, 
					addr	=sourcePtr[0]._ISAKMP_policy[k]._my_address.__name, 
					gateway	=if_undefined(sourcePtr[0]._ISAKMP_policy[k]._secure_gateway_address[0]._address) +str,
					cont	= vstr,  
					active	=convert_to_bolean(2, sourcePtr[0]._ISAKMP_policy[k]._active),  
					orig_active=convert_to_bolean(2, sourcePtr[0]._ISAKMP_policy[k]._active) );
	}
	/**********************/
//	if (start_index!=0) return;
	var sourcePtr = top.frames[4]['result_'+ (start_index+1)]; 
	var sourcecnt = sourcePtr.length; 
	var theDataPtr = gl.ConfigEtherList;
	theDataPtr.dataSet.splice(0,theDataPtr.dataCount());
	for( var j=0; j<sourcecnt; j++){
		theDataPtr.addNewRecord(
			Active="", 
			Name=sourcePtr[j]._Name, 
			selNum=( sourcePtr[j]._Address_Type == "dhcp" ? 0 : 1 ), 
			ip=if_undefined(sourcePtr[j]._IP_Address),
			gwip="", 
			subnetmask=if_undefined(sourcePtr[j]._Mask),  
			ifport=0, vlantag="", vlancomm="", upstream="", 
			downstream="", mtu="", ripdir="", MultiEnable=0, MultiVer=0,
			area=0, linkcost="", passive=0, auth=0, sak="", mad="", mak="",
			dhcpflag=2, dhcpserver="", dhcprelay1="", dhcprelay2="");
	}
	var sourcePtr = top.frames[4]['result_'+ (start_index+2)]; 
    var sourcecnt = sourcePtr.length;
    var theDataPtr = gl.ConfigEtherList;
    for( var j=0; j<sourcecnt; j++){
        theDataPtr.addNewRecord(
                       Active="",
                                Name=sourcePtr[j]._Name,
                                selNum=( sourcePtr[j]._Address_Type == "dhcp" ? 0 : 1 ), 
                                ip=if_undefined(sourcePtr[j]._IP_Address),
                                gwip="",
				subnetmask=if_undefined(sourcePtr[j]._Mask),  
                                ifport=0, vlantag="", vlancomm="", upstream="",
                                downstream="", mtu="", ripdir="", MultiEnable=0, MultiVer=0,
                                area=0, linkcost="", passive=0, auth=0, sak="", mad="", mak="",
                                dhcpflag=2, dhcpserver="", dhcprelay1="", dhcprelay2="");
	}
    var sourcePtr = top.frames[4]['result_'+ (start_index+3)];
	if (sourcePtr[0]._ppp_interface != undefined) {
    	var sourcecnt = sourcePtr[0]._ppp_interface.length;
	} else {
    	var sourcecnt = 0;
	}
    var theDataPtr = gl.ConfigEtherList;
    for( var j=0; j<sourcecnt; j++){
        theDataPtr.addNewRecord(
                                Active=sourcePtr[0]._ppp_interface[j]._active,
                                Name=sourcePtr[0]._ppp_interface[j]._interface_name,
                                '',//selNum=( sourcePtr[j]._Address_Type == "dhcp" ? 0 : 1 ), 
                              	ip=if_undefined(sourcePtr[0]._ppp_interface[j]._IP_address),
                                gwip="",
					subnetmask=if_undefined(sourcePtr[0]._ppp_interface[j]._netmask),  
                                ifport=0, vlantag="", vlancomm="", upstream="",
                                downstream="", mtu="", ripdir="", MultiEnable=0, MultiVer=0,
                                area=0, linkcost="", passive=0, auth=0, sak="", mad="", mak="",
                                dhcpflag=2, dhcpserver="", dhcprelay1="", dhcprelay2="");
	}
    var sourcePtr = top.frames[4]['result_'+ (start_index+4)];
    var sourcecnt = sourcePtr.length;
    var theDataPtr = gl.ConfigEtherList;
    for( var j=0; j<sourcecnt; j++){
        theDataPtr.addNewRecord(
                            Active="",
                            Name=sourcePtr[j]._Name,
                            selNum=( sourcePtr[j]._Address_Type == "dhcp" ? 0 : 1 ), 
                            ip=if_undefined(sourcePtr[j]._IP_Address),
                            gwip="",
				subnetmask=if_undefined(sourcePtr[j]._Mask),  
                            ifport=0, vlantag="", vlancomm="", upstream="",
                            downstream="", mtu="", ripdir="", MultiEnable=0, MultiVer=0,
                            area=0, linkcost="", passive=0, auth=0, sak="", mad="", mak="",
                            dhcpflag=2, dhcpserver="", dhcprelay1="", dhcprelay2="");
	}
	/***************************/
	var sourcePtr = top.frames[4]['result_'+ (start_index+5)]; 
	var sourcecnt = sourcePtr.length; 
	var theDataPtr = gl.mycertList; 
	if (sourcePtr[0]._certificate != undefined) {
    		var sourcecnt = sourcePtr[0]._certificate.length;
	} else {
    		var sourcecnt = 0;
	}
		theDataPtr.dataSet.splice(0,theDataPtr.dataCount());
		for( j=0; j<sourcecnt; j++){
	//reebuf, name, type, subject, issuer, validfrom, validto, id, content, status
			theDataPtr.addNewRecord(0, 
			sourcePtr[0]._certificate[j].__name,
			sourcePtr[0]._certificate[j]._type,
			sourcePtr[0]._certificate[j]._subject,
			sourcePtr[0]._certificate[j]._issuer,
			sourcePtr[0]._certificate[j]._valid_from,
			sourcePtr[0]._certificate[j]._valid_to,
			sourcePtr[0]._certificate[j]._ID._type,
			sourcePtr[0]._certificate[j]._ID.__name,
			sourcePtr[0]._certificate[j]._status);
	}
	
	authmeth_fetch_1(start_index+6);
}

function vpnedit_fetch_1(start_index, index){
	var sourcePtr = top.frames[4]['result_'+start_index]; 
	/*============= IKE ==============*/
	var theikePtr = gl.vpncList.getRecord(index).edike;
	theikePtr.dataSet.splice(0,theikePtr.dataCount());
	theikePtr.addNewRecord(	ndes="",
				protocol=sourcePtr[0]._cryptography_mapping[0]._active_protocol, 
				encap	=sourcePtr[0]._cryptography_mapping[0]._encapsulation, 
				satime	=if_undefined(sourcePtr[0]._cryptography_mapping[0]._SA_lifetime), 
				pfs	=if_undefined(sourcePtr[0]._cryptography_mapping[0]._PFS), 
				lpolicy	=if_undefined(sourcePtr[0]._cryptography_mapping[0]._local_policy), 
				lpolicydesc="", 
				rpolicy	=if_undefined(sourcePtr[0]._cryptography_mapping[0]._remote_policy),  	
				rpolicydesc="",  
				nailup	=convert_to_bolean(2, sourcePtr[0]._cryptography_mapping[0]._nail_up), 
				replay	=convert_to_bolean(2, sourcePtr[0]._cryptography_mapping[0]._replay_detection), 	
				netbios	=convert_to_bolean(2, sourcePtr[0]._cryptography_mapping[0]._NetBIOS_broadcast_over_IPSec), 
				advflag	=0,
				proposal_tmp = "" ,
				policyenforce = sourcePtr[0]._cryptography_mapping[0]._policy_enforcement);
				
	theikePtr.getRecord(0).proposal.dataSet.splice(0,theikePtr.getRecord(0).proposal.dataCount());
	for(var i=0; i<sourcePtr[0]._cryptography_mapping[0]._transform_set.length; i++){
		if( sourcePtr[0]._cryptography_mapping[0]._transform_set[i]._authentication != "" ){			
			theikePtr.getRecord(0).proposal.addNewRecord( 
				sourcePtr[0]._cryptography_mapping[0]._transform_set[i]._encryption , 
				sourcePtr[0]._cryptography_mapping[0]._transform_set[i]._authentication
				);
			if ( sourcePtr[0]._cryptography_mapping[0]._active_protocol == "esp" ){
				theikePtr.getRecord(0).proposal_tmp = if_undefined(theikePtr.dataSet[0].proposal_tmp) + sourcePtr[0]._cryptography_mapping[0]._active_protocol +"-"+ sourcePtr[0]._cryptography_mapping[0]._transform_set[i]._encryption +"-"+  sourcePtr[0]._cryptography_mapping[0]._transform_set[i]._authentication+" ";
			}else{
				theikePtr.getRecord(0).proposal_tmp = if_undefined(theikePtr.dataSet[0].proposal_tmp) + sourcePtr[0]._cryptography_mapping[0]._active_protocol +"-"+ sourcePtr[0]._cryptography_mapping[0]._transform_set[i]._authentication+" ";
			}
		}	
	}
	
	if ( sourcePtr[0]._cryptography_mapping[0]._outbound_SNAT != "" || sourcePtr[0]._cryptography_mapping[0]._inbound_SNAT != "" ) {
		var str = 'no';
		if(sourcePtr[0]._cryptography_mapping[0]._inbound_DNAT.__name != undefined){
			str = sourcePtr[0]._cryptography_mapping[0]._inbound_DNAT.__name;
		}else{
			str = sourcePtr[0]._cryptography_mapping[0]._inbound_DNAT;
		}
		theikePtr.getRecord(0).tnat.addNewRecord(
							outtraffic=convert_to_bolean(2, sourcePtr[0]._cryptography_mapping[0]._outbound_SNAT.__name), 
							osnatsrc=sourcePtr[0]._cryptography_mapping[0]._outbound_SNAT._source, 
							osnatdest=sourcePtr[0]._cryptography_mapping[0]._outbound_SNAT._destination, 
							osnat=sourcePtr[0]._cryptography_mapping[0]._outbound_SNAT._target,								
							intrafficsrc=convert_to_bolean(2, sourcePtr[0]._cryptography_mapping[0]._inbound_SNAT.__name), 
							isnatsrc=sourcePtr[0]._cryptography_mapping[0]._inbound_SNAT._source, 
							isnatdest=sourcePtr[0]._cryptography_mapping[0]._inbound_SNAT._destination, 
							isnat=sourcePtr[0]._cryptography_mapping[0]._inbound_SNAT._target, 
							idnatf=convert_to_bolean(2, str),
							idnat_cnt=0	  );										
		theikePtr.dataSet[0].advflag = 1;
	}
	if (sourcePtr[0]._cryptography_mapping[0]._inbound_DNAT._index != undefined) {
		for (var i=0; i<(sourcePtr[0]._cryptography_mapping[0]._inbound_DNAT._index.length); i++){
			if( sourcePtr[0]._cryptography_mapping[0]._inbound_DNAT._index[i] != undefined ){
				theikePtr.getRecord(0).tnat.dataSet[0].idnat.addNewRecord( 
											oip=sourcePtr[0]._cryptography_mapping[0]._inbound_DNAT._index[i]._original_IP, 
											mip=sourcePtr[0]._cryptography_mapping[0]._inbound_DNAT._index[i]._mapped_IP, 
										   	protocol=sourcePtr[0]._cryptography_mapping[0]._inbound_DNAT._index[i]._protocol,  
											opf=sourcePtr[0]._cryptography_mapping[0]._inbound_DNAT._index[i]._original_port_begin, 
											opt=sourcePtr[0]._cryptography_mapping[0]._inbound_DNAT._index[i]._original_port_end, 
											mpf=sourcePtr[0]._cryptography_mapping[0]._inbound_DNAT._index[i]._mapped_port_begin, 
											mpt=sourcePtr[0]._cryptography_mapping[0]._inbound_DNAT._index[i]._mapped_port_end);
			}
		}
		theikePtr.getRecord(0).tnat.dataSet[0].idnat_cnt = sourcePtr[0]._cryptography_mapping[0]._inbound_DNAT._index.length;
	}


	/*************** Manual Key ***************/
	var themkPtr = gl.vpncList.getRecord(index).edmk;
	themkPtr.dataSet.splice(0,themkPtr.dataCount());
	themkPtr.addNewRecord(	spi 	= if_undefined(sourcePtr[0]._cryptography_mapping[0]._SPI),
				encap	= sourcePtr[0]._cryptography_mapping[0]._encapsulation, 
				espah	= sourcePtr[0]._cryptography_mapping[0]._active_protocol, 
				espencry= if_undefined(sourcePtr[0]._cryptography_mapping[0]._transform_set[0]._encryption), 
				espauth	= if_undefined(sourcePtr[0]._cryptography_mapping[0]._transform_set[0]._authentication), 
				encrykey= if_undefined(sourcePtr[0]._cryptography_mapping[0]._encryption_key), 
				authkey	= if_undefined(sourcePtr[0]._cryptography_mapping[0]._authentication_key), 
				lpolicy	= if_undefined(sourcePtr[0]._cryptography_mapping[0]._local_policy), 
				lpolicydesc="", 
				rpolicy	= if_undefined(sourcePtr[0]._cryptography_mapping[0]._remote_policy), 
				rpolicydesc="", 
				myaddr	= if_undefined(sourcePtr[0]._cryptography_mapping[0]._my_address), 
				securegy= if_undefined(sourcePtr[0]._cryptography_mapping[0]._secure_gateway_address), 
				enetbios= convert_to_bolean(2, sourcePtr[0]._cryptography_mapping[0]._NetBIOS_broadcast_over_IPSec), 
				advflag	= 0); 

	if ( sourcePtr[0]._cryptography_mapping[0]._outbound_SNAT != "" || sourcePtr[0]._cryptography_mapping[0]._inbound_SNAT != "" ){
		var str = 'no';
		if(sourcePtr[0]._cryptography_mapping[0]._inbound_DNAT.__name != undefined){
			str = sourcePtr[0]._cryptography_mapping[0]._inbound_DNAT.__name;
		}else{
			str = sourcePtr[0]._cryptography_mapping[0]._inbound_DNAT;
		}
		themkPtr.dataSet[0].tnat.addNewRecord(
							outtraffic=convert_to_bolean(2, sourcePtr[0]._cryptography_mapping[0]._outbound_SNAT.__name), 
							osnatsrc=sourcePtr[0]._cryptography_mapping[0]._outbound_SNAT._source, 
							osnatdest=sourcePtr[0]._cryptography_mapping[0]._outbound_SNAT._destination, 
							osnat=sourcePtr[0]._cryptography_mapping[0]._outbound_SNAT._target, 
							intrafficsrc=convert_to_bolean(2, sourcePtr[0]._cryptography_mapping[0]._inbound_SNAT.__name), 
							isnatsrc=sourcePtr[0]._cryptography_mapping[0]._inbound_SNAT._source, 
							isnatdest=sourcePtr[0]._cryptography_mapping[0]._inbound_SNAT._destination, 
							isnat=sourcePtr[0]._cryptography_mapping[0]._inbound_SNAT._target, 
							idnatf=convert_to_bolean(2, str),
											idnat_cnt = 0		);
		themkPtr.dataSet[0].advflag = 1;
	}
	if (sourcePtr[0]._cryptography_mapping[0]._inbound_DNAT._index != undefined) {
		for(var i=0; i<(sourcePtr[0]._cryptography_mapping[0]._inbound_DNAT._index.length); i++){
			if( sourcePtr[0]._cryptography_mapping[0]._inbound_DNAT._index[i] != undefined ){
			themkPtr.dataSet[0].tnat.dataSet[0].idnat.addNewRecord(
										oip=sourcePtr[0]._cryptography_mapping[0]._inbound_DNAT._index[i]._original_IP, 
										mip=sourcePtr[0]._cryptography_mapping[0]._inbound_DNAT._index[i]._mapped_IP, 
							   			protocol=sourcePtr[0]._cryptography_mapping[0]._inbound_DNAT._index[i]._protocol,  
								 		opf=sourcePtr[0]._cryptography_mapping[0]._inbound_DNAT._index[i]._original_port_begin, 
										opt=sourcePtr[0]._cryptography_mapping[0]._inbound_DNAT._index[i]._original_port_end, 
										mpf=sourcePtr[0]._cryptography_mapping[0]._inbound_DNAT._index[i]._mapped_port_begin, 
										mpt=sourcePtr[0]._cryptography_mapping[0]._inbound_DNAT._index[i]._mapped_port_end);
		
			}
		}
		themkPtr.dataSet[0].tnat.dataSet[0].idnat_cnt = sourcePtr[0]._cryptography_mapping[0]._inbound_DNAT._index.length;
	}
}

function vpnc_fetch_1(start_index){
	var sourcePtr = top.frames[4]['result_'+start_index]; 
	var theDataPtr = gl.vpncList;
	theDataPtr.dataSet.splice(0,theDataPtr.dataCount());
	if (sourcePtr[0]._cryptography_mapping != undefined) { 
		var sourcecnt = sourcePtr[0]._cryptography_mapping.length; 
		for( var j = 0; j < sourcecnt; j++){
		var gyname= 0;
			if (sourcePtr[0]._cryptography_mapping[j]._VPN_gateway == "manual key"){
				gyname = 1;
			}
			var algorStr='';
			for(var g=0; g<(sourcePtr[0]._cryptography_mapping[j]._transform_set.length); g++){
				algorStr += sourcePtr[0]._cryptography_mapping[j]._transform_set[g]._encryption +"/"
								+ sourcePtr[0]._cryptography_mapping[j]._transform_set[g]._authentication+' '; 
			}
			theDataPtr.addNewRecord(name=sourcePtr[0]._cryptography_mapping[j].__name, 
						gateway=sourcePtr[0]._cryptography_mapping[j]._VPN_gateway,
						encap=sourcePtr[0]._cryptography_mapping[j]._encapsulation, 
						algor=algorStr,
						policy=sourcePtr[0]._cryptography_mapping[j]._local_policy + '/' + sourcePtr[0]._cryptography_mapping[j]._remote_policy, 
						active=convert_to_bolean(2, sourcePtr[0]._cryptography_mapping[j]._active),
						gyname,
							orig_active=convert_to_bolean(2, sourcePtr[0]._cryptography_mapping[j]._active),
							connected = sourcePtr[0]._cryptography_mapping[j]._connected//yes or no
						); 
		}
	}
	//if (start_index != 0) return;
	/*******************************************/
	var sourcePtr = top.frames[4]['result_'+(start_index+1)]; 
	var sourcecnt = sourcePtr.length; 
	var theDataPtr = gl.AddressList; 
	theDataPtr.dataSet.splice(0,theDataPtr.dataCount());
	for( var j = 0; j < sourcecnt; j++){
		theDataPtr.addNewRecord( sourcePtr[j]._Object_name, sourcePtr[j]._Type, sourcePtr[j]._Address, sourcePtr[j]._Address_2);
	}
	
	/* show isakmp policy */
	var sourcePtr = top.frames[4]['result_'+(start_index+2)]; 
	var theDataPtr = gl.vpngList;
	theDataPtr.dataSet.splice(0,theDataPtr.dataCount());
	if (sourcePtr[0]._ISAKMP_policy != undefined) { 
		var sourcecnt = sourcePtr[0]._ISAKMP_policy.length; 
	} else {
		var sourcecnt = 0;
	}
	for (var k = 0; k < sourcecnt; k++) {
		var str = (if_undefined(sourcePtr[0]._ISAKMP_policy[k]._secure_gateway_address[1]._address) == "")? "":", "+if_undefined(sourcePtr[0]._ISAKMP_policy[k]._secure_gateway_address[1]._address);	
		var vstr='';	
		if(if_undefined(sourcePtr[0]._ISAKMP_policy[k]._VPN_connection)!=''){
			vstr += if_undefined(sourcePtr[0]._ISAKMP_policy[k]._VPN_connection);
			for(var jj = 2; jj < 1024; jj++) {
				if(if_undefined(sourcePtr[0]._ISAKMP_policy[k]['_VPN_connection_'+jj])!=''){
					vstr += ", "+(if_undefined(sourcePtr[0]._ISAKMP_policy[k]['_VPN_connection_'+jj]));
				}else{
					break;
				}
			}
		}
		theDataPtr.addNewRecord(sourcePtr[0]._ISAKMP_policy[k].__name,
								sourcePtr[0]._ISAKMP_policy[k]._my_address.__name,
								if_undefined(sourcePtr[0]._ISAKMP_policy[k]._secure_gateway_address[0]._address) +str,
								vstr,
								convert_to_bolean(2, sourcePtr[0]._ISAKMP_policy[k]._active),
								convert_to_bolean(2, sourcePtr[0]._ISAKMP_policy[k]._active) );
	}
}

function virEdit_fetch( idx , start_index){
	var sourcePtr = top.frames[4]['result_'+start_index]; 
	var sourcecnt = sourcePtr.length; 
	var virPtr = top.frames[1].ConfigEtherList.getRecord(idx); 

	virPtr.GWIP.address	= sourcePtr[0]._gateway;
	virPtr.VlanComm		= sourcePtr[0]._description;
	virPtr.Upstream 	= sourcePtr[0]._upstream;
	virPtr.Downstream 	= sourcePtr[0]._downstream;
	virPtr.Metric		= sourcePtr[0]._metric;
}

function dhcp_server_fetch_1(index,start_index){
	
	/*-------- show ip dhcp pool & show ip dhcp server status ---------*/
	var dynDhcpPtr = top.frames[1].dyndhcpList; 
	dynDhcpPtr.dataSet.splice(0,dynDhcpPtr.dataCount()); 
		
	var bindCnt = "";
	var k = 2;
		
	/*-- check which one is static DHCP or network pool --*/
	var Ptr = top.frames[4]['result_'+start_index];
		
	/*-- check bind interface --*/ 
	var Ptr_1 = top.frames[4]['result_'+(start_index+1)];
		
	/*-- Within specific ethernet --*/
	var theIfacePtr = top.frames[1].ConfigEtherList.getRecord(index); // get data pointer	
		
	theIfacePtr.dhcpserver.dataSet.splice(0,theIfacePtr.dhcpserver.dataCount());
	gl.BufferObj.getRecord(index).objMb1.dataSet.splice(0, gl.BufferObj.getRecord(index).objMb1.dataCount());
	
	var noNetPool = true;
	
	if(Ptr_1[0]._binding_interface != undefined ){ 
		for(var i = 0; i < Ptr_1[0]._binding_interface.length; i++) {
			if(Ptr_1[0]._binding_interface[i].__name == theIfacePtr.name) {
				while(Ptr_1[0]._binding_interface[i]['_binding_pool'+bindCnt] != undefined) {
					for(var j = 0; j < Ptr[0]._DHCP_pool_name.length; j++) {
						if(Ptr_1[0]._binding_interface[i]['_binding_pool'+bindCnt] == Ptr[0]._DHCP_pool_name[j].__name) {
							if(Ptr[0]._DHCP_pool_name[j]._network != "none") {
								noNetPool = false;
								dynDhcpPtr.addNewRecord("network_pool",
								                        Ptr[0]._DHCP_pool_name[j]._starting_address,
								                        Ptr[0]._DHCP_pool_name[j]._pool_size,
								                        Ptr[0]._DHCP_pool_name[j].__name,'',
								                        Ptr[0]._DHCP_pool_name[j]._first_dns_server,
								                        Ptr[0]._DHCP_pool_name[j]._second_dns_server,
								                        Ptr[0]._DHCP_pool_name[j]._third_dns_server,
								                        Ptr[0]._DHCP_pool_name[j]._lease_time,'','',
								                        Ptr[0]._DHCP_pool_name[j]._first_wins_server,
								                        Ptr[0]._DHCP_pool_name[j]._second_wins_server);										
								theIfacePtr.dhcpserver.addNewRecord("network_pool",Ptr[0]._DHCP_pool_name[j].__name);
							}else if(Ptr[0]._DHCP_pool_name[j]._host != "none") {
								dynDhcpPtr.addNewRecord("static_pool",
											Ptr[0]._DHCP_pool_name[j]._host,
											Ptr[0]._DHCP_pool_name[j]._hardware_address,
											Ptr[0]._DHCP_pool_name[j].__name,
											true);
								theIfacePtr.dhcpserver.addNewRecord("static_pool",Ptr[0]._DHCP_pool_name[j].__name);
							}	
							
						}
					}
					bindCnt = "_"+k;
					k++;
				}
			}
		}
	}
	if(noNetPool) {
		dynDhcpPtr.insertRecord(0,"network_pool",'','','','','','','', '3 day 0 hour 0 min','','','','');
	}
	/*===================== DHCP Relay Fetch ==================*/
	
	var dataPtr = top.frames[4]['result_'+(start_index+2)];
	
	if(dataPtr[0]._dhcp_relay != undefined ){ 
		var sourcecnt = dataPtr[0]._dhcp_relay.length; 
		for(var j = 0; j < sourcecnt; j++){
			if(dataPtr[0]._dhcp_relay[j].__name == theIfacePtr.name){
				for(var k = 0; k < dataPtr[0]._dhcp_relay[j]._server.length; k++) {
					theIfacePtr["dhcprelay"+(k+1)] = dataPtr[0]._dhcp_relay[j]._server[k].__name;
				}
			}
		}
	}	
}

function auxiliary_fetch_1(start_index){
	var sourcePtr = top.frames[4]['result_'+start_index]; 
	var sourcecnt = sourcePtr.length; 
	var theDataPtr = gl.AuxList;
	theDataPtr.dataSet.splice(0, theDataPtr.dataCount());

	if ( sourcePtr[0]._description != undefined ) {
		var comments = sourcePtr[0]._description;
	}else {
		var comments = "";
	}
	theDataPtr.addNewRecord( 	comments	=comments, 
					port_speed	=if_undefined(sourcePtr[0]._port_speed), 
					dialing_type	=if_undefined(sourcePtr[0]._dialing_type), 
					init_string	=if_undefined(sourcePtr[0]._initial_string), 
					phone_number	=if_undefined(sourcePtr[0]._phone_number), 
					user_name	=if_undefined(sourcePtr[0]._username), 
					password	=if_undefined(sourcePtr[0]._password), 
					auth_type	=if_undefined(sourcePtr[0]._authentication_type), 
					timeout		=if_undefined(sourcePtr[0]._dial_timeout), 
					'', 
					'',
					active		=if_undefined(sourcePtr[0]._active),  
					idletime 	=if_undefined(sourcePtr[0]._idle_timeout) );
	if(sourcePtr[0]._timeout != "0") {
		gl.TimeOuthasBeenChg = true;
 	}
}

function zone_fetch_1(start_index){
	/*===== Show zone =======*/
	var sourcePtr = top.frames[4]['result_'+start_index]; 
	var theDataPtr = gl.ZoneDataList;
	var theBufPtr = gl.BufferObj;
	theDataPtr.dataSet.splice(0,theDataPtr.dataCount());
	theBufPtr.dataSet.splice(0,theBufPtr.dataCount());
	
	var sourcecnt = sourcePtr.length; 
	for(var i = 0; i < sourcecnt; i++){
		var k = "";
		var j = 1;
		var mbr = "";
		while(sourcePtr[i]["_Member"+k] != undefined) {
			mbr+=(sourcePtr[i]["_Member"+k]+", ");
			j++;
			k = "_"+j;
		}
		mbr = mbr.slice(0, mbr.length-2);
		theDataPtr.addNewRecord(sourcePtr[i]._Name, mbr, "", "", "", convert_to_bolean(2, sourcePtr[i]._Block));
		theBufPtr.addNewRecord(sourcePtr[i]._Name, convert_to_bolean(2, sourcePtr[i]._Block));
	}
	/*======  Show zone none-binding --> Unbound interfaces ======*/
	var sourcePtr = top.frames[4]['result_'+(start_index+1)]; 
	var sourcecnt = sourcePtr.length; 
	var theDataPtr = gl.ifaceList;	
	theDataPtr.dataSet.splice(0,theDataPtr.dataCount());
	for (var i = 0; i < sourcecnt; i++){
			theDataPtr.addNewRecord(sourcePtr[i], "", "", type="iface");
	}
	/*======  Show zone none-binding --> Unbound tunnels ======*/
	var sourcePtr = top.frames[4]['result_'+(start_index+1)+'_1']; 
	var sourcecnt = sourcePtr.length;
	for ( var i = 0; i < sourcecnt; i++){
			theDataPtr.addNewRecord(sourcePtr[i], "", "", type="ipsec");
	}
	/*======  Show zone none-binding --> Unbound SSL VPN ======*/
	var sourcePtr = top.frames[4]['result_'+(start_index+1)+'_2']; 
	var sourcecnt = sourcePtr.length;
	for ( var i = 0; i < sourcecnt; i++){
			theDataPtr.addNewRecord(sourcePtr[i], "", "", type="sslvpn");
	}
}

function zone_fetch_2(index){
	var sourcePtr = top.frames[4]['result_0_1'];
	var theDataPtr = gl.ZoneDataList.getRecord(index).gnormal; // get Zone member pointer	
	theDataPtr.bindSet.splice(0,theDataPtr.dataCount());
 
  	var sourcecnt = sourcePtr.length;
	var noneBindPtr = gl.ifaceList;
	var kk = 0;
	for(var k = 0; k < noneBindPtr.length; k++) {
		if(noneBindPtr.getRecord(k).type == "ipsec") {
			kk = k-1;
			break;
		}
	}
	
	gl.old_zone_iflist.splice(0,gl.old_zone_iflist.length);
	for(var i = 0; i < sourcecnt; i++){
		if ( sourcePtr[i]._Type == "interface" ) {
			var member = "IFACE/" + sourcePtr[i]._Member;
			noneBindPtr.insertRecord(kk++, sourcePtr[i]._Member, "", "", "iface");
		}else if ( sourcePtr[i]._Type == "tunnel" ){
			var member = "IPSEC/" + sourcePtr[i]._Member;
			noneBindPtr.addNewRecord(sourcePtr[i]._Member, "", "", "ipsec");
		}else if ( sourcePtr[i]._Type == "sslvpn" ){
			var member = "SSLVPN/" + sourcePtr[i]._Member;
			noneBindPtr.addNewRecord(sourcePtr[i]._Member, "", "", "sslvpn");
		}
		theDataPtr.addNewRecord(member);
		gl.old_zone_iflist[i] = member;
	}
}

function ospfedit_fetch_1(i){
	gl.BufferObj.getRecord(i).objMb1.dataSet.splice(0,gl.BufferObj.getRecord(i).objMb1.dataCount());
	var sourcePtr = top.frames[4].result_0;
	if(sourcePtr.length == 0) return; // no virtual-link was defined
	
	var theDataPtr = gl.OSPFAreaList.getRecord(i).link; // get data pointer	
	var sourcePtr = top.frames[4].result_0[0]._peer_router_id;
	var sourcecnt = sourcePtr.length; 
	theDataPtr.dataSet.splice(0,theDataPtr.dataCount());
	
	for(var j=0; j<sourcecnt; j++){
		theDataPtr.addNewRecord(sourcePtr[j].__name, sourcePtr[j]._authentication_type,
														(sourcePtr[j]._text_string == "none" ? "" : sourcePtr[j]._text_string),
														(sourcePtr[j]._MD5_key == "none" ? "" : sourcePtr[j]._MD5_key),
														(sourcePtr[j]._MD5_string == "none" ? "" : sourcePtr[j]._MD5_string));
		gl.BufferObj.getRecord(i).objMb1.addNewRecord(sourcePtr[j].__name);
	}
	if ( sourcecnt > 0 ) 
		gl.OSPFAreaList.dataSet[i].active = true; 
	else 
		gl.OSPFAreaList.dataSet[i].active = false; 
}

function ospf_fetch_1(start_index){
	var sourcePtr, sourcecnt, auth_select, router_id;	
	
	sourcePtr = top.frames[4]['result_'+start_index]; 
	sourcecnt = sourcePtr.length; 
	var theDataPtr = gl.OSPFList; 		// get data pointer	
	if ( sourcePtr[0]._router_id == "default"){
		auth_select = 0;
		router_id = "";
	}else {
		auth_select = 1;
		router_id = sourcePtr[0]._router_id;
	}
	theDataPtr.dataSet.splice(0,theDataPtr.dataCount());
	theDataPtr.addNewRecord(auth_select, router_id,  
													convert_to_bolean(2,sourcePtr[0]._redistribute_RIP ), 
													convert_to_bolean(2,sourcePtr[0]._redistribute_static ), 
													(sourcePtr[0]._redistribute_RIP_type == "none" ? "1" : sourcePtr[0]._redistribute_RIP_type), 
													(sourcePtr[0]._redistribute_static_type == "none" ? "1" : sourcePtr[0]._redistribute_static_type),  
													(sourcePtr[0]._redistribute_RIP_metric == "none" ? "" : sourcePtr[0]._redistribute_RIP_metric), 
													(sourcePtr[0]._redistribute_static_metric == "none" ? "" : sourcePtr[0]._redistribute_static_metric));
	/************************/
	sourcePtr = top.frames[4]['result_'+(start_index+1)]; 
	var theOAreaPtr = gl.OSPFAreaList; 	// get data pointer	
	theOAreaPtr.dataSet.splice(0,theOAreaPtr.dataCount());
	gl.BufferObj.dataSet.splice(0,gl.BufferObj.dataCount());
	
	if(sourcePtr.length != 0) {
		sourcecnt = sourcePtr[0]._OSPF_Area.length;
	}else {
		sourcecnt = 0;
	}
	var area_type, auth_type;
	for ( var i=0; i<sourcecnt; i++){
		if ( sourcePtr[0]._OSPF_Area[i]._area_type == "stub"){
			area_type = 1;
		}else if ( sourcePtr[0]._OSPF_Area[i]._area_type == "nssa"){
			area_type = 2;
		}else {
			area_type = 0;
		}

		if ( sourcePtr[0]._OSPF_Area[i]._authentication_type == "text"){
			auth_type = 1;
		}else if ( sourcePtr[0]._OSPF_Area[i]._authentication_type == "md5"){
			auth_type = 2;
		}else{
			auth_type = 0;
		}
		theOAreaPtr.addNewRecord(	sourcePtr[0]._OSPF_Area[i].__name, 
															area_type, auth_type, 
															(sourcePtr[0]._OSPF_Area[i]._text_string == "none" ? "" : sourcePtr[0]._OSPF_Area[i]._text_string), 
															(sourcePtr[0]._OSPF_Area[i]._MD5_key == "none" ? "" : sourcePtr[0]._OSPF_Area[i]._MD5_key), 
															(sourcePtr[0]._OSPF_Area[i]._MD5_string == "none" ? "" : sourcePtr[0]._OSPF_Area[i]._MD5_string),	0);
		
		gl.BufferObj.addNewRecord(area_type, auth_type, 
															(sourcePtr[0]._OSPF_Area[i]._text_string == "none" ? "" : sourcePtr[0]._OSPF_Area[i]._text_string), 
															(sourcePtr[0]._OSPF_Area[i]._MD5_key == "none" ? "" : sourcePtr[0]._OSPF_Area[i]._MD5_key), 
															(sourcePtr[0]._OSPF_Area[i]._MD5_string == "none" ? "" : sourcePtr[0]._OSPF_Area[i]._MD5_string),	0);
	}
}

function rip_fetch_1(start_index){
	var auth_select = 0;
	var sourcePtr = top.frames[4]['result_'+start_index]; 
	var theDataPtr = gl.RipList; // get data pointer	

	if ( sourcePtr[0]._authentication_type == "text"){
		auth_select = 1;
	}else if ( sourcePtr[0]._authentication_type == "md5"){
		auth_select = 2;
	}	
	theDataPtr.dataSet.splice(0,theDataPtr.dataCount());
	theDataPtr.addNewRecord(auth_select, 
												(sourcePtr[0]._text_string == "none" ? "" : sourcePtr[0]._text_string), 
												(sourcePtr[0]._MD5_key == "none" ? "" : sourcePtr[0]._MD5_key), 
												(sourcePtr[0]._MD5_string == "none" ? "" : sourcePtr[0]._MD5_string), 
												(sourcePtr[0]._redistribute_OSPF_metric == "none" ? "" : sourcePtr[0]._redistribute_OSPF_metric ), 
												convert_to_bolean(2, sourcePtr[0]._redistribute_OSPF), 
												(sourcePtr[0]._redistribute_static_metric == "none" ? "" : sourcePtr[0]._redistribute_static_metric ), 
												convert_to_bolean(2, sourcePtr[0]._redistribute_static) );
}

function gpEdit_fetch(index, pos) {
	var sourcePtr = top.frames[4].result_0; 
	var sourcecnt = sourcePtr.length; 
	var theDataPtr = gl.GupsDataList;
	
	if (sourcecnt != 0)  {
		if(sourcePtr[0]._Algorithm == "wrr" ) {// Weight Round Robin
			gl.gpeditIdx  = 2;	
		}else if (sourcePtr[0]._Algorithm == "llf") {// Least Load First
			gl.gpeditIdx  = 3;	
		}else if(sourcePtr[0].algorithm == "spill-over") {// Spillover
			gl.gpeditIdx  = 4;
		}
	}
	
	var sourcePtr = top.frames[4].result_0_1;
	var sourcecnt = sourcePtr.length;
	
	gl.ifaceGrpMem.splice(0, gl.ifaceGrpMem.length);
	
	if (theDataPtr.getRecord(index).attribute == 1) { // Trunk
		var theDataPtr = gl.GupsDataList.getRecord(index).gbinding; 
		theDataPtr.bindSet.splice(0,theDataPtr.dataCount());
		for (var i = 0; i < sourcecnt; i++) {
			var passive = (sourcePtr[i]._Passive == 0 ? 1 : 0);
		
			theDataPtr.addNewRecord(sourcePtr[i]._Interface, 
						passive, 
						sourcePtr[i]._Weight_Limit, 
						sourcePtr[i]._Upstream_kbps_, 
						sourcePtr[i]._Downstream_kbps_, 
						sourcePtr[i]._Weight_Limit);
			
			gl.ifaceGrpMem.push(sourcePtr[i]._Interface);
		}
	}else{ //Normal
		gl.gpeditIdx  = 1;
		var theDataPtr = gl.GupsDataList.getRecord(index).gnormal; 
		theDataPtr.bindSet.splice(0,theDataPtr.dataCount());
		for ( var i = 0; i < sourcecnt; i++) {
			theDataPtr.addNewRecord(sourcePtr[i]._Interface);
			gl.ifaceGrpMem.push(sourcePtr[i]._Interface);
		}
	}
	iface_fetch_1(pos);
}


function group_fetch_1(start_index){
	var sourcePtr = top.frames[4]["result_"+start_index]; 
	var sourcecnt = sourcePtr.length; 
	var theDataPtr = gl.GupsDataList;
	theDataPtr.dataSet.splice(0,theDataPtr.dataCount());
	for(var j = 0; j < sourcecnt; j++) {
		if (sourcePtr[j]._Mode == "NORMAL") {
			var attribute = 0; 
		}else {
 			var attribute = 1; 
		}
		theDataPtr.addNewRecord(sourcePtr[j]._Interface_Group, attribute, sourcePtr[j]._Algorithm);
	}
}

function iface_fetch_1(start_index, pos1, _begin, _end){
	var arg = new Array();
	/*----- Show interface -----*/
	var theDataPtr = gl.ifaceList;	
	theDataPtr.dataSet.splice(0,theDataPtr.dataCount());

	// ethernet / vlan/ bridge / ppp / auxiliary  
	for(var j = 0; j < 3; j++){
		var sourcePtr = top.frames[4]['result_'+(start_index+j)]; 	
		var sourcecnt = sourcePtr.length; 
		for (var i = 0; i < sourcecnt; i++){
			theDataPtr.addNewRecord(sourcePtr[i]._Name);
		}
	}
	var sourcePtr = top.frames[4]['result_'+(start_index+3)]; 
	if (sourcePtr[0]._ppp_interface != undefined) {
		var sourcecnt = sourcePtr[0]._ppp_interface.length; 
		for ( var i = 0; i < sourcecnt; i++){
			theDataPtr.addNewRecord(sourcePtr[0]._ppp_interface[i]._interface_name); 
		}
	}
	//FIXME: need saxon add aux name .
	var sourcePtr = top.frames[4]['result_'+(start_index+4)];	
	theDataPtr.addNewRecord('aux');	
	/*--- upstream & downstream is useful when trnk mode ---*/
	if(gl.ifaceInfoFetch && gl.GupsDataList.getRecord(pos1).attribute == 1) {
		for(var k = _begin; k < _end; k++) {
			var theDataPtr = gl.GupsDataList.getRecord(pos1).gbinding.getRecord(k);
			var sourcePtr = top.frames[4]['result_'+(start_index+5+k-_begin)];

			theDataPtr.upstream = sourcePtr[0]._upstream;
			theDataPtr.downstream = sourcePtr[0]._downstream;
		}
	}
	gl.ifaceInfoFetch = false;
}


function getDeviceData(cmd_no, next_html, clicnt, arg, fail_page){
	
	var cmd = Get_CLI( next_html, cmd_no, arg); 
	txcli(cmd, next_html+'.html', clicnt, fail_page);
}

function policyroute_fetch(start_index){
	var sourcePtr, sourcecnt;
	sourcePtr = top.frames[4]['result_'+start_index]; 
	var theDataPtr = top.frames[1].PRouteList; 
	theDataPtr.dataSet.splice(0,theDataPtr.dataCount());
	if (sourcePtr[0]._index != undefined) {
		sourcecnt = sourcePtr[0]._index.length;
	} else {
		sourcecnt = 0;
	}
	for(var j = 0; j < sourcecnt; j++){
		theDataPtr.addNewRecord(
		                      sourcePtr[0]._index[j]._user,
		                      sourcePtr[0]._index[j]._interface,
		                      sourcePtr[0]._index[j]._source,
		                      sourcePtr[0]._index[j]._destination,
		                      sourcePtr[0]._index[j]._service,
		                      sourcePtr[0]._index[j]._nexthop,
		                      sourcePtr[0]._index[j]._SNAT,
		                      sourcePtr[0]._index[j]._bandwidth,
		                      sourcePtr[0]._index[j].__name,
		                      sourcePtr[0]._index[j]._description,
		                      sourcePtr[0]._index[j]._bandwidth_priority,
		                      '',
		                      sourcePtr[0]._index[j]._nexthop_type.toLowerCase(),
		                      '', '', '', '',
		                      sourcePtr[0]._index[j]._active,
		                      sourcePtr[0]._index[j]._schedule,
		                      sourcePtr[0]._index[j]._tunnel,
		                      sourcePtr[0]._index[j]._sslvpn,
		                      sourcePtr[0]._index[j]._maximize_bandwidth_usage);
	}
	
}
function ForceUser_fetch(start_index){
	var sourcecnt;	
	var sourcePtr1 = top.frames[4]['result_'+(start_index)];
	var sourcePtr2 = top.frames[4]['result_'+(start_index+1)];
	var sourcePtr3 = top.frames[4]['result_'+(start_index+2)];
	var sourcePtr4 = top.frames[4]['result_'+(start_index+3)];
	var sourcePtr5 = top.frames[4]['result_'+(start_index+4)];
	var userPtr = top.frames[1].usersList; 	
	userPtr.dataSet.splice(0, userPtr.dataCount()); 
	userPtr.addNewRecord( '', 
			sourcePtr2[0]._enable_simultaneous_logon_limitation_for_administration_account, 
			sourcePtr2[0]._maximum_simultaneous_logon_per_administration_account, 
			sourcePtr2[0]._enable_simultaneous_logon_limitation_for_access_account, 
			sourcePtr2[0]._maximum_simultaneous_logon_per_access_account, 
			sourcePtr3[0]._enable_logon_retry_limit,
			sourcePtr1[0]._logon_lease_time,
			0,
			sourcePtr1[0]._logon_reauth_time,
			sourcePtr3[0]._maximum_retry_count, 
			sourcePtr3[0]._lockout_period,
			sourcePtr1[0]._user_type
			);
	gl.BufferObj.dataSet.splice(0, gl.BufferObj.dataCount());
	gl.BufferObj.addNewRecord(
			sourcePtr4[0]._enable_user_update_lease_automation,
			sourcePtr5[0]._enable_user_idle_detection,
			sourcePtr5[0]._user_idle_timeout
			);	
			
	var sourcePtr = top.frames[4]['result_'+(start_index+5)]; 			
	var theDataPtr = top.frames[1].ForceUserList; 
	theDataPtr.dataSet.splice(0,theDataPtr.dataCount());
	if (sourcePtr[0]._force_authentication_policy != undefined) {
		sourcecnt = sourcePtr[0]._force_authentication_policy.length;
	} else {
		sourcecnt = 0;
	}
	for(var j = 0; j < sourcecnt; j++){
		theDataPtr.addNewRecord(
			sourcePtr[0]._force_authentication_policy[j].__name,
			sourcePtr[0]._force_authentication_policy[j]._schedule,
			sourcePtr[0]._force_authentication_policy[j]._source_address,
			sourcePtr[0]._force_authentication_policy[j]._destination_address,
			sourcePtr[0]._force_authentication_policy[j]._authentication,
			sourcePtr[0]._force_authentication_policy[j]._active,
			sourcePtr[0]._force_authentication_policy[j]._description
		);
	}
}
function virsvr_fetch(start_index){
	var sourcePtr;
	
	sourcePtr = top.frames[4]['result_'+start_index]; 
	var theDataPtr = top.frames[1].VirSvrList;
	var bufPtr     = top.frames[1].BufferObj;
	 
	theDataPtr.dataSet.splice(0,theDataPtr.dataCount());
	bufPtr.dataSet.splice(0,bufPtr.dataCount());
	
	if (sourcePtr[0]._virtual_server != undefined) { 
		var sourcecnt = sourcePtr[0]._virtual_server.length; 
	} else {
		var sourcecnt = 0;
	}
	for(var j = 0; j < sourcecnt; j++){
		theDataPtr.addNewRecord(
					sourcePtr[0]._virtual_server[j].__name,
					sourcePtr[0]._virtual_server[j]._interface,
					sourcePtr[0]._virtual_server[j]._original_IP,
					'',
					sourcePtr[0]._virtual_server[j]._mapped_IP,
					sourcePtr[0]._virtual_server[j]._mapping_type,
					sourcePtr[0]._virtual_server[j]._protocol_type,
					sourcePtr[0]._virtual_server[j]._original_start_port == 'none' ? '' : sourcePtr[0]._virtual_server[j]._original_start_port,
					sourcePtr[0]._virtual_server[j]._original_end_port == 'none' ? '' : sourcePtr[0]._virtual_server[j]._original_end_port,
					sourcePtr[0]._virtual_server[j]._mapped_start_port == 'none' ? '' : sourcePtr[0]._virtual_server[j]._mapped_start_port,
					sourcePtr[0]._virtual_server[j]._mapped_end_port == 'none' ? '' : sourcePtr[0]._virtual_server[j]._mapped_end_port,
					sourcePtr[0]._virtual_server[j]._active
					);
		bufPtr.addNewRecord(sourcePtr[0]._virtual_server[j]._active, sourcePtr[0]._virtual_server[j].__name);
	}
}

function acctedit_fetch_1(start_index, index){
	var sourcePtr = top.frames[4]['result_'+start_index];  
	var sourcecnt = sourcePtr.length; 
	var theDataPtr = gl.AcctList;

	theDataPtr.dataSet[index].encryption = if_undefined(sourcePtr[0]._encryption_method);
	theDataPtr.dataSet[index].pw         = if_undefined(sourcePtr[0]._password);
	theDataPtr.dataSet[index].rtpw       = if_undefined(sourcePtr[0]._password);
	theDataPtr.dataSet[index].cntid      = if_undefined(sourcePtr[0]._connection_ID);
	theDataPtr.dataSet[index].svrname    = if_undefined(sourcePtr[0]._service_name);
	theDataPtr.dataSet[index].servername = if_undefined(sourcePtr[0]._server_name);
	theDataPtr.dataSet[index].idletime   = if_undefined(sourcePtr[0]._idle_timeout);
	/*-- budget --*/
//theDataPtr.dataSet[index].period 		= if_undefined(sourcePtr[0]._period);
//theDataPtr.dataSet[index].albudget 	= if_undefined(sourcePtr[0]._allocated_budget);
	theDataPtr.dataSet[index].comp = (sourcePtr[0]._compression == "yes" ? 0 : 1);
 }

function account_fetch_1(start_index){
	/*--- show account ---*/
	var sourcePtr = top.frames[4]["result_"+start_index]; 
	var theDataPtr = gl.AcctList;
	theDataPtr.dataSet.splice(0,theDataPtr.dataCount());
	
		for(var j = 0; j < sourcePtr[0]._total_pppoe_account_file; j++){
		theDataPtr.addNewRecord(if_undefined(sourcePtr[0]._PPPoE_account._profile_name[j].__name), 
					 									if_undefined(sourcePtr[0]._PPPoE_account._profile_name[j]._authentication_type), 
					 									if_undefined(sourcePtr[0]._PPPoE_account._profile_name[j]._username)); 
		theDataPtr.getRecord(j).type = 'pppoe';
	}				
	
		for(var j = 0; j < sourcePtr[0]._total_pptp_account_file; j++){
		theDataPtr.addNewRecord(if_undefined(sourcePtr[0]._PPTP_account._profile_name[j].__name), 
					 									if_undefined(sourcePtr[0]._PPTP_account._profile_name[j]._authentication_type), 
					 									if_undefined(sourcePtr[0]._PPTP_account._profile_name[j]._username));
		var k = parseInt(sourcePtr[0]._total_pppoe_account_file) + parseInt(j);
		theDataPtr.getRecord(k).type = 'pptp';
	}
 }

function pppedit_fetch(idx, index, buf_obj){
	var buffPtr = gl.BufferObj.getRecord(index);
	
	/*--- show account ---*/
	var sourcePtr = top.frames[4]["result_"+(idx+2)]; 
	var theDataPtr = gl.AcctList;
	theDataPtr.dataSet.splice(0,theDataPtr.dataCount());
	
	if(sourcePtr[0]._total_pppoe_account_file > 0){
		for(var j = 0; j < sourcePtr[0]._total_pppoe_account_file; j++){
			theDataPtr.addNewRecord(
							if_undefined(sourcePtr[0]._PPPoE_account._profile_name[j].__name),
							if_undefined(sourcePtr[0]._PPPoE_account._profile_name[j]._authentication_type), 
							if_undefined(sourcePtr[0]._PPPoE_account._profile_name[j]._username),
							if_undefined(sourcePtr[0]._PPPoE_account._profile_name[j]._password), '', '',
							if_undefined(sourcePtr[0]._PPPoE_account._profile_name[j]._service_name),
							0,0,0,0,'pppoe','','');
		}				
	}
	if(sourcePtr[0]._total_pptp_account_file > 0){
		for(var j = 0; j < sourcePtr[0]._total_pptp_account_file; j++){
			theDataPtr.addNewRecord(
							if_undefined(sourcePtr[0]._PPTP_account._profile_name[j].__name),
							if_undefined(sourcePtr[0]._PPTP_account._profile_name[j]._authentication_type), 
							if_undefined(sourcePtr[0]._PPTP_account._profile_name[j]._username), 
							if_undefined(sourcePtr[0]._PPTP_account._profile_name[j]._password), '', '',
							if_undefined(sourcePtr[0]._PPTP_account._profile_name[j]._service_name), 
							0,0,0,0,'pptp', '','');
		}		
	}
	
	if(idx == 0 && !buf_obj) { // At first, show interface pppx
		var sourcePtr = top.frames[4]['result_'+idx]; 
		var sourcecnt = sourcePtr.length; 
		var theDataPtr = gl.PPPoEList.dataSet[index];

		theDataPtr.active = if_undefined(sourcePtr[0]._active);
		buffPtr.mb2       = if_undefined(sourcePtr[0]._active);
		
		theDataPtr.ipaddr = if_undefined(sourcePtr[0]._IP_address);
		buffPtr.mb3       = if_undefined(sourcePtr[0]._IP_address);
		
		theDataPtr.netmask = if_undefined(sourcePtr[0]._netmask);
		buffPtr.mb4        = if_undefined(sourcePtr[0]._netmask);
		
		theDataPtr.metric = if_undefined(sourcePtr[0]._metric);
		buffPtr.mb5       = if_undefined(sourcePtr[0]._metric);
		
		theDataPtr.selnd = (sourcePtr[0]._connectivity == "nail-up" ? 0 : 1); 		
		buffPtr.mb6      = (sourcePtr[0]._connectivity == "nail-up" ? 0 : 1);
		
		theDataPtr.comms = sourcePtr[0]._description; 
		buffPtr.mb7      = sourcePtr[0]._description;
		
		theDataPtr.autoip = (sourcePtr[0]._local_address != "0.0.0.0" ? 1 : 0); 
		buffPtr.mb8       = (sourcePtr[0]._local_address != "0.0.0.0" ? 1 : 0); 
		
		theDataPtr.baseip = if_undefined(sourcePtr[0]._local_address); 
		buffPtr.mb9       = if_undefined(sourcePtr[0]._local_address);
		
		theDataPtr.upbwm = if_undefined(sourcePtr[0]._upstream); 
		buffPtr.mb10     = if_undefined(sourcePtr[0]._upstream); 
		
		theDataPtr.downbwm = if_undefined(sourcePtr[0]._downstream); 
		buffPtr.mb11       = if_undefined(sourcePtr[0]._downstream);
		
		theDataPtr.mtu = if_undefined(sourcePtr[0]._MTU); 
		buffPtr.mb12   = if_undefined(sourcePtr[0]._MTU);
		 
		theDataPtr.ripdir = if_undefined(sourcePtr[0]._remote_address);
	
		for (var i = 0 ; i < gl.AcctList.dataCount(); i++){
			if ( gl.AcctList.getRecord(i).profilename == sourcePtr[0]._account_profile )
				theDataPtr.apsel = i+2;
				buffPtr.mb13 = sourcePtr[0]._account_profile;
		}
		
		buffPtr.mb14 = if_undefined(sourcePtr[0]._base_interface);
	}
		
	/*--- ethernet, vlan, bridget ---*/
	var sourcePtr = top.frames[4]['result_'+(idx+1)];
	var ConfigPtr = gl.ConfigEtherList;
	ConfigPtr.dataSet.splice(0,ConfigPtr.dataCount());
	
	for(var j = 0; j < sourcePtr[0]._available_bind_count; j++){
		if(j == 0){
			ConfigPtr.addNewRecord(sourcePtr[0]._available_bind);
		}else{
			ConfigPtr.addNewRecord(sourcePtr[0]['_available_bind_'+(j+1)]);
		}
	}
}

function pppoe_fetch_1(start_index){
	/*---------  show interface ppp ----------*/
	var sourcePtr = top.frames[4]['result_'+ (start_index)]; 
	var theDataPtr = gl.PPPoEList;
	theDataPtr.dataSet.splice(0,theDataPtr.dataCount());
	gl.BufferObj.dataSet.splice(0,gl.BufferObj.dataCount());
	
	if (sourcePtr[0]._ppp_interface == undefined) {
		var sourcecnt = 0;
	} else {
		var sourcecnt = sourcePtr[0]._ppp_interface.length; 
	}
	
	for(var j = 0; j < sourcecnt; j++){
		theDataPtr.addNewRecord(sourcePtr[0]._ppp_interface[j]._interface_name); 
		gl.BufferObj.addNewRecord(sourcePtr[0]._ppp_interface[j]._interface_name);
		
		theDataPtr.getRecord(j).active      = convert_to_bolean(2, sourcePtr[0]._ppp_interface[j]._active);
		theDataPtr.getRecord(j).baseiface   = sourcePtr[0]._ppp_interface[j]._base_interface;
		theDataPtr.getRecord(j).acctprofile = sourcePtr[0]._ppp_interface[j]._account_profile; 
		theDataPtr.getRecord(j).orig_active = convert_to_bolean(2, sourcePtr[0]._ppp_interface[j]._active); 
		theDataPtr.getRecord(j).curr_addr   = sourcePtr[0]._ppp_interface[j]._IP_address;
		theDataPtr.getRecord(j).connected   = sourcePtr[0]._ppp_interface[j]._connected;
	}		
}

function ifEdit_fetch( index , start_index){
	var buffPtr = gl.BufferObj.getRecord(index);
	
	/*---- Show interface all ----*/
		gl.wanOptAry.splice(0,gl.wanOptAry.length);
		var dataPtr = top.frames[4]['result_'+ start_index];
		for(var i = 0; i < dataPtr.length; i++) {
			if(dataPtr[i]._IP_Assignment == "DHCP client" || dataPtr[i]._IP_Assignment == "Dynamic") {
				gl.wanOptAry.push(dataPtr[i]._Name);
			}
		}
	/*--------- show interface [NAME] ----------*/
	var sourcePtr = top.frames[4]['result_'+ (start_index+1)]; 
	var sourcecnt = sourcePtr.length; 

	var theDataPtr = gl.ConfigEtherList.getRecord(index);
	
	theDataPtr.active = if_undefined(sourcePtr[0]._active);
	buffPtr.mb2       = if_undefined(sourcePtr[0]._active);
	
	theDataPtr.GWIP.address = if_undefined(sourcePtr[0]._gateway);
	buffPtr.mb3             = if_undefined(sourcePtr[0]._gateway);
	
	theDataPtr.Metric = if_undefined(sourcePtr[0]._metric);
	buffPtr.mb4       = if_undefined(sourcePtr[0]._metric);

	theDataPtr.VlanComm = if_undefined(sourcePtr[0]._description);
	buffPtr.mb5         = if_undefined(sourcePtr[0]._description);
	
	theDataPtr.Upstream = if_undefined(sourcePtr[0]._upstream);
	buffPtr.mb6         = if_undefined(sourcePtr[0]._upstream);
	
	theDataPtr.Downstream = if_undefined(sourcePtr[0]._downstream);
	buffPtr.mb7           = if_undefined(sourcePtr[0]._downstream);
	
	theDataPtr.MTU = if_undefined(sourcePtr[0]._mtu);
	buffPtr.mb8    = if_undefined(sourcePtr[0]._mtu);
	
	buffPtr.mb9 = if_undefined(sourcePtr[0]._IP_address);
	buffPtr.mb10 = if_undefined(sourcePtr[0]._netmask);
	
	/*-----------  show ip dhcp pool & show ip dhcp server status & show dhcp-relay----------*/
	dhcp_server_fetch_1(index,start_index+2);
		
	/*------------ show ospf interface ETH_IFACE -----------------*/
	var sourcePtr = top.frames[4]['result_'+(start_index+5)]; 
	var buffPtr3 = buffPtr.objMb3;
	buffPtr3.dataSet.splice(0,buffPtr3.dataCount());

	if (sourcePtr.length == 1) {
		theDataPtr.linkcost = if_undefined(sourcePtr[0]._link_cost);
		theDataPtr.priOSPF = if_undefined(sourcePtr[0]._priority);
		
		if (if_undefined(sourcePtr[0]._authentication_type) != "") {
			if ( sourcePtr[0]._authentication_type == "none") {// authentication 0:same-as-area 1:None 2:Text 3.MD5
				theDataPtr.auth = 1;
			}else if (sourcePtr[0]._authentication_type == "text") {	
				theDataPtr.auth = 2;
			}else if (sourcePtr[0]._authentication_type == "md5") {	
				theDataPtr.auth = 3;
			}else {
				theDataPtr.auth = 0;
			}
		}
		
		theDataPtr.area = ( sourcePtr[0]._OSPF_Area == " " ? "none" : sourcePtr[0]._OSPF_area );
		
		theDataPtr.sak = (sourcePtr[0]._text_string == "none" ? "" : sourcePtr[0]._text_string);
		
		theDataPtr.mad = (sourcePtr[0]._MD5_key == "none" ? "" : sourcePtr[0]._MD5_key);
		
		theDataPtr.mak = (sourcePtr[0]._MD5_string == "none" ? "" : sourcePtr[0]._MD5_string);
		
		theDataPtr.passive 	= convert_to_bolean(2, sourcePtr[0]._passive);
		
		buffPtr3.addNewRecord(theDataPtr.linkcost,
		                      theDataPtr.auth,
		                      theDataPtr.area,
		                      theDataPtr.sak,
		                      theDataPtr.mad,
		                      theDataPtr.mak,
		                      theDataPtr.passive,
		                      theDataPtr.priOSPF);
	}
	
	/*------------- show rip interface ETH_IFACE -----------------*/
	var sourcePtr = top.frames[4]['result_'+(start_index+6)]; 
	if (sourcePtr.length==1){
		/* Enable */
		theDataPtr.eRIP = sourcePtr[0]._network;
		buffPtr.mb15 = theDataPtr.eRIP;
		
		/* direction */
		if (sourcePtr[0]._direction == "BiDir") { 
			theDataPtr.ripDir = 0;
		}else if (sourcePtr[0]._direction == "In-Only") { 
			theDataPtr.ripDir = 1;
		}else if (sourcePtr[0]._direction == "Out-Only") { 
			theDataPtr.ripDir = 2;
		}
		buffPtr.mb11 = theDataPtr.ripDir;
		
		/* send version */
		if(sourcePtr[0]._send_version == "1") {
			theDataPtr.MultiVer = 0;
		}else if(sourcePtr[0]._send_version == "2") {
			theDataPtr.MultiVer = 1;
		}else {
			theDataPtr.MultiVer = 2;
		}
		buffPtr.mb12 = theDataPtr.MultiVer;
		
		/* receive version */
		if(sourcePtr[0]._receive_version == "1") {
			theDataPtr.RecvVer = 0;
		}else if(sourcePtr[0]._receive_version == "2") {
			theDataPtr.RecvVer = 1;
		}else {
			theDataPtr.RecvVer = 2;
		}
		buffPtr.mb13 = theDataPtr.RecvVer;
		
		theDataPtr.MultiEnable = (sourcePtr[0]._v2_broadcast == "no" ? false : true);
		buffPtr.mb14 = theDataPtr.MultiEnable;
	}

	/*------------- show ospf area ------------*/
	sourcePtr = top.frames[4]['result_'+(start_index+7)]; 
	var theOAreaPtr = gl.OSPFAreaList; 	// get data pointer	
	theOAreaPtr.dataSet.splice(0,theOAreaPtr.dataCount());
	
	if(sourcePtr.length != 0) {
		var sourcecnt = sourcePtr[0]._OSPF_Area.length;
	}else {
		var sourcecnt = 0;
	}
	
	for ( var i = 0; i < sourcecnt; i++) {
		if (sourcePtr[0]._OSPF_Area[i]._area_type == "stub") {
			var area_type = 1;
		}else if (sourcePtr[0]._OSPF_Area[i]._area_type == "nssa") {
			var area_type = 2;
		}else {
			var area_type = 0;
		}

		if (sourcePtr[0]._OSPF_Area[i]._authentication_type == "text") {
			var auth_type = 1;
		}else if (sourcePtr[0]._OSPF_Area[i]._authentication_type == "md5") {
			var auth_type = 2;
		}else {
			var auth_type = 0;
		}
		theOAreaPtr.addNewRecord(
			areaid=sourcePtr[0]._OSPF_Area[i].__name, 
			area_type, 
			auth_type, 
			areasak=sourcePtr[0]._OSPF_Area[i]._text_string, 
			areamid=sourcePtr[0]._OSPF_Area[i]._MD5_key, 
			aresmak=sourcePtr[0]._OSPF_Area[i]._MD5_string, 
			active=0 );
	}
	
	
	/*----------- show dhcp-status ETH_IFACE --------------*/

	var sourcePtr = top.frames[4]['result_'+(start_index+8)];
	var dataPtr = gl.ConfigEtherList.getRecord(index);
	if (sourcePtr[0]._DHCP_Status == "DHCP server") {
		theDataPtr.dhcpflag = 2;
	} else if (sourcePtr[0]._DHCP_Status == "DHCP relay") {
		theDataPtr.dhcpflag = 1;
	}else if(sourcePtr[0]._DHCP_Status == "none") {
		theDataPtr.dhcpflag = 0;
	}
}

function interface_fetch_1( start_index, vpn ){
	var sourcePtr = top.frames[4]["result_"+start_index]; 
	var sourcecnt = sourcePtr.length; 
	var theDataPtr = gl.ConfigEtherList;
	theDataPtr.dataSet.splice(0,theDataPtr.dataCount());
	gl.BufferObj.dataSet.splice(0,gl.BufferObj.dataCount());

	var virPtr = top.frames[4]["result_"+(start_index+1)];
	var virCnt = virPtr.length; 
	
	if(virCnt != "undefined") {
		virCnt = virCnt;
	}else {
		virCnt = 0;
	}
	
	if(vpn == undefined) {
	var bindIfacePtr =  top.frames[4]['result_'+(start_index+3)];

	if(bindIfacePtr[0]._binding_interface != undefined) {
		var bindIfacecnt = bindIfacePtr[0]._binding_interface.length;
	}else {
		var bindIfacecnt = 0; 
	}
	gl.no_DHCP.splice(0, gl.no_DHCP.length);
	}
	for( var j = 0; j < sourcecnt; j++){
		if (sourcePtr[j]._Address_Type == "dhcp") {
				var selNum = 0;
		}else {
				var selNum = 1;
		}
		
		theDataPtr.addNewRecord(active=convert_to_bolean(2, sourcePtr[j]._Active),
					name=sourcePtr[j]._Name, 
					selNum=selNum, 
					ip=if_undefined(sourcePtr[j]._IP_Address),
					gwip="", 
					subnetmask=if_undefined(sourcePtr[j]._Mask),  
					ifport=0, vlantag="", vlancomm="", upstream="", 
					downstream="", mtu="", ripdir="", MultiEnable=0, MultiVer=0,
					area="none", linkcost="", passive=0, auth=0, sak="", mad="", mak="",
					dhcpflag=2, dhcpserver="", dhcprelay1="", dhcprelay2="", type=0,
					orig_active=convert_to_bolean(2, sourcePtr[j]._Active ),"");			
		gl.BufferObj.addNewRecord(sourcePtr[j]._Name);
		for (var i = 0; i < virCnt; i++) {
			if (virPtr[i]._Name.indexOf(sourcePtr[j]._Name, 0) != -1){
				theDataPtr.addNewRecord(active=convert_to_bolean(2, virPtr[i]._Active),
							name=virPtr[i]._Name, 
							1, 
							ip=if_undefined(virPtr[i]._IP_Address),
							gwip="", 
							subnetmask=if_undefined(virPtr[i]._Mask),  
							ifport=0, vlantag="", vlancomm="", upstream="", 
							downstream="", mtu="", ripdir="", MultiEnable=0, MultiVer=0,
							area="none", linkcost="", passive=0, auth=0, sak="", mad="", mak="",
							dhcpflag=2, dhcpserver="", dhcprelay1="", dhcprelay2="", type=1,
							orig_active=convert_to_bolean(2, virPtr[i]._Active),"" );
				gl.BufferObj.addNewRecord(virPtr[i]._Name);
			}
		}
		if(vpn == undefined) {
			var n = 2;		
			var bindCnt = "";
		
			for(var m = 0; m < bindIfacecnt; m++) {
				if(bindIfacePtr[0]._binding_interface[m].__name == sourcePtr[j]._Name) {
					while(bindIfacePtr[0]._binding_interface[m]['_binding_pool'+bindCnt] != undefined) {
						gl.no_DHCP.push([sourcePtr[j]._Name, bindIfacePtr[0]._binding_interface[m]['_binding_pool'+bindCnt]]);
						bindCnt = "_"+n;
						n++;
					}
				}
			}	
		}
	}
}


function authmeth_convert(str_method){
	var str_return;
	
	if ( str_method == "radius" ) {
		str_return = "group radius";
	}else if ( str_method == "ldap" ) {
		str_return = "group ldap";
	}else if ( str_method == "local" ) {
		str_return = "local";
	}else {
		str_return = "group " + str_method;
	}
	return str_return;	
}

function authmeth_fetch_1(start_index){
	var sourcePtr = top.frames[4]["result_"+start_index]; 
	var sourcecnt = sourcePtr.length; 

	var theDataPtr = gl.authmList; // get data pointer	

	theDataPtr.dataSet.splice(0,theDataPtr.dataCount());
	gl.BufferObj.dataSet.splice(0,gl.BufferObj.dataCount());
	
	for( j = 0; j < sourcecnt; j++){

		var auth1 = authmeth_convert(sourcePtr[j]._Method);
		var auth2 = (sourcePtr[j]._Method_2 != undefined ? authmeth_convert(sourcePtr[j]._Method_2) : "");
		var auth3 = (sourcePtr[j]._Method_3 != undefined ? authmeth_convert(sourcePtr[j]._Method_3) : "");
		var auth4 = (sourcePtr[j]._Method_4 != undefined ? authmeth_convert(sourcePtr[j]._Method_4) : "");
		
		theDataPtr.addNewRecord(sourcePtr[j]._Name, auth1+" "+auth2+" "+auth3+" "+auth4);	
		gl.BufferObj.addNewRecord(sourcePtr[j]._Name);
		
		var theMemberPtr = theDataPtr.getRecord(j).member;
		theMemberPtr.addNewRecord( auth1 ); 
		if ( auth2 != "" ) theMemberPtr.addNewRecord( auth2 ); 
		if ( auth3 != "" ) theMemberPtr.addNewRecord( auth3 );
		if ( auth4 != "" ) theMemberPtr.addNewRecord( auth4 ); 
	}
	if ( start_index != 0 ) return;
	/*******************************/
	var sourcePtr = top.frames[4]["result_"+(start_index+1)]; 
	var sourcecnt = sourcePtr.length; 
	var theDataPtr = gl.ardsgList;
	theDataPtr.dataSet.splice(0,theDataPtr.dataCount());
	for( j = 0; j < sourcecnt; j++){
		theDataPtr.addNewRecord(sourcePtr[j]._Name, sourcePtr[j]._Reference);
	}
	
	/***************************/
	var sourcePtr = top.frames[4]["result_"+(start_index+2)];
	var sourcecnt = sourcePtr.length; 
	var theDataPtr = gl.aldapgList; 
	theDataPtr.dataSet.splice(0,theDataPtr.dataCount());
	
	for( j = 0; j < sourcecnt; j++){
		theDataPtr.addNewRecord(sourcePtr[j]._Name, sourcePtr[j]._Reference);	
	}

	/* AD */
	var sourcePtr = top.frames[4]["result_"+(start_index+3)];
	var sourcecnt = sourcePtr.length; 
	var theDataPtr = gl.adgList; 
	theDataPtr.dataSet.splice(0,theDataPtr.dataCount());
	
	for( j = 0; j < sourcecnt; j++){
		theDataPtr.addNewRecord(sourcePtr[j]._Name, sourcePtr[j]._Reference);	
	}
}

function aradiusg_fetch(start_index){
	/********************************/
	var sourcePtr = top.frames[4]["result_"+start_index]; 
	var sourcecnt = sourcePtr.length; 
	var theDataPtr = gl.ardsgList;
	theDataPtr.dataSet.splice(0,theDataPtr.dataCount());
	gl.BufferObj.dataSet.splice(0,gl.BufferObj.dataCount());
	
	for(var j = 0; j < sourcecnt; j++) {
		theDataPtr.addNewRecord(sourcePtr[j]._Name, sourcePtr[j]._Reference,'', 0);		
		gl.BufferObj.addNewRecord(sourcePtr[j]._Name);
	}
}

function aradiusgedit_fetch(i, start_index){
	var sourcePtr = top.frames[4]["result_"+start_index];  
	
	var theGrpEdPtr = gl.ardsgList.getRecord(i);

	theGrpEdPtr.gkey 			= sourcePtr[0]._key;
	theGrpEdPtr.gtimeout 	= sourcePtr[0]._timeout;
	
	var theHmbrPtr = theGrpEdPtr.hostmbr;

	gl.original_data = new Array(); 

	var sourcePtr2 = top.frames[4]["result_"+start_index+"_1"]; 
	var sourcecnt2 = sourcePtr2.length; 
	
	theHmbrPtr.dataSet.splice(0,theHmbrPtr.dataCount());
	for(var j = 0; j < sourcecnt2; j++) {
		theHmbrPtr.addNewRecord(sourcePtr2[j]._Host_Member, 
														sourcePtr2[j]._Auth__Port 
														);
		gl.original_data[gl.original_data.length] = sourcePtr2[j]._Host_Member; 

	}
}

function aradiusd_fetch_1(start_index){
	var sourcePtr = top.frames[4]["result_"+start_index]; 
	var sourcecnt = sourcePtr.length; 
	var theDataPtr = gl.ardsdList;
	theDataPtr.dataSet.splice(0,theDataPtr.dataCount());
	for( j = 0; j < sourcecnt; j++){
		theDataPtr.addNewRecord(dhost=if_undefined(sourcePtr[j]._host), 
					authport=if_undefined(sourcePtr[j]._authentication_port), 
					acctport=if_undefined(sourcePtr[j]._accounting_port), 
					rkey=sourcePtr[j]._key, 
					rtimeout=sourcePtr[j]._timeout);
	}
}

function aldapgedit_fetch(start_index, idx){
	var sourcePtr = top.frames[4]["result_"+start_index]; 
	var sourcecnt = sourcePtr.length; 
	
	var theGrpEdPtr = gl.aldapgList.getRecord(idx);
	
	theGrpEdPtr.dport 	= sourcePtr[0]._port; 
	theGrpEdPtr.dpwd 		= sourcePtr[0]._password; 
	theGrpEdPtr.basedn	= sourcePtr[0]._basedn; 
	theGrpEdPtr.binddn	= sourcePtr[0]._binddn; 
	theGrpEdPtr.cnid		= sourcePtr[0]._CN_identifier;
	theGrpEdPtr.stime		= sourcePtr[0]._search_time_limit; 
	theGrpEdPtr.ussl	  = convert_to_bolean(2, sourcePtr[0]._SSL);
	
	var sourcePtr2 = top.frames[4]["result_"+start_index+"_1"]; 
	var sourcecnt2 = sourcePtr2.length; 

	var theHmbrPtr = theGrpEdPtr.hostmbr;
	
	theHmbrPtr.dataSet.splice(0,theHmbrPtr.dataCount());
	gl.original_data = new Array(); 
	for( j = 0; j < sourcecnt2; j++){
		theHmbrPtr.addNewRecord( sourcePtr2[j]._Host_Member );
		gl.original_data[gl.original_data.length] = sourcePtr2[j]._Host_Member; 
	}
}

function aldapd_fetch_1(start_index){
	var sourcePtr = top.frames[4]["result_"+start_index]; 
	var sourcecnt = sourcePtr.length; 
	var theLdapPtr = gl.aldapdList;
	theLdapPtr.dataSet.splice(0,theLdapPtr.dataCount());
	for( j = 0; j < sourcecnt; j++){
		var ssl = convert_to_bolean(2, sourcePtr[j]._SSL);
		theLdapPtr.addNewRecord(if_undefined(sourcePtr[j]._host), 
					if_undefined(sourcePtr[j]._port), 
					if_undefined(sourcePtr[j]._password), 
					sourcePtr[j]._basedn, 
					sourcePtr[j]._binddn, 
					sourcePtr[j]._CN_identifier,
					sourcePtr[j]._search_time_limit, 
					ssl );
	}
}

function aldapg_fetch_1(start_index) {
	var sourcePtr = top.frames[4]["result_"+start_index]; 
	var sourcecnt = sourcePtr.length; 
	var theDataPtr = gl.aldapgList; 
	theDataPtr.dataSet.splice(0,theDataPtr.dataCount());
	gl.BufferObj.dataSet.splice(0,gl.BufferObj.dataCount());
	
	for( j = 0; j < sourcecnt; j++){
		theDataPtr.addNewRecord(sourcePtr[j]._Name, sourcePtr[j]._Reference);
		gl.BufferObj.addNewRecord(sourcePtr[j]._Name);
	}
}

function users_fetch_1(){
	var sourcePtr = top.frames[4].result_0; 
	var sourcecnt = sourcePtr.length; 
	var theDataPtr = gl.usersList; // get data pointer	

	theDataPtr.dataSet.splice(0,theDataPtr.dataCount());
	
	for( j=0; j<sourcecnt; j++){
		theDataPtr.addNewRecord(sourcePtr[j].inuser); 	//(name, methodlist, password, description, access, visibility, idletimeout, refCnt);
	}
}

function cfilter_fetch_1(start_index){	
	/* show content-filter settings */
	var sourcePtr = top.frames[4]["result_"+start_index]; 
	var sourcecnt = sourcePtr.length; 
	var theDataPtr = gl.cfilterList; 	
	
	theDataPtr.dataSet.splice(0,theDataPtr.dataCount());
	gl.BufferObj.dataSet.splice(0,gl.BufferObj.dataCount());
	
	theDataPtr.addNewRecord(sourcePtr[0]._block_message,
	                        sourcePtr[0]._block_redirect,0,
	                        sourcePtr[0]._service_timeout,
	                        sourcePtr[0]._cache_timeout,
	                        convert_to_bolean(2, sourcePtr[0]._active),
	                        convert_to_bolean(2, sourcePtr[0]._default_block));
	gl.BufferObj.addNewRecord(sourcePtr[0]._block_message,
	                          sourcePtr[0]._block_redirect,
	                          convert_to_bolean(2, sourcePtr[0]._active),
	                          convert_to_bolean(2, sourcePtr[0]._default_block));
	                          
	/* show content-filter policy */
	var sourcePtr = top.frames[4]["result_"+(start_index+1)];
	var sourcecnt = (sourcePtr[0]._policy == undefined) ? 0 : sourcePtr[0]._policy.length;
	
	var _theDataPtr = theDataPtr.getRecord(0).sublnk;

	_theDataPtr.dataSet.splice(0,_theDataPtr.dataCount());
	
	for( j = 0; j < sourcecnt; j++){
		_theDataPtr.addNewRecord(sourcePtr[0]._policy[j]._address,
		                         sourcePtr[0]._policy[j]._schedule,
		                         sourcePtr[0]._policy[j]._filter_profile,
		                         convert_to_bolean(2, sourcePtr[0]._policy[j]._active),
		                         sourcePtr[0]._policy[j]._user,
		                         convert_to_bolean(2, sourcePtr[0]._policy[j]._active));
	}
	
	/* show service-register status content-filter */
	var sourcePtr = top.frames[4]["result_"+(start_index+2)];	
	theDataPtr.getRecord(0).reg = sourcePtr[0]._Status;
	theDataPtr.getRecord(0).type = sourcePtr[0]._Type;

	/* show content-filter profile */
	var sourcePtr = top.frames[4]["result_"+(start_index+3)];
	var sourcecnt = sourcePtr.length; 
	var ptr = gl.cfprofileList; 	

	ptr.dataSet.splice(0, ptr.dataCount());
	for( j = 0; j < sourcecnt; j++){
		ptr.addNewRecord(sourcePtr[j]._Name);
	}
}

function cfcache_fetch_1(start_index){
	/*---- show content-filter url-cache ----*/
	var sourcePtr = top.frames[4]["result_"+start_index]; 
	var sourcecnt = sourcePtr.length; 
	var theDataPtr = gl.cfcacheList; 	
	theDataPtr.dataSet.splice(0,theDataPtr.dataCount());
	for(i = 0; i < sourcecnt; i++){  
		theDataPtr.addNewRecord(sourcePtr[i]._Category, sourcePtr[i]._URL, sourcePtr[i]._TTL);
	}
	/*----- show content-filter settings ----*/
/*	var sourcePtr = top.frames[4]["result_"+(start_index+1)]; 
	var sourcecnt = sourcePtr.length; 
	var theDataPtr = gl.cfilterList; 	
	theDataPtr.dataSet[0].cache_timeout = sourcePtr[0]._cache_timeout;*/
}

function cfpcyedit_fetch_1(index){
	/*---- show content-filter profile XXXX ----*/
	var sourcePtr = top.frames[4].result_0; 
	var sourcecnt = sourcePtr.length; 
	
	var theDataPtr = gl.cfprofileList; 
	var buf 				= gl.BufferObj.getRecord(index);
	var buf_Cat		 	= gl.BufferObj.getRecord(index).objMb1;
	var buf_Cus_Trust 		= gl.BufferObj.getRecord(index).objMb2;
	var buf_Cus_Forbid 		= gl.BufferObj.getRecord(index).objMb3;
	var buf_Cus_Keyword		= gl.BufferObj.getRecord(index).objMb4;
	/*******************************  
	 *        Get Categories       *
	 *******************************/
	var thecfcyPtr = theDataPtr.getRecord(index).cylnk; //only belong to such Profile Name 
	thecfcyPtr.dataSet.splice(0,thecfcyPtr.dataCount());
	buf_Cat.dataSet.splice(0, buf_Cat.dataCount());

	var totalCate = 62;
	var category_array = new Array(); 
					
	for(var cc = 0; cc < totalCate; cc++) {
		category_array[cc] = false;
	}
	thecfcyPtr.addNewRecord( gl.cfilterList.getRecord(0).reg);
	
		category_array[0]  = false;	// 1:select all 	
		category_array[1]  = false; 	// 2:unselect all
		category_array[2]  = convert_to_bolean(2, sourcePtr[0]._Adult_Mature_Content);
		category_array[3]  = convert_to_bolean(2, sourcePtr[0]._Pornography);
		category_array[4]  = convert_to_bolean(2, sourcePtr[0]._Sex_Education);
		category_array[5]  = convert_to_bolean(2, sourcePtr[0]._Intimate_Apparel_Swimsuit);
		category_array[6]  = convert_to_bolean(2, sourcePtr[0]._Nudity);
		category_array[7]  = convert_to_bolean(2, sourcePtr[0]._Alcohol_Tobacco);
		category_array[8]  = convert_to_bolean(2, sourcePtr[0]._Illegal_Questionable);
		category_array[9]  = convert_to_bolean(2, sourcePtr[0]._Gambling);
		category_array[10] = convert_to_bolean(2, sourcePtr[0]._Violence_Hate_Racism);
		category_array[11] = convert_to_bolean(2, sourcePtr[0]._Weapons);
		category_array[12] = convert_to_bolean(2, sourcePtr[0]._Abortion);
		category_array[13] = convert_to_bolean(2, sourcePtr[0]._Hacking);
		category_array[14] = convert_to_bolean(2, sourcePtr[0]._Phishing);
		category_array[15] = convert_to_bolean(2, sourcePtr[0]._Arts_Entertainment);
		category_array[16] = convert_to_bolean(2, sourcePtr[0]._Business_Economy);
		category_array[17] = convert_to_bolean(2, sourcePtr[0]._Alternative_Spirituality_Occult);
		category_array[18] = convert_to_bolean(2, sourcePtr[0]._Illegal_Drugs);
		category_array[19] = convert_to_bolean(2, sourcePtr[0]._Education);
		category_array[20] = convert_to_bolean(2, sourcePtr[0]._Cultural_Charitable_Organization);
		category_array[21] = convert_to_bolean(2, sourcePtr[0]._Financial_Services);
		category_array[22] = convert_to_bolean(2, sourcePtr[0]._Brokerage_Trading);
		category_array[23] = convert_to_bolean(2, sourcePtr[0]._Online_Games);
		category_array[24] = convert_to_bolean(2, sourcePtr[0]._Government_Legal);
		category_array[25] = convert_to_bolean(2, sourcePtr[0]._Military);
		category_array[26] = convert_to_bolean(2, sourcePtr[0]._Political_Activist_Groups);
		category_array[27] = convert_to_bolean(2, sourcePtr[0]._Health);
		category_array[28] = convert_to_bolean(2, sourcePtr[0]._Computers_Internet);
		category_array[29] = convert_to_bolean(2, sourcePtr[0]._Search_Engines_Portals);
		category_array[30] = convert_to_bolean(2, sourcePtr[0]._Spyware_Malware_Sources);
		category_array[31] = convert_to_bolean(2, sourcePtr[0]._Spyware_Effects_Privacy_Concerns);
		category_array[32] = convert_to_bolean(2, sourcePtr[0]._Job_Search_Careers);
		category_array[33] = convert_to_bolean(2, sourcePtr[0]._News_Media);
		category_array[34] = convert_to_bolean(2, sourcePtr[0]._Personals_Dating);
		category_array[35] = convert_to_bolean(2, sourcePtr[0]._Reference);
		category_array[36] = convert_to_bolean(2, sourcePtr[0]._Open_Image_Media_Search);
		category_array[37] = convert_to_bolean(2, sourcePtr[0]._Chat_Instant_Messaging);
		category_array[38] = convert_to_bolean(2, sourcePtr[0]._Email);
		category_array[39] = convert_to_bolean(2, sourcePtr[0]._Blogs_Newsgroups);
		category_array[40] = convert_to_bolean(2, sourcePtr[0]._Religion);
		category_array[41] = convert_to_bolean(2, sourcePtr[0]._Social_Networking);
		category_array[42] = convert_to_bolean(2, sourcePtr[0]._Online_Storage);
		category_array[43] = convert_to_bolean(2, sourcePtr[0]._Remote_Access_Tools);
		category_array[44] = convert_to_bolean(2, sourcePtr[0]._Shopping);
		category_array[45] = convert_to_bolean(2, sourcePtr[0]._Auctions);
		category_array[46] = convert_to_bolean(2, sourcePtr[0]._Real_Estate);
		category_array[47] = convert_to_bolean(2, sourcePtr[0]._Society_Lifestyle);	
		category_array[48] = convert_to_bolean(2, sourcePtr[0]._Sexuality_Alternative_Lifestyles);
		category_array[49] = convert_to_bolean(2, sourcePtr[0]._Restaurants_Dining_Food);
		category_array[50] = convert_to_bolean(2, sourcePtr[0]._Sports_Recreation_Hobbies);
		category_array[51] = convert_to_bolean(2, sourcePtr[0]._Travel);
		category_array[52] = convert_to_bolean(2, sourcePtr[0]._Vehicles);
		category_array[53] = convert_to_bolean(2, sourcePtr[0]._Humor_Jokes);
		category_array[54] = convert_to_bolean(2, sourcePtr[0]._Software_Downloads);
		category_array[55] = convert_to_bolean(2, sourcePtr[0]._Pay_to_Surf);
		category_array[56] = convert_to_bolean(2, sourcePtr[0]._Peer_to_Peer);
		category_array[57] = convert_to_bolean(2, sourcePtr[0]._Streaming_Media_MP3s);
		category_array[58] = convert_to_bolean(2, sourcePtr[0]._Proxy_Avoidance);
		category_array[59] = convert_to_bolean(2, sourcePtr[0]._For_Kids);
		category_array[60] = convert_to_bolean(2, sourcePtr[0]._Web_Advertisements);
		category_array[61] = convert_to_bolean(2, sourcePtr[0]._Web_Hosting);
	
	thecfcyPtr.getRecord(0).extreg    = gl.cfilterList.dataSet[0].reg;
	thecfcyPtr.getRecord(0).enextsvr  = convert_to_bolean(2, sourcePtr[0]._service_active);
	thecfcyPtr.getRecord(0).blkmweb   = convert_to_bolean(2, sourcePtr[0]._url_match._block);
	thecfcyPtr.getRecord(0).logmweb   = convert_to_bolean(2, sourcePtr[0]._url_match._log);
	thecfcyPtr.getRecord(0).blkutweb  = convert_to_bolean(2, sourcePtr[0]._url_unrate._block);
	thecfcyPtr.getRecord(0).logutweb  = convert_to_bolean(2, sourcePtr[0]._url_unrate._log);
	thecfcyPtr.getRecord(0).blkunaver = convert_to_bolean(2, sourcePtr[0]._service_offline._block);
	thecfcyPtr.getRecord(0).logunaver = convert_to_bolean(2, sourcePtr[0]._service_offline._log);
	thecfcyPtr.getRecord(0).testblk   = convert_to_bolean(2, sourcePtr[0]._service);
	thecfcyPtr.getRecord(0).category	= category_array;
	
	var count = 0;	
	for(var cc = 2; cc < totalCate; cc++) {
		if(category_array[cc] == true) {
			count++;
		}
	}
	if(count == 60) {
		category_array[0] = true;
	}
	/**********************
	 *   For compareing   *
	 **********************/
	buf.mb2 = convert_to_bolean(2, sourcePtr[0]._service_active);
	buf.mb3 = convert_to_bolean(2, sourcePtr[0]._url_match._block);
	buf.mb4 = convert_to_bolean(2, sourcePtr[0]._url_match._log);
	buf.mb5 = convert_to_bolean(2, sourcePtr[0]._url_unrate._block);
	buf.mb6 = convert_to_bolean(2, sourcePtr[0]._url_unrate._log);
	buf.mb7 = convert_to_bolean(2, sourcePtr[0]._service_offline._block);
	buf.mb8 = convert_to_bolean(2, sourcePtr[0]._service_offline._log);
	buf.mb9 = convert_to_bolean(2, sourcePtr[0]._service);
	
	for(var cc = 0; cc < totalCate; cc++) {
		buf_Cat.addNewRecord(category_array[cc]);
	}
	
		
	/**********************************  
	 *        Get Customization       *
	 **********************************/
	var theCustomPtr = theDataPtr.getRecord(index).cslnk; //only belong to such Profile Name
	theCustomPtr.dataSet.splice(0,theCustomPtr.dataCount());
	buf_Cus_Trust.dataSet.splice(0, buf_Cus_Trust.dataCount());
	buf_Cus_Forbid.dataSet.splice(0, buf_Cus_Forbid.dataCount());
	buf_Cus_Keyword.dataSet.splice(0, buf_Cus_Keyword.dataCount());
	
	theCustomPtr.addNewRecord(convert_to_bolean(2, sourcePtr[0]._custom_active),
														convert_to_bolean(2, sourcePtr[0]._allow_traffic_to_trusted_hosts_only), 
														convert_to_bolean(2, sourcePtr[0]._block_activex), 
														convert_to_bolean(2, sourcePtr[0]._block_java), 
														convert_to_bolean(2, sourcePtr[0]._block_cookie), 
														convert_to_bolean(2, sourcePtr[0]._block_proxy), 
														convert_to_bolean(2, sourcePtr[0]._allow_features_to_trusted_hosts));
	
	/**********************
	 *   For compareing   *
	 **********************/
	buf.mb10 = convert_to_bolean(2, sourcePtr[0]._custom_active);
	buf.mb11 = convert_to_bolean(2, sourcePtr[0]._allow_traffic_to_trusted_hosts_only);
	buf.mb12 = convert_to_bolean(2, sourcePtr[0]._block_activex);
	buf.mb13 = convert_to_bolean(2, sourcePtr[0]._block_java);
	buf.mb14 = convert_to_bolean(2, sourcePtr[0]._block_cookie);
	buf.mb15 = convert_to_bolean(2, sourcePtr[0]._block_proxy);
	buf.mb16 = convert_to_bolean(2, sourcePtr[0]._allow_features_to_trusted_hosts);
	
		var theTrustWeb 	= theCustomPtr.getRecord(0).trsweb;
		var theForbidWeb 	= theCustomPtr.getRecord(0).forbidden;
		var theKeywordList	= theCustomPtr.getRecord(0).bkul;
		
		var sourcePtr2 = top.frames[4].result_0_1; 
		var sourcecnt2 = sourcePtr2.length; 
		for( j = 0; j < sourcecnt2; j++){
			theTrustWeb.addNewRecord( sourcePtr2[j]._Trusted_Host );
		buf_Cus_Trust.addNewRecord( sourcePtr2[j]._Trusted_Host);
		}
	
		var sourcePtr3 = top.frames[4].result_0_2; 
		var sourcecnt3 = sourcePtr3.length; 
		for( j = 0; j < sourcecnt3; j++){
			theForbidWeb.addNewRecord( sourcePtr3[j]._Forbidden_Host);
		buf_Cus_Forbid.addNewRecord( sourcePtr3[j]._Forbidden_Host);
		}
	
		var sourcePtr4 = top.frames[4].result_0_3; 
		var sourcecnt4 = sourcePtr4.length; 
		for( j = 0; j < sourcecnt4; j++){
			theKeywordList.addNewRecord( sourcePtr4[j]._Keyword );
		buf_Cus_Keyword.addNewRecord( sourcePtr4[j]._Keyword);
	}
}

function cfprofile_fetch_1(start_index) {
	/*--- show content-filter profile ---*/
	var sourcePtr = top.frames[4]["result_"+start_index]; 
	var sourcecnt = sourcePtr.length; 
	var theDataPtr = gl.cfprofileList; 	
	theDataPtr.dataSet.splice(0,theDataPtr.dataCount());
	gl.BufferObj.dataSet.splice(0,gl.BufferObj.dataCount());

	for( j = 0; j < sourcecnt; j++) {
		theDataPtr.addNewRecord( sourcePtr[j]._Name );
		gl.BufferObj.addNewRecord( sourcePtr[j]._Name);
	}
	/*-- show content-filter setting ---*/ 
	var sourcePtr = top.frames[4]["result_"+(start_index+1)];
	var theDataPtr = gl.cfilterList;
	theDataPtr.getRecord(0).service_timeout = sourcePtr[0]._service_timeout;
}

function svrgrpedit_edit(start_index) {
	var gpeditIdx = gl.gpeditIdx;	//this var place in panel.html
	var ifIdx = gl.ifdx;	//this var place in panel.html

	var theAuthPtr = gl.svrgrpList.getRecord(ifIdx);	
	var theDataPtr = theAuthPtr.submbr; // gl.theMethList;
	var gpName = "";

	/*******************************************/
	var sourcePtr = top.frames[4]["result_"+start_index]; 
	var sourcecnt = sourcePtr.length; 
	if (theDataPtr.dataCount() > 0) {
		 theDataPtr.dataSet.splice(0,theDataPtr.dataCount());//clear member
	}
	for(var j = 0; j < sourcecnt; j++) {		
		theDataPtr.addNewRecord( sourcePtr[j]._Object_Group_name + '/'+sourcePtr[j]._Type);
		var temp = sourcePtr[j]._Object_Group_name+ '/'+sourcePtr[j]._Type+",";
		gpName+=temp;
	}
	theAuthPtr.protl = gpName; //for buffering the member
}

function svrgrp_fetch_1(start_index){
	/* show service-object */
	var sourcePtr = top.frames[4]["result_"+start_index]; 
	var sourcecnt = sourcePtr.length; 
	var theDataPtr = gl.svrList; 
	theDataPtr.dataSet.splice(0,theDataPtr.dataCount());
	
	for( j = 0; j < sourcecnt; j++){
		theDataPtr.addNewRecord(sourcePtr[j]._Object_name);
	}
	
	/* show object-group service */
	var sourcePtr = top.frames[4]["result_"+(start_index+1)]; 
	var sourcecnt = sourcePtr.length; 
	var theDataPtr = gl.svrgrpList; 
	theDataPtr.dataSet.splice(0,theDataPtr.dataCount());
	gl.BufferObj.dataSet.splice(0,gl.BufferObj.dataCount());
	
	for( j = 0; j < sourcecnt; j++){
		theDataPtr.addNewRecord( sourcePtr[j]._Group_name, sourcePtr[j]._Description, "", "", "" );
		gl.BufferObj.addNewRecord(sourcePtr[j]._Group_name, sourcePtr[j]._Description);
	}
}

function service_fetch_1(start_index){
	/*******************************************/
	var sourcePtr = top.frames[4]["result_"+start_index]; 
	var sourcecnt = sourcePtr.length; 
	var theDataPtr = gl.svrList; 
	theDataPtr.dataSet.splice(0,theDataPtr.dataCount());
	gl.BufferObj.dataSet.splice(0,gl.BufferObj.dataCount());
	
	for( j = 0; j < sourcecnt; j++){

		var type = sourcePtr[j]._Protocol;
		
		if (sourcePtr[j]._Protocol == "TCP") 
			var protl = 0;
		else if (sourcePtr[j]._Protocol == "UDP") 
			var protl = 1;
		else if (sourcePtr[j]._Protocol == "ICMP") {
			var protl = 2;
			var type = sourcePtr[j]._Minmum_port;
		}else {
			var protl = 3;
		}
		
		if ( sourcePtr[j]._Maxmum_port == sourcePtr[j]._Minmum_port ) sourcePtr[j]._Maxmum_port = "";
			
		theDataPtr.addNewRecord(sourcePtr[j]._Object_name,type,protl,
		                        sourcePtr[j]._Minmum_port,
		                        sourcePtr[j]._Maxmum_port);
					
		gl.BufferObj.addNewRecord(sourcePtr[j]._Object_name);
	}
}

function schedule_fetch_1(start_index){
	/*******************************************/
	var sourcePtr = top.frames[4]["result_"+start_index]; 
	if (sourcePtr == undefined) {
		return ;
	}
	var sourcecnt = sourcePtr.length; 

	var theOnetmPtr = gl.onetmList;
	var theRecuPtr = gl.recuList;

	theOnetmPtr.dataSet.splice(0,theOnetmPtr.dataCount());
	theRecuPtr.dataSet.splice(0,theRecuPtr.dataCount());

	for( j=0; j<sourcecnt; j++) {
		if(sourcePtr[j]._Type == "Once" ) {
			theOnetmPtr.addNewRecord(sourcePtr[j]._Object_name, sourcePtr[j]._Start_End, sourcePtr[j]._Start_End_2, 
															 sourcePtr[j]._Start_End_3, starthour=sourcePtr[j]._Start_End_4, startminute=sourcePtr[j]._Start_End_5, 
															 stopyear=sourcePtr[j]._Start_End_6, stopmonth=sourcePtr[j]._Start_End_7, stopday=sourcePtr[j]._Start_End_8, 
															 stophour=sourcePtr[j]._Start_End_9, stopminute=sourcePtr[j]._Start_End_10, 
															 w1="", w2="", w3="", w4="", w5="", w6="", w7="");
		}else {
			var ww1,ww2,ww3,ww4,ww5,ww6,ww7;
			
			divWeek(sourcePtr[j]._Start_End_5, "Mon") == true ? ww1 = true : ww1 = false;
			divWeek(sourcePtr[j]._Start_End_5, "Tue") == true ? ww2 = true : ww2 = false;
			divWeek(sourcePtr[j]._Start_End_5, "Wed") == true ? ww3 = true : ww3 = false;
			divWeek(sourcePtr[j]._Start_End_5, "Thu") == true ? ww4 = true : ww4 = false;
			divWeek(sourcePtr[j]._Start_End_5, "Fri") == true ? ww5 = true : ww5 = false;
			divWeek(sourcePtr[j]._Start_End_5, "Sat") == true ? ww6 = true : ww6 = false;
			divWeek(sourcePtr[j]._Start_End_5, "Sun") == true ? ww7 = true : ww7 = false;
			
			theRecuPtr.addNewRecord(sourcePtr[j]._Object_name, "", "", "",
															starthour=sourcePtr[j]._Start_End, startminute=sourcePtr[j]._Start_End_2, 
															stopyear="", stopmonth="", stopday="", 
															stophour=sourcePtr[j]._Start_End_3, stopminute=sourcePtr[j]._Start_End_4, 
															w1=ww1, w2=ww2, w3=ww3, w4=ww4, w5=ww5, w6=ww6, w7=ww7);			
		}
	}
}

function divWeek(val, WeekType) {
	var ret = false;
	if(val.indexOf(WeekType, 0) != -1) {
				ret = true;
	}
	return  ret;
}

function address_fetch_1(start_index){
	var sourcePtr = top.frames[4]["result_"+start_index]; 
	var sourcecnt = sourcePtr.length; 
	var theDataPtr = gl.AddressList; 
	theDataPtr.dataSet.splice(0,theDataPtr.dataCount());
	for( j=0; j<sourcecnt; j++){
		if(sourcePtr[j]._Type == "HOST") {
			theDataPtr.addNewRecord(
					sourcePtr[j]._Object_name,
					sourcePtr[j]._Type,
					sourcePtr[j]._Address,
					"");
		}else {
			theDataPtr.addNewRecord(
					sourcePtr[j]._Object_name,
					sourcePtr[j]._Type,
					sourcePtr[j]._Address,
					sourcePtr[j]._Address_2);
		}
	}
	
}

function addrgps_fetch_1(start_index){
	/* show address-object */
	var sourcePtr = top.frames[4]["result_"+start_index]; 
	var sourcecnt = sourcePtr.length; 
	var theDataPtr = gl.AddressList; 
	theDataPtr.dataSet.splice(0,theDataPtr.dataCount());
	for( j=0; j<sourcecnt; j++){
		theDataPtr.addNewRecord(sourcePtr[j]._Object_name);
	}
	
	/* show object-group address */
	var sourcePtr = top.frames[4]["result_"+(start_index+1)]; 
	var sourcecnt = sourcePtr.length; 
	gl.AddrGpsList.dataSet.splice(0,gl.AddrGpsList.dataCount());
	gl.BufferObj.dataSet.splice(0,gl.BufferObj.dataCount());
	
	for(var j = 0; j < sourcecnt; j++){
		gl.AddrGpsList.addNewRecord(sourcePtr[j]._Group_name, "", "", "", "", "", sourcePtr[j]._Description);
		gl.BufferObj.addNewRecord(sourcePtr[j]._Group_name,sourcePtr[j]._Description);
	}
}

function addrgps_edit(idx, start_index) {	
	var theAddrGrpPtr = gl.AddrGpsList.getRecord(idx);
	var theDataPtr = theAddrGrpPtr.gbinding; // get data pointer	
	var gpName = "";
	
	/*******************************************/
	var sourcePtr = top.frames[4]["result_"+start_index]; 
	var sourcecnt = sourcePtr.length;
	theDataPtr.bindSet.splice(0,theDataPtr.dataCount());
	
	for(var j = 0; j < sourcecnt; j++) {
		var prefix = (sourcePtr[j]._Type == "Object" ? "Object" : "Group");
		theDataPtr.addNewRecord(sourcePtr[j]._Object_Group_name + '/'+prefix);
		var temp = sourcePtr[j]._Object_Group_name+ '/'+prefix+",";
		gpName += temp;
	}
	theAddrGrpPtr.attribute = gpName; //for buffering the member 
}

function cert_fetch_1(type, result_start ){
	var result_percentage = '';
	var theDataPtr;
	var sourcePtr = top.frames[4]["result_"+(result_start +1)];
	if (type == "local") {
		result_percentage = sourcePtr[0]._in_use_3._my_certificate_3;
	} else {
		result_percentage = sourcePtr[0]._in_use_3._trusted_certificate_3;
	}
	result_percentage = result_percentage.slice(1, result_percentage.length -2);	

	var sourcePtr = top.frames[4]["result_"+result_start];
	
	if ( type == "local"){
		theDataPtr = gl.mycertList; 
	}else{
		theDataPtr = gl.trscertList; 
	}		
	theDataPtr.dataSet.splice(0,theDataPtr.dataCount());

	if (sourcePtr[0]._certificate != undefined) {
		var sourcecnt = sourcePtr[0]._certificate.length;
	} else {
		var sourcecnt = 0;
	}
	for( j=0; j<sourcecnt; j++){
		theDataPtr.addNewRecord(result_percentage, 
					sourcePtr[0]._certificate[j].__name,
					sourcePtr[0]._certificate[j]._type,
					sourcePtr[0]._certificate[j]._subject,
					sourcePtr[0]._certificate[j]._issuer,
					sourcePtr[0]._certificate[j]._valid_from,
					sourcePtr[0]._certificate[j]._valid_to,
					'',
					'',
					sourcePtr[0]._certificate[j]._status);
	}
	//show users current
	var limitPtr = gl.LimitObj;
	limitPtr.dataSet.splice(0, limitPtr.dataCount());//clear data object
	var sourcePtr = top.frames[4]['result_'+(result_start+2)]; 		
	limitPtr.addNewRecord(sourcePtr[0]._Type);//mb1
	
}

function hostData_fetch(start_index){ /*System : hostname domainname*/
	var fr = 1;//frames
	var gl = top.frames[fr];
	var theDataPtr = gl.HnameList;

	var data = theDataPtr.getRecord(0);
	var sourcePtr = top.frames[4]['result_'+start_index]; 
	/*title used */
	data.fqdn = sourcePtr[0]._FQDN;
	if(sourcePtr[0]._host_name != "none" || sourcePtr[0]._domain_name != "none") {	
		if(sourcePtr[0]._host_name == "none") {
			data.sname = "";
			data.dname = sourcePtr[0]._domain_name;
		}else if(sourcePtr[0]._domain_name == "none"){
			data.sname = sourcePtr[0]._host_name;
			data.dname = "";
		}else {
			data.sname = sourcePtr[0]._host_name;
			data.dname = sourcePtr[0]._domain_name;
		}
	}else {
		data.sname = "";
		data.dname = "";
	}
}
function br_fetch(start_index) { /*BaudRate*/
	var fr = 1;//frames
	var gl = top.frames[fr];
	var theDataPtr = gl.ConsoleList;
	var data = theDataPtr.getRecord(0);
	data.sname = top.frames[4]['result_'+start_index][0]._console_speed;
}

function mc_fetch(start_index){ /*Date/Time*/ 
	var fr = 1;//frames 
	var gl = top.frames[fr]; 
	var theDataPtr = gl.ClockList; 
	var data = theDataPtr.getRecord(0); 
	var sourcePtr1 = top.frames[4]['result_'+(start_index-3)]; 
	var sourcePtr2 = top.frames[4]['result_'+(start_index-2)]; 
	var sourcePtr3 = top.frames[4]['result_'+(start_index-1)]; 
	var sourcePtr4 = top.frames[4]['result_'+(start_index)]; 
 
 	//time zone
	var tzone = sourcePtr1[0]._timezone[0].__name; 	
	data.tzone = timezone_formatChange(tzone.length,tzone); 
 	
 
	//daylight saving
	data.dsaveE = sourcePtr1[0]._daylight_saving[0]._active;
	//data.dsaveT = sourcePtr1[0].clock_daylightSavingTime; 
		
	data.dsaveDur_1 = sourcePtr1[0]._daylight_saving[0]._duration;
	data.dsaveDur_2 = sourcePtr1[0]._daylight_saving[0]._duration_2;
	data.dsaveDur_3 = sourcePtr1[0]._daylight_saving[0]._duration_3;
	data.dsaveDur_4 = sourcePtr1[0]._daylight_saving[0]._duration_4;
	data.dsaveDur_5 = sourcePtr1[0]._daylight_saving[0]._duration_5;
	data.dsaveDur_6 = sourcePtr1[0]._daylight_saving[0]._duration_6;
	data.dsaveDur_7 = sourcePtr1[0]._daylight_saving[0]._duration_7;
	data.dsaveDur_8 = sourcePtr1[0]._daylight_saving[0]._duration_8;
	data.dsaveDur_9 = sourcePtr1[0]._daylight_saving[0]._duration_9;
	data.dsaveDur_10= sourcePtr1[0]._daylight_saving[0]._duration_10;
	
	data.dsaveO = sourcePtr1[0]._daylight_saving[0]._offset;
 	
	//ntp server
	if(sourcePtr2[0]._NTP_server == "none")
		data.ntp = "";
	else
		data.ntp = sourcePtr2[0]._NTP_server; 	
 
	data.ntpS = sourcePtr2[0]._NTP_active;
	
	/*clock time & date setting*/
	data.clktime = sourcePtr3[0]._current_time; 
	data.clkdate = sourcePtr4[0]._current_date; 
} 

function timezone_formatChange(length,string) { 
			if(length == 3) { // -xx  x:integer 
				string = string + ":00"; 
			}else if(length == 2) { 
				if(string.charAt(0) == "-") { //-x 
					string = "-0"+string.slice(1,2)+":00"; 
				}else { // xx 
					string = "+"+string+":00"; 
				} 
			}else { 
				if(string.charAt(0) == "0") {//0 
					string = "00:00"; 
				}else { //x 
					string = "+0"+string+":00"; 
				} 
			}				 
	return string; 
} 

function  DHCP_fetch(start_index) {
 
	var sourcePtr;
	var comma = /[\w.]+/g; //for search dns_server format
	sourcePtr = top.frames[4]['result_'+start_index]; 
	
	if(sourcePtr[0]._DHCP_pool_name != undefined ){ 		
		var sourcecnt = sourcePtr[0]._DHCP_pool_name.length;
	}else {			
		var sourcecnt = 0;
	}
	
	var dynDhcpPtr = top.frames[1].dyndhcpList; 
	var staDhcpPtr = top.frames[1].stadhcpList;
	
	dynDhcpPtr.dataSet.splice(0,dynDhcpPtr.dataCount()); //dynamic DHCP 
	staDhcpPtr.dataSet.splice(0, staDhcpPtr.dataCount()); //static DHCP
	
	for(var j = 0; j < sourcecnt; j++) {	
		
		if(sourcePtr[0]._DHCP_pool_name[j]._network_2 != "none"){
			if(sourcePtr[0]._DHCP_pool_name[j]._dns_server_2 == "none") {//5TODO:if the CIDR format eg, /xx , xx can be 8 ,16, 24, 23..
				this.dns1 = "";
				this.dns2 = "";
				this.dns3 = ""; //3
			}else { //sourcePtr[j].dhcp_dnsServer hava value. 
				var result = sourcePtr[0]._DHCP_pool_name[j]._dns_server_2.match(comma);
				if(result.length == 1) { // 1 DNS
					this.dns1 = result[0];
					this.dns2 = "";
					this.dns3 = "";		
				}else if(result.length == 2) {// 2 DNS
					this.dns1 = result[0];
					this.dns2 = result[1];
					this.dns3 = "";	
				}else if(result.length == 3) { // 3 DNS
					this.dns1 = result[0];
					this.dns2 = result[1];
					this.dns3 = result[2];
				}
		 	}
		 	var default_router;
		 	if (sourcePtr[0]._DHCP_pool_name[j]._default_router_2 == "none")
		 		default_router = "";		 		
		 	else
		 		default_router = sourcePtr[0]._DHCP_pool_name[j]._default_router_2;	
		 		
		 	var lease_day;
		 	var lease_hr;
		 	var lease_min;		
			if (sourcePtr[0]._DHCP_pool_name[j]._lease_time_2 == "infinite") {
				lease_day = "infinite";
			}else {
				lease_day = sourcePtr[0]._DHCP_pool_name[j]._lease_time_2;
		 		lease_hr = sourcePtr[0]._DHCP_pool_name[j]._lease_time_4;
		 		lease_min = sourcePtr[0]._DHCP_pool_name[j]._lease_time_6;
			}
							
			dynDhcpPtr.addNewRecord(
							sourcePtr[0]._DHCP_pool_name[j].__name,
							sourcePtr[0]._DHCP_pool_name[j]._network_2,
							'',
							sourcePtr[0]._DHCP_pool_name[j]._netmask_2,
							default_router,
							this.dns1,
							this.dns2,
							this.dns3,						
							lease_day,
							lease_hr,
							lease_min
							);							
		}
		
		if(sourcePtr[0]._DHCP_pool_name[j]._host_2 != "none") {		
			staDhcpPtr.addNewRecord(
							sourcePtr[0]._DHCP_pool_name[j].__name,
							sourcePtr[0]._DHCP_pool_name[j]._host_2,
							sourcePtr[0]._DHCP_pool_name[j]._hardware_address_2,
						'', '', '', '', '', '', '', '');
		}
		
	}
}

function  DNS_fetch(start_index) {
	var sourcePtr = top.frames[4]['result_'+start_index];
	
	var aDnsPtr = top.frames[1].adnsList; 
	var nsDnsPtr = top.frames[1].nsdnsList;
	var mxDnsPtr = top.frames[1].mxdnsList;
		
	aDnsPtr.dataSet.splice(0,aDnsPtr.dataCount()); 
	nsDnsPtr.dataSet.splice(0, nsDnsPtr.dataCount()); 
	mxDnsPtr.dataSet.splice(0, mxDnsPtr.dataCount());
	
	if(sourcePtr[0]._A_record != undefined) {
		var sourcecnt = sourcePtr[0]._A_record.length;
		for(var i = 0; i < sourcecnt; i++) {
			aDnsPtr.addNewRecord(sourcePtr[0]._A_record[i]._FQDN,
			                     sourcePtr[0]._A_record[i]._IP_Address);	
		}			
	}
	
	if(sourcePtr[0]._Zone_Forwarder != undefined) {
		var sourcecnt = sourcePtr[0]._Zone_Forwarder.length;
		var result_forDefault = new Array('','','');
		
		for(var i = 0; i < sourcecnt; i++) {
			var k = 2;
			var bindCnt = "";
			var result = new Array('N/A','N/A','N/A');
			
			while(sourcePtr[0]._Zone_Forwarder[i]['_DNS_Server'+bindCnt] != undefined) {
				if(i != sourcecnt-1) {
					result[k-2] = sourcePtr[0]._Zone_Forwarder[i]['_DNS_Server'+bindCnt];
				}else {
					result_forDefault[0] += sourcePtr[0]._Zone_Forwarder[i]['_DNS_Server'+bindCnt]+",";
				}
				bindCnt = "_"+k;
				k++;
			}
			var private_IP = (sourcePtr[0]._Zone_Forwarder[i]._From == "User-Defined") ? sourcePtr[0]._Zone_Forwarder[i]._Private_IP : "";
			nsDnsPtr.addNewRecord(sourcePtr[0]._Zone_Forwarder[i]._Domain_Name,
			                      sourcePtr[0]._Zone_Forwarder[i]._From,
			                      result[0],result[1],result[2], 
			                      result_forDefault[0],
			                      result_forDefault[1],
			                      result_forDefault[2],
			                      private_IP);
		}
	}		
	
	if(sourcePtr[0]._MX_record != undefined) {
		var sourcecnt = sourcePtr[0]._MX_record.length;
		for(var i = 0; i < sourcecnt; i++) {
			mxDnsPtr.addNewRecord(sourcePtr[0]._MX_record[i]._Domain_Name,
			                      sourcePtr[0]._MX_record[i]._IP_Address_FQDN);		
		}
	}
}

function HTTPS_fetch(start_index){
	var sourcePtr = top.frames[4]['result_'+start_index]; 
	var sourcePtr2 = top.frames[4]['result_'+start_index+'_1']; 
	var sourcePtr3 = top.frames[4]['result_'+start_index+'_2']; 
	
	var sourcecnt = sourcePtr.length; 

	var httpsPtr = top.frames[1].httpsList; 
	var httpsCtrlPtrAdmin = top.frames[1].httpsAdmin;	
	var httpsCtrlPtrUser = top.frames[1].httpsUser;	
	
	httpsPtr.dataSet.splice(0,httpsPtr.dataCount());	
	httpsCtrlPtrAdmin.dataSet.splice(0,httpsCtrlPtrAdmin.dataCount()); 
	httpsCtrlPtrUser.dataSet.splice(0,httpsCtrlPtrUser.dataCount()); 	
	gl.BufferObj.dataSet.splice(0,gl.BufferObj.dataCount());
	
	httpsPtr.addNewRecord(sourcePtr[0]._active,
	                      sourcePtr[0]._port,
	                      '',
	                      sourcePtr[0]._authentication_client,
	                      sourcePtr[0]._certificate,
	                      sourcePtr[0]._force_redirect);
									      				
	gl.BufferObj.addNewRecord(sourcePtr[0]._active,
	                          sourcePtr[0]._port,
	                          sourcePtr[0]._authentication_client,
	                          sourcePtr[0]._certificate,
	                          sourcePtr[0]._force_redirect);
			      								
	var sourcecnt2 = sourcePtr2.length; 
	for(var j = 0; j < sourcecnt2; j++) {				
		httpsCtrlPtrAdmin.addNewRecord(sourcePtr2[j]._Zone,
		                               sourcePtr2[j]._Address,
		                               sourcePtr2[j]._Action);
	}
	var sourcecnt3 = sourcePtr3.length; 
	for(var j = 0; j < sourcecnt3; j++) {				
		httpsCtrlPtrUser.addNewRecord(sourcePtr3[j]._Zone,
		                              sourcePtr3[j]._Address,
		                              sourcePtr3[j]._Action);
	}
	
}

function WWW_fetch(start_index){
	var sourcePtr = top.frames[4]['result_'+start_index]; 
	var sourcePtr2 = top.frames[4]['result_'+start_index+'_1']; 
	var sourcePtr3 = top.frames[4]['result_'+start_index+'_2']; 

	var sourcecnt = sourcePtr.length; 
	
	var wwwPtr = top.frames[1].wwwList; 
	var wwwCtrlPtrAdmin = top.frames[1].wwwAdmin;	
	var wwwCtrlPtrUser = top.frames[1].wwwUser;	
		
	wwwPtr.dataSet.splice(0,wwwPtr.dataCount());
	wwwCtrlPtrAdmin.dataSet.splice(0,wwwCtrlPtrAdmin.dataCount()); 
	wwwCtrlPtrUser.dataSet.splice(0,wwwCtrlPtrUser.dataCount()); 
	var buffPtr = gl.BufferObj.getRecord(0);
	
	wwwPtr.addNewRecord(sourcePtr[0]._active,
	                    sourcePtr[0]._port,
	                    sourcePtr[0]._authentication_method,0);
											
	buffPtr.mb6 = sourcePtr[0]._active;
	buffPtr.mb7 = sourcePtr[0]._port;
	buffPtr.mb8 = sourcePtr[0]._authentication_method;
     
	var sourcecnt2 = sourcePtr2.length;
	for(var j = 0; j < sourcecnt2; j++) {		
		wwwCtrlPtrAdmin.addNewRecord(
					sourcePtr2[j]._Zone,
					sourcePtr2[j]._Address,
					sourcePtr2[j]._Action);
	}
	var sourcecnt3 = sourcePtr3.length;
	for(var j = 0; j < sourcecnt3; j++) {		
		wwwCtrlPtrUser.addNewRecord(
					sourcePtr3[j]._Zone,
					sourcePtr3[j]._Address,
					sourcePtr3[j]._Action);
	}
	
}

function SSH_fetch(start_index){	
	var sourcePtr = top.frames[4]['result_'+start_index]; 
	var sourcePtr2 = top.frames[4]['result_'+start_index+'_1']; 
	var sourcecnt2 = sourcePtr2.length; 
	var sourcecnt = sourcePtr.length; 


	var sshPtr = top.frames[1].sshList; 
	var sshCtrlPtr = top.frames[1].ssh;
	sshPtr.dataSet.splice(0,sshPtr.dataCount());	
	sshCtrlPtr.dataSet.splice(0,sshCtrlPtr.dataCount()); 
	gl.BufferObj.dataSet.splice(0,gl.BufferObj.dataCount());
	
	sshPtr.addNewRecord(sourcePtr[0]._active,
	                    sourcePtr[0]._port,
	                    sourcePtr[0]._v1_supported,
	                    0,
	                    sourcePtr[0]._certificate);				
	gl.BufferObj.addNewRecord(sourcePtr[0]._active,
	                          sourcePtr[0]._port,
	                          sourcePtr[0]._v1_supported,
	                          sourcePtr[0]._certificate);		
	for(var j = 0; j < sourcecnt2; j++) {		
		sshCtrlPtr.addNewRecord(
					sourcePtr2[j]._Zone,
					sourcePtr2[j]._Address,
					sourcePtr2[j]._Action
					);
	}
}

function Telnet_fetch(start_index){
	var sourcePtr;	
	sourcePtr = top.frames[4]['result_'+start_index]; 
	var sourcePtr2 = top.frames[4]['result_'+start_index+'_1']; 
	var sourcecnt2 = sourcePtr2.length; 
	
	var sourcecnt = sourcePtr.length; 

	var telnetPtr = top.frames[1].telnetList; 	
	var telnetCtrlPtr = top.frames[1].telnet;
	telnetPtr.dataSet.splice(0,telnetPtr.dataCount());		
	telnetCtrlPtr.dataSet.splice(0,telnetCtrlPtr.dataCount()); 
	gl.BufferObj.dataSet.splice(0,gl.BufferObj.dataCount());
	
	telnetPtr.addNewRecord(sourcePtr[0]._active,
	                       sourcePtr[0]._port,
	                       0, 0);
	gl.BufferObj.addNewRecord(sourcePtr[0]._active,
	                          sourcePtr[0]._port);
													
	for(var j = 0; j < sourcecnt2; j++) {		
		telnetCtrlPtr.addNewRecord(
					sourcePtr2[j]._Zone,
					sourcePtr2[j]._Address,
					sourcePtr2[j]._Action
					);
	}
}

function FTP_fetch(start_index){
	var sourcePtr;	
	sourcePtr = top.frames[4]['result_'+start_index]; 
	var sourcePtr2 = top.frames[4]['result_'+start_index+'_1']; 
	var sourcecnt2 = sourcePtr2.length; 
	
	var sourcecnt = sourcePtr.length; 

	var ftpPtr = top.frames[1].ftpList; 
		
	var ftpCtrlPtr = top.frames[1].ftp;
	ftpPtr.dataSet.splice(0,ftpPtr.dataCount());
	ftpCtrlPtr.dataSet.splice(0,ftpCtrlPtr.dataCount()); 
	gl.BufferObj.dataSet.splice(0,gl.BufferObj.dataCount());
	
	ftpPtr.addNewRecord(sourcePtr[0]._active,
	                    sourcePtr[0]._port,
	                    sourcePtr[0]._TLS, 
	                    sourcePtr[0]._certificate);
	gl.BufferObj.addNewRecord(sourcePtr[0]._active,
	                          sourcePtr[0]._port,
	                          sourcePtr[0]._TLS, 
	                          sourcePtr[0]._certificate);

	for(var j = 0; j < sourcecnt2; j++) {
		ftpCtrlPtr.addNewRecord(
					sourcePtr2[j]._Zone,
					sourcePtr2[j]._Address,
					sourcePtr2[j]._Action
					);
	}
}

function SNMP_fetch(start_index){
	var sourcePtr;	
	sourcePtr = top.frames[4]['result_'+start_index]; 
	var sourcePtr2 = top.frames[4]['result_'+start_index+'_1']; 
	var sourcecnt2 = sourcePtr2.length; 
	
	
	var sourcecnt = sourcePtr.length; 

	var snmpPtr = top.frames[1].snmpList; 
	var snmpCtrlPtr = top.frames[1].snmp;	
	snmpPtr.dataSet.splice(0,snmpPtr.dataCount());
	snmpCtrlPtr.dataSet.splice(0,snmpCtrlPtr.dataCount()); 
	gl.BufferObj.dataSet.splice(0,gl.BufferObj.dataCount());
	
	if(sourcePtr[0]._trap_community == "none") {
		sourcePtr[0]._trap_community = "";
	}
	if(sourcePtr[0]._trap_host == "none") {
		sourcePtr[0]._trap_host = "0.0.0.0";
	}
	
	snmpPtr.addNewRecord(sourcePtr[0]._active,
	                     sourcePtr[0]._port,
	                     0,0,0,0,
	                     sourcePtr[0]._get_community,
	                     sourcePtr[0]._set_community,
	                     sourcePtr[0]._trap_community,
	                     sourcePtr[0]._trap_host);
	gl.BufferObj.addNewRecord(sourcePtr[0]._active,
	                          sourcePtr[0]._port,
	                          sourcePtr[0]._get_community,
	                          sourcePtr[0]._set_community,
	                          sourcePtr[0]._trap_community,
	                          sourcePtr[0]._trap_host);

	for(var j = 0; j < sourcecnt2; j++) {
		snmpCtrlPtr.addNewRecord(
					sourcePtr2[j]._Zone,
					sourcePtr2[j]._Address,
					sourcePtr2[j]._Action
					);
	}
}

function ICMP_fetch(start_index){
	var sourcePtr;	
	sourcePtr = top.frames[4]['result_'+start_index]; 
	var sourcePtr2 = top.frames[4]['result_'+start_index+'_1']; 
	var sourcecnt2 = sourcePtr2.length; 
	
	var sourcecnt = sourcePtr.length; 

	var icmpPtr = top.frames[1].icmpList; 
		
	var icmpCtrlPtr = top.frames[1].icmp;
	icmpPtr.dataSet.splice(0,icmpPtr.dataCount());
	icmpCtrlPtr.dataSet.splice(0,icmpCtrlPtr.dataCount()); 
	gl.BufferObj.dataSet.splice(0,gl.BufferObj.dataCount());
	
	icmpPtr.addNewRecord(sourcePtr[0]._Activate);
	gl.BufferObj.addNewRecord(sourcePtr[0]._Activate);

	for(var j = 0; j < sourcecnt2; j++) {
		icmpCtrlPtr.addNewRecord(
		                         sourcePtr2[j]._Zone,
		                         sourcePtr2[j]._Address,
		                         sourcePtr2[j]._Action,
		                         sourcePtr2[j]._ICMP_Type
		                         );
	}
}
function chkDNS_whichRecord(str){
	if(str == undefined) {
		str = "";
		return str;
	}else {
		return str;
	}
}

function  USER_fetch(start_index) {
 	var arg = new Array();
	var sourcePtr;
	
	sourcePtr = top.frames[4]['result_'+start_index]; 
	sourcePtr1 = top.frames[4]['result_'+(start_index+1)];
	sourcePtr2 = top.frames[4]['result_'+(start_index+2)];
	sourcePtr3 = top.frames[4]['result_'+(start_index+3)];
	sourcePtr4 = top.frames[4]['result_'+(start_index+4)];
	sourcePtr5 = top.frames[4]['result_'+(start_index+5)];
	
	var sourcecnt = sourcePtr.length; 
	var userPtr = top.frames[1].usersList; 
	
	userPtr.dataSet.splice(0, userPtr.dataCount()); 
	gl.BufferObj.dataSet.splice(0,gl.BufferObj.dataCount());	
	for(var j= 0 ; j < sourcecnt; j++) {		
		userPtr.addNewRecord( sourcePtr[j]._Username,
			      0, 0,
			      sourcePtr[j]._Description,
			      0, 0, 0, 0, 0, 0, 0, 0
			      );	
	}
	
	userPtr.addNewRecord( '', 
		sourcePtr2[0]._enable_simultaneous_logon_limitation_for_administration_account, 
		sourcePtr2[0]._maximum_simultaneous_logon_per_administration_account, 
		sourcePtr2[0]._enable_simultaneous_logon_limitation_for_access_account, 
		sourcePtr2[0]._maximum_simultaneous_logon_per_access_account, 
		sourcePtr3[0]._enable_logon_retry_limit,
		sourcePtr1[0]._logon_lease_time,
		0,
		sourcePtr1[0]._logon_reauth_time,
		sourcePtr3[0]._maximum_retry_count, 
		sourcePtr3[0]._lockout_period,
		sourcePtr1[0]._user_type
		);
	gl.BufferObj.addNewRecord(
		sourcePtr4[0]._enable_user_update_lease_automation,
		sourcePtr5[0]._enable_user_idle_detection,
		sourcePtr5[0]._user_idle_timeout
		);
}
function USER_fetch_1(start_index,idx) {
	var sourcePtr;	
	sourcePtr = top.frames[4]['result_'+start_index]; 
	var userPtr = top.frames[1].usersList.getRecord(idx);
	userPtr.password = sourcePtr[0]._password;
	userPtr.description = sourcePtr[0]._description;
	userPtr.idletimeout = sourcePtr[0]._lease_time;
	userPtr.reauthtimeout = sourcePtr[0]._re_auth_time;
	userPtr.usetype = sourcePtr[0]._user_type;
}

function  USERGRP_fetch(start_index) {
	/* username */
	var sourcePtr = top.frames[4]['result_'+start_index];
	var sourcecnt = sourcePtr.length; 
	var userPtr = top.frames[1].usersList; 
	
	userPtr.dataSet.splice(0, userPtr.dataCount());
	for(var j = 0 ; j < sourcecnt; j++) {		
		userPtr.addNewRecord( sourcePtr[j]._Username);
	}
	
	/* groupname */
 	var sourcePtr = top.frames[4]['result_'+(start_index+1)];
	var sourcecnt = sourcePtr.length; 
	var usgrpPtr = top.frames[1].usgrpList;
	
	usgrpPtr.dataSet.splice(0, usgrpPtr.dataCount()); 
	gl.BufferObj_2.dataSet.splice(0,gl.BufferObj_2.dataCount());
	
	if(sourcePtr[0]._Group != undefined) {
		var sourcecnt = sourcePtr[0]._Group.length;				
		for(var j = 0; j < sourcecnt ; j++) {
			var member_list = "";
			for(var k = 0; k < sourcePtr[0]._Group[j]._member_count; k++){
				if(k==0){
					member_list += sourcePtr[0]._Group[j]._member;
				}else{
					member_list += (","+sourcePtr[0]._Group[j]['_member_'+(k+1)]);
				}
			}
			usgrpPtr.addNewRecord(sourcePtr[0]._Group[j].__name, member_list, 0, sourcePtr[0]._Group[j]._description);	
			gl.BufferObj_2.addNewRecord(sourcePtr[0]._Group[j].__name,sourcePtr[0]._Group[j]._description);
		}
	}
}

function usergrpedit_edit(start_index, idx) {
	var theDataPtr = gl.usgrpList.getRecord(idx);	
	var grpmemPtr = theDataPtr.member;
	var sourcePtr = top.frames[4]["result_"+start_index]; 
	grpmemPtr.dataSet.splice(0,grpmemPtr.dataCount());
	
	for(var i = 0; i < sourcePtr[0]._Group[0]._member_count; i++){
		var str = (i == 0) ? "_member" : "_member_"+(i+1);
		for(var j = 0; j < gl.usgrpList.dataCount(); j++) {
			var isGrp = false; 
			if(sourcePtr[0]._Group[0][str] == gl.usgrpList.getRecord(j).name) {
				var prefix = "/Group";
				isGrp = true;
				break;
			}
		}
		if(!isGrp) {
			for(var j = 0; j < gl.usersList.dataCount(); j++) {
				if(sourcePtr[0]._Group[0][str] == gl.usersList.getRecord(j).name) {
					var prefix = "/User";
					break;
				}
			}
		}
		grpmemPtr.addNewRecord(sourcePtr[0]._Group[0][str] + prefix );
	}
}

function assummary_fetch_1(start_index){
	var sourcePtr_0 = top.frames[4]["result_"+start_index]; 
	var sourcePtr_1 = top.frames[4]["result_"+(start_index+1)]; 
	var sourcePtr_2 = top.frames[4]["result_"+(start_index+2)]; 
	var sourcePtr_4 = top.frames[4]["result_"+(start_index+4)]; 
	
	var theDataPtr = gl.asList; // get data pointer	
	theDataPtr.dataSet.splice( 0, theDataPtr.dataCount() );
	theDataPtr.addNewRecord( 	convert_to_bolean(2, sourcePtr_0[0]._anti_spam_status), 
														convert_to_bolean(2, sourcePtr_1[0]._active), 
														sourcePtr_1[0]._message, 
														sourcePtr_4[0]._Status,
														sourcePtr_4[0]._Type);
					
	/**********************************/
	var theZonePtr = theDataPtr.getRecord(0).zonelink;
	theZonePtr.dataSet.splice( 0, theZonePtr.dataCount() );

	if (sourcePtr_2[0]._anti_spam_profile != undefined) {
		var sourcecnt = sourcePtr_2[0]._anti_spam_profile.length; 
	} else {
		var sourcecnt = 0; 
	}
	for ( var j = 0; j<sourcecnt; j++ ){
		var zone_pair = sourcePtr_2[0]._anti_spam_profile[j].__name.split(" to ");
		var proto_list = "";
		if (sourcePtr_2[0]._anti_spam_profile[j]._SMTP._active == "yes") {
			proto_list = "SMTP ";
		}
		if (sourcePtr_2[0]._anti_spam_profile[j]._POP3._active == "yes") {
			proto_list = proto_list + "POP3";
		}
		theZonePtr.addNewRecord(zone_pair[0], 
					 									zone_pair[1], 
					 									proto_list, 
					 									convert_to_bolean(2, sourcePtr_2[0]._anti_spam_profile[j]._active), 
					 									"", 
					 									""  ,
					 									convert_to_bolean(2, sourcePtr_2[0]._anti_spam_profile[j]._active) ); 
	}
	
	var sourcePtr = top.frames[4]['result_'+(start_index+3)]; 
	var sourcecnt = sourcePtr.length; 
	var theDataPtr = gl.ZoneDataList;
	theDataPtr.dataSet.splice(0,theDataPtr.dataCount());
	for(var i=0; i<sourcecnt; i++){
		theDataPtr.addNewRecord(sourcePtr[i]._Name);
	}
}

function assumedit_fetch_1(index){
	var sourcePtr = top.frames[4]['result_0']; 

	var theDataPtr = gl.asList; // get data pointer	
	var theZonePtr = theDataPtr.getRecord(0).zonelink;
	
	theZonePtr.getRecord(index).score = sourcePtr[0]._anti_spam_profile[index]._spam_score;
	theZonePtr.getRecord(index).log 	= sourcePtr[0]._anti_spam_profile[index]._spam_log;
	
	var theSubPtr = theDataPtr.dataSet[0].sublink;
	theSubPtr.dataSet.splice(0, theSubPtr.dataCount());
	
	theSubPtr.addNewRecord(	"SMTP", 
													sourcePtr[0]._anti_spam_profile[index]._SMTP._action, 
													sourcePtr[0]._anti_spam_profile[index]._SMTP._mark, 
													convert_to_bolean(2, sourcePtr[0]._anti_spam_profile[index]._SMTP._active) , 
													convert_to_bolean(2, sourcePtr[0]._anti_spam_profile[index]._SMTP._active) );
	
	theSubPtr.addNewRecord(	"POP3", 
													sourcePtr[0]._anti_spam_profile[index]._POP3._action, 
													sourcePtr[0]._anti_spam_profile[index]._POP3._mark, 
													convert_to_bolean(2, sourcePtr[0]._anti_spam_profile[index]._POP3._active) ,
													convert_to_bolean(2, sourcePtr[0]._anti_spam_profile[index]._POP3._active) );
}

function aswblist_fetch_1(start_index){
	var sourcePtr_0 = top.frames[4]["result_"+start_index]; 
	var sourcePtr_1 = top.frames[4]["result_"+(start_index+1)]; 
	var theDataPtr = gl.wbList; // get data pointer	
	theDataPtr.dataSet.splice(0, theDataPtr.dataSet.length);
	theDataPtr.addNewRecord( 	ewhite = convert_to_bolean(2, sourcePtr_0[0]._active), 
							eblack = convert_to_bolean(2, sourcePtr_1[0]._active) );

	/****************************/
	var sourcePtr_2 = top.frames[4]["result_"+(start_index+2)]; 
	var sourcecnt = sourcePtr_2.length;
	var theWtePtl = theDataPtr.dataSet[0].subwhite;
	theWtePtl.dataSet.splice(0, theWtePtl.dataSet.length);
	var str = new Array();
	for ( var j=0; j<sourcecnt; j++){
		if (( sourcePtr_2[j]._Type == "mail-header" )||(sourcePtr_2[j]._Type == "ip")) {
			str[0] = sourcePtr_2[j]._Content;
			str[1] = sourcePtr_2[j]._Content_2;
		}
		theWtePtl.addNewRecord( 	list 		= "White List", 
								type		= sourcePtr_2[j]._Type, 
								emailaddr= ( 	sourcePtr_2[j]._Type == "e-mail" ? sourcePtr_2[j]._Content : "" ), 
								ipaddr	= ( 	sourcePtr_2[j]._Type == "ip" ? str[0] : "" ), 
								network	= ( 	sourcePtr_2[j]._Type == "ip" ? str[1] : "" ), 
								mailheader=( 	sourcePtr_2[j]._Type == "mail-header" ? str[0] : "" ), 
								value	= ( 	sourcePtr_2[j]._Type == "mail-header" ? str[1] : "" ), 
								active	= convert_to_bolean(2, sourcePtr_2[j]._Active), 		
								orig_active= convert_to_bolean(2, sourcePtr_2[j]._Active) 	);
		
	}

	/****************************/
	var sourcePtr_3 = top.frames[4]["result_"+(start_index+3)]; 
	var sourcecnt = sourcePtr_3.length;
	var theBlkPtl = theDataPtr.dataSet[0].subblack;
	theBlkPtl.dataSet.splice(0, theBlkPtl.dataSet.length);
	for ( var j=0; j<sourcecnt; j++){

		if ( sourcePtr_3[j]._Type == "mail-header" || sourcePtr_3[j]._Type == "ip") {
			str[0] = sourcePtr_3[j]._Content;
			str[1] = sourcePtr_3[j]._Content_2;
		}

		theBlkPtl.addNewRecord( 	list 		= "Black List", 
								type		= sourcePtr_3[j]._Type, 
								emailaddr= ( sourcePtr_3[j]._Type == "e-mail" ? sourcePtr_3[j]._Content : "" ), 
								ipaddr	= ( sourcePtr_3[j]._Type == "ip" ? str[0] : ""), 
								network	= ( sourcePtr_3[j]._Type == "ip" ? str[1] : ""), 
								mailheader=( sourcePtr_3[j]._Type == "mail-header" ? str[0]: ""), 
								value	= ( sourcePtr_3[j]._Type == "mail-header" ? str[1] : ""), 
								active	= convert_to_bolean(2, sourcePtr_3[j]._Active), 
								orig_active= convert_to_bolean(2, sourcePtr_3[j]._Active) 	);
	}


}

function asrbl_fetch_1(start_index){
	var sourcePtr_0 = top.frames[4]["result_"+start_index]; 
	var sourcePtr_2 = top.frames[4]["result_"+(start_index+2)]; 

	var theDataPtr = gl.rbList; // get data pointer	
	theDataPtr.dataSet.splice(0, theDataPtr.dataSet.length);
	theDataPtr.addNewRecord( 	enale = convert_to_bolean(2, sourcePtr_0[0]._active), 
							maxquery = sourcePtr_2[0]._maximal_query_IPs);
	/***************************/
	var theRblPtr = theDataPtr.dataSet[0].subrbl;
	theRblPtr.dataSet.splice(0, theRblPtr.dataSet.length);
	var sourcePtr_1 = top.frames[4]["result_"+(start_index+1)]; 
	var sourcecnt = sourcePtr_1.length
	for ( var i=0; i<sourcecnt; i++){
		theRblPtr.addNewRecord(	"", 
					svrquery    = sourcePtr_1[i]._Content, 
					active	    = convert_to_bolean(2, sourcePtr_1[i]._Active), 
					orig_active = convert_to_bolean(2, sourcePtr_1[i]._Active)  );
	}
}

function configfile_fetch_1(start_index){
	var sourcePtr = top.frames[4]["result_"+start_index]; 
	var sourcecnt = sourcePtr.length; 

	var theDataPtr = gl.ConFileList;
	theDataPtr.dataSet.splice(0, theDataPtr.dataCount());
	for ( var i=0; i<sourcecnt; i++){
		theDataPtr.addNewRecord(FileName=sourcePtr[i]._File_Name,
					Size=sourcePtr[i]._Size, 
  					Created	=sourcePtr[i]._Modified_Time );
	}
	//show users current
	var limitPtr = gl.LimitObj;
	limitPtr.dataSet.splice(0, limitPtr.dataCount());//clear data object
	var sourcePtr = top.frames[4]["result_"+(start_index+1)]; 
	limitPtr.addNewRecord(sourcePtr[0]._Type);//mb1
}

function shellfile_fetch_1(start_index){
	var sourcePtr = top.frames[4]["result_"+start_index]; 
	var sourcecnt = sourcePtr.length; 

	var theDataPtr = gl.scriptFileList;
	theDataPtr.dataSet.splice(0, theDataPtr.dataCount());
	for ( var i=0; i<sourcecnt; i++){
		theDataPtr.addNewRecord(FileName=sourcePtr[i]._File_Name,
					Size=sourcePtr[i]._Size, 
  					Created	=sourcePtr[i]._Modified_Time );
	}
	//show users current
	var limitPtr = gl.LimitObj;
	limitPtr.dataSet.splice(0, limitPtr.dataCount());//clear data object
	var sourcePtr = top.frames[4]["result_"+(start_index+1)]; 
	limitPtr.addNewRecord(sourcePtr[0]._Type);//mb1
}

function fwfile_fetch_1(start_index){
	var sourcePtr = top.frames[4]["result_"+start_index]; 
	var sourcecnt = sourcePtr.length; 

	var theDataPtr = gl.fwFileList;
	theDataPtr.dataSet.splice(0, theDataPtr.dataCount());
	for ( var i=0; i<sourcecnt; i++){
		theDataPtr.addNewRecord(FileName=sourcePtr[i]._BM_version,
					Size=sourcePtr[i]._firmware_version, 
  					Created	=sourcePtr[i]._build_date );
	}
	//show users current
	var limitPtr = gl.LimitObj;
	limitPtr.dataSet.splice(0, limitPtr.dataCount());//clear data object
	var sourcePtr = top.frames[4]["result_"+(start_index+1)]; 
	limitPtr.addNewRecord(sourcePtr[0]._Type);//mb1
}

function portGp_fetch_1(start_index){
	var sourcePtr = top.frames[4]["result_"+start_index]; 
	var sourcecnt = sourcePtr.length; 

	var theDataPtr = gl.portGpList;
	theDataPtr.dataSet.splice(0, theDataPtr.dataCount());
	if(top.frames[1].ModelType == "ZyWALL 1050") {
		for ( var i = 0; i < sourcecnt; i++) {
			theDataPtr.addNewRecord(iface=sourcePtr[i]._Representative_Name,
			            port1=if_undefined(sourcePtr[i]._Port1),
			            port2=if_undefined(sourcePtr[i]._Port2),
			            port3=if_undefined(sourcePtr[i]._Port3), 
			            port4=if_undefined(sourcePtr[i]._Port4),
			            port5=if_undefined(sourcePtr[i]._Port5)
	  					);
		}
	}else if(top.frames[1].ModelType == "ZyWALL USG 300") {
		for ( var i = 0; i < sourcecnt; i++) {
			theDataPtr.addNewRecord(iface=sourcePtr[i]._Representative_Name,
			            port1=if_undefined(sourcePtr[i]._Port1),
			            port2=if_undefined(sourcePtr[i]._Port2),
			            port3=if_undefined(sourcePtr[i]._Port3), 
			            port4=if_undefined(sourcePtr[i]._Port4),
			            port5=if_undefined(sourcePtr[i]._Port5),
			            port6=if_undefined(sourcePtr[i]._Port6),
			            port7=if_undefined(sourcePtr[i]._Port7)
	  					);
		}
	}
}

function vrrpgrp_fetch_1(start_index){
	var sourcePtr = top.frames[4]["result_"+start_index]; 
	var theDataPtr = gl.VrrpGrpList;
	theDataPtr.dataSet.splice(0, theDataPtr.dataCount());
	if(sourcePtr[0]._vrrp_group_name == undefined) {
		var sourcecnt = 0;
	}else {
		var sourcecnt = sourcePtr[0]._vrrp_group_name.length; 
		for ( var i = 0; i < sourcecnt; i++){
			theDataPtr.addNewRecord(sourcePtr[0]._vrrp_group_name[i].__name, 
						sourcePtr[0]._vrrp_group_name[i]._VRID, 
						sourcePtr[0]._vrrp_group_name[i]._role, 
						sourcePtr[0]._vrrp_group_name[i]._interface,
						sourcePtr[0]._vrrp_group_name[i]._backup_only_configuration._priority,
						sourcePtr[0]._vrrp_group_name[i]._backup_only_configuration._preempt,
						sourcePtr[0]._vrrp_group_name[i]._backup_only_configuration._manage_IP,
						sourcePtr[0]._vrrp_group_name[i]._backup_only_configuration._manage_IP_netmask,
						sourcePtr[0]._vrrp_group_name[i]._authentication,
						sourcePtr[0]._vrrp_group_name[i]._key_string,
						sourcePtr[0]._vrrp_group_name[i]._description,
						"n/a",//device ha status
						sourcePtr[0]._vrrp_group_name[i]._active
						); 
		}
	}
	
	/* map HA status to vrrpgrp object*/
	var hacnt = 0;
	var sourcePtr = top.frames[4]["result_"+(start_index+1)]; 
	if(sourcePtr[0]._vrrp_group_name != undefined ){ 
		hacnt = sourcePtr[0]._vrrp_group_name.length; 
	}else{
		hacnt = 0;
	}
	var theVrrpPtr = top.frames[1].VrrpGrpList; 
	var dcnt= theVrrpPtr.dataCount(); //interface counter
	for(var i=0; i<dcnt; i++){
		var data = theVrrpPtr.getRecord(i);
		for(var j=0; j<hacnt; j++){
			if(data.name == sourcePtr[0]._vrrp_group_name[j].__name){
				data.devhastatus = sourcePtr[0]._vrrp_group_name[j]._HA_status;
			}
		}
	}
}

function REGIST_fetch(start_index){
	var sourcePtr;
	var regPtr = top.frames[1].regList;
	
	sourcePtr0 = top.frames[4]['result_'+ start_index];
	sourcePtr1 = top.frames[4]['result_'+ (start_index+1)];
	sourcePtr2 = top.frames[4]['result_'+ (start_index+2)];

	regPtr.dataSet.splice(0, regPtr.dataCount());
	if(sourcePtr0[0]._username == "none") {
		sourcePtr0[0]._username = "";
	}
	if(sourcePtr0[0]._password == "none") {
		sourcePtr0[0]._password = "";
	}
	regPtr.addNewRecord( sourcePtr0[0]._device_register_status,
	                     sourcePtr0[0]._username,
	                     sourcePtr0[0]._password,'');		
		            
	/*----- For register service management --------*/
	var regservPtr = regPtr.getRecord(0).regserv;

	/* IDP */	
	regservPtr.addNewRecord(sourcePtr1[0]._Status,
	                        sourcePtr1[0]._Type,
	                        sourcePtr2[0]._Remaining_Day);
	
	/* AV */
	regservPtr.addNewRecord(sourcePtr1[1]._Status,
	                        sourcePtr1[1]._Type,
	                        sourcePtr2[1]._Remaining_Day);
	
	/* SSL VPN */
	regservPtr.addNewRecord(sourcePtr1[2]._Status,
					sourcePtr1[2]._Type,
					sourcePtr1[2]._Count);
	/* CF */          
	regservPtr.addNewRecord(sourcePtr1[3]._Status,
					sourcePtr1[3]._Type,
					sourcePtr2[2]._Remaining_Day);
	
	/*regservPtr.addNewRecord(sourcePtr1[3]._Status,
	                        sourcePtr1[3]._Type,
	                        sourcePtr2[3]._Remaining_Day);							
	*/
}

function HTTPRED_fetch(start_index){
	var sourcePtr;	
	sourcePtr = top.frames[4]['result_'+start_index]; 
	var httpredPtr = top.frames[1].httpredList;
	var bufPtr = top.frames[1].BufferObj; 
	
	httpredPtr.dataSet.splice(0,httpredPtr.dataCount()); 		
	bufPtr.dataSet.splice(0, bufPtr.dataCount());
	
	var sourcecnt = sourcePtr.length;
		
	for(var j = 0; j < sourcecnt; j++) {
		httpredPtr.addNewRecord(sourcePtr[j]._Name,
					sourcePtr[j]._Interface,
					sourcePtr[j]._Proxy_Server,
					sourcePtr[j]._Port,
					sourcePtr[j]._Active);
		bufPtr.addNewRecord(sourcePtr[j]._Active);
	}
}
function VOIP_fetch(start_index){
	var sourcePtr0;
	var sourcePtr1;
	var voipPtr = top.frames[1].voipalgList; 
	
	sourcePtr0 = top.frames[4]['result_'+ start_index];
	sourcePtr1 = top.frames[4]['result_'+ (start_index+1)];
	sourcePtr2 = top.frames[4]['result_'+ (start_index+2)];

	voipPtr.dataSet.splice(0, voipPtr.dataCount());
	voipPtr.addNewRecord( sourcePtr0[0]._active,
	                      sourcePtr0[0]._signaling_inactivity_timeout,
	                      sourcePtr0[0]._media_inactivity_timeout,
	                      sourcePtr0[0]._signaling_port,
	                      sourcePtr0[0]._additional_signaling_port,
	                      sourcePtr1[0]._active,
	                      sourcePtr1[0]._signaling_port,
	                      sourcePtr1[0]._additional_signaling_port,
	                      sourcePtr2[0]._active,
	                      sourcePtr2[0]._signaling_port,
	                      sourcePtr2[0]._additional_signaling_port);		
}
function MailServ_fetch(start_index){
	var sourcePtr;
	var colon = /\w+/g;
	
	var mailPtr = top.frames[1].LogMailList; 
	sourcePtr = top.frames[4]['result_'+start_index];
	var sourcecnt = sourcePtr.length; 
	mailPtr.dataSet.splice(0, mailPtr.dataCount());
	for(var j = 0; j < sourcecnt ; j++) {

		temp = sourcePtr[j]._mail_server;
		var cnt = temp.length;   //cnt = 2 ;
		for( var ii = 0 ; ii < cnt ; ii++){
			var result =  temp[ii]._time_for_sending_log.match(colon);
			mailPtr.addNewRecord( temp[ii]._server_address,
			                      temp[ii]._mail_subject,
			                      temp[ii]._send_log_to,
			                      temp[ii]._send_alerts_to,
			                      temp[ii]._sending_log_schedule,
			                      temp[ii]._day_for_sending_log,
			                      result[0],
			                      result[1],
			                      temp[ii]._enable,
			                      temp[ii]._smtp,
			                      temp[ii]._user,
			                      temp[ii]._from,
			                      temp[ii]._password);
	    }
	}
}
function RemServ_fetch(start_index) {
	var sourcePtr;
	var remservPtr = top.frames[1].SysLogList; 
	sourcePtr = top.frames[4]['result_'+start_index];
	var sourcecnt = sourcePtr.length; 
	remservPtr.dataSet.splice(0, remservPtr.dataCount());	
	for(var j = 0; j < sourcecnt; j++) {
		temp = sourcePtr[j]._remote_server;	
		var cnt = temp.length;   //cnt = 4 ;
	  		for (var ii = 0; ii < cnt; ii++) {
				remservPtr.addNewRecord( temp[ii]._log_format,
				 temp[ii]._server_address,
				 temp[ii]._log_facility,
				 temp[ii]._enable,
				 '');	
	
			}
	}
}
function ViewLog_fetch(start_index){
	var colon = /\w+/g;
	var sourcePtr = top.frames[4]['result_'+start_index];
	var vlogPtr = top.frames[1].vlogList;
	 
	var sourcecnt = sourcePtr.length;
	vlogPtr.dataSet.splice(0, vlogPtr.dataCount());
		
	for(var j = 0; j < sourcecnt; j++) {
		var day 	= sourcePtr[j]._Date_Time;
		var msg 	= sourcePtr[j]._Message;
		var note 	= sourcePtr[j]._Note;
		var dst 	= sourcePtr[j]._Destination;
		var src 	= sourcePtr[j]._Source;
		var pri 	= sourcePtr[j]._Priority;
		var cat 	= sourcePtr[j]._Category;
				
		
		msg 	= (msg == "" ? "&nbsp" : msg); 
		src 	= (src == "" ? "&nbsp" : src);
		dst 	= (dst == "" ? "&nbsp" : dst); 
		note 	= (note == ""? "&nbsp" : note); 
		pri 	= (pri == "" ? "&nbsp" : pri); 
		cat 	= (cat == "" ? "&nbsp" : cat); 
		
		vlogPtr.addNewRecord( day,msg,src,dst,note,0,pri,cat);
	}
	gl.EndPt = ((gl.StartPt+gl.Lists) > sourcecnt ? sourcecnt : (gl.StartPt+gl.Lists));
}

function Report_fetch(start_index, idx) {
	var count = 0;
	var reportPtr = top.frames[1].ReportList; 
	reportPtr.dataSet.splice(0, reportPtr.dataCount());
		
	/*-- show report status --*/
	var sourcePtr = top.frames[4]['result_'+(start_index+5)];
//	reportPtr.period = sourcePtr[0]._Collection_period;
	reportPtr.status = sourcePtr[0]._Report_status;
	if(sourcePtr[0]._Collect_Statistics != undefined) {
		reportPtr.period = (sourcePtr[0]._Collect_Statistics + "/" +
		                    sourcePtr[0]._Collect_Statistics_2 + "/" +
		                    sourcePtr[0]._Collect_Statistics_3 + "/" + 
		                    sourcePtr[0]._Collect_Statistics_4 + "/" +
		                    sourcePtr[0]._Collect_Statistics_5);												
	}else {
		reportPtr.period = "";
	}
		
	/*-- interface list --*/
	gl.interfaceList.splice(0, gl.interfaceList.length);

	//gigabit ethernet
	var sourcePtr = top.frames[4]['result_'+start_index];
	var sourcecnt = sourcePtr.length; 
		for(var j=0;j<sourcecnt ; j++) { 
		gl.interfaceList.push(sourcePtr[j]._Name);
	}
	gl.interfaceList.push('aux');
	
	//vlan
	var sourcePtr = top.frames[4]['result_'+(start_index+1)];
	var sourcecnt = sourcePtr.length; 
	for(var j = 0; j < sourcecnt; j++) {
		gl.interfaceList.push(sourcePtr[j]._Name);
	}
	
	//bridge
	var sourcePtr = top.frames[4]['result_'+(start_index+2)];
	var sourcecnt = sourcePtr.length; 
	for(var j = 0; j < sourcecnt; j++) {
		gl.interfaceList.push(sourcePtr[j]._Name);
	}
	
	//ppp
	var sourcePtr = top.frames[4]['result_'+(start_index+3)];
	if (sourcePtr[0]._ppp_interface == undefined) {
		var sourcecnt = 0;
	} else {
		var sourcecnt = sourcePtr[0]._ppp_interface.length; 
	}
	for( j = 0; j < sourcecnt; j++){
		gl.interfaceList.push(sourcePtr[0]._ppp_interface[j]._interface_name);
	}
	
	/*-- show report xx xx --*/
	var sourcePtr = top.frames[4]['result_'+(start_index+4)];
	var sourcecnt = sourcePtr.length;
	
	if(idx == 2) {
		for(var j = 0; j < sourcecnt ; j++) { //FIXME: Freq pipe js error
			var num = parseInt(sourcePtr[j]._Hit);	
			count+=num;
		}
	}else {
		for(var j = 0; j < sourcecnt; j++) {
			var num = parseInt(sourcePtr[j]._Amount);
			var unit = sourcePtr[j]._Amount_2;
			if(unit == "KBytes") {	
				count+=num*1024;
			}else if(unit == "MBytes"){
				count+=num*1024*1024;
			}else if(unit == "GBytes") {
				count+=num*1024*1024*1024;
			}else if(unit == "TBytes") {
				count+=num*1024*1024*1024*1024;
			}else {
				count+=num;
			}
		}
	}
	if(idx == 2) {		
		for(var j = 0 ; j < sourcecnt ; j++) {
			var num = parseInt(sourcePtr[j]._Hit);	
			var percent = Math.round((num/count)*100);
			reportPtr.addNewRecord(	'','','','',
				  										sourcePtr[j]._URL, // website
															'','',
			     										sourcePtr[j]._Hit,
				     									percent);	
		}
	}else {
		for(var j = 0 ; j < sourcecnt; j++) {
			var num = parseInt(sourcePtr[j]._Amount); 
			var unit = sourcePtr[j]._Amount_2;
			if(unit == "KBytes") {	
				num = num*1024;
			}else if(unit == "MBytes"){
				num = num*1024*1024;
			}else if(unit == "GBytes") {
				num = num*1024*1024*1024;
			}else if(unit == "TBytes") {
				num = num*1024*1024*1024*1024;
			}
			
			var percent = Math.round((num/count)*100);
			reportPtr.addNewRecord( sourcePtr[j]._IP_Address,
													  	sourcePtr[j]._User,
													  	sourcePtr[j]._Service, 
													  	sourcePtr[j]._Port, 
													  	'', 
															sourcePtr[j]._Direction,
													    sourcePtr[j]._Amount+"("+sourcePtr[j]._Amount_2+")",
													    '',
					     								percent);	
		}	
	}
}

function transMon(str){
	switch(str) {
		case "Jan" :
			str = "01";
			break;
		case "Feb" :
			str = "02";
			break;
		case "Mar" :
			str = "03";
			break;
		case "Apr" :
			str = "04";
			break;
		case "May" :
			str = "05";
			break;
		case "Jun" :
			str = "06";
			break;
		case "Jul" :
			str = "07";
			break;
		case "Aug" :
			str = "08";
			break;
		case "Sep" :
			str = "09";
			break;
		case "Oct" :
			str = "10";
			break;
		case "Nov" :
			str = "11";
			break;
		case "Dec" :
			str = "12";
			break;
	}
	return str;
}
 function transUpper(str) {
		switch(str) {
			case "ppp" :
				str = "PPP";
				break;
			case "app-patrol" :
				str = "Application Patrol";
				break;
			case "content-filter" :
				str = "Content Filter";
				break;
			case "traffic-log" :
				str = "Traffic Log";
				break;
			case "user" :
				str = "User";
				break;
			case "idp" :
				str = "IDP";
				break;
			case "myzyxel-dot-com" :
				str = "myZyXEL.com";
				break;		
			case "zysh" :
				str = "ZySH";
				break;
			case "as" :
				str = "AS";
				break;
			case "ike" :
				str = "IKE";
				break;
			case "ipsec" :
				str = "IPSec";
				break;
			case "kernel" :
				str = "Kernel";
				break;
			case "login" :
				str = "Login";
				break;
			case "syslog" :
				str = "Syslog";
				break;
			case "test" :
				str = "Test";
				break;
			case "firewall" :
				str = "Firewall";
				break;
			case "undef" :
				str = "Undef";
				break;
			case "sessions-limit" :
				str = "Sessions Limit";
				break;
			case "av":
				str = "AV" ;
				break;
			case "built-in-service":
				str = "Built-in Service";
				break;
			case "default" :
				str = "Default";
				break;
			case "system" :
				str = "System";
				break;
			case "policy-route" :
				str = "Policy Route";
				break;
			case "connectivity-check" :
				str = "Connectivity Check";
				break;
			case "file-manage" :
				str = "File Manager";
				break;
			case "device-ha" :
				str = "Device HA";
				break;
			case "nat" :
				str = "NAT";
				break;
			case "pki" :
				str = "PKI";
				break;
			case "interface" :
				str = "Interface";
				break;
			case "account" :
				str = "Account";
				break;
			case "routing-protocol" :
				str = "Routing Protocol";
				break;
			case "port-grouping" :
				str = "Port Grouping";
				break;
			case "forward-web-sites" :
				str = "Forward web sites";
				break;
			case "blocked-web-sites" :
				str = "Blocked web sites";
				break;
			case "show" :
				str = "Show";
				break;
			case "force-auth" :
				str = "Force Authentication";
				break;
			case "interface-statistics" :
				str = "Interface Statistics";
				break;
			case "l2tp-over-ipsec" :
				str = "L2TP Over IPSec";
				break;
			case "anti-virus" :
				str = "Anti-Virus";
				break;
			case "white-list" :
			 	str = "White List";
			 	break;
			case "black-list" :
				str = "Black List";
				break;
			case "ssl-vpn" :
				str = "SSL VPN";
				break;
			case "cnm" :
				str = "Vantage CNM";
				break;
			case "dial-in" :
				str = "Dial-in Mgmt.";
				break;
			case "adp" :
				str = "ADP";
				break;

		}
		return str;
	}
	
function DDNS_fetch(start_index){
	var sourcePtr;
	var ddnsPtr = top.frames[1].ddnsList; 
	sourcePtr = top.frames[4]['result_'+start_index];
	ddnsPtr.dataSet.splice(0, ddnsPtr.dataCount());
	var sourcecnt = sourcePtr.length;	
	
	for(var j = 0; j < sourcecnt; j++) {
		var service_type;
		if(sourcePtr[j]._DDNS_Type == "dyndns") {
			service_type = "DynDNS";
		}else if(sourcePtr[j]._DDNS_Type == "dyndns-static") {
			service_type = "DynDNS static";
		}else {
			service_type = "DynDNS custom";
	  }
	                 
		ddnsPtr.addNewRecord( 
					sourcePtr[j]._Profile_Name,
					service_type,
					sourcePtr[j]._Domain_Name,
					sourcePtr[j]._IP_Address_Update_Policy,
					sourcePtr[j]._Wildcard,
					sourcePtr[j]._WAN_Interface,
					sourcePtr[j]._HA_Interface);
	}	
}

function DDNS_item_fetch(start_index){
	var ddnsItem = top.frames[1].ddnsItem; 
	var sourcePtr = top.frames[4]['result_'+start_index];
	
	ddnsItem.username		= sourcePtr[0]._User_Name;
	ddnsItem.password		= sourcePtr[0]._Password;
	ddnsItem.srvType		= sourcePtr[0]._DDNS_Type;
	ddnsItem.host				= sourcePtr[0]._Doamin_Name;
	ddnsItem.upPolicy		= sourcePtr[0]._IP_Address_Update_Policy;
	ddnsItem.myip				= sourcePtr[0]._Custom_IP;
	ddnsItem.wildcard		= sourcePtr[0]._Wildcard;
	ddnsItem.waniface	 	= sourcePtr[0]._WAN_Interface;
	ddnsItem.haiface		= sourcePtr[0]._HA_Interface;
	ddnsItem.backmx			= sourcePtr[0]._BackMX;
	ddnsItem.mx					= sourcePtr[0]._MX;
}

function DDNS_iface_fetch(start_index){
	var ddnsIface = top.frames[1].ddnsIfaceList; 
	var ethPtr = top.frames[4]['result_'+(start_index)];
	var vlanPtr = top.frames[4]['result_'+(start_index+1)];
	var brPtr = top.frames[4]['result_'+(start_index+2)];
	var pppPtr = top.frames[4]['result_'+(start_index+3)];
	
	ddnsIface.dataSet.splice(0, ddnsIface.dataCount());
	ddnsIface.addNewRecord("none");

	var ethCnt = ethPtr.length;		
	for(var j = 0; j < ethCnt; j++) {
		ddnsIface.addNewRecord(ethPtr[j]._Name);
	}	
	var vlanCnt = vlanPtr.length;		
	for(var j = 0; j < vlanCnt; j++) {
		ddnsIface.addNewRecord(vlanPtr[j]._Name);
	}	
	var brCnt = brPtr.length;		
	for(var j = 0; j < brCnt; j++) {
		ddnsIface.addNewRecord(brPtr[j]._Name);
	}	
	if (pppPtr[0]._ppp_interface == undefined) {
		var pppCnt = 0;
	} else {
		var pppCnt = pppPtr[0]._ppp_interface.length; 
	}
	for( j = 0; j < pppCnt; j++){
		ddnsIface.addNewRecord(pppPtr[0]._ppp_interface[j]._interface_name);
	}
	ddnsIface.addNewRecord('aux');
}
					
function VLAN_fetch(start_index) {
	var sourcePtr;
	var vlanPtr = top.frames[1].vlanList; 
	var sourcePtr = top.frames[4]['result_'+start_index];
	var virtualPtr = top.frames[4]['result_'+(start_index+1)]; //for virtual interface
	var bindIfacePtr =  top.frames[4]['result_'+(start_index+3)];

	if(bindIfacePtr[0]._binding_interface != undefined) {
		var bindIfacecnt = bindIfacePtr[0]._binding_interface.length;
	}else {
		var bindIfacecnt = 0; 
	}
		
	var sourcecnt = sourcePtr.length; 
	var virtualcnt = virtualPtr.length; // for virtual interface count

	vlanPtr.dataSet.splice(0, vlanPtr.dataCount());
	gl.BufferObj.dataSet.splice(0,gl.BufferObj.dataCount());
	gl.no_DHCP.splice(0, gl.no_DHCP.length);
	
	for(var j = 0; j < sourcecnt ; j++) {		
		var port = (sourcePtr[j]._Port == "none" ? "" : sourcePtr[j]._Port);
		var vid = (sourcePtr[j]._VID == "none" ? "" : sourcePtr[j]._VID);
		var port_vid = port+"/"+vid;
		vlanPtr.addNewRecord(
					sourcePtr[j]._Name,
					port_vid,
					sourcePtr[j]._IP_address,
					sourcePtr[j]._Mask,
					sourcePtr[j]._Active,
					sourcePtr[j]._Address_type);
		gl.BufferObj.addNewRecord(sourcePtr[j]._Name, sourcePtr[j]._Active);
		for(var k = 0; k < virtualcnt; k++) {			
			var vlan = virtualPtr[k]._Name.match(/\w+/g);
			if(vlan[0] == sourcePtr[j]._Name) {
				vlanPtr.addNewRecord(	
							virtualPtr[k]._Name,
							"&nbsp;",
							virtualPtr[k]._IP_Address,
							virtualPtr[k]._Mask,
							virtualPtr[k]._Active);
				gl.BufferObj.addNewRecord(virtualPtr[k]._Name, virtualPtr[k]._Active);
			}
		}
		var n = 2;		
		var bindCnt = "";
		
		for(var m = 0; m < bindIfacecnt; m++) {
			if(bindIfacePtr[0]._binding_interface[m].__name == sourcePtr[j]._Name) {
				while(bindIfacePtr[0]._binding_interface[m]['_binding_pool'+bindCnt] != undefined) {
					gl.no_DHCP.push([sourcePtr[j]._Name, bindIfacePtr[0]._binding_interface[m]['_binding_pool'+bindCnt]]);
					bindCnt = "_"+n;
					n++;
				}
			}	
		}
	}
}

function VLAN_detail_fetch(idx,start_index) {
	var sourcePtr;
	var vlanPtr = top.frames[1].vlanList.getRecord(idx); 
	var buffPtr = gl.BufferObj.getRecord(idx);
	
	sourcePtr = top.frames[4]['result_'+ (start_index+1)];
	dhcpPtr = top.frames[4]['result_'+ (start_index+2)];
	
	    
	
	vlanPtr.gwip 		= sourcePtr[0]._gateway;    
	buffPtr.mb3 		= sourcePtr[0]._gateway;
	
	vlanPtr.port 		= sourcePtr[0]._port;	   
	buffPtr.mb4 		= sourcePtr[0]._port;
	
	vlanPtr.vlantag 	= sourcePtr[0]._virtual_LAN_tag;	            
	buffPtr.mb5 		= sourcePtr[0]._virtual_LAN_tag;
	
	vlanPtr.vlandesc 	= sourcePtr[0]._description;
	buffPtr.mb6 		= sourcePtr[0]._description;
	
	vlanPtr.upstream 	= sourcePtr[0]._upstream;			 
	buffPtr.mb7 		= sourcePtr[0]._upstream;

	vlanPtr.downstream = sourcePtr[0]._downstream;
	buffPtr.mb8 = sourcePtr[0]._downstream;
	
	vlanPtr.mtu 	= sourcePtr[0]._MTU;
	buffPtr.mb9	= sourcePtr[0]._MTU;
	
	vlanPtr.metric	= sourcePtr[0]._metric;
	buffPtr.mb10	= sourcePtr[0]._metric;
	
	vlanPtr.member.dataSet.splice(0, vlanPtr.member.dataCount());
	buffPtr.objMb1.dataSet.splice(0, buffPtr.objMb1.dataCount());
	
	vlanPtr.dhcpIP	= sourcePtr[0]._IP_address;
	buffPtr.mb11	= sourcePtr[0]._IP_address;
	buffPtr.mb12	= sourcePtr[0]._netmask;
	buffPtr.mb13	= sourcePtr[0]._IP_type;
	
	if(dhcpPtr[0]._DHCP_Status == "DHCP server") {
		vlanPtr.dhcpflag = 2;
	}else if(dhcpPtr[0]._DHCP_Status == "DHCP relay") {
		vlanPtr.dhcpflag = 1;
	}else if(dhcpPtr[0]._DHCP_Status == "none") {
		vlanPtr.dhcpflag = 0;
	}
	
	vlanPtr.dhcprelay1 = "";
	vlanPtr.dhcprelay2 = "";
}

function VIR_detail_fetch(idx,start_index) { //virtual interface for vlan
	var sourcePtr;
	var virPtr = top.frames[1].vlanList.getRecord(idx); 
	sourcePtr = top.frames[4]['result_'+start_index];
	virPtr.gwip = sourcePtr[0]._gateway;				     	                
	virPtr.vlandesc = sourcePtr[0]._description;
	virPtr.upstream = sourcePtr[0]._upstream;			 
	virPtr.downstream = sourcePtr[0]._downstream;
	virPtr.metric = sourcePtr[0]._metric;
	}

function  bridge_fetch_1(start_index){
	var sourcePtr = top.frames[4]['result_'+start_index]; 
	var virtualPtr = top.frames[4]['result_'+(start_index+1)]; //for virtual interface
	var bindIfacePtr =  top.frames[4]['result_'+(start_index+2)];

	if(bindIfacePtr[0]._binding_interface != undefined) {
		var bindIfacecnt = bindIfacePtr[0]._binding_interface.length;
	}else {
		var bindIfacecnt = 0; 
	}
	
	var sourcecnt = sourcePtr.length; 
	var virdcnt = virtualPtr.length; 

	var theDataPtr = gl.bridgeList; // get data pointer	
	theDataPtr.dataSet.splice(0,theDataPtr.dataCount());
	gl.BufferObj.dataSet.splice(0,gl.BufferObj.dataCount());
	gl.no_DHCP.splice(0, gl.no_DHCP.length);
	
	for( var j = 0; j < sourcecnt; j++) {
		theDataPtr.addNewRecord(
					sourcePtr[j]._Name, '', 
					sourcePtr[j]._IP_address, 
					sourcePtr[j]._Member, 
					sourcePtr[j]._Active,
					sourcePtr[j]._Address_type);
		gl.BufferObj.addNewRecord(sourcePtr[j]._Name,sourcePtr[j]._Active);
		for(var k = 0; k < virdcnt; k++) {
			var vlan = virtualPtr[k]._Name.match(/\w+/g);
			if(vlan[0] == sourcePtr[j]._Name) {
				theDataPtr.addNewRecord(
							virtualPtr[k]._Name,'-',
							virtualPtr[k]._IP_Address,
							virtualPtr[k]._Mask,
							virtualPtr[k]._Active);
				gl.BufferObj.addNewRecord(virtualPtr[k]._Name, virtualPtr[k]._Active);
			}
		}
		
		var n = 2;		
		var bindCnt = "";
		
		for(var m = 0; m < bindIfacecnt; m++) {
			if(bindIfacePtr[0]._binding_interface[m].__name == sourcePtr[j]._Name) {
				while(bindIfacePtr[0]._binding_interface[m]['_binding_pool'+bindCnt] != undefined) {
					gl.no_DHCP.push([sourcePtr[j]._Name, bindIfacePtr[0]._binding_interface[m]['_binding_pool'+bindCnt]]);
					bindCnt = "_"+n;
					n++;
				}
			}	
		}
	}
}


function BRI_Detail_fetch(idx,start_index) {
	var briPtr = top.frames[1].bridgeList.getRecord(idx); 
	var sourcePtr = top.frames[4]['result_'+(start_index+3)];
	dhcpPtr = top.frames[4]['result_'+(start_index+6)];
	var buffPtr = gl.BufferObj.getRecord(idx);
	
	briPtr.gwip 		= sourcePtr[0]._gateway;           
	buffPtr.mb3 		= sourcePtr[0]._gateway;
	
	briPtr.vlandesc 	= sourcePtr[0]._description;
	buffPtr.mb4 		= sourcePtr[0]._description;
	
	briPtr.upstream 	= sourcePtr[0]._upstream;			 
	buffPtr.mb5 		= sourcePtr[0]._upstream;
	
	briPtr.downstream	= sourcePtr[0]._downstream;
	buffPtr.mb6 		= sourcePtr[0]._downstream;
	
	briPtr.mtu 		= sourcePtr[0]._MTU;
	buffPtr.mb7 		= sourcePtr[0]._MTU;
	
	briPtr.metric 		= sourcePtr[0]._metric;
	buffPtr.mb8 		= sourcePtr[0]._metric;
	
	briPtr.member.dataSet.splice(0, briPtr.member.dataCount());
	buffPtr.objMb1.dataSet.splice(0, buffPtr.objMb1.dataCount());
	
	buffPtr.mb9	= sourcePtr[0]._IP_address;
	briPtr.dhcpIP	= sourcePtr[0]._netmask
	buffPtr.mb10	= sourcePtr[0]._netmask;
	buffPtr.mb11	= sourcePtr[0]._IP_type;
	
	if(dhcpPtr[0]._DHCP_Status == "DHCP server") {
		briPtr.dhcpflag = 2;
	}else if(dhcpPtr[0]._DHCP_Status == "DHCP relay") {
		briPtr.dhcpflag = 1;
	}else if(dhcpPtr[0]._DHCP_Status == "none") {
		briPtr.dhcpflag = 0;
	}
	
	briPtr.dhcprelay1 = "";
	briPtr.dhcprelay2 = "";
		
	/*-------- Member ---------*/
	var MemberPtr =  gl.bridgeMemList;
	MemberPtr.dataSet.splice(0, MemberPtr.dataCount());//clear data object
	
	for(var k = 0; k < sourcePtr[0]._join_count; k++){
		if(k == 0){
			MemberPtr.addNewRecord(sourcePtr[0]._member);
		}else{
			MemberPtr.addNewRecord(sourcePtr[0]['_member_'+(k+1)]);
		}
	}
}

function BIR_detail_fetch(idx,start_index) { //virtual interface for bridge
	var sourcePtr;
	var virPtr = top.frames[1].bridgeList.getRecord(idx); 
	sourcePtr = top.frames[4]['result_'+start_index];
	virPtr.gwip = sourcePtr[0]._gateway;				     	                
	virPtr.vlandesc = sourcePtr[0]._description;
	virPtr.upstream = sourcePtr[0]._upstream;			 
	virPtr.downstream = sourcePtr[0]._downstream;
	virPtr.metric = sourcePtr[0]._metric;
}
/* ZyWALL USG 300 */
function sslvpn_fetch(start_index){
	var sourcePtr, sourcecnt;
	var theDataPtr = gl.globalList; 
	var bufPtr = gl.BufferObj_2;
	
	sourcePtr = top.frames[4]['result_'+start_index]; 
	theDataPtr.dataSet.splice(0, theDataPtr.dataCount());
	bufPtr.dataSet.splice(0, bufPtr.dataCount());

	if (sourcePtr[0]._index != undefined) {
		sourcecnt = sourcePtr[0]._index.length;
	} else {
		sourcecnt = 0;
	}
	for(var j = 0; j < sourcecnt; j++){
		theDataPtr.addNewRecord(
			mb1 = sourcePtr[0]._index[j]._name,
			mb2 = sourcePtr[0]._index[j]._description,
			mb3 = '',//sourcePtr[0]._index[j]._idle_timeout,
			mb4 = sourcePtr[0]._index[j]._network_extension,
			mb5 = sourcePtr[0]._index[j]._ip_pool,
			mb6 = sourcePtr[0]._index[j]._dns_server_1,
			mb7 = sourcePtr[0]._index[j]._dns_server_2,
			mb8 = '',//sourcePtr[0]._index[j]._dns_server_3,
			mb9 = sourcePtr[0]._index[j]._wins_server_1,
			mb10 = sourcePtr[0]._index[j]._wins_server_2,
			mb11 = sourcePtr[0]._index[j]._reference_count,
			mb12 = sourcePtr[0]._index[j]._active
		);

		bufPtr.addNewRecord("");
		
		/*get User list */
		var n = 2;		
		var bindCnt = "";
		theDataPtr.dataSet[j].objMb1.dataSet.splice(0, theDataPtr.dataSet[j].objMb1.dataCount());
		bufPtr.dataSet[j].objMb1.dataSet.splice(0, bufPtr.dataSet[j].objMb1.dataCount());
		while(sourcePtr[0]._index[j]['_user'+bindCnt] != undefined) {
			theDataPtr.dataSet[j].objMb1.addNewRecord(sourcePtr[0]._index[j]['_user'+bindCnt]);
			bufPtr.dataSet[j].objMb1.addNewRecord(sourcePtr[0]._index[j]['_user'+bindCnt]);
			bindCnt = "_"+n;
			n++;
		}
		/*get application list */
		var n = 2;		
		var bindCnt = "";
		theDataPtr.dataSet[j].objMb2.dataSet.splice(0, theDataPtr.dataSet[j].objMb2.dataCount());
		bufPtr.dataSet[j].objMb2.dataSet.splice(0, bufPtr.dataSet[j].objMb2.dataCount());
		while(sourcePtr[0]._index[j]['_ssl_applicaiton'+bindCnt] != undefined) {
			theDataPtr.dataSet[j].objMb2.addNewRecord(sourcePtr[0]._index[j]['_ssl_applicaiton'+bindCnt]);
			bufPtr.dataSet[j].objMb2.addNewRecord(sourcePtr[0]._index[j]['_ssl_applicaiton'+bindCnt]);
			bindCnt = "_"+n;
			n++;
		}
		/*get network list */
		var n = 2;		
		var bindCnt = "";
		theDataPtr.dataSet[j].objMb3.dataSet.splice(0, theDataPtr.dataSet[j].objMb3.dataCount());
		bufPtr.dataSet[j].objMb3.dataSet.splice(0, bufPtr.dataSet[j].objMb3.dataCount());
		while(sourcePtr[0]._index[j]['_network'+bindCnt] != undefined) {
			theDataPtr.dataSet[j].objMb3.addNewRecord(sourcePtr[0]._index[j]['_network'+bindCnt]);
			bufPtr.dataSet[j].objMb3.addNewRecord(sourcePtr[0]._index[j]['_network'+bindCnt]);
			bindCnt = "_"+n;
			n++;
		}
	}
	
}
/* ZyWALL USG 300 */
function sslvpnmonitor_fetch(start_index){
	var sourcePtr, sourcecnt;
	var theDataPtr = gl.globalList; 
	sourcePtr = top.frames[4]['result_'+start_index]; 
	theDataPtr.dataSet.splice(0, theDataPtr.dataCount());
	if (sourcePtr[0]._User != undefined) {
		sourcecnt = sourcePtr[0]._User.length;
	} else {
		sourcecnt = 0;
	}
	for(var j = 0; j < sourcecnt; j++){
		theDataPtr.addNewRecord(
			mb1 = sourcePtr[0]._User[j].__name,
			mb2 = sourcePtr[0]._User[j]._Access,
			mb3 = sourcePtr[0]._User[j]._Login_Addr,
			mb4 = sourcePtr[0]._User[j]._Connected_Time,
			mb5 = sourcePtr[0]._User[j]._In,
			mb6 = sourcePtr[0]._User[j]._Vpnid,
			mb7 = sourcePtr[0]._User[j]._Out
		);
	}
	
}
/* ZyWALL USG 300 */
function sslvpnGlobal_fetch(start_index){
	var theDataPtr = gl.globalList; 
	var srcPtr_1 = top.frames[4]['result_'+start_index]; 
	var srcPtr_2 = top.frames[4]['result_'+(start_index+1)]; 
	var srcPtr_3 = top.frames[4]['result_'+(start_index+2)]; 
	theDataPtr.dataSet.splice(0, theDataPtr.dataCount());
	theDataPtr.addNewRecord(
		mb1 = if_undefined(srcPtr_1[0]._SSL_Network_Extension_Local_IP),
		mb2 = srcPtr_2[0]._Login_Message,
		mb3 = srcPtr_3[0]._Logout_Message
	);
}
/* ZyWALL USG 300 */
function application_fetch(start_index){
	var sourcePtr, sourcecnt;
	var theDataPtr = gl.BufferObj;	
	sourcePtr = top.frames[4]['result_'+start_index]; 
	theDataPtr.dataSet.splice(0, theDataPtr.dataCount());
	if (sourcePtr[0]._SSL_Application != undefined) {
		sourcecnt = sourcePtr[0]._SSL_Application.length;
	} else {
		sourcecnt = 0;
	}
	for(var j = 0; j < sourcecnt; j++){		
		if(sourcePtr[0]._SSL_Application[j]._Server_Type == 'none'){//none
			theDataPtr.addNewRecord(
				mb1 = sourcePtr[0]._SSL_Application[j].__name,
				mb2 = sourcePtr[0]._SSL_Application[j]._Server_Type,
				mb3 = '',
				mb4 = '',
				mb5 = sourcePtr[0]._SSL_Application[j]._Web_Page_Encryption,
				mb6 = ''
			);
		}else if(sourcePtr[0]._SSL_Application[j]._Server_Type == 'file-sharing'){//File sharing
			theDataPtr.addNewRecord(
				mb1 = sourcePtr[0]._SSL_Application[j].__name,
				mb2 = sourcePtr[0]._SSL_Application[j]._Server_Type,
				mb3 = sourcePtr[0]._SSL_Application[j]._Share_Path	,
				mb4 = '',
				mb5 = sourcePtr[0]._SSL_Application[j]._Web_Page_Encryption,
				mb6 = sourcePtr[0]._SSL_Application[j]._Entry_Point
			);
		}else {//Web application, internet-mail, owa
			theDataPtr.addNewRecord(
				mb1 = sourcePtr[0]._SSL_Application[j].__name,
				mb2 = sourcePtr[0]._SSL_Application[j]._Server_Type,
				mb3 = sourcePtr[0]._SSL_Application[j]._URL,
				mb4 = sourcePtr[0]._SSL_Application[j]._Encrypted_URL,
				mb5 = sourcePtr[0]._SSL_Application[j]._Web_Page_Encryption,
				mb6 = sourcePtr[0]._SSL_Application[j]._Entry_Point
			);
		}
	}
}
