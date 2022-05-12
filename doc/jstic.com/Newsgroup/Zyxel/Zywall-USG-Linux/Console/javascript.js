var fr = 1;//frames 		
var fsyName;
var CopyRename=0;//copy=1, rename=2
var MaxCLI =1000;
var DataTemp;
var fromTo = 0;
var GdataPtr = new objFileList();
//var theDptr = top.frames[fr].dptr; // get CLI pointer


var ns, ie;
var Vmajor = parseInt(navigator.appVersion);
if(navigator.appName == "Netscape") {
	ns = true; ie = false;
} else if(navigator.appName == "Microsoft Internet Explorer") {
	ie = true; ns = false;
} else {
	 ns = false; ie = false;
}
/*
alert(top.frames[0].name);//banner
alert(top.frames[1].name);//panel
alert(top.frames[2].name);//main
alert(top.frames[2].frames[0].name);//path
alert(top.frames[2].frames[1].name);//[main], name is empty
alert(top.frames[2].frames[2].name);//status
alert(top.frames[3].name);//Post CLI command and retriver data 
alert(top.frames[4].name);//Mapping date pointer
*/
/*========= Help ===========*/
function OpenHelp(context, topic){
	var str;
	if((context=='')&&(topic=='')){
		str = '';
	}else{
		str = '?context='+context+'&topic='+topic;		
	}
	if((top.frames[1].subHelp == undefined)||(top.frames[1].subHelp.closed)){
		top.frames[1].subHelp = window.open('helps/index.html'+str, 'helps', 'width=800,height=600, status=no, scrollbars=yes, resizable=yes');
	}else{
		top.frames[1].subHelp.location.href = 'helps/index.html'+str;
	}
	//alert(str);
	
}
/*========= Help ===========*/
/*
function mouseDown(e) {
	if(top.frames[1].zyshdcnt == 0){
		//alert ('Mouse-click is disabled for this page, getting data from device now.');
		//return false;		
	}
	if (parseInt(navigator.appVersion)>3) {
		var clickType=1;
		if (navigator.appName=="Netscape") clickType=e.which;
		else clickType=event.button;
		if (clickType==1) {			
			self.status='Left button.';
			page_init();			
		}else if (clickType!=1) {
			self.status='Right button.';
		}
	}
	return true;
}
if (parseInt(navigator.appVersion)>3) {
	document.onmousedown = mouseDown;
	if (navigator.appName=="Netscape") 
	document.captureEvents(Event.mouseDown);
}
*/
function objVOIPalgList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(mb1, mb2, mb3, mb4, mb5, mb6, mb7, mb8, mb9, mb10, mb11){		
		var data=new objVOIPalgRecord(mb1, mb2, mb3, mb4, mb5, mb6, mb7, mb8, mb9, mb10, mb11);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx,mb1, mb2, mb3, mb4, mb5, mb6, mb7, mb8, mb9, mb10, mb11){
		var data=new objVOIPalgRecord(mb1, mb2, mb3, mb4, mb5, mb6, mb7, mb8, mb9, mb10, mb11);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx,mb1, mb2, mb3, mb4, mb5, mb6, mb7, mb8, mb9, mb10, mb11){
		var data=new objVOIPalgRecord(mb1, mb2, mb3, mb4, mb5, mb6, mb7, mb8, mb9, mb10, mb11);
		this.dataSet.splice(idx,0,data);
		var olddata = this.dataSet.pop();
		this.dataSet.splice(idx,0,olddata);		
	}
	this.deleteRecord = function(idx){				
		return this.dataSet.splice(idx,1); 
	}
	this.dataCount = function(){		
		return this.dataSet.length;
	}
}

function  objVOIPalgRecord(mb1, mb2, mb3, mb4, mb5, mb6, mb7, mb8, mb9, mb10, mb11){
	this.sipact 	= mb1;
	this.stimeout = mb2;
	this.mtimeout = mb3;
	this.sport 		= mb4;
	this.port 		= mb5;
	this.h323act 	= mb6;
	this.h323sport= mb7;
	this.h323port	= mb8;
	this.ftpact 	= mb9;
	this.ftpsport = mb10;
	this.ftpport	= mb11;
}
function objForceUserList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(name, schedule, source, destination, authentication, active, description){		
		var data=new objForceUserRecord(name, schedule, source, destination, authentication, active, description);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, name, schedule, source, destination, authentication, active, description){
		var data=new objForceUserRecord(name, schedule, source, destination, authentication, active, description);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, name, schedule, source, destination, authentication, active, description){
		var data=new objForceUserRecord(name, schedule, source, destination, authentication, active, description);
		this.dataSet.splice(idx,0,data);
		var olddata = this.dataSet.pop();
		this.dataSet.splice(idx,0,olddata);		
	}
	this.deleteRecord = function(idx){				
		return this.dataSet.splice(idx,1); 
	}
	this.dataCount = function(){		
		return this.dataSet.length;
	}
}

function  objForceUserRecord(name, schedule, source, destination, authentication, active, description){
	this.name = name; //rule number
	this.schedule = schedule;		
	this.source = source;
	this.destination = destination;
	this.authentication = authentication;
	this.active = active;	
	this.description = description;	
}
function objHTTPRedList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(name,iface, proxserv, port, isactive){		
		var data=new objHTTPRedRecord(name,iface, proxserv, port, isactive);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx,name, iface, proxserv, port, isactive){
		var data=new objHTTPRedRecord(name,iface, proxserv, port, isactive);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx,name,iface, proxserv, port, isactive){
		var data=new objHTTPRedRecord(name,iface, proxserv, port, isactive);
		this.dataSet.splice(idx,0,data);
		var olddata = this.dataSet.pop();
		this.dataSet.splice(idx,0,olddata);		
	}
	this.deleteRecord = function(idx){				
		return this.dataSet.splice(idx,1); 
	}
	this.dataCount = function(){		
		return this.dataSet.length;
	}	
}

function  objHTTPRedRecord(name,iface, proxserv, port, isactive){
	this.name = name;
	this.iface = iface;
	this.proxserv = proxserv;
	this.port = port;
	this.isactive = isactive;
}

function objRegList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(enable, name, passwd, exist){/* registration account*/ 
				             //   cfreg, cfregType, cfregExpir, /* content-filter*/
				             //    idpreg, idpregType, idpregExpir /* IDP/AV/AS */){		
		var data=new objRegRecord(enable, name, passwd, exist);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, enable, name, passwd, exist){
		var data=new objRegRecord(enable, name, passwd, exist);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, enable, name, passwd, exist){
		var data=new objRegRecord(enable, name, passwd, exist);
		this.dataSet.splice(idx,0,data);
		var olddata = this.dataSet.pop();
		this.dataSet.splice(idx,0,olddata);		
	}
	this.deleteRecord = function(idx){				
		return this.dataSet.splice(idx,1); 
	}
	this.dataCount = function(){		
		return this.dataSet.length;
	}		
}
function objRegRecord(enable, name, passwd, exist){
	/* registration account*/
	this.enable = enable;
	this.name = name;
	this.passwd = passwd;
	this.exist = exist;	
	this.regserv = new objRegServ(); //for register service management
}

function objRegServ()
{	
	this.bindSet = new Array();	
	this.addNewRecord = function(Reg, TYPE, Expir){		
		var data=new objRegServRecord(Reg, TYPE, Expir);
		this.bindSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.bindSet[Idx]; 				
	}	
	this.insertRecord = function(idx, Reg, TYPE, Expir){
		var data=new objRegServRecord(Reg, TYPE, Expir);
		this.bindSet.splice(idx,0,data);
	}	
	this.insertafterRecord = function(idx, Reg, TYPE, Expir){
		var data=new objRegServRecord(Reg, TYPE, Expir);
		this.bindSet.splice(idx,0,data);
		var olddata = this.bindSet.pop();
		this.bindSet.splice(idx,0,olddata);		
	}
	this.deleteRecord = function(idx){				
		return this.bindSet.splice(idx,1); 
	}
	this.dataCount = function(){		
		return this.bindSet.length;
	}		
	
}
function objRegServRecord(Reg, TYPE, Expir){
	this.reg = Reg;
	this.type = TYPE;
	this.expir = Expir;
}

function objProtocolList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(Name,  Active){		
		var data=new objProtolRecord(Name, Active);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.editRecord = function(idx, Name, Active){
		this.dataSet[idx].name = Name;
		this.dataSet[idx].active = Active;
	}
	this.insertRecord = function(idx, Name, Active){
		var data=new objProtolRecord(Name, Active);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, Name, Active){
		var data=new objProtolRecord(Name, Active);
		this.dataSet.splice(idx,0,data);
		var olddata = this.dataSet.pop();
		this.dataSet.splice(idx,0,olddata);		
	}
	this.deleteRecord = function(idx){				
		return this.dataSet.splice(idx,1); 
	}
	this.dataCount = function(){		
		return this.dataSet.length;
	}		
}
function objProtolRecord(Name, Active){
	this.name = Name;
	this.protocol = new objPolList();
	this.active = Active;	
}
function objPolList()
{	
	this.bindSet = new Array();	
	this.addNewRecord = function(Member, Mode, Weight){		
		var data=new objPolRecord(Member, Mode, Weight);
		this.bindSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.bindSet[Idx]; 				
	}	
	this.insertRecord = function(idx, Member, Mode, Weight){
		var data=new objPolRecord(Member, Mode, Weight);
		this.bindSet.splice(idx,0,data);
	}	
	this.insertafterRecord = function(idx, Member, Mode, Weight){
		var data=new objPolRecord(Member, Mode, Weight);
		this.bindSet.splice(idx,0,data);
		var olddata = this.bindSet.pop();
		this.bindSet.splice(idx,0,olddata);		
	}
	this.deleteRecord = function(idx){				
		return this.bindSet.splice(idx,1); 
	}
	this.dataCount = function(){		
		return this.bindSet.length;
	}		
	
}
function objPolRecord(Member, Mode, Weight){
	this.member = Member;
	this.mode = Mode;
	this.weight = Weight;
}

function objScheList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(Name, Inverse, Active){		
		var data=new objScheRecord(Name, Inverse, Active);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, Name, Inverse, Active){
		var data=new objScheRecord(Name, Inverse, Active);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, Name, Inverse, Active){
		var data=new objScheRecord(Name, Inverse, Active);
		this.dataSet.splice(idx,0,data);
		var olddata = this.dataSet.pop();
		this.dataSet.splice(idx,0,olddata);		
	}
	this.deleteRecord = function(idx){				
		return this.dataSet.splice(idx,1); 
	}
	this.dataCount = function(){		
		return this.dataSet.length;
	}		
}
function objScheRecord(Name, Inverse, Active){
	this.name = Name;
	this.inverse = Inverse;
	this.time = new objTimeList();
	this.active = Active;	
}

function objTimeList()
{	
	this.bindSet = new Array();	
	this.addNewRecord = function(Member, Mode, Weight){		
		var data=new objTimeRecord(Member, Mode, Weight);
		this.bindSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.bindSet[Idx]; 				
	}	
	this.insertRecord = function(idx, Member, Mode, Weight){
		var data=new objTimeRecord(Member, Mode, Weight);
		this.bindSet.splice(idx,0,data);
	}	
	this.insertafterRecord = function(idx, Member, Mode, Weight){
		var data=new objTimeRecord(Member, Mode, Weight);
		this.bindSet.splice(idx,0,data);
		var olddata = this.bindSet.pop();
		this.bindSet.splice(idx,0,olddata);		
	}
	this.deleteRecord = function(idx){				
		return this.bindSet.splice(idx,1); 
	}
	this.dataCount = function(){		
		return this.bindSet.length;
	}		
	
}
function objTimeRecord(Member, Mode, Weight){
	this.member = Member;
	this.mode = Mode;
	this.weight = Weight;
}

function objAddrList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(Name, Active){		
		var data=new objAddrRecord(Name, Active);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, Name, Active){
		var data=new objAddrRecord(Name, Active);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, Name, Active){
		var data=new objAddrRecord(Name, Active);
		this.dataSet.splice(idx,0,data);
		var olddata = this.dataSet.pop();
		this.dataSet.splice(idx,0,olddata);		
	}
	this.deleteRecord = function(idx){				
		return this.dataSet.splice(idx,1); 
	}
	this.dataCount = function(){		
		return this.dataSet.length;
	}		
}
function objAddrRecord(Name, Active){
	this.name = Name;
	this.ipaddr = new objIPAddrList;
	this.active = Active;
}

function objIPAddrList()
{	
	this.bindSet = new Array();	
	this.addNewRecord = function(Member, Mode, Weight){		
		var data=new objIPAddrRecord(Member, Mode, Weight);
		this.bindSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.bindSet[Idx]; 				
	}	
	this.insertRecord = function(idx, Member, Mode, Weight){
		var data=new objIPAddrRecord(Member, Mode, Weight);
		this.bindSet.splice(idx,0,data);
	}	
	this.insertafterRecord = function(idx, Member, Mode, Weight){
		var data=new objIPAddrRecord(Member, Mode, Weight);
		this.bindSet.splice(idx,0,data);
		var olddata = this.bindSet.pop();
		this.bindSet.splice(idx,0,olddata);		
	}
	this.deleteRecord = function(idx){				
		return this.bindSet.splice(idx,1); 
	}
	this.dataCount = function(){		
		return this.bindSet.length;
	}		
	
}
function objIPAddrRecord(Member, Mode, Weight){
	this.member = Member;
	this.mode = Mode;
	this.weight = Weight;
}

function objFileList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(FileName, Size, Created, Modified){		
		var data=new objFileRecord(FileName, Size, Created, Modified);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx];	
	}	
	this.insertRecord = function(idx, FileName, Size, Created, Modified){
		var data=new objFileRecord(FileName, Size, Created, Modified);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, FileName, Size, Created, Modified){
		var data=new objFileRecord(FileName, Size, Created, Modified);
		this.dataSet.splice(idx,0,data);
		var olddata = this.dataSet.pop();
		this.dataSet.splice(idx,0,olddata);		
	}
	this.deleteRecord = function(idx){				
		return this.dataSet.splice(idx,1); 
	}
	this.dataCount = function(){		
		return this.dataSet.length;
	}		
}
function objFileRecord(FileName, Size, Created, Modified){
	this.filename = FileName;
	this.size = Size;
	this.created = Created;
	this.modified = Modified;	
}

function objRmMagList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(enable, svrport, cltauth, cltcert, svrcert,frcredir, /*www, ssh, telnet, ftp*/
						getcom, setcom, trapcom, trapdest,/*snmp*/
						regstatus, regtime, svraddr, encapalgor, encapkey){		
		var data=new objRmMagRecord(enable, svrport, cltauth, cltcert, svrcert,frcredir, /*www, ssh, telnet, ftp*/
						getcom, setcom, trapcom, trapdest,/*snmp*/
						regstatus, regtime, svraddr, encapalgor, encapkey);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, enable, svrport, cltauth, cltcert, svrcert,frcredir, /*www, ssh, telnet, ftp*/
						getcom, setcom, trapcom, trapdest,/*snmp*/
						regstatus, regtime, svraddr, encapalgor, encapkey){
		var data=new objRmMagRecord(enable, svrport, cltauth, cltcert, svrcert,frcredir, /*www, ssh, telnet, ftp*/
						getcom, setcom, trapcom, trapdest,/*snmp*/
						regstatus, regtime, svraddr, encapalgor, encapkey);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, enable, svrport, cltauth, cltcert, svrcert,frcredir, /*www, ssh, telnet, ftp*/
						getcom, setcom, trapcom, trapdest,/*snmp*/
						regstatus, regtime, svraddr, encapalgor, encapkey){
		var data=new objRmMagRecord(enable, svrport, cltauth, cltcert, svrcert,frcredir, /*www, ssh, telnet, ftp*/
						getcom, setcom, trapcom, trapdest,/*snmp*/
						regstatus, regtime, svraddr, encapalgor, encapkey);
		this.dataSet.splice(idx,0,data);
		var olddata = this.dataSet.pop();
		this.dataSet.splice(idx,0,olddata);		
	}
	this.deleteRecord = function(idx){				
		return this.dataSet.splice(idx,1); 
	}
	this.dataCount = function(){		
		return this.dataSet.length;
	}		
}
function objRmMagRecord(enable, svrport, cltauth, cltcert, svrcert,frcredir, /*www, ssh, telnet, ftp*/
						getcom, setcom, trapcom, trapdest,/*snmp*/
						regstatus, regtime, svraddr, encapalgor, encapkey){/*CNM*/
	this.enable = enable;
	this.svrport = svrport;
	this.cltauth = cltauth;
	this.cltcert = cltcert;
	this.svrcert = svrcert;	
	this.frcredir = frcredir; //force redirect
	/* SNMP */
	this.getcom = getcom;
	this.setcom = setcom;
	this.trapcom = trapcom;
	this.trapdest = trapdest;
	/* CNM */
	this.regstatus = regstatus;
	this.regtime = regtime;
	this.svraddr = svraddr;
	this.encapalgor = encapalgor;
	this.encapkey = encapkey;
	
	this.svrctrl = new objSvrCtrlList();	
	this.svrctrl2 = new objSvrCtrlList();	
}
function objSvrCtrlList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(From, Addr,  Action, Type){		//Type is used for "ICMP type"
		var data=new objSvrCtrlRecord(From, Addr,  Action, Type);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, From, Addr, Action, Type){
		var data=new objSvrCtrlRecord(From, Addr, Action, Type);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, From, Addr, Action, Type){
		var data=new objSvrCtrlRecord(From, Addr, Action, Type);
		this.dataSet.splice(idx,0,data);
		var olddata = this.dataSet.pop();
		this.dataSet.splice(idx,0,olddata);		
	}
	this.deleteRecord = function(idx){				
		return this.dataSet.splice(idx,1); 
	}
	this.dataCount = function(){		
		return this.dataSet.length;
	}		
}
function objSvrCtrlRecord(From, Addr, Action, Type){	
	this.from = From;
	this.address = Addr;
	this.action = Action;
	this.type = Type;
}
/*function objSvrCtrlList()
{
	this.dataSet = new Array();
	this.addNewRecord = function( Cnt, From, Addr, Action){		
		var data=new objSvrCtrlRecord( Cnt, From, Addr, Action);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, Cnt, From, Addr, Action){
		var data=new objSvrCtrlRecord(Cnt, From, Addr, Action);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, Cnt, From, Addr, Action){
		var data=new objSvrCtrlRecord(Cnt, From, Addr, Action);
		this.dataSet.splice(idx,0,data);
		var olddata = this.dataSet.pop();
		this.dataSet.splice(idx,0,olddata);		
	}
	this.deleteRecord = function(idx){				
		return this.dataSet.splice(idx,1); 
	}
	this.dataCount = function(){		
		return this.dataSet.length;
	}		
}
function objSvrCtrlRecord(Cnt, From, Addr, Action){	
	this.cnt = Cnt;
	this.from = From;
	this.address = Addr;
	this.action = Action;
}
*/
function objDNSList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(Hsp, Hsipflag, Hsipaddr, Wsp, Wsipflag, Wsipaddr){		
		var data=new objDNSRecord(Hsp, Hsipflag, Hsipaddr, Wsp, Wsipflag, Wsipaddr);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, Hsp, Hsipflag, Hsipaddr, Wsp, Wsipflag, Wsipaddr){
		var data=new objDNSRecord(ScHsp, Hsipflag, Hsipaddr, Wsp, Wsipflag, Wsipaddr);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, Hsp, Hsipflag, Hsipaddr, Wsp, Wsipflag, Wsipaddr){
		var data=new objDNSRecord(Hsp, Hsipflag, Hsipaddr, Wsp, Wsipflag, Wsipaddr);
		this.dataSet.splice(idx,0,data);
		var olddata = this.dataSet.pop();
		this.dataSet.splice(idx,0,olddata);		
	}
	this.deleteRecord = function(idx){				
		return this.dataSet.splice(idx,1); 
	}
	this.dataCount = function(){		
		return this.dataSet.length;
	}		
}
function objDNSRecord(Hsp, Hsipflag, Hsipaddr, Wsp, Wsipflag, Wsipaddr){
	this.hsp = Hsp;
	this.hsipflag = Hsipflag;
	this.hsipaddr = Hsipaddr;
	this.wsp = Wsp;
	this.wsipflag = Wsipflag;
	this.wsipaddr = Wsipaddr;	
}

function objRouteList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(User, Ininterface, Source, Destination, Service, Nexthop, Snat, Bwm, 
						Protocol, Userdefine, Srcport, Destport, Type, Gateway, Hudefine, Vpntunel, 
						Trunk, Active, schedule, intunnel, sslvpn, mb1){		
		var data=new objRouteRecord(User, Ininterface, Source, Destination, Service, Nexthop, Snat, Bwm, 
						Protocol, Userdefine, Srcport, Destport, Type, Gateway, Hudefine, Vpntunel, 
						Trunk, Active, schedule, intunnel, sslvpn, mb1);
		this.dataSet.push(data); 
		//alert("dataSet="+this.dataSet[0]);
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, User, Ininterface, Source, Destination, Service, Nexthop, Snat, Bwm, 
						Protocol, Userdefine, Srcport, Destport, Type, Gateway, Hudefine, Vpntunel, 
						Trunk, Active, schedule, intunnel, sslvpn, mb1){
		var data=new objRouteRecord(User, Ininterface, Source, Destination, Service, Nexthop, Snat, Bwm, 
						Protocol, Userdefine, Srcport, Destport, Type, Gateway, Hudefine, Vpntunel, 
						Trunk, Active, schedule, intunnel, sslvpn, mb1);
		this.dataSet.splice(idx,0,data);
		//alert(this.dataSet);
	}
	this.insertafterRecord = function(idx, User, Ininterface, Source, Destination, Service, Nexthop, Snat, Bwm, 
						Protocol, Userdefine, Srcport, Destport, Type, Gateway, Hudefine, Vpntunel, 
						Trunk, Active, schedule, intunnel, sslvpn, mb1){
		var data=new objRouteRecord(User, Ininterface, Source, Destination, Service, Nexthop, Snat, Bwm, 
						Protocol, Userdefine, Srcport, Destport, Type, Gateway, Hudefine, Vpntunel, 
						Trunk, Active, schedule, intunnel, sslvpn, mb1);
		this.dataSet.splice(idx,0,data);
		var olddata = this.dataSet.pop();
		this.dataSet.splice(idx,0,olddata);		
	}
	this.deleteRecord = function(idx){				
		return this.dataSet.splice(idx,1); 
	}
	this.dataCount = function(){		
		return this.dataSet.length;
	}		
}
function objRouteRecord(User, Ininterface, Source, Destination, Service, Nexthop, Snat, Bwm, 
						Protocol, Userdefine, Srcport, Destport, Type, Gateway, Hudefine, Vpntunel, 
						Trunk, Active, schedule, intunnel, sslvpn, mb1){
	this.user = User;
	this.ininterface = Ininterface;
	this.source = Source;
	this.destination = Destination;
	this.service = Service;
	this.nexthop = Nexthop;
	this.snat = Snat;
	this.bwm = Bwm;	
	this.protocol = Protocol;	
	this.userdefine = Userdefine;
	this.srcport = Srcport;	
	this.destport = Destport;
	this.type = Type;	
	this.gateway = Gateway;
	this.hudefine = Hudefine;	
	this.vpntunel = Vpntunel;
	this.trunk = Trunk;
	this.active =Active;
	this.schedule = schedule;
	this.intunnel = intunnel;
	this.sslvpn = sslvpn;
	this.mb1 = mb1;
	this.trigger = new objTriggerList();
}
function objTriggerList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(incomingsvr, triggersvr){		
		var data=new objTriggerRecord(incomingsvr, triggersvr);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, incomingsvr, triggersvr){
		var data=new objTriggerRecord(incomingsvr, triggersvr);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, incomingsvr, triggersvr){
		var data=new objTriggerRecord(incomingsvr, triggersvr);
		this.dataSet.splice(idx,0,data);
		var olddata = this.dataSet.pop();
		this.dataSet.splice(idx,0,olddata);		
	}
	this.deleteRecord = function(idx){				
		return this.dataSet.splice(idx,1); 
	}
	this.dataCount = function(){		
		return this.dataSet.length;
	}		
}
function objTriggerRecord(incomingsvr, triggersvr){
	this.incomingsvr = incomingsvr;
	this.triggersvr = triggersvr;
}
function objPFilterList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(Maddr, Action, Active){		
		var data=new objPFilterRecord(Maddr, Action, Active);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, Maddr, Action, Active){
		var data=new objPFilterRecord(Maddr, Action, Active);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, Maddr, Action, Active){
		var data=new objPFilterRecord(Maddr, Action, Active);
		this.dataSet.splice(idx,0,data);
		var olddata = this.dataSet.pop();
		this.dataSet.splice(idx,0,olddata);		
	}
	this.deleteRecord = function(idx){				
		return this.dataSet.splice(idx,1); 
	}
	this.dataCount = function(){		
		return this.dataSet.length;
	}		
}
function objPFilterRecord(Maddr, Action, Active){
	this.macaddr = Maddr;
	this.action = Action;
	this.active = Active;	
}
function objConfigEtherList(){
	var ret_code;
	this.dataSet = new Array();
	this.addNewRecord = function(Active, Name, selNum, ip, gwip, subnetmask, 
						ifport, vlantag, vlancomm, upstream, downstream, mtu, ripdir, MultiEnable, MultiVer,
						area, linkcost, passive, auth, sak, mad, mak,
						dhcpflag, dhcpserver, dhcprelay1, dhcprelay2, type, orig_active, metric, RecvVer,eRIP,priOSPF)
	{
	var data=new objConfigEtherRecord(Active, Name, selNum, ip, gwip, subnetmask, 
						ifport, vlantag, vlancomm, upstream, downstream, mtu, ripdir, MultiEnable, MultiVer,
						area, linkcost, passive, auth, sak, mad, mak,
						dhcpflag, dhcpserver, dhcprelay1, dhcprelay2, type, orig_active, metric, RecvVer,eRIP,priOSPF);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}		
	this.insertRecord = function(idx, Active, Name, selNum, ip, gwip, subnetmask, 
						ifport, vlantag, vlancomm, upstream, downstream, mtu, ripdir, MultiEnable, MultiVer,
						area, linkcost, passive, auth, sak, mad, mak,
						dhcpflag, dhcpserver, dhcprelay1, dhcprelay2, type, orig_active, metric, RecvVer,eRIP,priOSPF)
	{	var data=new objConfigEtherRecord(Active, Name, selNum, ip, gwip, subnetmask, 
						ifport, vlantag, vlancomm, upstream, downstream, mtu, ripdir, MultiEnable, MultiVer,
						area, linkcost, passive, auth, sak, mad, mak,
						dhcpflag, dhcpserver, dhcprelay1, dhcprelay2, type, orig_active, metric, RecvVer,eRIP,priOSPF);
		this.dataSet.splice(idx,0,data);		
	}
	this.insertafterRecord = function(idx, Active, Name, selNum, ip, gwip, subnetmask, 
						ifport, vlantag, vlancomm, upstream, downstream, mtu, ripdir, MultiEnable, MultiVer,
						area, linkcost, passive, auth, sak, mad, mak,
						dhcpflag, dhcpserver, dhcprelay1, dhcprelay2, type, orig_active, metric, RecvVer,eRIP,priOSPF)
	{	var data=new objConfigEtherRecord(Active, Name, selNum, ip, gwip, subnetmask, 
						ifport, vlantag, vlancomm, upstream, downstream, mtu, ripdir, MultiEnable, MultiVer,
						area, linkcost, passive, auth, sak, mad, mak,
						dhcpflag, dhcpserver, dhcprelay1, dhcprelay2, type, orig_active, metric, RecvVer,eRIP,priOSPF);				
		this.dataSet.splice(idx,0,data);
		var olddata = this.dataSet.pop();
		this.dataSet.splice(idx,0,olddata);		
	}
	this.deleteRecord = function(idx){				
		return this.dataSet.splice(idx,1); 
	}
	this.dataCount = function(){		
		return this.dataSet.length;
	}		
}
		
function objConfigEtherRecord(Active, Name, selNum, ip, gwip, subnetmask, 
	ifport, vlantag, vlancomm, upstream, downstream, mtu, ripdir, MultiEnable, MultiVer,
	area, linkcost, passive, auth, sak, mad, mak,
	dhcpflag, dhcpserver, dhcprelay1, dhcprelay2, type, orig_active, metric, RecvVer,eRIP,priOSPF)
{
	this.active = Active;	
	this.name = Name;
	this.selNum = selNum;	
	this.IP = new objIP(ip);
	this.GWIP = new objGateway(gwip);
	this.SubnetMask = new objSubnetMask(subnetmask);				
	//
	this.ifport = ifport;
	this.VlanTag = vlantag;
	this.VlanComm = vlancomm;
	//
	this.Upstream = upstream;
	this.Downstream = downstream;
	this.MTU = mtu;
	//
	this.ripDir = ripdir;
	this.MultiEnable = MultiEnable;
	this.MultiVer = MultiVer;
	this.RecvVer = RecvVer;
	//
	this.area = area;
	this.linkcost = linkcost;
	this.passive = passive;
	this.auth = auth;
	this.sak = sak;
	this.mad = mad;
	this.mak = mak;
	//dhcp binding
	this.dhcpflag = dhcpflag;
	this.dhcpserver = new objVLANList();
	this.dhcprelay1 = dhcprelay1;
	this.dhcprelay2 = dhcprelay2;

	this.type = type; 
	this.orig_active = orig_active;

	this.Metric = metric;
	
	this.eRIP = eRIP;
	this.priOSPF = priOSPF;
}

function objGroupsList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(iface, Attribute, LBAlgorithm, Timeframe, LBIndex, active, Description){		
		var data=new objGroupsRecord(iface, Attribute, LBAlgorithm, Timeframe, LBIndex, active, Description);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, iface, Attribute, LBAlgorithm, Timeframe, LBIndex, active, Description){
		var data=new objGroupsRecord(iface, Attribute, LBAlgorithm, Timeframe, LBIndex, active, Description);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, iface, Attribute, LBAlgorithm, Timeframe, LBIndex, active, Description){
		var data=new objGroupsRecord(iface, Attribute, LBAlgorithm, Timeframe, LBIndex, active, Description);
		this.dataSet.splice(idx,0,data);
		var olddata = this.dataSet.pop();
		this.dataSet.splice(idx,0,olddata);		
	}
	this.deleteRecord = function(idx){				
		return this.dataSet.splice(idx,1); 
	}
	this.dataCount = function(){		
		return this.dataSet.length;
	}		
}
function objGroupsRecord(iface, Attribute, LBAlgorithm, Timeframe, LBIndex, active, Description ){
	this.giface = iface;
	this.attribute = Attribute;
	this.lbalgorithm = LBAlgorithm;
	this.timeframe = Timeframe;
	this.lbindex = LBIndex;
	this.gbinding = new objBindList();
	this.gnormal = new objBindList();
	this.gactive = active;	
	this.description = Description ;
}
function objBindList()
{	
	this.bindSet = new Array();	
	this.addNewRecord = function(Member, Mode, Weight, Upstream, Downstream, Spillover){		
		var data=new objBindRecord(Member, Mode, Weight, Upstream, Downstream, Spillover);
		this.bindSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.bindSet[Idx]; 				
	}	
	this.insertRecord = function(idx, Member, Mode, Weight, Upstream, Downstream, Spillover){
		var data=new objBindRecord(Member, Mode, Weight, Upstream, Downstream, Spillover);
		this.bindSet.splice(idx,0,data);
	}	
	this.insertafterRecord = function(idx, Member, Mode, Weight, Upstream, Downstream, Spillover){
		var data=new objBindRecord(Member, Mode, Weight, Upstream, Downstream, Spillover);
		this.bindSet.splice(idx,0,data);
		var olddata = this.bindSet.pop();
		this.bindSet.splice(idx,0,olddata);		
	}
	this.deleteRecord = function(idx){				
		return this.bindSet.splice(idx,1); 
	}
	this.dataCount = function(){		
		return this.bindSet.length;
	}		
	
}
function objBindRecord(Member, Mode, Weight, Upstream, Downstream, Spillover){
	this.member = Member;
	this.mode = Mode;
	this.weight = Weight;
	this.upstream = Upstream;
	this.downstream = Downstream;
	this.spillover = Spillover;
}

function objIfaceList()
{	
	this.dataSet = new Array();	
	this.addNewRecord = function(Member, DownStream, UpStream, type){		
		var data=new objIfaceRecord(Member, DownStream, UpStream, type);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, Member, DownStream, UpStream, type){
		var data=new objIfaceRecord(Member, DownStream, UpStream, type);
		this.dataSet.splice(idx,0,data);
	}	
	this.insertafterRecord = function(idx, Member, DownStream, UpStream, type){
		var data=new objIfaceRecord(Member, DownStream, UpStream, type );
		this.dataSet.splice(idx,0,data);
		var olddata = this.dataSet.pop();
		this.dataSet.splice(idx,0,olddata);		
	}
	this.deleteRecord = function(idx){				
		return this.dataSet.splice(idx,1); 
	}
	this.dataCount = function(){		
		return this.dataSet.length;
	}		
	
}
function objIfaceRecord(Member, DownStream, UpStream, type ){
	this.name = Member;
	this.DownStream = DownStream;
	this.UpStream = UpStream;
	this.type = type;
}

	function addCLI(data)
	{
		this.CLI = new Array();		
		this.addCLI = function(data){
			this.CLI.push(data); 		
		}
		this.CLILength =function(){	
			if(this.CLI.length > MaxCLI){
				this.CLI.splice(MaxCLI,1); 
				return MaxCLI;
			}else{
				return	this.CLI.length;				
			}
		}
		this.getCLIbyIdx = function(idx){
			return this.CLI[idx];
		}
		this.deleteCLI = function(idx){				
			this.CLI.splice(idx,1); 		
		}		
	}
	function objIP(ip)
	{
		this.address = ip;
		this.verifyIP = chkIP;
	}
	function objGateway(gwip)
	{
		this.address = gwip;
		this.verifyGWIP = chkIP;
	}
	function objSubnetMask(subnetmask)
	{
		this.address = subnetmask;	
		this.verifySubnetMask = chkSubnetMask;
	}
	
	function chkNumber(bit){
		if(isNaN(bit)){
			alert("MAC format error ,MAC format=xx:xx:xx:xx:xx:xx");
			getEthReloadEntry();
			return false;
		}
		return true;
	}
	function chkNull(number, str){
		if(number==""){
			alert(str+"is empty.");
			return false;
		}
		return true;
	}
	
	function chkAuthKey(str, msg){
		var ret = true;
		var result = str.match(/^[\w\-]+$/);
		if(!str) {
			alert(msg+" is empty.");
			ret = false;
		}else if(!result){
			alert(msg+" is invalid.");
			ret = false;
		}
		return ret;
	}
	
	function chkRange(value, min, max, msg){
		if(value==null){
			alert(msg+" is empty");
			return false;
		}
		var result = value.toString().match(/^[0-9]+$/);
		if(!result) {
			alert(msg+" is not a number.");
			return false;
		}
		
		if((parseInt(value) < parseInt(min))||(parseInt(value) > parseInt(max))){
			alert("The "+msg+" should be between "+ min+" and "+max);
			return false;
		}
		return true;
	}
	
	function del_LeadZero(val) {
		var k = 0;
		var str = val.toString();
		while((str.charAt(k) == 0) && (k < str.length)) {
				k++;
		}
		
		if(k == str.length) { // all zero
			str = 0;
		}else {
			str = str.slice(k, str.length);
		}
		return str;
	}

	function chkURL(str, msg){
		var ret = true; 
		var result = str.match(/^(http):\/\/([\w-'&\(\)\+\/:=\?;!\*@$%\.\\~])*(#[\w'&\(\)\+\/:=\?;!\*@$%\.\\~]{1,255})?$/);
		var result2 = str.match(/^([\w-'&\(\)\+\/:=\?;!\*@$%\.\\~])*(#[\w'&\(\)\+\/:=\?;!\*@$%\.\\~]{1,255})?$/)
		if(!str) {
			alert(msg+" is empty");
			ret = false;
		}else if(!result && result2) {
			alert(msg+" is invalid.It must start with 'http://' .");
			ret = false;
		}else if(!result){
			alert(msg+" is invalid.");
			ret = false;
		}
		return ret;
	}
	
	function chkRedirectURL(str, msg){
		var ret = true; 
		var result = str.match(/^(http[s]{0,1}):\/\/([\w-'&\(\)\+\/:=\?;!\*@$%\.\\~])*(#[\w'&\(\)\+\/:=\?;!\*@$%\.\\~]{1,255})?$/);
		var result2 = str.match(/^([\w-'&\(\)\+\/:=\?;!\*@$%\.\\~])*(#[\w'&\(\)\+\/:=\?;!\*@$%\.\\~]{1,255})?$/)
		if(!str) {
			alert(msg+" is empty");
			ret = false;
		}else if(!result && result2) {
			alert(msg+" is invalid.It must start with 'http://' or 'https://' .");
			ret = false;
		}else if(!result){
			alert(msg+" is invalid.");
			ret = false;
		}
		return ret;
	}
	
	function chkWebFileURL(str, msg){ /* for newappobj.html and applicationedit.html */
	var ret = true; 
	var result = str.match(/^(http[s]{0,1}):\/\/([\w-'&\(\)\+\/:=\?;!\*@$%\.\\~])*(#[\w'&\(\)\+\/:=\?;!\*@$%\.\\~]{1,1023})?$/);
	var result2 = str.match(/^([\w-'&\(\)\+\/:=\?;!\*@$%\.\\~])*(#[\w'&\(\)\+\/:=\?;!\*@$%\.\\~]{1,1023})?$/)
	if(!str) {
		alert(msg+" is empty");
		ret = false;
	}else if(!result && result2) {
		alert(msg+" is invalid.It must start with 'http://' or 'https://' .");
		ret = false;
	}else if(!result){
		alert(msg+" is invalid.");
		ret = false;
	}
	return ret;
}
	function chkSubnetMask(address, fieldname) {
		if ( fieldname == undefined ) fieldname = "";
		var SNMask = address;
		var SNMaskRanhge=[255,254,252,248,240,224,192,128];
		var Valid=0;
		SNMask = SNMask.replace(/ /g, "");
		//address.value = SNMask;
		var SNMaskSplit = SNMask.split(".");
		if(SNMaskSplit.length != 4) {
			alert("Subnet Mask format error.");
			return false;
		}
		for(i = 3; i >= 0; i--){
			if(Valid==0){
				for(j=0;j<SNMaskRanhge.length;j++){
					if(parseInt(SNMaskSplit[i], 10) == SNMaskRanhge[j]) {
						Valid=1;
					}
				}
				if((Valid==0) && (parseInt(SNMaskSplit[i], 10) != 0)) break;
			}else{
				if(parseInt(SNMaskSplit[i], 10) != 255)  {
						Valid=0;
						break;
				}				
			}
			if(isNaN(SNMaskSplit[i])) {
				Valid=0;
				break;
			}
		}
		if(Valid==0){
			alert("Subnet Mask format error.");
			return false;					
		}
		return true;
	}
	
	function chkAuxiliaryString(str, msg) {
		var ret  = true;				
		var result = str.match(/^[\w\s'\(\)\+,\/:=!\*#@$%\.&-]+$/);//'
		if(!str) {
			alert(msg+" is empty.");
			ret = false;
		}else if(!result) {
			alert(msg+" is invalid.");
			ret = false;
		}
		return ret;
	}
	 
	function chkString(str, msg) {
		var ret  = true;								
		var result = str.match(/^[\w@$\.\/\-]+[\w#@$\.\/\-]+$/);
		if(!str) {
			alert(msg+" is empty.");
			ret = false;
		}else if(!result) {
			alert(msg+" is invalid.");
			ret = false;
		}
		return ret;
	}	

	function chkUsr(str, msg) { /* checking isp account username */
		var ret  = true;
		var result = str.match(/^[\w@$\.\/\-]+[\w#@:%$\.\/\-]+$/);
		if(!str) {
			alert(msg+" is empty.");
			ret = false;
		}else if(!result) {
			alert(msg+" is invalid.");
			ret = false;
		}
		return ret;
	}	
		
	function chkIPFQDN(str, msg) {
		if ( msg == undefined ) {
			msg = "";
		}
		var ret = true;
			
		if(!str) {
			alert(msg+" is empty.");
			ret = false;
		}else {
			var IPsplit = str.split(".");
			if(isNaN(IPsplit[IPsplit.length-1]) || IPsplit[IPsplit.length-1] == "") { //check FQDN 1) xx.yy 2) xx.yy.
//			var result = str.match(/^([a-z0-9](-*[a-z0-9]+)*\.)+([a-z]{2,}\.?)$/);
				var result = str.match(/^[a-zA-Z0-9-]+[\.a-zA-Z0-9-]{0,254}$/);
				if(!result) {
					alert(msg+" : Invalid FQDN.");
					ret = false;	
				}
				for(var i = 0; i < IPsplit.length; i++) {
					if(IPsplit[i].length > 63) {
						alert(msg+" : Invalid FQDN.");
						break;
					}
				}
			}else {//check IPV4
				var result = str.match(/^((([0-1]?[0-9]?[0-9])|(2[0-4][0-9])|(25[0-5]))\.){3}(([0-1]?[0-9]?[0-9])|(2[0-4][0-9])|(25[0-5]))$/);
				if(!result) {
					alert(msg+" : IP format error.");
					ret = false;
				}
			}
		}
		return ret;
	}

	function chkSysDomain(str, msg) { // For RD Fanny's system , (ns/mx)dns
		var result = str.match(/^[a-zA-Z0-9-]+[\.a-zA-Z0-9-]*$/);
		var ret = true;
		if(!str) {
			alert(msg+" is empty.");
			ret = false;
		}else if(!result) {
			alert(msg+" is invalid.");
			ret = false;
		}
		 return ret;
	}
	
	function chkDomain(str, msg) {
		var result = str.match(/^[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,4})$/);
		var ret = true;
		if(!str) {
			alert(msg+" is empty.");
			ret = false;
		}else if(!result) {
			alert(msg+" is invalid.");
			ret = false;
		}
		 return ret;
	}
	
	function chkIPHOST(str, msg) {
		if ( msg == undefined ) {
			msg = "";
		}
		var ret = true;
			
		if(!str) {
			alert(msg+" is empty.");
			ret = false;
		}else {
			var IPsplit = str.split(".");
			if(isNaN(IPsplit[IPsplit.length-1]) || IPsplit[IPsplit.length-1] == "") { //check FQDN 1) xx.yy 2) xx.yy.
				var result = str.match(/^[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,4})$/);
				if(!result) {
					alert(msg+" : Invalid FQDN.");
					ret = false;	
				}
			}else {//check IPV4
				var result = str.match(/^((([0-1]?[0-9]?[0-9])|(2[0-4][0-9])|(25[0-5]))\.){3}(([0-1]?[0-9]?[0-9])|(2[0-4][0-9])|(25[0-5]))$/);
				if(!result) {
					alert(msg+" : IP format error.");
					ret = false;
				}
			}
		}
		return ret;
	}
	
	function transURLencode(str) {
		if(str.indexOf("'", 0) != -1) {
			str = str.replace(/[']/g, "&#39;");//'
		}
		return str;
	}
	
	function chkVersion(){
		var ret = true;
		if(navigator.appName == "Netscape") {
			ret = false;
		}
		return ret;
	}
	function chkDesc(str, msg) { //description
		if ( msg == undefined ) {
			msg = "Description";
		}
		var result = str.match(/^[\w\s'\(\)\+,\/:=\?;!\*#@$%-]+$/);//'
		var ret = true;
		if(!result) {
			alert(msg+" is invalid.");
			ret = false;	
		}
		return ret;
	}
	
	function chkMailSubject(str, msg) { // for mail subject
		var result = str.match(/^[\w\s'\(\)\+,\/:=\?;!\.\*#@$%-]+$/);//'
		var ret = true;
		if(!result) {
			alert(msg+" is invalid.");
			ret = false;	
		}
		return ret;
	}
	
	function chkCAsrv(str, msg) { // for "certificate server address"
		if ( msg == undefined ) {
			msg = "CA server address";
		}
		var result = str.match(/^[\w'\(\)\+,\/:\.=\?;!\*#@$%-]+$/);//'
		var ret = true;
		if(!result) {
			alert(msg+" is invalid.");
			ret = false;	
		}
		return ret;
	}
	
	function chkVPNpasswd(str, msg, category) { //for "VPN Extended Authentication password"  or "Certificate key" or "pre-share key"
			if ( msg == undefined ) {
				msg = "Password";
			}
			if(category != undefined) {
				var result = str.match(/^[\w'\(\)\+,\/:=!\*^@$%-]{8,32}$/);//'
			}else {
				var result = str.match(/^[\w'\(\)\{\}\<\>^`\+\/:!\*#@&=$\\\.~%,\|;\-]+$/);//'
			}
			var ret = true;
			if(!str) {
				alert(msg+" is empty.");
				ret = false;
			}else if(!result) {
				if(category != undefined) {
					alert(msg+" is invalid. The minimum length is 8.");
				}else {
					alert(msg+" is invalid.");
				}
				ret = false;	
			}
			return ret;
	}
	
	function chkVPNPreshareKey(str, msg) { 
		var result = str.match(/^((0[xX][a-fA-F0-9]{16,64})|([\w'\(\)\{\}\<\>^`\+\/:!\*#@&=$\\\.~%,\|;\-]{8,32}))$/);//'
		var ret = true;
		if(!str) {
			alert(msg+" is empty.");
			ret = false;
		}else if(!result) {
			alert(msg+" is invalid.");
			ret = false;	
		}
		return ret;
	}
	
	function chkVPN_Domain(str, msg) { // for "VPN Domain name" or "certificate domain name"
			if ( msg == undefined ) {
				msg = "Domain Name";
			}
			var result = str.match(/^[\w\.-]+$/);
			var ret = true;
			if(!str) {
				alert(msg+" is empty.");
				ret = false;
			}else if(!result) {
				alert(msg+" is invalid.");
				ret = false;	
			}
			return ret;
	}
	
	function chkNotifyMsg(str, msg) { //for "AS Notification SMTP sender"  
		if ( msg == undefined ) {
			msg = "Notification message";
		}
		var result = str.match(/^[\w\s'\(\)\+,\/:=\?;!\*#@$%-]*$/);//'
		var ret = true;
		if(!result) {
			alert(msg+" is invalid.");
			ret = false;	
		}
		return ret;
	}
	
	function chkSpamMark(str, msg) { //for "AS Spam Mark"
		if ( msg == undefined ) {
			msg = "Mark on Subject";
		}
		var result = str.match(/^[\.\w:\[\]-]*$/);
		var ret = true;
		if(!result) {
			alert(msg+" is invalid.");
			ret = false;	
		}
		return ret;
	}
	
	function chkFQDN(str, msg) {
//	var result = str.match(/^([a-z0-9](-*[a-z0-9]+)*\.)+([a-z]{2,}\.?)$/);
		var result = str.match(/^[a-zA-Z0-9-]+[\.a-zA-Z0-9-]{0,254}$/);
		var FQDNsplit = str.split(".");
		
		var ret = true;
		if(!str) {
			alert(msg+" is empty.");
			ret = false;
		}else if(!result) {
			alert(msg+" is invalid.");
			ret = false;	
		}
		for(var i = 0; i < FQDNsplit.length; i++) {
			if(FQDNsplit[i].length > 63) {
				alert(msg+" is invalid.");
				ret = false;
				break;
			}
		}
		return ret;
	}
	
	function chkMAC(val, str) {
		if ( str == undefined ) {
			str = "MAC";
		}
		var ret = true;
		var result = val.match(/^(([0-9a-fA-F]{2}:){5}[0-9a-fA-F]{2})$/);
		if(!val) {
			alert(str+" is empty.");
			ret = false;
		}else if(!result) {
			alert(str+ " format error ,MAC format=xx:xx:xx:xx:xx:xx");
			ret = false;
		}
		return ret;
	}
	
	function chkIP(address, fieldname, str) {
		if(str == undefined) {
			if(address == '0.0.0.0'){
				alert(fieldname+ " : IP format error.");
				return false;
			}
		}
		if ( fieldname == undefined ) {
			fieldname = "";
		}
		var ret = true;
		var result = address.match(/^((([0-1]?[0-9]?[0-9])|(2[0-4][0-9])|(25[0-5]))\.){3}(([0-1]?[0-9]?[0-9])|(2[0-4][0-9])|(25[0-5]))$/);
		if(!address) {
			alert(fieldname+" is empty.");
			ret = false;
		}else if(!result) {
			alert(fieldname+ " : IP format error.");
			ret = false;
		}
		return ret;
	}
	
	function chkIP_CIDR(address, fieldname) { // IPV4/CIDR
		var ret = true;
		var result = address.match(/^((([0-1]?[0-9]?[0-9])|(2[0-4][0-9])|(25[0-5]))\.){3}(([0-1]?[0-9]?[0-9])|(2[0-4][0-9])|(25[0-5]))\/([1-9]|[1-2][0-9]|3[0-2])|0\.0\.0\.0\/0$/);
		if(!address) {
			alert(fieldname+" is empty.");
			ret = false;
		}else if(!result) {
			alert(fieldname+ " : IP subnet in CIDR format error.");
			ret = false;
		}
		return ret;
	}
	
	function chkIP_IP(address, fieldname) { // IPV4 - IPV4
		var ret = true;
		var result = address.match(/^((([0-1]?[0-9]?[0-9])|(2[0-4][0-9])|(25[0-5]))\.){3}(([0-1]?[0-9]?[0-9])|(2[0-4][0-9])|(25[0-5]))-((([0-1]?[0-9]?[0-9])|(2[0-4][0-9])|(25[0-5]))\.){3}(([0-1]?[0-9]?[0-9])|(2[0-4][0-9])|(25[0-5]))$/);
		if(!address) {
			alert(fieldname+" is empty.");
			ret = false;
		}else if(!result) {
			alert(fieldname+ " range error.");
			ret = false;
		}
		return ret;
	}
	
	function chkIPnp(address) {
		var ret = true;
		var result = address.match(/^((([0-1]?[0-9]?[0-9])|(2[0-4][0-9])|(25[0-5]))\.){3}(([0-1]?[0-9]?[0-9])|(2[0-4][0-9])|(25[0-5]))$/);
		if(!address) {
			ret = false;
		}else if(!result) {
			ret = false;
		}
		return ret;
	}
	
	function chgIntoSubNet(netIP, netMask) {
			var i = 0, j = 0; 
      var tempArray1 = new Array(); 
      var tempArray2 = new Array(); 
      var ret = "";
      tempArray1[0] = ""; 
      tempArray2[0] = ""; 
      while ((i < netIP.length) && (j<4)){ 
         if (netIP.charAt(i) != '.'){ 
            tempArray1[j] = tempArray1[j] + netIP.charAt(i); 
         }else{ 
            j++; 
            tempArray1[j] = ""; 
         } 
         ++i; 
      } 
      i = 0; 
      j = 0; 
      while ((i < netMask.length) && (j<4)){ 
         if (netMask.charAt(i) != '.'){ 
            tempArray2[j] = tempArray2[j] + netMask.charAt(i); 
         }else{ 
            j++; 
            tempArray2[j] = ""; 
         } 
         ++i; 
      } 
      for(i=0 ; i<=3 ; ++i){ 
         ret = ret + (tempArray2[i] & tempArray1[i]); 
         if (i != 3) ret += "."; 
      }
      return ret;    
	}	

	function CIDR_Netmask_Transfer(str, category) {
		switch(category) {
			case 'netmask' :
				var num = "";
				var bitNum = 0;
				var temp = str.split('.');
				for(var i = 0; i < temp.length; i++) {
					num += parseInt(temp[i]).toString(2);
				}
								
				for(var j = 0; j < num.length; j++) {
					if(num.charAt(j) != 0) { // just count "1"
						bitNum++;
					}
				}
				str = bitNum;
				break;
			case 'cidr' :
				var bitNum = "";
				for(var j = 0; j < str; j++) {
					bitNum += "1";
				}
				while(bitNum.length != 32) {
					bitNum += "0";
				}
				var subBit = "";
				var subip = 0;
				var start = 0;
				while(start < 32) {
					for(var j = start; j < start+8; j++) {
						subip += parseInt(bitNum.charAt(j)) * (1 << ((7+start) - j));
					}
					
					if((32 - start) > 8) {
						subBit += (subip+".");
					}else {
						subBit += subip;
					}
					start+=8;
					subip = 0;
				}
			
				str = subBit;
				break;
		}	
		return str;
	}
	
	function transMaxRange(num, stIp, defaultIp) {
		var dot1 = stIp.split(".");
		var dot2 = defaultIp.split(".");
		/*
		if((parseInt(dot1[3]) + parseInt(num)) >= 255) {
			num = 254 - dot1[3]; // 254 stand for 255 minus 1;
		}
		
		if(dot1[3] <= dot2[3]) { //default ip is within range.
 			num--;	
		}
		if(dot1[3] <= 1) { //default ip is x.x.x.1
			num--;
		}	
		*/
		return num;
	}
	
	function chkRealPass(str, msg) {
		var ret = true;           
		var result = str.match(/^[\w'\(\)\{\}\<\>^`\+\/:!\*#@&=$\\\.~%,\|;\-]+$/);//'
		var result2 = str.match(/^[\|;]{1}$/);
		
		if(!str) {
			alert(msg+" is empty.");
			ret = false;
		}else if(!result) {
			alert(msg+" is invalid.");
			ret = false;
		}else if(result2) {
			alert(msg+" is invalid.");
			ret = false;
		}
		return ret;
	}
	
	function chkDevHA_SyncPass(str, msg) {
		var ret = true; 
		var result = str.match(/^[\w\{\}^:\*#=\.~%,\-]+$/);
		
		if(!str) {
			alert(msg+" is empty.");
			ret = false;
		}else if(!result) {
			alert(msg+" is invalid.");
			ret = false;
		}
		return ret;
	}
	
	function chkLogMailPass(str, msg) {
		var ret = true;           
		var result = str.match(/^[\w'\(\)\{\}\<\>^`\+\/:!\*#@&=$\\\.~%\|;\-]+$/);//'
		var result2 = str.match(/^[\|;]{1}$/);
		
		if(!str) {
			alert(msg+" is empty.");
			ret = false;
		}else if(!result) {
			alert(msg+" is invalid.");
			ret = false;
		}else if(result2) {
			alert(msg+" is invalid.");
			ret = false;
		}
		return ret;
	}
	
	function chkSMTPUser(str, msg) {
		var ret = true;
		var result = str.match(/^[\w\@\.-]+$/);
		if(!str) {
			alert(msg+" is empty.");
			ret = false;
		}else if(!result) {
			alert(msg+" is invalid.");
			ret = false
		}
		return ret;
	}
	
	function chkName(str,msg,category,arg,idx) {
			var ret = true;
			var limit = str.match(/^[a-zA-Z_-]+[\w-]{0,30}$/);
			if(!str) {
				alert(msg+" is empty.");
				ret = false;
			}else if(!limit) {
				alert(msg+" is invalid.");
				ret = false;
			}else {
				for(var i = 0; i < gl[category].dataCount(); i++) {
					var objName = gl[category].getRecord(i);
					if(i != idx) {
						/*if(str == objName[arg]) {
							alert(msg+" is duplicate of "+objName[arg]);
							ret = false;
							break;
						}*/
					}
				}		
			}
			return ret;
	}

	function chkMemberNum(category, arg, maxCnt, msg, idx) {
		var ret = true;
		if(arg == undefined) {
			var dcnt = gl[category].dataCount();
		}else {
			var ptr = gl[category].getRecord(idx);
			var dcnt = ptr[arg].dataCount();
		}
		if(dcnt == maxCnt) {
			alert("Adding "+msg+ " has reached the maximum number.");
			ret = false;
		}		
		return ret;
	}
	
	function compareObjTextString(a, b) {
		if (a.text < b.text) return -1;
		if (a.text > b.text) return 1;
		return 0;
	}
	
	function moveOptions(fromObj, toObj, sort) {
		var optionArray = new Array();
		optionArray = optionsToArray(fromObj, 1, 1);
		insertOptions(toObj, optionArray, toObj.selectedIndex + 1);
		toObj.selectedIndex = -1;
		selectOptions(toObj, optionArray, compareObjTextString, true, false);
		if (sort == true) {
			sortOptions(toObj, compareObjTextString);
		}
		return optionArray;
	}
	
	function insertOptions(selectObj, optionArray, index) {
		var resultArray;
		resultArray = optionsToArray(selectObj, 2, 1);
		arrayToOptions(selectObj, resultArray.splice(0, index).concat(optionArray).concat(resultArray));
	} 

	function arrayToOptions(selectObj, optionArray) {
		for (var i = 0; i < optionArray.length; i++) {
			selectObj.options[selectObj.options.length] = new Option(optionArray[i].text, optionArray[i].value, optionArray[i].selected);
			selectObj.options[selectObj.options.length - 1].selected = optionArray[i].selected;
		}
	} 

	function optionsToArray(selectObj, mode, remove, startIndex, endIndex) {
		var optionArray = new Array();
		var i = startIndex ? startIndex : 0;
		endIndex = endIndex ? endIndex : selectObj.length;
	
		while (i < endIndex) {
			switch (mode) {
			case 0 : // not selected
				if (!selectObj[i].selected) {
					optionArray[optionArray.length] = selectObj[i];
					if (remove) {
						selectObj.removeChild(selectObj[i]);
						endIndex--;
						break;
					}
				}
				i++;
				break; 
			case 1 : // selected
				if (selectObj[i].selected) {					
					optionArray[optionArray.length] = selectObj[i];
					if (remove == 1) {
						selectObj.removeChild(selectObj[i]);
						endIndex--;
						break;
					}
				}
				i++;
				break;
			case 2 :
			default : // all
				optionArray[optionArray.length] = selectObj[i];				
				if (remove == 1) {
					selectObj.removeChild(selectObj[i]);
					endIndex--;
					break;
				}
				i++;
			}
		}
		return optionArray;
	} 

	function selectOptions(selectObj, optionArray, compareFn, selectValue, scroll) {
		var numberSelected = 0;
		var objOption = null;
	
		for (var i = 0; i < selectObj.length; i ++) {
			for (var j = 0; j < optionArray.length; j++) {
				if (compareFn(selectObj[i], optionArray[j]) == 0) {
					selectObj[i].selected = selectValue;
					numberSelected++;
					objOption = selectObj[i];
					break;
				}
			}
		}
		if (scroll && objOption) objOption.scrollIntoView();
		return numberSelected;
	} 

	function sortOptions(selectObj, compareFn) {
		var optionArray = optionsToArray(selectObj, 2, 1);
		optionArray.sort(compareFn);
		arrayToOptions(selectObj, optionArray);
	} 

function showPathEntry(path){		
	var pathName =top.frames[1].fileName+' > '+path;
	document.write('<font color=\'#003366\'>');
	document.write('<img src=\'images/i_icon2.gif\'>&nbsp;');
	document.write(pathName);
	document.write('<\/font >');
}

function SwitchMenu(obj){
	if(document.getElementById){
	var el = document.getElementById(obj);
	var ar = document.getElementById("cont").getElementsByTagName("DIV");
		if(el.style.display == "none"){
			el.style.display = "";
		}else{
			el.style.display = "none";
		}
	}
}
function ChangeClass(menu, newClass) { 
	 if (document.getElementById) { 
	 	document.getElementById(menu).className = newClass;
	 } 
} 
//document.onselectstart = new Function("return false");
function ChangeColor(IName,str){
	document.images[IName].src = str;
}
function doLink(htmlFile){
	top.frames[2].location= htmlFile;
}

function setCopyRename(value, DataList){
	DataTemp = DataList;
	CopyRename= value;
}
function getCopyRename(){
	return CopyRename;
}
selected1=0;
selected2=0;  	 
function selLeisure(Cont){
	sel = document.theform.filelists;
	str1="";
	str2=""; 	
	j=0;
	i=0;
	 	
//alert(sel.options.length);		
	for( i=0; i<sel.options.length; i++ ){
		
     		if( sel.options[i].selected == true) {     		
			j++; 
			if(j==1){
				str1 = sel.options[i].text ; 
				selected1=i;
				//document.title= str1.substring(0,15);
			}
			if(j==2){
				str2 = sel.options[i].text ;  
				selected2=i;
			}
		}
      }
	if(j==0){
		alert("No file selected.");
		return ;
	}
	switch(Cont){
		case 1:
			if(j>1){
				alert("You can only select one file.");
				return false;
			}else if(j==1){
				//alert("file name : " + str1);
			}
			break;
		case 2:
			if((j==1)||(j>2)){
				alert("Two files must be selected.");
				return false;
			}else if(j==2){
				//alert("1st file : " + str1 +"\n" + "2nd file : " + str2 );
			}
			break;
		default:			
			alert("No file is selected.");
			return ;
			break;
		}       
//alert("i"+selected1);
//alert("str1" + str1);
	return true;
   }
function SaveFiletoPC(){
	alert(str1.substring(0,15));
	window.location = str1.substring(0,15);	
}
function EditConffile(DataList){	
	var data = DataList.getRecord(selected1);			
	var str = data.filename.toString();	
	//top.document.title= '.:: Welcome to ZyWALL ('+str+') ::.';
	//displayStr(str);
	//top.frames[1].fileName = str;

	var cmd="file%20edit%20"+str;
	sendCLI(cmd, 'GdataPtr');
	top.frames[2].document.location.reload();
}
function setFullPath(str){
	top.frames[1].fullpath = str;
}
function showFullPath(str1, sel){
    //str = "<font font-family= \"Verdana, Arial, Helvetica, sans-serif\" ";
    //str += "font-size=\"11\" ";
    //str += "font-weight=\"bold\" ";
    //str += "color=\"#FFFFFF\">&nbsp;";
    str = "&nbsp;";
	if(sel == 1){
		str += str1;// home.html use only
	}else{
	    str += (top.frames[1].fileName +" > "+ str1);	
	}
	//str += "<\/font>";
	fr =0;	
	parent.frames[fr].document.open('text\/html');
	
	parent.frames[fr].document.writeln(' <html>');
	parent.frames[fr].document.writeln(' <head>');
	parent.frames[fr].document.writeln(' <title>Untitled Document<\/title>');
	parent.frames[fr].document.writeln(' <meta http-equiv=\"Content-Type\" content=\"text\/html; charset=UTF-8\">');	
	parent.frames[fr].document.writeln(' <link href=\"zw.css\" rel=\"stylesheet\" type=\"text/css\">');
	parent.frames[fr].document.writeln(' <\/head>');
	parent.frames[fr].document.writeln(' <body background=\"images\/full_path_bg.gif\">');	
	parent.frames[fr].document.writeln(' <table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\">');
	parent.frames[fr].document.writeln(' <tr>');
	parent.frames[fr].document.writeln(' <td height=\"19\" class=\"path\" style = \"font-family: Verdana, Arial, Helvetica, sans-serif;font-size: 11px;font-weight: bold;color:white\">&nbsp;<img src=\"images\/dot_2.gif\" width=\"12\" height=\"12\" align=\"absmiddle\">'+str+'<\/td>');
	parent.frames[fr].document.writeln(' <\/tr>');
	parent.frames[fr].document.writeln(' <\/table>');
	parent.frames[fr].document.writeln(' <\/body>');
	parent.frames[fr].document.writeln(' <\/html>');	
	parent.frames[fr].document.close();
	
	
	
}

function ifEdit(ii,strfile, action_code){
	var fr=1;
	top.frames[fr].ifdx = ii;                               //save index to panel.html
	top.frames[fr].action = action_code;    //save action code to panel.htm  // 0:add 1:edit 2:Insert
	top.frames[fr].bkaction = action_code;// IDP use only
	top.frames[2].location= strfile;
}

function ifEdit_app(ii,strfile, action_code){ // only for app. patrol
	top.frames[1].appIdx   = ii;             //save index to panel.html
	top.frames[1].action   = action_code;    //save action code to panel.htm  // 0:add 1:edit 2:Insert
	top.frames[2].location = strfile;
}

function CopyFile(DataList){		
	var data = DataList.getRecord(selected1);
	DataList.dataSet.splice(selected1,0,data);
	if(fsyName != ''){	
		var cmd="file%20copy%20"+data.filename+"%20"+fsyName;
		sendCLI(cmd, 'GdataPtr');
		top.frames[2].document.location.reload();
		//alert(cmd);
		sendCLI('file%20list', 'ConFileList');
		top.frames[2].document.location.reload();

		//var olddata = DataList.deleteRecord(selected1);	
		//olddata.filename = fsyName;
		//olddata.created = getFileCreated(data);
		//olddata.size = getFileSize(data);
		//olddata.modified = getFileModify();	
		//DataList.dataSet.splice(selected1,0,olddata);
		displayStr('Ready.');
	}else{
		displayStr('Name is empty.');
	}		
}

function openwins(DataList){
	var data = DataList.getRecord(selected1);		
	var str = data.filename;
	var subWin = window.open('keyin.html', 'Jerry', 'left=316,top=218,width=420,height=140,scrollbars=no,resizable=yes');	
	subWin.document.forms[0].ifn.value = str;
}

function RemoveFile(DataList){	
	if(confirm("Remove this configure?")){			
		var data = DataList.getRecord(selected1);	
		var cmd="file%20remove%20"+data.filename;
		sendCLI(cmd, 'GdataPtr');
		top.frames[2].document.location.reload();
		var data = DataList.deleteRecord(selected1);
	}
}
function Rename(DataList){		
	var data = DataList.getRecord(selected1);	
	if(fsyName != ''){		
		var cmd="file%20rename%20"+data.filename+"%20"+fsyName;		
		sendCLI(cmd, 'GdataPtr');
		top.frames[2].document.location.reload();
		
		data.filename =  fsyName;
		data.modified = getFileModify();
		var FT = getFrom();
		if(FT==1) window.location.href = 'ConfigFile.html';
		else if(FT==2) window.location.href = 'ShellFile.html';
		else if(FT==3) window.location.href = 'IDSPFile.html';		
		
		displayStr('Ready.');
	}else{
		displayStr('Name is empty.');
	}
}
function SaveHtmltoPC(){	
	//get file name length =25 , and copy to title 
	//top.document.title= '.:: Welcome to ZyWALL ('+sel.options[selected1].text.substring(0,25)+') ::.';
	
}

function displayStr(str){
	if(top.frames[1].action != "DIAL") {
		if ( top.frames[1].zyshdcnt != 1 ) return ;
	}	
	top.frames[2].frames[2].showReadyStr(str);	
}
function popUp(fr){	
	var fr=1;
	top.frames[fr].sts= true;	
	top.frames[2][1].getEthReloadEntry();
}
	
function getEthReloadEntry() {			
		document.location.reload();
		//top.document.title='Jerry Chou';				
	}	
	function getEthPifEntry(data) {		
		return data.name.toString();
	}	
	function AddEdit2Record(objidx, subidx, sid, info){
		var addFlag = false;

		var dptr = top.frames[1].EditList;	
		for(var i=0; i<dptr.dataCount(); i++){
			var data = dptr.getRecord(i);	
			if((data.sid == sid)&&(data.info == info)){
				return;
			}else{
				addFlag = true;				
			}
		}
		if((addFlag)||(dptr.dataCount()==0)){
			dptr.addNewRecord(objidx, subidx, sid, info);		
		}
	}
	function ClrEdit2Record(){
		var dptr = top.frames[1].EditList;	
		var decnt = dptr.dataCount();
		for(var i=0; i<decnt; i++){
			dptr.deleteRecord(0);	
		}
	}
	function editRecord(i,cnt,value){
		var fr=1;
		var gl=top.frames[fr];	
		var dataptr = gl.ConfigEtherList.getRecord(i);	
		var gpdataptr = gl.GupsDataList.getRecord(i);
		var theCLIptr = gl.cliCmd;
		switch(cnt){
			case 1:					
				dataptr.selNum = value;
				theCLIptr.addCLI("IP Address Selected, "+value);
				gl.ConfigEtherList.dataSet.splice(i,1,dataptr);
				break;
			case 2:
				var ret_code = dataptr.IP.verifyIP(value);
				
				if(ret_code == false){
					getEthReloadEntry();
					return;
				}
				theCLIptr.addCLI("IP Address, "+value);
				dataptr.IP.address = value;
				gl.ConfigEtherList.dataSet.splice(i,1,dataptr);
				break;
			case 3:	
				if(chkSubnetMask(value)){
					theCLIptr.addCLI("IP Subnet Mask, "+value);
					dataptr.SubnetMask.address = value;
					gl.ConfigEtherList.dataSet.splice(i,1,dataptr);
				}else{
					getEthReloadEntry();
					return;
				}
				break;
			case 4://Active				
				theCLIptr.addCLI("Item "+value);
				gl.ConfigEtherList.dataSet.splice(i,1,dataptr);
				break;
			case 5://Delete, Add					
				theCLIptr.addCLI("Item "+value);
				gl.ConfigEtherList.dataSet.splice(i,1,dataptr);
				break;		
			case 6://Groups used					
				theCLIptr.addCLI("Item "+value);
				gpdataptr.giface = value;
				gl.GupsDataList.dataSet.splice(i,1,gpdataptr);
				break;
			case 7://default used					
				theCLIptr.addCLI("Item "+value);
				break;
		}
	}
	function getFileName(data){
		return	data.filename;
	}
	function getFileSize(data){		
		return	data.size;
	}
	function getFileCreated( data){
		return	data.created;
	}
	function getFileModify(){

		var mydate=new Date()
		var year=mydate.getYear()
		if (year < 1000)
			year+=1900
		var day=mydate.getDay()
		var month=mydate.getMonth()+1
		if (month<10)
			month="0"+month
		var daym=mydate.getDate()
		if (daym<10)
			daym="0"+daym
	
		return	year+"/"+month+"/"+daym;
	}	
	
	function ShowFileLists(cnt, DataList){		
		var filenameL = 24;
		var filesizeL =8;
		var filecrL=16;
		var space = "_";		
		var dcnt = DataList.dataCount();
		if  (dcnt == 0 ) {
			displayStr('Loading...');
			return;
		}
		displayStr('Rendering...');
		for ( var i = 0 ; i < dcnt ; i++ ) {
			var data = DataList.getRecord(i);			
			document.write('<option>');			
			var outFileName = data.filename.toString();				
			var rFL= outFileName.length;	
			var fileL= filenameL-rFL;			
			if(rFL < filenameL){
			for(var j=0; j< fileL; j++){
				outFileName = outFileName + space;
				}	
			}				
			var outFileSize = data.size.toString();			
			var rsFL= outFileSize.length;
			var SizeL= filesizeL-rsFL;
			if(rsFL < filesizeL){
			for(var j=0; j< SizeL; j++){
				outFileSize = outFileSize + space;
				}	
			}
			var outFileCrl = data.created.toString();
			var rcrL= outFileCrl.length;				
			var crL= filecrL-rcrL;			
			if(rcrL < filecrL){
			for(var j=0; j< crL; j++){
				outFileCrl = outFileCrl + space;
				}	
			}
			var outFileMdy = data.modified.toString();			
			
			document.write( outFileName	+ outFileSize+ outFileCrl+ outFileMdy);			
		}
		displayStr('Ready.');
	}
	function getGif(i){
		var fr=1;
		gl=top.frames[fr];		
		var dataptr = gl.ConfigEtherList.getRecord(i);	
		//alert(dataptr.active);
		if(!dataptr.active) {
			return 'images/i_active_off.gif';			
		}else {
			return 'images/i_active_on.gif';
		}
	}
	function ChangeGif(i){
		var fr=1;
		gl=top.frames[fr];					
		var dataptr = gl.ConfigEtherList.getRecord(i);	
		name= "menu1"+i;
		if(document.images[name].src.indexOf("i_active_off.gif",0) == -1) {
//			dataptr.active = false;
			ChangeColor(name,'images/i_active_off.gif');
			editRecord(i,4,'Inactive');
		}else {
//			dataptr.active = true;
			ChangeColor(name,'images/i_active_on.gif');
			editRecord(i,4,'Active');
		}
//		gl.ConfigEtherList.dataSet.splice(i,1,dataptr);
	}
	function ChangeAliasGif(i, k){
		var fr=1;
		gl=top.frames[fr];
		var dataptr = gl.ConfigEtherList.getRecord(i);
		name= "m1"+i+k;
		if(document.images[name].src.indexOf("i_active_off.gif",0) == -1) {				
			dataptr.active = false;			
			ChangeColor(name,'images/i_active_off.gif');
			editRecord(i,4,'Inactive');
		}else {
			dataptr.active = true;
			ChangeColor(name,'images/i_active_on.gif');
			editRecord(i,4,'Active');
		}
		gl.ConfigEtherList.dataSet.splice(i,1,dataptr);
	}
			
/* PoP up status windows */			
	function popupwins(){	
		document.write('<div id="showimage" style="LEFT: 316px; TOP: 282px; POSITION: absolute; z-index:1;">');
		document.write('<TABLE  borderColor=#003366 height=150 cellSpacing=0 cellPadding=0 width=250 bgColor=#003366 border=1>');
		document.write('<TR>');
		document.write('<TD width="90%" bgColor=#003366 align=center>');
		document.write('<font color="#ffffff"> STATUS </font>');
		document.write('<\/td>');
		document.write('<td width="10%" style=cursor:pointer align=right>');
		document.write('<img src="images/i_close.gif" border=0 onClick="hidebox();return false" title="Close" >');
		document.write('<\/td><\/tr><tr>');
		document.write('<td colspan="2" width="100%" bgColor=#ffffff height=130><b>'); 
		document.write('<textarea name="StatusMsg" cols="60" rows="8" WRAP READONLY>');
		//document.write('---------------------------------------------------------');
		document.write('\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n');
		//document.write('--------------------------------------------------- -----')
		document.write('<\/textarea>');
		document.write('<\/b> <\/td><\/tr>');
		document.write('<\/table>');
		document.write('<\/div >');
	}
	function ShowWins(){
		var df=1;
		var flags= top.frames[df].sts;	//this var place in panel.html
		if(flags == true){
			var sTatus="iStatus.html";
			window.open(sTatus, 'Status', 'screenX=120, screenY=150, left=120,top=150, width=660,height=150,scrollbars=no,resizable=yes');
			top.frames[df].sts = false;
		}
	}
	function GetStatusList(){
		var fr=1;
		var theCLIptr = window.opener.top.frames[fr].cliCmd; // get CLI pointer	
		
		document.write('<textarea name="StatusMsg" cols="80" rows="10" WRAP READONLY>');
		document.write("List Length = "+theCLIptr.CLILength()+'\n');
		for(var n=0; n<theCLIptr.CLILength() ; n++){
			var strPtr = theCLIptr.getCLIbyIdx(n)
			document.write("["+n+"] "+strPtr+"\n");
		}
		document.write('</textarea>');
		
	}
	function hidebox(){
		var fr=1;
		top.frames[fr].sts = false;
		if (document.all)
			showimage.style.visibility="hidden"
		else if (document.layers[showimage])
			document.showimage.visibility="hide"
		
	}
	
	/* PoP up status windows */ 		

	function setChars(chars){
		
		str1="-----------Begin Message ---------";
		str2="\n";
		str3="-----------End Message ---------";		

		document.showimage.StatusMsg.value=str1+str2+chars+str3;
	}
	
	function logOut(){	
		top.location= 'logout.html';
	}
	
	function home(){
		top.location= 'http://www.zyxel.com';
	}
	
	function mailTo(){
		top.location="mailto:support@zyxel.com.tw"
	}
	function getMACNumberEntry(i) {
		return i+1;
	}	
	function setFilename(str){	
		displayStr("Loading...");
		fsyName = str.toString();	
		var getvalue = getCopyRename();				
		switch(getvalue){
			case 1:
				CopyFile(DataTemp);
				var FT = getFrom();
				if(FT==1) window.location.href = 'ConfigFile.html';
				else if(FT==2) window.location.href = 'ShellFile.html';
				else if(FT==3) window.location.href = 'IDSPFile.html';		
				
				//getEthReloadEntry();
				break;
			case 2:
				Rename(DataTemp);
				break;
		}
		displayStr("Ready.");
	}
	function  setScroll(event) {
		var fr=1;
		var gl=top.frames[fr];		
		gl.XV = (document.layers) ? event.pageX : document.body.scrollLeft;
		gl.YV = (document.layers) ? event.pageY : document.body.scrollTop;			
	}
	function ScrollTo(){
	/*	var fr=1;
		var gl=top.frames[fr];			 
		 scrollTo(gl.XV, gl.YV);*/
	}
	function setFrom(value){
		fromTo = value;
	}
	function getFrom(){
		return fromTo;
	}

function showLayer(layerName) {
	if (document.all) { 
		document.all[layerName].style.display = 'block';
	} else if (document.layers) { 
		document.layers[layerName].dispaly = 'block';
	} else if (document.getElementById) {
		document.getElementById(layerName).style.display = 'block';
	}
}

function hideLayer(layerName) {
	if (document.all) {
		document.all[layerName].style.display = 'none';
	} else if (document.layers) {
		document.layers[layerName].display = 'none';
	} else if (document.getElementById) {
		document.getElementById(layerName).style.display = 'none';
	}
}

function MM_swapImgRestore() { //v3.0
  var i,x,a=document.MM_sr; for(i=0;a&&i<a.length&&(x=a[i])&&x.oSrc;i++) x.src=x.oSrc;
}

function MM_preloadImages() { //v3.0
  var d=document; if(d.images){ if(!d.MM_p) d.MM_p=new Array();
    var i,j=d.MM_p.length,a=MM_preloadImages.arguments; for(i=0; i<a.length; i++)
    if (a[i].indexOf("#")!=0){ d.MM_p[j]=new Image; d.MM_p[j++].src=a[i];}}
}

function MM_findObj(n, d) { //v4.01
  var p,i,x;  if(!d) d=document; if((p=n.indexOf("?"))>0&&parent.frames.length) {
    d=parent.frames[n.substring(p+1)].document; n=n.substring(0,p);}
  if(!(x=d[n])&&d.all) x=d.all[n]; for (i=0;!x&&i<d.forms.length;i++) x=d.forms[i][n];
  for(i=0;!x&&d.layers&&i<d.layers.length;i++) x=MM_findObj(n,d.layers[i].document);
  if(!x && d.getElementById) x=d.getElementById(n); return x;
}

function MM_swapImage() { //v3.0
  var i,j=0,x,a=MM_swapImage.arguments; document.MM_sr=new Array; for(i=0;i<(a.length-2);i+=3)
   if ((x=MM_findObj(a[i]))!=null){document.MM_sr[j++]=x; if(!x.oSrc) x.oSrc=x.src; x.src=a[i+2];}
}

function MM_reloadPage(init) {  //reloads the window if Nav4 resized
  if (init==true) with (navigator) {if ((appName=="Netscape")&&(parseInt(appVersion)==4)) {
    document.MM_pgW=innerWidth; document.MM_pgH=innerHeight; onresize=MM_reloadPage; }}
  else if (innerWidth!=document.MM_pgW || innerHeight!=document.MM_pgH) location.reload();
}
MM_reloadPage(true);

function MM_openBrWindow(theURL,winName,features) { //v2.0
  window.open(theURL,winName,features);
}

	function AddArray(er, datacnt, AddrPtr){
		for ( var fi = 0 ; fi < datacnt ; fi++ ) {			
			var outName = AddrPtr.dataSet[fi].name;				
			er.options[fi] = new Option(outName, fi, false, false);
		}
	}
	function getType(idx, DataList) {
		return DataList.dataSet[idx].type;
	}
function getAddr(idx, DataList) {
	if(DataList.dataSet[idx].type == "HOST"){
		return DataList.dataSet[idx].startaddr.toString();
	}else if(DataList.dataSet[idx].type == "RANGE"){
		return DataList.dataSet[idx].startaddr+" - "+DataList.dataSet[idx].endaddr;
	}else{
		return DataList.dataSet[idx].startaddr+"/"+DataList.dataSet[idx].endaddr;
	}	
}
function chkMail(str, msg){
	var ret = true;
	if ( msg == undefined ) {
		msg = "E-Mail";
	}
	//^[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,3})$
	//var result = str.match(/^[\w\-]+(\.[\w\-]+)*@(([a-zA-Z0-9\-])+\.)+[a-zA-Z0-9]{2,4}$/);
	//var result = str.match(/^[\w\-]+(\.[\w\-]+)*@(([a-zA-Z0-9\-])+\.)+([a-zA-Z0-9]{2,4})+$/);
	var result = str.match(/^[\w\-]+(\.[\w\-]+)*@(([\w\-])+\.)+[\w\-]+$/);
	if(!str) {
		alert(msg+" is empty.");
		ret = false;
	}else if(!result) {
		alert(msg+" is invalid.");
		ret = false;
	}
	return ret;
}

function chkPasswd(str, msg) { //for register password
	var ret = true;
	var result = str.match(/^[\.\w@\-]{6,20}$/);
	if(!str) {
		alert(msg+" is empty.");
		ret = false;
	}else if(!result) {
		alert(msg+" is invalid.");
		ret = false;
	}
	return ret;
}

function chkAAA_Passwd(str, msg) {
	var ret = true;
	var result = str.match(/^[\w'\(\)\{\}\<\>^`\+\/:!\*#@&=$\\\.~%,\|;\-]+$/);//'
	var result2 = str.match(/^[\|;]{1}$/);
	if(!str) {
		alert(msg+" is empty.");
		ret = false;
	}else if(!result) {
		alert(msg+" is invalid.");
		ret = false;
	}else if(result2) {
		alert(msg+" is invalid.");
		ret = false;
	}
	return ret;
} 

function chkAAA_DN(str, msg) {
	var ret = true;
	var result = str.match(/^[\w\s'\(\)\{\}\<\>^`\+\/:!\*#@&=$\\\.~%,;\-]+$/);//'
	var result2 = str.match(/^[\|;]{1}$/);
	if(!str) {
		alert(msg+" is empty.");
		ret = false;
	}else if(!result) {
		alert(msg+" is invalid.");
		ret = false;
	}else if(result2) {
		alert(msg+" is invalid.");
		ret = false;
	}
	return ret;
} 

function chkMailHeader(str, msg) {
	//Note: Also used in "MAIL-HEADER-VALUE" & "RBL-QUERY_POSTFIX" & "RBL-SERVER-ADDRESS" within AS
	var ret = true;
	var result = str.match(/^[\.\w\-]{1,64}$/);
	if(!str) {
		alert(msg+" is empty.");
		ret = false;
	}else if(!result) {
		alert(msg+" is invalid.");
		ret = false;
	}
	return ret;
}

function chkPhone(str, msg){
	if (msg == undefined ) {
		msg = "Phone Number";
	}
	var ret = true;
//	([0-9]|[,]|[+]|[#]|[*]){1,20}
	var result = str.match(/^[0-9,\+\*#]{1,20}$/);
	if(!str) {
		alert(msg+" is empty.");
		ret = false;
	}else if(!result) {
		alert(msg+" is invalid.");
		ret = false;
	}
	return ret;
}

function getNumber(i) {
	return 	i+1;		
}
function nameChk(name, msg){
	if(name==""){
		alert(msg+" is empty");
		return false;
	}
	return true;
}
function FileNameChk(name, msg, sname, extname, extcnt, ExtFlag){
	if(name ==""){
		alert(msg+" is empty");
		return false;
	}
	if(name == sname){
		alert(msg+" can't copy or rename to itself.");
		return false;
	}																					        				
	if(!name.slice(0, name.length-(extcnt+1)).match(/^[\w'\(\)\{\}^`\+!#@&=$\.~%,;\-]{1,26}$/)) {//'
		alert(msg+" is an invalid filename.");
		return false;
	}else if(name.slice(0, name.length-(extcnt+1)).match(/^[;]{1}$/)) {
		alert(msg+" is an invalid filename.");
		return false;
	}
	if(ExtFlag){
	if(name.slice(name.length-extcnt, name.length) != extname){
		alert(msg+" file format *."+extname);
		return false;
	}
	}
	return true;
}

function time_format( covert_data, str_format ){
	var str = covert_data.replace(" ", "");
	var length_add = str_format.length - str.length;
	var str = str_format.substring(0,length_add) + str;
	return str;
}

function page_init() {
	// before open a configure page, reset data
	top.frames[1].zyshdcnt = 0;
}

function initDataPage(){
	top.frames[3].window.location.href = 'data.html';
}
function chkGlobalName(str,msg) {
		var ret = true;
		var upper = str.match(/^[A-Za-z]+\w*$/);
		if(!str) {
			alert(msg+" is empty.");
			ret = false;
		}else if(!upper) {
			alert(msg+" is invalid.");
			ret = false;
		}
		return ret;
}

function chkIDPName(str,msg) {
		var ret = true;
		var upper = str.match(/^[a-zA-Z_-]+[\w-]{0,30}$/);
		if(!str) {
			alert(msg+" is empty.");
			ret = false;
		}else if(!upper) {
			alert(msg+" is invalid.");
			ret = false;
		}
		return ret;
}

function chkFileExtName(str,msg) {
		var ret = true;
		var upper = str.match(/^[\/\\\w]{1,3}$/);
		if(!str) {
			alert(msg+" is empty.");
			ret = false;
		}else if(!upper) {
			alert(msg+" is invalid.");
			ret = false;
		}
		return ret;
}
/*************** Wizard *******************/
function UIgoto(html){
	window.location.href = html;
}
function TxCLI(cmd ){
	var frs=2;	
	var str = cmd.split(/(&cmd=)/);

	/*for netscape , firefox, ...*/
	for(var i=0; i<str.length; i++){
		if(str[i] == '&cmd='){
			str.splice(i, 1);//delete array element
		}
	}
	/* save clicnt to global data*/
	top.frames[0].dataRecord[0].clicnt = str.length;
	
	top.frames[frs].document.open('text\/html');
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
	top.frames[frs].document.writeln('<form name="postCli" action="/cgi-bin/zysh-cgi" enctype="application/x-www-form-urlencoded" method="post">');
	for(var i=0; i<str.length; i++){
			top.frames[frs].document.writeln(" <input type='text' name='cmd'  value='"+str[i]+"'> ");
		}
	top.frames[frs].document.writeln(' <\/form>');
	top.frames[frs].document.writeln('<\/body>');
	top.frames[frs].document.writeln(' <\/html>');	
	top.frames[frs].document.close();
	//top.frames[frs].document.location.reload();

}
/** maximum limit checked */
function chkLimit(ptr, num){
	var dcnt = ptr.dataCount();	
	if(dcnt >= num){
		alert("Maximum limit is "+num+".");
		return false;
	}
	return true;
}
/* import , reset to default file*/
function reset_txdata(str){
	/* Reset to default*/		
	document[str].return_errno.value = "ZLD";
	top.frames[3].window.location.href = 'data.html';
}

/* Start AJAX function*/
var http_request = false;

function makeRequest(url) {

    http_request = false;

    if (window.XMLHttpRequest) { // Mozilla, Safari,...
        http_request = new XMLHttpRequest();
        if (http_request.overrideMimeType) {
            http_request.overrideMimeType('text/xml');
        }
    } else if (window.ActiveXObject) { // IE
        try {
            http_request = new ActiveXObject("Msxml2.XMLHTTP");
        } catch (e) {
            try {
                http_request = new ActiveXObject("Microsoft.XMLHTTP");
            } catch (e) {}
        }
    }

    if (!http_request) {
        alert('Giving up :( Cannot create an XMLHTTP instance');
        return false;
    }
    http_request.onreadystatechange = alertContents;
    http_request.open('GET', url, true);
    http_request.send(null);

}

function alertContents() {

    if (http_request.readyState == 4) {
        if (http_request.status == 200) {
            alert(http_request.responseText);
        } else {
            alert('There was a problem with the request.');
        }
    }

}
/* End of AJAX function */
