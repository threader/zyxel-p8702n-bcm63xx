var fr = 1;//frames
var gl = top.frames[fr];	//this var place in panel.html

/* global usage */
function objGlobalList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(mb1, mb2, mb3, mb4, mb5, mb6, mb7, mb8, mb9, mb10, mb11, mb12, mb13, mb14, mb15, mb16, mb17, mb18, mb19, mb20,
															 objMb1, objMb2, objMb3, objMb4, objMb5){		
		var data=new objGlobalRecord(mb1, mb2, mb3, mb4, mb5, mb6, mb7, mb8, mb9, mb10, mb11, mb12, mb13, mb14, mb15, mb16, mb17, mb18, mb19, mb20,
															 objMb1, objMb2, objMb3, objMb4, objMb5);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, mb1, mb2, mb3, mb4, mb5, mb6, mb7, mb8, mb9, mb10, mb11, mb12, mb13, mb14, mb15, mb16, mb17, mb18, mb19, mb20,
															 objMb1, objMb2, objMb3, objMb4, objMb5){
		var data=new objGlobalRecord(mb1, mb2, mb3, mb4, mb5, mb6, mb7, mb8, mb9, mb10, mb11, mb12, mb13, mb14, mb15, mb16, mb17, mb18, mb19, mb20,
															 objMb1, objMb2, objMb3, objMb4, objMb5);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, mb1, mb2, mb3, mb4, mb5, mb6, mb7, mb8, mb9, mb10, mb11, mb12, mb13, mb14, mb15, mb16, mb17, mb18, mb19, mb20,
															 objMb1, objMb2, objMb3, objMb4, objMb5){
		var data=new objGlobalRecord(mb1, mb2, mb3, mb4, mb5, mb6, mb7, mb8, mb9, mb10, mb11, mb12, mb13, mb14, mb15, mb16, mb17, mb18, mb19, mb20,
															 objMb1, objMb2, objMb3, objMb4, objMb5);
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
function objGlobalRecord(mb1, mb2, mb3, mb4, mb5, mb6, mb7, mb8, mb9, mb10, mb11, mb12, mb13, mb14, mb15, mb16, mb17, mb18, mb19, mb20,
															 objMb1, objMb2, objMb3, objMb4, objMb5){
	this.mb1 	= mb1;
	this.mb2 	= mb2;
	this.mb3 	= mb3;
	this.mb4 	= mb4;
	this.mb5 	= mb5;
	this.mb6 	= mb6;
	this.mb7 	= mb7;
	this.mb8 	= mb8;
	this.mb9 	= mb9;
	this.mb10 = mb10;
	this.mb11 = mb11;
	this.mb12 = mb12;
	this.mb13 = mb13;
	this.mb14 = mb14;
	this.mb15 = mb15;
	this.mb16 = mb16;
	this.mb17 = mb17;
	this.mb18 = mb18;
	this.mb19 = mb19;
	this.mb20 = mb20;
	this.objMb1 = new objGlobalList();
	this.objMb2 = new objGlobalList();
	this.objMb3 = new objGlobalList();
	this.objMb4 = new objGlobalList();
	this.objMb5 = new objGlobalList();
}

function objportGpList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(iface, port1, port2, port3, port4, port5, port6, port7){		
		var data=new objportGpRecord(iface, port1, port2, port3, port4, port5, port6, port7);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, iface, port1, port2, port3, port4, port5, port6, port7){
		var data=new objportGpRecord(iface, port1, port2, port3, port4, port5, port6, port7);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, iface, port1, port2, port3, port4, port5, port6, port7){
		var data=new objportGpRecord(iface, port1, port2, port3, port4, port5, port6, port7);
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
function objportGpRecord(iface, port1, port2, port3, port4, port5, port6, port7){
	this.iface = iface;
	this.port1 = port1;
	this.port2 = port2;
	this.port3 = port3;
	this.port4 = port4;
	this.port5 = port5;
	this.port6 = port6;
	this.port7 = port7;
}
function objidpFileList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(version, date, autoupdate, mode, day, hour, signum){		
		var data=new objidpFileRecord(version, date, autoupdate, mode, day, hour, signum);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, version, date, autoupdate, mode, day, hour, signum){
		var data=new objidpFileRecord(version, date, autoupdate, mode, day, hour, signum);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, version, date, autoupdate, mode, day, hour, signum){
		var data=new objidpFileRecord(version, date, autoupdate, mode, day, hour, signum);
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
function objidpFileRecord(version, date, autoupdate, mode, day, hour, signum){
	this.version = version;
	this.date = date;
	this.autoupdate = autoupdate;
	this.mode = mode;
	this.day = day;
	this.hour = hour;
	this.signum = signum;
}
function objClockList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(zone, ctime,date,enable, dur_1, dur_2, dur_3, dur_4, 
		dur_5, dur_6, dur_7, dur_8, dur_9, dur_10, offset, ntp, ntpState){		
		var data=new objClockRecord(zone, ctime,date,enable, dur_1, dur_2, dur_3, dur_4, 
		dur_5, dur_6, dur_7, dur_8, dur_9, dur_10, offset, ntp, ntpState);
		this.dataSet.unshift(data); 	
	}
	
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, zone, ctime,date,enable, dur_1, dur_2, dur_3, dur_4, 
		dur_5, dur_6, dur_7, dur_8, dur_9, dur_10, offset, ntp, ntpState){
		var data=new objClockRecord(zone, ctime,date,enable, dur_1, dur_2, dur_3, dur_4, 
		dur_5, dur_6, dur_7, dur_8, dur_9, dur_10, offset, ntp, ntpState);
		this.dataSet.splice(idx,0,data);
	}
	
	this.insertafterRecord = function(idx, zone, ctime,date,enable, dur_1, dur_2, dur_3, dur_4, 
		dur_5, dur_6, dur_7, dur_8, dur_9, dur_10, offset, ntp, ntpState){
		var data=new objClockRecord(zone, ctime,date,enable, dur_1, dur_2, dur_3, dur_4, 
		dur_5, dur_6, dur_7, dur_8, dur_9, dur_10, offset, ntp, ntpState);
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

function objClockRecord(zone, ctime,date,enable, dur_1, dur_2, dur_3, dur_4, 
		dur_5, dur_6, dur_7, dur_8, dur_9, dur_10, offset, ntp, ntpState){
	/*time zone*/
	this.tzone = zone;
	/*clock time & date setting*/
	this.clktime = ctime; //clock time
	this.clkdate = date
	/*daylight saving*/
	this.dsaveE = enable;
	//this.dsaveT = time;
	this.dsaveDur_1 = dur_1;
	this.dsaveDur_2 = dur_2;
	this.dsaveDur_3 = dur_3;
	this.dsaveDur_4 = dur_4;
	this.dsaveDur_5 = dur_5;
	this.dsaveDur_6 = dur_6;
	this.dsaveDur_7 = dur_7;
	this.dsaveDur_8 = dur_8;
	this.dsaveDur_9 = dur_9;
	this.dsaveDur_10 = dur_10;
	this.dsaveO = offset;
	/*ntp server*/
	this.ntp = ntp;
	this.ntpS = ntpState;
}
function objHostnameList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(sname, dname, fqdn){		
		var data=new objHostnameRecord(sname, dname, fqdn);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, sname, dname, fqdn){
		var data=new objHostnameRecord(sname, dname, fqdn);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, sname, dname, fqdn){
		var data=new objHostnameRecord(sname, dname, fqdn);
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
function objHostnameRecord(sname, dname, fqdn){
	this.sname = sname;
	this.dname = dname;
	this.fqdn = fqdn;
}
function objServiceList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(name, type, protl, sport, eport){		
		var data=new objServiceRecord(name, type, protl, sport, eport);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, name, type, protl, sport, eport){
		var data=new objServiceRecord(name, type, protl, sport, eport);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, name, type, protl, sport, eport){
		var data=new objServiceRecord(name, type, protl, sport, eport);
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
function objServiceRecord(name, type, protl, sport, eport){
	this.name = name;
	this.type = type;
	this.protl = protl;//protocol
	this.sport = sport;//mini 
	this.eport = eport;//max
	this.submbr = new objAuthmbrList(); 	//objASLgmbrList();
}
function objScheduleList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(name, startyear, startmonth, startday, starthour, startminute, 
						stopyear, stopmonth, stopday, stophour, stopminute, w1, w2, w3, w4, w5, w6, w7){		
		var data=new objScheduleRecord(name, startyear, startmonth, startday, starthour, startminute, 
						stopyear, stopmonth, stopday, stophour, stopminute, w1, w2, w3, w4, w5, w6, w7);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, name, startyear, startmonth, startday, starthour, startminute, 
						stopyear, stopmonth, stopday, stophour, stopminute, w1, w2, w3, w4, w5, w6, w7){
		var data=new objScheduleRecord(name, startyear, startmonth, startday, starthour, startminute, 
						stopyear, stopmonth, stopday, stophour, stopminute, w1, w2, w3, w4, w5, w6, w7);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, name, startyear, startmonth, startday, starthour, startminute, 
						stopyear, stopmonth, stopday, stophour, stopminute, w1, w2, w3, w4, w5, w6, w7){
		var data=new objScheduleRecord(name, startyear, startmonth, startday, starthour, startminute, 
						stopyear, stopmonth, stopday, stophour, stopminute, w1, w2, w3, w4, w5, w6, w7);
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
function objScheduleRecord(name, startyear, startmonth, startday, starthour, startminute, 
						stopyear, stopmonth, stopday, stophour, stopminute, w1, w2, w3, w4, w5, w6, w7){
	this.name = name;
	this.startyear = startyear;
	this.startmonth = startmonth;
	this.startday = startday;
	this.starthour = starthour;
	this.startminute = startminute;
	this.stopyear = stopyear;
	this.stopmonth = stopmonth;
	this.stopday = stopday;
	this.stophour = stophour;
	this.stopminute = stopminute;
	this.w1 = w1;/* Monday */
	this.w2 = w2;
	this.w3 = w3;
	this.w4 = w4;
	this.w5 = w5;
	this.w6 = w6;
	this.w7 = w7;/* Sunday*/
}
function objASLdList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(dhost, dport, dpwd, basedn, binddn, cnid, stime, ussl){		
		var data=new objASLdRecord(dhost, dport, dpwd, basedn, binddn, cnid, stime, ussl);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, dhost, dport, dpwd, basedn, binddn, cnid, stime, ussl){
		var data=new objASLdRecord(dhost, dport, dpwd, basedn, binddn, cnid, stime, ussl);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, dhost, dport, dpwd, basedn, binddn, cnid, stime, ussl){
		var data=new objASLdRecord(dhost, dport, dpwd, basedn, binddn, cnid, stime, ussl);
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
function objASLdRecord(dhost, dport, dpwd, basedn, binddn, cnid, stime, ussl){
	this.dhost = dhost;
	this.dport = dport;
	this.dpwd = dpwd;
	this.basedn = basedn;
	this.binddn = binddn;
	this.cnid = cnid;
	this.stime = stime;
	this.ussl = ussl;
}
function objASLgList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(grpname, hostmember,dport, dpwd, basedn, binddn, cnid, stime, ussl,hostmbr){		
		var data=new objASLgRecord(grpname, hostmember,dport, dpwd, basedn, binddn, cnid, stime, ussl,hostmbr);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, grpname, hostmember,dport, dpwd, basedn, binddn, cnid, stime, ussl,hostmbr){
		var data=new objASLgRecord(grpname, hostmember,dport, dpwd, basedn, binddn, cnid, stime, ussl,hostmbr);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, grpname, hostmember,dport, dpwd, basedn, binddn, cnid, stime, ussl,hostmbr){
		var data=new objASLgRecord(grpname, hostmember,dport, dpwd, basedn, binddn, cnid, stime, ussl,hostmbr);
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
function objASLgRecord(grpname, hostmember,dport, dpwd, basedn, binddn, cnid, stime, ussl,hostmbr){
	this.grpname = grpname;
	this.hostmember = hostmember;	
	this.dport = dport;
	this.dpwd = dpwd;
	this.basedn = basedn;
	this.binddn = binddn;
	this.cnid = cnid;
	this.stime = stime;
	this.ussl = ussl;
	this.hostmbr = new objASLgmbrList();//Only group page use.
}

function objASLgmbrList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(hostmember){		
		var data=new objASLgmbrRecord(hostmember);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, hostmember){
		var data=new objASLgmbrRecord(hostmember);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, hostmember){
		var data=new objASLgmbrRecord(hostmember);
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
function objASLgmbrRecord(hostmember){
	this.hostmember = hostmember;
}
function objASRdsdList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(dhost, authport, acctport, rkey, rtimeout){		
		var data=new objASRdsdRecord(dhost, authport, acctport, rkey, rtimeout);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, dhost, authport, acctport, rkey, rtimeout){
		var data=new objASRdsdRecord(dhost, authport, acctport, rkey, rtimeout);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, dhost, authport, acctport, rkey, rtimeout){
		var data=new objASRdsdRecord(dhost, authport, acctport, rkey, rtimeout);
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
function objASRdsdRecord(dhost, authport, acctport, rkey, rtimeout){
	this.dhost = dhost;
	this.authport = authport;
	this.acctport = acctport;
	this.rkey = rkey;
	this.rtimeout = rtimeout;
}

function objASRdsgList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(grpname, hostmember,gkey, gtimeout,hostmbr){		
		var data=new objASRdsgRecord(grpname, hostmember,gkey, gtimeout,hostmbr);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, grpname, hostmember,gkey, gtimeout,hostmbr){
		var data=new objASRdsgRecord(grpname, hostmember,gkey, gtimeout,hostmbr);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, grpname, hostmember,gkey, gtimeout,hostmbr){
		var data=new objASRdsgRecord(grpname, hostmember,gkey, gtimeout,hostmbr);
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
function objASRdsgRecord(grpname,hostmember,gkey, gtimeout,hostmbr){
	this.grpname = grpname;
	this.hostmember = hostmember;
	this.gkey = gkey;
	this.gtimeout = gtimeout;	
	this.hostmbr = new objASRmbrList();
}

function objASRmbrList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(hostmember, authport){		
		var data=new objASRmbrRecord(hostmember, authport);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, hostmember, authport){
		var data=new objASRmbrRecord(hostmember, authport);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, hostmember, authport){
		var data=new objASRmbrRecord(hostmember, authport);
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
function objASRmbrRecord(hostmember, authport){
	this.hostmember = hostmember;
	this.authport = authport;
}
function objCfcacheList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(cate, url, remtime){		
		var data=new objCfcacheRecord(cate, url, remtime);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, cate, url, remtime){
		var data=new objCfcacheRecord(cate, url, remtime);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, cate, url, remtime){
		var data=new objCfcacheRecord(cate, url, remtime);
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
function objCfcacheRecord(cate, url, remtime){
	this.cate = cate;
	this.url = url;
	this.remtime = remtime;
}
function objCfprofileList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(pname){		
		var data=new objCfprofileRecord(pname);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, pname){
		var data=new objCfprofileRecord(pname);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, pname){
		var data=new objCfprofileRecord(pname);
		this.dataSet.splice(idx,0,data);
		var olddata = this.dataSet.pop();
		this.dataSet.splice(idx,0,olddata);		
	}
	this.deleteRecord = function(idx){		
		gl.Categories.splice(idx,1); 
		return this.dataSet.splice(idx,1); 
	}
	this.dataCount = function(){		
		return this.dataSet.length;
	}		
}
function objCfprofileRecord(pname){
	this.pname = pname;
	this.cylnk = new objCfcyList();
	this.cslnk = new objCfcsList();
}
function objCfcyList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(extreg, enextsvr, blkmweb, logmweb, blkutweb, logutweb, blkunaver, logunaver, testblk, category){		
		var data=new objCfcyRecord(extreg, enextsvr, blkmweb, logmweb, blkutweb, logutweb, blkunaver, logunaver, testblk, category);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, extreg, enextsvr, blkmweb, logmweb, blkutweb, logutweb, blkunaver, logunaver, testblk, category){
		var data=new objCfcyRecord(extreg, enextsvr, blkmweb, logmweb, blkutweb, logutweb, blkunaver, logunaver, testblk, category);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, extreg, enextsvr, blkmweb, logmweb, blkutweb, logutweb, blkunaver, logunaver, testblk, category){
		var data=new objCfcyRecord(extreg, enextsvr, blkmweb, logmweb, blkutweb, logutweb, blkunaver, logunaver, testblk, category);
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
function objCfcyRecord(extreg, enextsvr, blkmweb, logmweb, blkutweb, logutweb, blkunaver, logunaver, testblk, category){
	this.extreg 		= extreg;
	this.enextsvr 	= enextsvr;
	this.blkmweb 		= blkmweb;
	this.logmweb 		= logmweb;
	this.blkutweb 	= blkutweb;
	this.logutweb 	= logutweb;
	this.blkunaver 	= blkunaver;
	this.logunaver 	= logunaver;	
	this.testblk 		= testblk;
	this.category 	= new Array();
}

function objCfcsList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(ewscust, dwall, blkax, blkjv, blkck, blkprx, enall){		
		var data=new objCfcsRecord(ewscust, dwall, blkax, blkjv, blkck, blkprx, enall);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, ewscust, dwall, blkax, blkjv, blkck, blkprx, enall){
		var data=new objCfcsRecord(ewscust, dwall, blkax, blkjv, blkck, blkprx, enall);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, ewscust, dwall, blkax, blkjv, blkck, blkprx, enall){
		var data=new objCfcsRecord(ewscust, dwall, blkax, blkjv, blkck, blkprx, enall);
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
function objCfcsRecord(ewscust, dwall, blkax, blkjv, blkck, blkprx, enall){
	this.ewscust 	= ewscust;
	this.dwall 		= dwall;
	this.blkax 		= blkax;
	this.blkjv 		= blkjv;
	this.blkck 		= blkck;
	this.blkprx 	= blkprx;
	this.enall 		= enall;
	this.trsweb 		= new objCfAryList();
	this.forbidden 	= new objCfAryList();
	this.bkul 			= new objCfAryList();
}
function objCfAryList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(goldata){		
		var data=new objCfAryRecord(goldata);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}
	this.insertRecord = function(idx, goldata){
		var data=new objCfAryRecord(goldata);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, goldata){
		var data=new objCfAryRecord(goldata);
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
function objCfAryRecord(goldata){
	this.goldata = goldata;
}
function objGlList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(address, schedule, cfprofile, active, user, orig_active){		
		var data=new objGlRecord(address, schedule, cfprofile, active, user, orig_active);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, address, schedule, cfprofile, active, user, orig_active){
		var data=new objGlRecord(address, schedule, cfprofile, active, user, orig_active);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, address, schedule, cfprofile, active, user, orig_active){
		var data=new objGlRecord(address, schedule, cfprofile, active, user, orig_active);
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
function objGlRecord(address, schedule, cfprofile, active, user, orig_active){
	this.address = address;
	this.schedule = schedule;
	this.cfprofile = cfprofile;
	this.active = active;
	this.user = user;
	this.orig_active = orig_active; 
}
function objCfilterList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(dam, rurl, reg, service_timeout, cache_timeout, active, default_block, type){		
		var data=new objCfilterRecord(dam, rurl, reg, service_timeout, cache_timeout, active, default_block, type);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, dam, rurl, reg, service_timeout, cache_timeout, active, default_block, type){
		var data=new objCfilterRecord(dam, rurl, reg, service_timeout, cache_timeout, active, default_block, type);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, dam, rurl, reg, service_timeout, cache_timeout, active, default_block, type){
		var data=new objCfilterRecord(dam, rurl, reg, service_timeout, cache_timeout, active, default_block, type);
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
function objCfilterRecord(dam, rurl, reg, service_timeout, cache_timeout, active, default_block, type){
	this.dam = dam;				// block_message
	this.rurl = rurl;				// block_redirect
	this.reg = reg;				// license
	this.type =type; //registration type
	this.sublnk = new objGlList();	// content-filter policy
	this.service_timeout = service_timeout;
	this.cache_timeout = cache_timeout;
	this.active = active;
	this.default_block = default_block;
}
function objImptList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(pw){		
		var data=new objImptRecord(pw);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, pw){
		var data=new objImptRecord(pw);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, ipw){
		var data=new objImptRecord(pw);
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
function objImptRecord(pw){
	this.pw = pw;
}
function objCreateList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(cname, hostip, hostdomain, email, orgunt, org, cntry, keytype, keylen, enrol,
						enrolprt, encaser, encert, reqrefnb, reqkey){	
		var data=new objCreateRecord(cname, hostip, hostdomain, email, orgunt, org, cntry, keytype, keylen, enrol,
						enrolprt, encaser, encert, reqrefnb, reqkey);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, cname, hostip, hostdomain, email, orgunt, org, cntry, keytype, keylen, enrol,
						enrolprt, encaser, encert, reqrefnb, reqkey){
		var data=new objCreateRecord(cname, hostip, hostdomain, email, orgunt, org, cntry, keytype, keylen, enrol,
						enrolprt, encaser, encert, reqrefnb, reqkey);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, cname, hostip, hostdomain, email, orgunt, org, cntry, keytype, keylen, enrol,
						enrolprt, encaser, encert, reqrefnb, reqkey){
		var data=new objCreateRecord(cname, hostip, hostdomain, email, orgunt, org, cntry, keytype, keylen, enrol,
						enrolprt, encaser, encert, reqrefnb, reqkey);
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
function objCreateRecord(cname, hostip, hostdomain, email, orgunt, org, cntry, keytype, keylen, enrol,
						enrolprt, encaser, encert, reqrefnb, reqkey){
	this.cname = cname;
	this.hostip = hostip;
	this.hostdomain = hostdomain;	
	this.email = email;
	this.orgunt = orgunt;
	this.org = org;
	this.cntry = cntry;
	this.keytype = keytype;
	this.keylen = keylen;
	this.enrol = enrol;
	this.enrolprt = enrolprt;
	this.encaser = encaser;
	this.encert = encert;
	this.reqrefnb = reqrefnb;
	this.reqkey = reqkey;
}
function objEdCertList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(name, path, type, ver, sn, subj, issu, signa, validfr, validto,
						keyalgo, subjan, keyusage, baseconst, md5fing, sha1fing, encoded){	
		var data=new objEdCertRecord(name, path, type, ver, sn, subj, issu, signa, validfr, validto,
						keyalgo, subjan, keyusage, baseconst, md5fing, sha1fing, encoded);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, name, path, type, ver, sn, subj, issu, signa, validfr, validto,
						keyalgo, subjan, keyusage, baseconst, md5fing, sha1fing, encoded){
		var data=new objEdCertRecord(name, path, type, ver, sn, subj, issu, signa, validfr, validto,
						keyalgo, subjan, keyusage, baseconst, md5fing, sha1fing, encoded);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, name, path, type, ver, sn, subj, issu, signa, validfr, validto,
						keyalgo, subjan, keyusage, baseconst, md5fing, sha1fing, encoded){
		var data=new objEdCertRecord(name, path, type, ver, sn, subj, issu, signa, validfr, validto,
						keyalgo, subjan, keyusage, baseconst, md5fing, sha1fing, encoded);
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
function objEdCertRecord(name, path, type, ver, sn, subj, issu, signa, validfr, validto,
						keyalgo, subjan, keyusage, baseconst, md5fing, sha1fing, encoded){
	this.name = name;
	this.path = path;//self and CA only
	this.type = type;
	this.ver = ver;	
	this.sn = sn;
	this.subj = subj;
	this.issu = issu;
	this.signa = signa;
	this.validfr = validfr;
	this.validto = validto;
	this.keyalgo = keyalgo;
	this.subjan = subjan;
	this.keyusage = keyusage;
	this.baseconst = baseconst;//self and CA only
	this.md5fing = md5fing;
	this.sha1fing = sha1fing;
	this.encoded = encoded;	
}
function objTrsEditList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(name, path, crlocsp, ocsp, url, ocspid, ocsppw, ldap, addr, port, ldapid, ldappw,
						type, ver, sn, subj, issu, signa, validfr, validto,	keyalgo, subjan, keyusage, baseconst, 
						md5fing, sha1fing, encoded){	
		var data=new objTrsEditRecord(name, path, crlocsp, ocsp, url, ocspid, ocsppw, ldap, addr, port, ldapid, ldappw,
						type, ver, sn, subj, issu, signa, validfr, validto,	keyalgo, subjan, keyusage, baseconst, 
						md5fing, sha1fing, encoded);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, name, path, crlocsp, ocsp, url, ocspid, ocsppw, ldap, addr, port, ldapid, ldappw,
						type, ver, sn, subj, issu, signa, validfr, validto,	keyalgo, subjan, keyusage, baseconst, 
						md5fing, sha1fing, encoded){
		var data=new objTrsEditRecord(name, path, crlocsp, ocsp, url, ocspid, ocsppw, ldap, addr, port, ldapid, ldappw,
						type, ver, sn, subj, issu, signa, validfr, validto,	keyalgo, subjan, keyusage, baseconst, 
						md5fing, sha1fing, encoded);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, name, path, crlocsp, ocsp, url, ocspid, ocsppw, ldap, addr, port, ldapid, ldappw,
						type, ver, sn, subj, issu, signa, validfr, validto,	keyalgo, subjan, keyusage, baseconst, 
						md5fing, sha1fing, encoded){
		var data=new objTrsEditRecord(name, path, crlocsp, ocsp, url, ocspid, ocsppw, ldap, addr, port, ldapid, ldappw,
						type, ver, sn, subj, issu, signa, validfr, validto,	keyalgo, subjan, keyusage, baseconst, 
						md5fing, sha1fing, encoded);
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
function objTrsEditRecord(name, path, crlocsp, ocsp, url, ocspid, ocsppw, ldap, addr, port, ldapid, ldappw,
						type, ver, sn, subj, issu, signa, validfr, validto,	keyalgo, subjan, keyusage, baseconst, 
						md5fing, sha1fing, encoded){
	this.name = name;
	this.path = path;
	this.crlocsp = crlocsp;
	this.ocsp = ocsp;
	this.url = url;
	this.ocspid = ocspid;
	this.ocsppw = ocsppw;
	this.ldap = ldap;
	this.addr = addr;
	this.port = port;
	this.ldapid = ldapid;
	this.ldappw = ldappw;
	this.type = type;
	this.ver = ver;	
	this.sn = sn;
	this.subj = subj;
	this.issu = issu;
	this.signa = signa;
	this.validfr = validfr;
	this.validto = validto;
	this.keyalgo = keyalgo;
	this.subjan = subjan;
	this.keyusage = keyusage;
	this.baseconst = baseconst;
	this.md5fing = md5fing;
	this.sha1fing = sha1fing;
	this.encoded = encoded;	
}
function objCertList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(freebuf, name, type, subject, issuer, validfrom, validto, id, content, status){		
		var data=new objCertRecord(freebuf, name, type, subject, issuer, validfrom, validto, id, content, status);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.editRecord = function(idx, freebuf, name, type, subject, issuer, validfrom, validto, id, content, status){
		this.dataSet[idx].freebuf = freebuf;
		this.dataSet[idx].name = name;
		this.dataSet[idx].type = type;		
		this.dataSet[idx].subject = subject;
		this.dataSet[idx].issuer = issuer;
		this.dataSet[idx].validfrom = validfrom;		
		this.dataSet[idx].validto = validto;
		this.dataSet[idx].status = status;
	}
	this.insertRecord = function(idx, freebuf, name, type, subject, issuer, validfrom, validto, id, content, status){
		var data=new objCertRecord(freebuf, name, type, subject, issuer, validfrom, validto, id, content, status);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, freebuf, name, type, subject, issuer, validfrom, validto, id, content, status){
		var data=new objCertRecord(freebuf, name, type, subject, issuer, validfrom, validto, id, content, status);
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
function objCertRecord(freebuf, name, type, subject, issuer, validfrom, validto, id, content, status){
	this.freebuf = freebuf;//bar use  %
	this.name = name;
	this.type = type;	// My Certificate only
	this.subject = subject;
	this.issuer = issuer;
	this.validfrom = validfrom;
	this.validto = validto;
	this.status = status;
	this.crat = new objCreateList();
	this.impt = new objImptList();
	/* My Cert */
	this.self = new objEdCertList();
	this.ca = new objEdCertList();
	this.req = new objEdCertList();
	/* Trusted Cert */
	this.trsca = new objTrsEditList();
	/* for VPN Gateway use, show ID and Content */
	this.id = id;
	this.content = content;
}
function objvpncikeList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(ndes, protocol, encap, satime, pfs, lpolicy, lpolicydesc, rpolicy, 
							rpolicydesc, nailup, replay, netbios, advflag, proposal_tmp, policyenforce){		
		var data=new objvpncikeRecord(ndes, protocol, encap, satime, pfs, lpolicy, lpolicydesc, rpolicy, 
							rpolicydesc, nailup, replay, netbios, advflag, proposal_tmp, policyenforce);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){
		return this.dataSet[Idx];				
	}	
	this.insertRecord = function(idx, ndes, protocol, encap, satime, pfs, lpolicy, lpolicydesc, rpolicy, 
							rpolicydesc, nailup, replay, netbios, advflag, proposal_tmp, policyenforce){
		var data=new objvpncikeRecord(ndes, protocol, encap, satime, pfs, lpolicy, lpolicydesc, rpolicy, 
							rpolicydesc, nailup, replay, netbios, advflag, proposal_tmp, policyenforce);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, ndes, protocol, encap, satime, pfs, lpolicy, lpolicydesc, rpolicy, 
							rpolicydesc, nailup, replay, netbios, advflag, proposal_tmp, policyenforce){
		var data=new objvpncikeRecord(ndes, protocol, encap, satime, pfs, lpolicy, lpolicydesc, rpolicy, 
							rpolicydesc, nailup, replay, netbios, advflag, proposal_tmp, policyenforce);
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
function objvpncikeRecord(ndes, protocol, encap, satime, pfs, lpolicy, lpolicydesc, rpolicy, 
							rpolicydesc, nailup, replay, netbios, advflag, proposal_tmp, policyenforce){
	this.ndes = ndes;
	this.protocol = protocol;
	this.encap = encap;
	this.proposal = new objposlList();
	this.satime = satime;
	this.pfs = pfs;
	this.lpolicy = lpolicy;
	this.lpolicydesc = lpolicydesc;
	this.rpolicy = rpolicy;
	this.rpolicydesc = rpolicydesc;
	this.nailup = nailup;
	this.replay = replay;
	this.netbios = netbios;
	this.advflag = advflag;
	this.tnat = new objtnatList();
	this.proposal_tmp = proposal_tmp;
	this.policyenforce = policyenforce;
}
function objvpncmkList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(spi, encap, espah, espencry, espauth, encrykey, authkey, lpolicy, 
							lpolicydesc, rpolicy, rpolicydesc, myaddr, securegy, enetbios, advflag){		
		var data=new objvpncmkRecord(spi, encap, espah, espencry, espauth, encrykey, authkey, lpolicy, 
							lpolicydesc, rpolicy, rpolicydesc, myaddr, securegy, enetbios, advflag);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, spi, encap, espah, espencry, espauth, encrykey, authkey, lpolicy, 
							lpolicydesc, rpolicy, rpolicydesc, myaddr, securegy, enetbios, advflag){
		var data=new objvpncmkRecord(spi, encap, espah, espencry, espauth, encrykey, authkey, lpolicy, 
							lpolicydesc, rpolicy, rpolicydesc, myaddr, securegy, enetbios, advflag);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, spi, encap, espah, espencry, espauth, encrykey, authkey, lpolicy, 
							lpolicydesc, rpolicy, rpolicydesc, myaddr, securegy, enetbios, advflag){
		var data=new objvpncmkRecord(spi, encap, espah, espencry, espauth, encrykey, authkey, lpolicy, 
							lpolicydesc, rpolicy, rpolicydesc, myaddr, securegy, enetbios, advflag);
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
function objvpncmkRecord(spi, encap, espah, espencry, espauth, encrykey, authkey, lpolicy, 
							lpolicydesc, rpolicy, rpolicydesc, myaddr, securegy, enetbios, advflag){
	this.spi = spi;
	this.encap = encap;
	this.espah = espah;
	this.espencry = espencry;
	this.espauth = espauth;
	this.encrykey = encrykey;
	this.authkey = authkey;
	this.lpolicy = lpolicy;
	this.lpolicydesc = lpolicydesc;
	this.rpolicy = rpolicy;
	this.rpolicydesc = rpolicydesc;	
	this.myaddr = myaddr;
	this.securegy = securegy;
	this.enetbios = enetbios;
	this.advflag = advflag;
	this.tnat = new objtnatList();
}
function objposlList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(encap, auth){		
		var data=new objposlRecord(encap, auth);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, encap, auth){
		var data=new objposlRecord(encap, auth);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, encap, auth){
		var data=new objposlRecord(encap, auth);
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
function objposlRecord(encap, auth){
	this.encap = encap;
	this.auth = auth;
}
function objtnatList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(outtraffic, osnatsrc, osnatdest, osnat, intrafficsrc, isnatsrc, isnatdest, isnat, idnatf, idnat_cnt){		
		var data=new objtnatRecord(outtraffic, osnatsrc, osnatdest, osnat, intrafficsrc, isnatsrc, isnatdest, isnat, idnatf, idnat_cnt);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, outtraffic, osnatsrc, osnatdest, osnat, intrafficsrc, isnatsrc, isnatdest, isnat, idnatf, idnat_cnt){
		var data=new objtnatRecord(outtraffic, osnatsrc, osnatdest, osnat, intrafficsrc, isnatsrc, isnatdest, isnat, idnatf, idnat_cnt);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, outtraffic, osnatsrc, osnatdest, osnat, intrafficsrc, isnatsrc, isnatdest, isnat, idnatf, idnat_cnt){
		var data=new objtnatRecord(outtraffic, osnatsrc, osnatdest, osnat, intrafficsrc, isnatsrc, isnatdest, isnat, idnatf, idnat_cnt);
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
function objtnatRecord(outtraffic, osnatsrc, osnatdest, osnat, intrafficsrc, isnatsrc, isnatdest, isnat, idnatf, idnat_cnt){
	this.outtraffic = outtraffic;
	this.osnatsrc = osnatsrc;
	this.osnatdest = osnatdest;
	this.osnat = osnat;
	this.intrafficsrc = intrafficsrc;
	this.isnatsrc = isnatsrc;
	this.isnatdest = isnatdest;
	this.isnat = isnat;
	this.idnatf = idnatf;
	this.idnat_cnt = idnat_cnt;
	this.idnat = new objidnatList();
}
function objidnatList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(oip, mip, protocol, opf, opt, mpf, mpt){		
		var data=new objidnatRecord(oip, mip, protocol, opf, opt, mpf, mpt);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, oip, mip, protocol, opf, opt, mpf, mpt){
		var data=new objidnatRecord(oip, mip, protocol, opf, opt, mpf, mpt);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, oip, mip, protocol, opf, opt, mpf, mpt){
		var data=new objidnatRecord(oip, mip, protocol, opf, opt, mpf, mpt);
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
function objidnatRecord(oip, mip, protocol, opf, opt, mpf, mpt){
	this.oip = oip;
	this.mip = mip;
	this.protocol = protocol;
	this.opf = opf;
	this.opt = opt;
	this.mpf = mpf;
	this.mpt = mpt;
}
function objvpncList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(name, gateway, encap, algor, policy, active, gyname, orig_active, connected){		
		var data=new objvpncRecord(name, gateway, encap, algor, policy, active, gyname, orig_active, connected);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, name, gateway, encap, algor, policy, active, gyname, orig_active, connected){
		var data=new objvpncRecord(name, gateway, encap, algor, policy, active, gyname, orig_active, connected);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, name, gateway, encap, algor, policy, active, gyname, orig_active, connected){
		var data=new objvpncRecord(name, gateway, encap, algor, policy, active, gyname, orig_active, connected);
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
function objvpncRecord(name, gateway, encap, algor, policy, active, gyname, orig_active, connected){
	this.name = name;
	this.gateway = gateway;
	this.encap = encap;
	this.algor = algor;
	this.policy = policy;
	this.active = active;
	this.gyname = gyname;// gateway name use at IKE and Manual Page
	this.edike = new objvpncikeList();
	this.edmk = new objvpncmkList();
	this.orig_active = orig_active;
	this.connected = connected;
}
function objvpngyList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(negmode, satime, nattral, dpd, myaddr, inter, interdes, 
						dname, gateway1, gateway2, authmethod, psk, cert, lidtype, lidcontent, pidtype,
						pidcontent, enextauth, extauth, eun, epw, proposal_tmp, pfs, server_name){		
		var data=new objvpngyRecord(negmode, satime, nattral, dpd, myaddr, inter, interdes, 
						dname, gateway1, gateway2, authmethod, psk, cert, lidtype, lidcontent, pidtype,
						pidcontent, enextauth, extauth, eun, epw, proposal_tmp, pfs, server_name);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, negmode, satime, nattral, dpd, myaddr, inter, interdes, 
						dname, gateway1, gateway2, authmethod, psk, cert, lidtype, lidcontent, pidtype,
						pidcontent, enextauth, extauth, eun, epw, proposal_tmp, pfs, server_name){
		var data=new objvpngyRecord(negmode, satime, nattral, dpd, myaddr, inter, interdes, 
						dname, gateway1, gateway2, authmethod, psk, cert, lidtype, lidcontent, pidtype,
						pidcontent, enextauth, extauth, eun, epw, proposal_tmp, pfs, server_name);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, negmode, satime, nattral, dpd, myaddr, inter, interdes, 
						dname, gateway1, gateway2, authmethod, psk, cert, lidtype, lidcontent, pidtype,
						pidcontent, enextauth, extauth, eun, epw, proposal_tmp, pfs, server_name){
		var data=new objvpngyRecord(negmode, satime, nattral, dpd, myaddr, inter, interdes, 
						dname, gateway1, gateway2, authmethod, psk, cert, lidtype, lidcontent, pidtype,
						pidcontent, enextauth, extauth, eun, epw, proposal_tmp, pfs, server_name);
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
function objvpngyRecord(negmode, satime, nattral, dpd, myaddr, inter, interdes, 
						dname, gateway1, gateway2, authmethod, psk, cert, lidtype, lidcontent, pidtype,
						pidcontent, enextauth, extauth, eun, epw, proposal_tmp, pfs, server_name){
	this.negmode = negmode;
	this.proposal = new objposlList();
	this.satime = satime;
	this.nattral = nattral;	
	this.dpd = dpd;
	this.myaddr = myaddr;
	this.inter = inter;
	this.interdes = interdes;
	this.dname = dname;
	this.gateway1 = gateway1;
	this.gateway2 = gateway2;
	this.authmethod = authmethod;
	this.psk = psk;
	this.cert = cert;
	this.lidtype = lidtype;
	this.lidcontent = lidcontent;
	this.pidtype = pidtype;
	this.pidcontent = pidcontent;
	this.enextauth = enextauth;
	this.extauth = extauth;
	this.eun = eun;
	this.epw = epw;
	this.proposal_tmp = proposal_tmp;
	this.pfs = pfs;
	this.server_name = server_name; 
}
function objvpngList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(name, addr, gateway, cont, active, orig_active){		
		var data=new objvpngRecord(name, addr, gateway, cont, active, orig_active);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, name, addr, gateway, cont, active, orig_active){
		var data=new objvpngRecord(name, addr, gateway, cont, active, orig_active);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, name, addr, gateway, cont, active, orig_active){
		var data=new objvpngRecord(name, addr, gateway, cont, active, orig_active);
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
function objvpngRecord(name, addr, gateway, cont, active, orig_active){
	this.name = name;
	this.addr = addr;
	this.gateway = gateway;
	this.cont = cont;
	this.active = active;
	this.edgy = new objvpngyList();
	this.orig_active = orig_active ;
}
function objvpnsaList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(name, encap, policy, algor, cont, uptime, timeout, spi, inbound, outbound){		
		var data=new objvpnsaRecord(name, encap, policy, algor, cont, uptime, timeout, spi, inbound, outbound);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, name, encap, policy, algor, cont, uptime, timeout, spi, inbound, outbound){
		var data=new objvpnsaRecord(name, encap, policy, algor, cont, uptime, timeout, spi, inbound, outbound);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, name, encap, policy, algor, cont, uptime, timeout, spi, inbound, outbound){
		var data=new objvpnsaRecord(name, encap, policy, algor, cont, uptime, timeout, spi, inbound, outbound);
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
function objvpnsaRecord(name, encap, policy, algor, cont, uptime, timeout, spi, inbound, outbound){
	this.name = name;
	this.encap = encap;
	this.policy = policy;
	this.algor = algor;
	this.cont = cont;
	this.uptime = uptime;
	this.timeout = timeout;
	this.spi = spi;
	this.inbound = inbound;
	this.outbound = outbound;
}
function objAuthmList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(name, methodlist, password, description, access, visibility, idletimeout, refCnt,reauthtimeout,remoteadmin,remoteaccess,usetype){		
		var data=new objAuthmRecord(name, methodlist, password, description, access, visibility, idletimeout, refCnt,reauthtimeout,remoteadmin,remoteaccess,usetype);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, name, methodlist, password, description, access, visibility, idletimeout, refCnt,reauthtimeout,remoteadmin,remoteaccess,usetype){
		var data=new objAuthmRecord(name, methodlist, password, description, access, visibility, idletimeout, refCnt,reauthtimeout,remoteadmin,remoteaccess,usetype);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, name, methodlist, password, description, access, visibility, idletimeout, refCnt,reauthtimeout,remoteadmin,remoteaccess,usetype){
		var data=new objAuthmRecord(name, methodlist, password, description, access, visibility, idletimeout, refCnt,reauthtimeout,remoteadmin,remoteaccess,usetype);
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
function objAuthmRecord(name, methodlist, password, description, access, visibility, idletimeout, refCnt,reauthtimeout,remoteadmin,remoteaccess,usetype){
	this.name = name;
	this.password=password;
	this.description=description;
	this.access=access;
	this.visibility=visibility;
	this.idletimeout=idletimeout;
	this.refCnt=refCnt;
	this.methodlist = methodlist;
	this.reauthtimeout = reauthtimeout;
	this.remoteadmin = remoteadmin;
	this.remoteaccess = remoteaccess;
	this.usetype = usetype;
	this.member = new objAuthmbrList();
}
function objAuthmbrList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(mbrlist){		
		var data=new objAuthmbrRecord(mbrlist);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, mbrlist){
		var data=new objAuthmbrRecord(mbrlist);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, mbrlist){
		var data=new objAuthmbrRecord(mbrlist);
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
function objAuthmbrRecord(mbrlist){
	this.mbrlist = mbrlist;
}
function objPayloadList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(ofseten, ofsel, startoffset, endoffset, content, casesen, cwithin, cwithinbyte, dcodeuri){		
		var data=new objPayloadRecord(ofseten, ofsel, startoffset, endoffset, content, casesen, cwithin, cwithinbyte, dcodeuri);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, ofseten, ofsel, startoffset, endoffset, content, casesen, cwithin, cwithinbyte, dcodeuri){
		var data=new objPayloadRecord(ofseten, ofsel, startoffset, endoffset, content, casesen, cwithin, cwithinbyte, dcodeuri);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, ofseten, ofsel, startoffset, endoffset, content, casesen, cwithin, cwithinbyte, dcodeuri){
		var data=new objPayloadRecord(ofseten, ofsel, startoffset, endoffset, content, casesen, cwithin, cwithinbyte, dcodeuri);
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
function objPayloadRecord(ofseten, ofsel, startoffset, endoffset,content, casesen, cwithin, cwithinbyte, dcodeuri){
	this.ofseten = ofseten;
	this.ofsel = ofsel;
	this.startoffset = startoffset;
	this.endoffset = endoffset;
	this.content = content;
	this.casesen = casesen;
	this.cwithin = cwithin;
	this.cwithinbyte = cwithinbyte;	
	this.dcodeuri = dcodeuri;
}
function objDetailList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(name, sid, severity, platform, service, policytype){		
		var data=new objDetailRecord(name, sid, severity, platform, service, policytype);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, name, sid, severity, platform, service, policytype){
		var data=new objDetailRecord(name, sid, severity, platform, service, policytype);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, name, sid, severity, platform, service, policytype){
		var data=new objDetailRecord(name, sid, severity, platform, service, policytype);
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
function objDetailRecord(name, sid, severity, platform, service, policytype){
	/* Information */
	this.name = name;
	this.sid = sid;
	this.severity  = severity;
	this.platform   = platform;	
	this.service = service;	
	this.policytype = policytype;	
}
function objHeaderList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(threshold, packet, second, 
							typeofservice, typeofserviceequ, typeofservicevalue, identification, identificationvalue,
							fragmentation, rerved, dont, more, fragmentoffset, fragmentoffsetequ, fragmentoffsetvalue,
							timetolive,timetoliveequ, timetoliveval,ipoptions, ipoptionsvalue, sameip ,							
							transport, port, srcport, destport, flow, flowdir1, flowdir2, flowdir3, flags,
							flagssyn, flagsfin, flagsrst, flagsphs, flagsack, flagsurg, flagsr1, flagsr2,
							sequencenumber, sequencenumbervalue, acknumber, acknumbervalue,
							windowsize, windowequ, windowsizevalue,
							payloadsize, payloadsizeequ, payloadsizevalue
							){		
		var data=new objHeaderRecord(threshold, packet, second, 
							typeofservice, typeofserviceequ, typeofservicevalue, identification, identificationvalue,
							fragmentation, rerved, dont, more, fragmentoffset, fragmentoffsetequ, fragmentoffsetvalue,
							timetolive,timetoliveequ, timetoliveval,ipoptions, ipoptionsvalue, sameip ,							
							transport, port, srcport, destport, flow, flowdir1, flowdir2, flowdir3, flags,
							flagssyn, flagsfin, flagsrst, flagsphs, flagsack, flagsurg, flagsr1, flagsr2,
							sequencenumber, sequencenumbervalue, acknumber, acknumbervalue,
							windowsize, windowequ, windowsizevalue,
							payloadsize, payloadsizeequ, payloadsizevalue
							);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, threshold, packet, second, 
							typeofservice, typeofserviceequ, typeofservicevalue, identification, identificationvalue,
							fragmentation, rerved, dont, more, fragmentoffset, fragmentoffsetequ, fragmentoffsetvalue,
							timetolive,timetoliveequ, timetoliveval,ipoptions, ipoptionsvalue, sameip ,							
							transport, port, srcport, destport, flow, flowdir1, flowdir2, flowdir3, flags,
							flagssyn, flagsfin, flagsrst, flagsphs, flagsack, flagsurg, flagsr1, flagsr2,
							sequencenumber, sequencenumbervalue, acknumber, acknumbervalue,
							windowsize, windowequ, windowsizevalue,
							payloadsize, payloadsizeequ, payloadsizevalue
							){
		var data=new objHeaderRecord(threshold, packet, second, 
							typeofservice, typeofserviceequ, typeofservicevalue, identification, identificationvalue,
							fragmentation, rerved, dont, more, fragmentoffset, fragmentoffsetequ, fragmentoffsetvalue,
							timetolive,timetoliveequ, timetoliveval,ipoptions, ipoptionsvalue, sameip ,							
							transport, port, srcport, destport, flow, flowdir1, flowdir2, flowdir3, flags,
							flagssyn, flagsfin, flagsrst, flagsphs, flagsack, flagsurg, flagsr1, flagsr2,
							sequencenumber, sequencenumbervalue, acknumber, acknumbervalue,
							windowsize, windowequ, windowsizevalue,
							payloadsize, payloadsizeequ, payloadsizevalue
							);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, threshold, packet, second, 
							typeofservice, typeofserviceequ, typeofservicevalue, identification, identificationvalue,
							fragmentation, rerved, dont, more, fragmentoffset, fragmentoffsetequ, fragmentoffsetvalue,
							timetolive,timetoliveequ, timetoliveval,ipoptions, ipoptionsvalue, sameip ,							
							transport, port, srcport, destport, flow, flowdir1, flowdir2, flowdir3, flags,
							flagssyn, flagsfin, flagsrst, flagsphs, flagsack, flagsurg, flagsr1, flagsr2,
							sequencenumber, sequencenumbervalue, acknumber, acknumbervalue,
							windowsize, windowequ, windowsizevalue,
							payloadsize, payloadsizeequ, payloadsizevalue
							){
		var data=new objHeaderRecord(threshold, packet, second, 
							typeofservice, typeofserviceequ, typeofservicevalue, identification, identificationvalue,
							fragmentation, rerved, dont, more, fragmentoffset, fragmentoffsetequ, fragmentoffsetvalue,
							timetolive,timetoliveequ, timetoliveval,ipoptions, ipoptionsvalue, sameip ,							
							transport, port, srcport, destport, flow, flowdir1, flowdir2, flowdir3, flags, 
							flagssyn, flagsfin, flagsrst, flagsphs, flagsack, flagsurg, flagsr1, flagsr2,
							sequencenumber, sequencenumbervalue, acknumber, acknumbervalue,
							windowsize, windowequ, windowsizevalue,
							payloadsize, payloadsizeequ, payloadsizevalue);
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
function objHeaderRecord(threshold, packet, second, 
							typeofservice, typeofserviceequ, typeofservicevalue, identification,identificationvalue,
							fragmentation, rerved, dont, more, fragmentoffset, fragmentoffsetequ, fragmentoffsetvalue,
							timetolive,timetoliveequ, timetoliveval,ipoptions, ipoptionsvalue, sameip ,							
							transport, port, srcport, destport, flow, flowdir1, flowdir2, flowdir3, flags, 
							flagssyn, flagsfin, flagsrst, flagsphs, flagsack, flagsurg, flagsr1, flagsr2,
							sequencenumber, sequencenumbervalue, acknumber, acknumbervalue,
							windowsize, windowequ, windowsizevalue,payloadsize, payloadsizeequ, payloadsizevalue
							){
	/* Frequency */
	this.threshold = threshold;
	this.packet = packet;
	this.second = second;	
	
	/* Header Options - Network Protocol */
	this.typeofservice  = typeofservice;
	this.typeofserviceequ = typeofserviceequ;
	this.typeofservicevalue = typeofservicevalue;	
	this.identification = identification;
	this.identificationvalue = identificationvalue;	
	this.fragmentation = fragmentation;
	this.rerved = rerved;
	this.dont = dont;
	this.more = more;
	this.fragmentoffset = fragmentoffset;
	this.fragmentoffsetequ = fragmentoffsetequ;
	this.fragmentoffsetvalue = fragmentoffsetvalue;	
	this.timetolive = timetolive;
	this.timetoliveequ = timetoliveequ;
	this.timetoliveval = timetoliveval;	
	this.ipoptions = ipoptions;
	this.ipoptionsvalue = ipoptionsvalue;
	this.sameip = sameip;	
	/* Header Options - transport Protocol */
	this.transport = transport;
	this.port = port;
	this.srcport = srcport;
	this.destport = destport;
	this.flow = flow;
	this.flowdir1 = flowdir1;
	this.flowdir2 = flowdir2;
	this.flowdir3 = flowdir3;
	this.flags = flags;
	this.flagssyn = flagssyn;
	this.flagsfin = flagsfin;
	this.flagsrst = flagsrst;
	this.flagsphs = flagsphs;
	this.flagsack = flagsack;
	this.flagsurg = flagsurg;
	this.flagsr1 = flagsr1;
	this.flagsr2 = flagsr2;
	this.sequencenumber = sequencenumber;
	this.sequencenumbervalue = sequencenumbervalue;
	this.acknumber = acknumber;
	this.acknumbervalue = acknumbervalue;
	this.windowsize = windowsize;
	this.windowequ = windowequ;
	this.windowsizevalue = windowsizevalue;	
	/* Payload */
	this.payloadsize = payloadsize;
	this.payloadsizeequ = payloadsizeequ;
	this.payloadsizevalue = payloadsizevalue;
}
function objHeader1List()
{
	this.dataSet = new Array();
	this.addNewRecord = function(itypechk, itype, icodechk, icode, icmpidchk, icmpid, icmpseqchk, icmpseq){		
		var data=new objHeader1Record(itypechk, itype, icodechk, icode, icmpidchk, icmpid, icmpseqchk, icmpseq);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx,itypechk, itype, icodechk, icode, icmpidchk, icmpid, icmpseqchk, icmpseq){
		var data=new objHeader1Record(	itypechk, itype, icodechk, icode, icmpidchk, icmpid, icmpseqchk, icmpseq);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, itypechk, itype, icodechk, icode, icmpidchk, icmpid, icmpseqchk, icmpseq){
		var data=new objHeader1Record(	itypechk, itype, icodechk, icode, icmpidchk, icmpid, icmpseqchk, icmpseq);
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
function objHeader1Record(itypechk, itype, icodechk, icode, icmpidchk, icmpid, icmpseqchk, icmpseq){	
	/* icmp */
	this.itypechk= itypechk;
	this.itype = itype;
	this.icodechk = icodechk;
	this.icode = icode;
	this.icmpidchk = icmpidchk;
	this.icmpid = icmpid;
	this.icmpseqchk = icmpseqchk;
	this.icmpseq = icmpseq;	
}

function objidpcsList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(sid, name, expt, delall){		
		var data=new objidpcsRecord(sid, name, expt, delall);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, sid, name, expt, delall){
		var data=new objidpcsRecord(sid, name, expt, delall);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, sid, name, expt, delall){
		var data=new objidpcsRecord(sid, name, expt, delall);
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
function objidpcsRecord(sid, name, expt, delall){
	this.sid = sid;
	this.name = name;
	this.expt = expt;
	this.delall = delall;
	this.dtllink = new objDetailList();//idp edit page 1/3
	this.hedlink = new objHeaderList();//idp edit page 1/3
	this.hed1link = new objHeader1List();//idp edit page 1/3
	this.pallink = new objPayloadList();// idp edit page 1/3
}
function objidppfedSubList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(flag, name, message, sid, severity, policy, activation, log, action){		
		var data=new objidppfedSubRecord(flag, name, message, sid, severity, policy, activation, log, action);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, flag, name, message, sid, severity, policy, activation, log, action){
		var data=new objidppfedSubRecord(flag, name, message, sid, severity, policy, activation, log, action);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, flag, name, message, sid, severity, policy, activation, log, action){
		var data=new objidppfedSubRecord(flag, name, message, sid, severity, policy, activation, log, action);
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
function objidppfedSubRecord(flag, name, message, sid, severity, policy, activation, log, action){
	this.flag = flag;
	this.name = name;
	this.message = message;
	this.sid = sid;
	this.severity = severity;
	this.policy = policy;
	this.activation = activation;
	this.log = log;
	this.action = action;
}
function objidppfedList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(flag, service, activation, log, action, mb1){		
		var data=new objidppfedRecord(flag, service, activation, log, action, mb1);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, flag, service, activation, log, action, mb1){
		var data=new objidppfedRecord(flag, service, activation, log, action, mb1);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, flag, service, activation, log, action, mb1){
		var data=new objidppfedRecord(flag, service, activation, log, action, mb1);
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
function objidppfedRecord(flag, service, activation, log, action, mb1){
	this.flag = flag;
	this.service = service;
	this.activation = activation;
	this.log = log;	
	this.action = action;
	this.threshold = mb1;
	this.sublink = new objidppfedSubList();
}
function objidpglList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(logperiod, plength, most, saddr, sport, nsignatures, osignatures){		
		var data=new objidpglRecord(logperiod, plength, most, saddr, sport, nsignatures, osignatures);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, logperiod, plength, most, saddr, sport, nsignatures, osignatures){
		var data=new objidpglRecord(logperiod, plength, most, saddr, sport, nsignatures, osignatures);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, logperiod, plength, most, saddr, sport, nsignatures, osignatures){
		var data=new objidpglRecord(logperiod, plength, most, saddr, sport, nsignatures, osignatures);
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
function objidpglRecord(logperiod, plength, most, saddr, sport, nsignatures, osignatures){
	this.logperiod = logperiod;
	this.plength = plength;
	this.most = most;
	this.saddr = saddr;
	this.sport = sport;
	this.nsignatures = nsignatures;
	this.osignatures = osignatures;
}
function objidpglbList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(mb1, mb2, mb3, mb4){		
		var data=new objidpglbRecord(mb1, mb2, mb3, mb4);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, mb1, mb2, mb3, mb4){
		var data=new objidpglbRecord(mb1, mb2, mb3, mb4);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, mb1, mb2, mb3, mb4){
		var data=new objidpglbRecord(mb1, mb2, mb3, mb4);
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
function objidpglbRecord(mb1, mb2, mb3, mb4){
	this.from = mb1;
	this.to   = mb2;
	this.idppf  = mb3;
	this.active = mb4;
}
function objidppfList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(name, baseprofile){		
		var data=new objidppfRecord(name, baseprofile);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, name, baseprofile){
		var data=new objidppfRecord(name, baseprofile);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, name, baseprofile){
		var data=new objidppfRecord(name, baseprofile);
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
function objidppfRecord(name, baseprofile){
	this.name= name;
	this.baseprofile = baseprofile;
	this.packet = new objidppfedList();
	this.trafficsd = new objidppfedList();
	this.trafficfd = new objidppfedList();	
	this.ptlhi = new objidppfedList();
	this.ptltd = new objidppfedList();
	this.ptlud = new objidppfedList();
	this.ptlid = new objidppfedList();
}
function objOSPFList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(routeid, routeud, redripactive, redstactive, redriptype, redsttype, redripmetric, redstmetric){
		var data=new objOSPFRecord(routeid, routeud, redripactive, redstactive, redriptype, redsttype, redripmetric, redstmetric);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, routeid, routeud, redripactive, redstactive, redriptype, redsttype, redripmetric, redstmetric){
		var data=new objOSPFRecord(routeid, routeud, redripactive, redstactive, redriptype, redsttype, redripmetric, redstmetric);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, routeid, routeud, redripactive, redstactive, redriptype, redsttype, redripmetric, redstmetric){
		var data=new objOSPFRecord(routeid, routeud, redripactive, redstactive, redriptype, redsttype, redripmetric, redstmetric);
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
function objOSPFRecord(routeid, routeud, redripactive, redstactive, redriptype, redsttype, redripmetric, redstmetric){
	this.routeid= routeid;
	this.routeud = routeud;	
	this.redripactive = redripactive;	
	this.redstactive = redstactive;
	this.redriptype = redriptype;
	this.redsttype = redsttype;
	this.redripmetric = redripmetric;	
	this.redstmetric = redstmetric;
}

function objOSPFAreaList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(areaid, areatype, areaauth, areasak, areamid, aresmak, active){		
		var data=new objOSPFAreaRecord(areaid, areatype, areaauth, areasak, areamid, aresmak, active);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, areaid, areatype, areaauth, areasak, areamid, aresmak, active){
		var data=new objOSPFAreaRecord(areaid, areatype, areaauth, areasak, areamid, aresmak, active);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, areaid, areatype, areaauth, areasak, areamid, aresmak, active){
		var data=new objOSPFAreaRecord(areaid, areatype, areaauth, areasak, areamid, aresmak, active);
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
function objOSPFAreaRecord(areaid, areatype, areaauth, areasak, areamid, aresmak, active){
	this.areaid= areaid;
	this.areatype = areatype;	
	this.areaauth = areaauth;	
	this.areasak = areasak;
	this.areamid = areamid;
	this.aresmak = aresmak;
	this.active= active;
	this.link = new objOSPFAreaVLList();	
}
function objOSPFAreaVLList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(peerid, auth,textKey, md5ID, md5Key){		
		var data=new objAreaVLRecord(peerid, auth,textKey, md5ID, md5Key);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, peerid, auth,textKey, md5ID, md5Key){
		var data=new objAreaVLRecord(peerid, auth,textKey, md5ID, md5Key);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, peerid, auth,textKey, md5ID, md5Key){
		var data=new objAreaVLRecord(peerid, auth,textKey, md5ID, md5Key);
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
function objAreaVLRecord(peerid, auth,textKey, md5ID, md5Key){
	this.peerid = peerid;	
	this.auth = auth;	
	this.textKey = textKey;	
	this.md5ID = md5ID;	
	this.md5Key = md5Key;	
}

function objRipList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(auth, sauthkey, md5authid, md5authkey, ospfmetric, ospfactive, stmetric, stactive){		
		var data=new objRipRecord(auth, sauthkey, md5authid, md5authkey, ospfmetric, ospfactive, stmetric, stactive);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, auth, sauthkey, md5authid, md5authkey, ospfmetric, ospfactive, stmetric, stactive){
		var data=new objRipRecord(auth, sauthkey, md5authid, md5authkey, ospfmetric, ospfactive, stmetric, stactive);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, auth, sauthkey, md5authid, md5authkey, ospfmetric, ospfactive, stmetric, stactive){
		var data=new objRipRecord(auth, sauthkey, md5authid, md5authkey, ospfmetric, ospfactive, stmetric, stactive);
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
function objRipRecord(auth, sauthkey, md5authid, md5authkey, ospfmetric, ospfactive, stmetric, stactive){
	this.auth= auth;
	this.sauthkey = sauthkey;	
	this.md5authid = md5authid;	
	this.md5authkey = md5authkey;
	this.ospfmetric = ospfmetric;
	this.ospfactive = ospfactive;
	this.stmetric = stmetric;	
	this.stactive = stactive;
}

function objAddressList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(Name, Type, Startaddr, Endaddr){		
		var data=new objAddressRecord(Name, Type, Startaddr, Endaddr);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, Name, Type, Startaddr, Endaddr){
		var data=new objAddressRecord(Name, Type, Startaddr, Endaddr);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, Name, Type, Startaddr, Endaddr){
		var data=new objAddressRecord(Name, Type, Startaddr, Endaddr);
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
function objAddressRecord(Name, Type, Startaddr, Endaddr){ //Network, Netmask
	this.name= Name;
	this.type = Type;	
	this.startaddr = Startaddr;	
	this.endaddr = Endaddr;
}

function objVirSvrList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(Name, iface, Originalip, Userdefine, Mappedip, Mappedtype, Protocol, Originalstartport, Originalendport, Mappedstartport, Mappedendport, Active){		
		var data=new objVirSvrRecord(Name, iface, Originalip, Userdefine, Mappedip, Mappedtype, Protocol, Originalstartport, Originalendport, Mappedstartport, Mappedendport, Active);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, Name, iface, Originalip, Userdefine, Mappedip, Mappedtype, Protocol, Originalstartport, Originalendport, Mappedstartport, Mappedendport, Active){
		var data=new objVirSvrRecord(Name, iface, Originalip, Userdefine, Mappedip, Mappedtype, Protocol, Originalstartport, Originalendport, Mappedstartport, Mappedendport, Active);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, Name, iface, Originalip, Userdefine, Mappedip, Mappedtype, Protocol, Originalstartport, Originalendport, Mappedstartport, Mappedendport, Active){
		var data=new objVirSvrRecord(Name, iface, Originalip, Userdefine, Mappedip, Mappedtype, Protocol, Originalstartport, Originalendport, Mappedstartport, Mappedendport, Active);
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
function objVirSvrRecord(Name, iface, Originalip, Userdefine, Mappedip, Mappedtype, Protocol, Originalstartport, Originalendport, Mappedstartport, Mappedendport, Active){
	this.name= Name;
	this.iface = iface;	
	this.originalip = Originalip;	
	this.userdefine = Userdefine;
	this.mappedip = Mappedip;
	this.mappedtype = Mappedtype;
	this.protocol = Protocol;
	this.originalstartport = Originalstartport;	
	this.originalendport = Originalendport;	
	this.mappedstartport = Mappedstartport;
	this.mappedendport = Mappedendport;
	this.active = Active;
}
function objStRouteList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(Destination, Subnetmask, options, nexthop, Metric, Active){		
		var data=new objStRouteRecord(Destination, Subnetmask, options, nexthop, Metric, Active);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, Destination, Subnetmask, options, nexthop, Metric, Active){
		var data=new objStRouteRecord(Destination, Subnetmask, options, nexthop, Metric, Active);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, Destination, Subnetmask, options, nexthop, Metric, Active){
		var data=new objStRouteRecord(Destination, Subnetmask, options, nexthop, Metric, Active);
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
function objStRouteRecord(Destination, Subnetmask, options, nexthop, Metric, Active){
	this.destination = Destination;
	this.subnetmask = Subnetmask;
	this.options = options;	
	this.nexthop = nexthop;
	this.metric = Metric;
	this.active = Active;	
}
function objSysLogList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(Servertype, Serveraddr, Logfacility, Active,idx){		
		var data=new objSysLogRecord(Servertype, Serveraddr, Logfacility, Active,idx);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, Servertype, Serveraddr, Logfacility, Active,idx){
		var data=new objSysLogRecord(Servertype, Serveraddr, Logfacility, Active,idx);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, Servertype, Serveraddr, Logfacility, Active,idx){
		var data=new objSysLogRecord(Servertype, Serveraddr, Logfacility, Active,idx);
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
function objSysLogRecord(Servertype, Serveraddr, Logfacility, Active,idx){
	this.servertype= Servertype;
	this.serveraddr = Serveraddr;
	this.logfacility = Logfacility;	
	this.active = Active;	
	this.idx = idx;
}


function objLogMailList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(Mailserver, Mailsubject, Sendlogto, Sendalertto, Sendinglog, Daylog, Hourlog, Minlog, Active,SmtpStatus,SmtpUsr, SendFrom, Passwd){		
		var data=new objLogMailRecord(Mailserver, Mailsubject, Sendlogto, Sendalertto, Sendinglog, Daylog, Hourlog, Minlog, Active,SmtpStatus,SmtpUsr, SendFrom, Passwd);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, Mailserver, Mailsubject, Sendlogto, Sendalertto, Sendinglog, Daylog, Hourlog, Minlog, Active,SmtpStatus,SmtpUsr, SendFrom, Passwd){
		var data=new objLogMailRecord(Mailserver, Mailsubject, Sendlogto, Sendalertto, Sendinglog, Daylog, Hourlog, Minlog, Active,SmtpStatus,SmtpUsr, SendFrom, Passwd);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, Mailserver, Mailsubject, Sendlogto, Sendalertto, Sendinglog, Daylog, Hourlog, Minlog, Active,SmtpStatus,SmtpUsr, SendFrom, Passwd){
		var data=new objLogMailRecord(Mailserver, Mailsubject, Sendlogto, Sendalertto, Sendinglog, Daylog, Hourlog, Minlog, Active,SmtpStatus,SmtpUsr, SendFrom, Passwd);
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
function objLogMailRecord(Mailserver, Mailsubject, Sendlogto, Sendalertto, Sendinglog, Daylog, Hourlog, Minlog, Active,SmtpStatus,SmtpUsr, SendFrom, Passwd){
	this.mailserver= Mailserver;
	this.mailsubject = Mailsubject;
	this.sendlogto = Sendlogto;
	this.sendalertto = Sendalertto;
	this.sendinglog = Sendinglog;
	this.daylog = Daylog;
	this.hourlog = Hourlog;
	this.minlog = Minlog;
	this.active = Active;	
	this.smtpstatus = SmtpStatus;
	this.smtpusr = SmtpUsr;
	this.sendfrom = SendFrom;
	this.passwd = Passwd;
}

function objLogDispList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(Item, InterLog, M1Log, M1Alert, M2Log, M2Alert, R1Log, R2Log, R3Log, R4Log){		
		var data=new objLogDispRecord(Item, InterLog, M1Log, M1Alert, M2Log, M2Alert, R1Log, R2Log, R3Log, R4Log);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, Item, InterLog, M1Log, M1Alert, M2Log, M2Alert, R1Log, R2Log, R3Log, R4Log){
		var data=new objLogDispRecord(Item, InterLog, M1Log, M1Alert, M2Log, M2Alert, R1Log, R2Log, R3Log, R4Log);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, Item, InterLog, M1Log, M1Alert, M2Log, M2Alert, R1Log, R2Log, R3Log, R4Log){
		var data=new objLogDispRecord(Item, InterLog, M1Log, M1Alert, M2Log, M2Alert, R1Log, R2Log, R3Log, R4Log);
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
function objLogDispRecord(Item, InterLog, M1Log, M1Alert, M2Log, M2Alert, R1Log, R2Log, R3Log, R4Log){
	this.item = Item;//Log Category Name
	this.interlog = InterLog;//Internal Log 0:none, 1:logs, 2:debug(more then logs message)
	this.m1log = M1Log;//0:none 1:log
	this.m1alert = M1Alert;//0:none 1:alert
	this.m2log = M2Log;//0:none 1:log
	this.m2alert = M2Alert;//0:none 1:alert
	this.r1log = R1Log;//remote server 1 log 0:none, 1:logs, 2:debug
	this.r2log = R2Log;//remote server 2 log 0:none, 1:logs, 2:debug
	this.r3log = R3Log;//remote server 3 log 0:none, 1:logs, 2:debug
	this.r4log = R4Log;//remote server 4 log 0:none, 1:logs, 2:debug
}


function objLogSetList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(Name, ServerType, Summary, Active){		
		var data=new objLogSetRecord(Name, ServerType, Summary, Active);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, Name, ServerType, Summary, Active){
		var data=new objLogSetRecord(Name, ServerType, Summary, Active);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, Name, ServerType, Summary, Active){
		var data=new objLogSetRecord(Name, ServerType, Summary, Active);
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
function objLogSetRecord(Name, ServerType, Summary, Active){
	this.name = Name;
	this.servertype = ServerType;
	this.summary = Summary;
	this.active = Active;	
}

function objVlogList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(DayTime, Message, Source, Destination, Note, Sort,pri,cat){		
		var data=new objVlogRecord(DayTime, Message, Source, Destination, Note, Sort,pri,cat);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, DayTime, Message, Source, Destination, Note, Sort,pri,cat){
		var data=new objVlogRecord(DayTime, Message, Source, Destination, Note, Sort,pri,cat);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, DayTime, Message, Source, Destination, Note, Sort,pri,cat){
		var data=new objVlogRecord(DayTime, Message, Source, Destination, Note, Sort,pri,cat);
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
function objVlogRecord(DayTime, Message, Source, Destination, Note, Sort,pri,cat){
	this.daytime = DayTime;
	this.message = Message;
	this.source = Source;
	this.destination = Destination;
	this.note = Note;	
	this.sort = Sort;
	this.pri = pri;
	this.cat = cat;
}

//FIXME: function  objRptList item add "Percentage"
function objRptList()
{
	this.dataSet = new Array();
	this.period = 0 ;
	this.status = "off" ;
	this.addNewRecord = function(IP, User, Service, port, Website, Direction, Amount,Freq,Percent){		
		var data=new objRptRecord(IP, User, Service, port, Website, Direction, Amount,Freq,Percent);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, IP, User, Service, port, Website, Direction, Amount,Freq,Percent){
		var data=new objRptRecord(IP, User, Service, port, Website, Direction, Amount,Freq,Percent);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, IP, User, Service, port, Website, Direction, Amount,Freq,Percent){
		var data=new objRptRecord(IP, User, Service, port, Website, Direction, Amount,Freq,Percent);
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
function objRptRecord(IP, User, Service, port, Website, Direction, Amount,Freq,Percent){
	this.ip = IP;
	this.user = User;
	this.service = Service;
	this.port = port;
	this.website = Website;	
	this.direction = Direction;
	this.amount = Amount;
	this.freq = Freq;
	this.percent = Percent;
}

function objSessionList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(user, src, dest, proto, Rx, Tx, duration, service){		
		var data=new objSessionRecord(user, src, dest, proto, Rx, Tx, duration, service);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, user, src, dest, proto, Rx, Tx, duration, service){
		var data=new objSessionRecord(user, src, dest, proto, Rx, Tx, duration, service);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx,user, src, dest, proto, Rx, Tx, duration, service){
		var data=new objSessionRecord(user, src, dest, proto, Rx, Tx, duration, service);
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

function objSessionRecord(user, src, dest, proto, Rx, Tx, duration, service){
	this.user = user;
	this.src = src;
	this.dest = dest;
	this.proto = proto;
	this.rx = Rx;
	this.tx = Tx;
	this.duration = duration;
	this.service = service;
}

function objSessionUsrList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(flag,user, src, dest, proto, Rx, Tx, duration, service, subSrv,buffer1,buffer2){		
		var data=new objSessionUsrRecord(flag,user, src, dest, proto, Rx, Tx, duration, service, subSrv,buffer1,buffer2);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, flag,user, src, dest, proto, Rx, Tx, duration, service, subSrv,buffer1,buffer2){
		var data=new objSessionUsrRecord(flag,user, src, dest, proto, Rx, Tx, duration, service, subSrv,buffer1,buffer2);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx,flag,user, src, dest, proto, Rx, Tx, duration, service, subSrv,buffer1,buffer2){
		var data=new objSessionUsrRecord(flag,user, src, dest, proto, Rx, Tx, duration, service, subSrv,buffer1,buffer2);
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

function objSessionUsrRecord(flag,user, src, dest, proto, Rx, Tx, duration, service, subSrv,buffer1,buffer2){
	this.flag = flag;
	this.user = user;
	this.src = src;
	this.dest = dest;
	this.proto = proto;
	this.rx = Rx;
	this.tx = Tx;
	this.duration = duration;
	this.service = service;
	this.subSrv = new objSessionList();
	this.buffer1 = buffer1;
	this.buffer2 = buffer2;
}

 function objVLANList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(name, portvid,ipaddr, mask, active, addrT, gwip,port,
				     vlantag, vlandesc,upstream, downstream, mtu,metric,
				     dhcpflag, dhcpserver, dhcprelay1, dhcprelay2,member,dhcpIP){	
				     	
		var data=new objVLANRecord(name, portvid, ipaddr, mask, active, addrT, gwip,port,
				           vlantag, vlandesc,upstream, downstream, mtu,metric,
				           dhcpflag, dhcpserver, dhcprelay1, dhcprelay2,member,dhcpIP);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, name, portvid,ipaddr, mask, active, addrT, gwip,port,
				     vlantag, vlandesc,upstream, downstream, mtu,metric,
				     dhcpflag, dhcpserver, dhcprelay1, dhcprelay2,member,dhcpIP){
				 
		var data=new objVLANRecord(name, portvid,ipaddr, mask, active, addrT, gwip,port,
				           vlantag, vlandesc,upstream, downstream, mtu,metric,
				  	   dhcpflag, dhcpserver, dhcprelay1, dhcprelay2,member,dhcpIP);

		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, name, portvid,ipaddr, mask, active, addrT, gwip,port,
				          vlantag, vlandesc,upstream, downstream, mtu,metric,
				          dhcpflag, dhcpserver, dhcprelay1, dhcprelay2,member,dhcpIP){

		var data=new objVLANRecord(name, portvid, ipaddr, mask, active, addrT, gwip,port,
				           vlantag, vlandesc,upstream, downstream, mtu,metric,
				           dhcpflag, dhcpserver, dhcprelay1, dhcprelay2,member,dhcpIP);

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
function objVLANRecord(name, portvid, ipaddr, mask, active,addrT, gwip,port,
	               vlantag, vlandesc,upstream, downstream, mtu,metric,
								 dhcpflag, dhcpserver, dhcprelay1, dhcprelay2,member,dhcpIP){
	this.name = name;
	this.portvid = portvid;
	this.ipaddr = ipaddr;
	this.mask = mask;
	this.active = active;
	this.addrtype = addrT; //address type
	this.gwip = gwip; //gatway;
	this.port = port;
	this.vlantag = vlantag;
	this.vlandesc = vlandesc;
	this.upstream = upstream;
	this.downstream = downstream;
	this.mtu = mtu;
	this.metric = metric; 
	this.dhcpflag = dhcpflag;
	this.dhcpserver = dhcpserver;
	this.dhcprelay1 = dhcprelay1;
	this.dhcprelay2 = dhcprelay2;
	
	/*-- for record bind --*/
	this.member = new objVLANList();
	
	this.dhcpIP = dhcpIP;

}
 function objBridgeList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(active, ifacename, description, ipaddr, gateway, netmask, metric, upstream, downstream, mtu, iptype, relay1,relay2, dhcpstatus, member){		
		var data=new objBridgeRecord(active, ifacename, description, ipaddr, gateway, netmask, metric, upstream, downstream, mtu, iptype,relay1,relay2, dhcpstatus,member);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, active, ifacename, description, ipaddr, gateway, netmask, metric, upstream, downstream, mtu, iptype,relay1,relay2, dhcpstatus, member){
		var data=new objBridgeRecord(active, ifacename, description, ipaddr, gateway, netmask, metric, upstream, downstream, mtu, iptype,relay1,relay2, dhcpstatus, member);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, active, ifacename, description, ipaddr, gateway, netmask, metric, upstream, downstream, mtu, iptype,relay1,relay2, dhcpstatus, member){
		var data=new objBridgeRecord(active, ifacename, description, ipaddr, gateway, netmask, metric, upstream, downstream, mtu, iptype,relay1,relay2, dhcpstatus, member);
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
function objBridgeRecord(active, ifacename, description, ipaddr, gateway, netmask, metric, upstream, downstream, mtu, iptype,relay1,relay2,dhcpstatus, member){
	this.active = active;
	this.ifacename = ifacename;
	this.description = description;
	this.ipaddr = ipaddr;
	this.gateway = gateway;
	this.netmask = netmask;
	this.metric = metric;
	this.upstream = upstream;
	this.downstream = downstream;
	this.mtu = mtu;
	this.iptype = iptype;
	this.relay1 = relay1;
	this.relay2 = relay2;
	this.dhcpstatus = dhcpstatus;
	this.member = new objBridgeList();	
}
 
function objDHCPList()
{
	this.dataSet = new Array();
	
	this.addNewRecord = function(name, addr, mac, subnet, gateway, dns1, dns2, dns3,lease_day, lease_hr, lease_min, mb1, mb2){		
		var data=new objDHCPRecord(name, addr, mac, subnet, gateway, dns1, dns2, dns3,lease_day, lease_hr, lease_min, mb1, mb2);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx];	
	}		
	this.insertRecord = function(idx, name, addr, mac, subnet, gateway, dns1, dns2, dns3,lease_day, lease_hr, lease_min, mb1, mb2){
		var data=new objDHCPRecord(name, addr, mac, subnet, gateway, dns1, dns2, dns3,lease_day, lease_hr, lease_min, mb1, mb2);
		this.dataSet.splice(idx,0,data);
	}
	
	this.insertafterRecord = function(idx, name, addr, mac, subnet, gateway, dns1, dns2, dns3,lease_day, lease_hr, lease_min, mb1, mb2){
		var data=new objDHCPRecord(name, addr, mac, subnet, gateway, dns1, dns2, dns3,lease_day, lease_hr, lease_min, mb1, mb2);
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
function objDHCPRecord(name, addr, mac, subnet, gateway, dns1, dns2, dns3, lease_day, lease_hr, lease_min, mb1, mb2){
	this.name = name;
	this.addr = addr;
	this.mac = mac;
	this.subnet = subnet;
	this.gateway = gateway;
	this.dns1 = dns1;
	this.dns2 = dns2;
	this.dns3 = dns3;
	this.lease_day = lease_day; //for lease time
	this.lease_hr = lease_hr; //for lease time
	this.lease_min = lease_min; //for lease time
	this.wins1 = mb1; // first wins server
	this.wins2 = mb2; // second wins server
	
}
function objPPPoEList(){
	this.dataSet = new Array();
	this.addNewRecord = function(name, selnd, comms, acctprotocol, acctuname, acctsname, 
						autoip, baseip, upbwm, downbwm, mtu, ripdir, multicase, active, 
						multicasever, bipsel, apsel, baseiface, acctprofile, orig_active, curr_addr, schedule, connected, ipaddr, netmask, metric)
	{
		var data=new objPPPoERecord(name, selnd, comms, acctprotocol, acctuname, acctsname, 
						autoip, baseip, upbwm, downbwm, mtu, ripdir, multicase, active, 
						multicasever, bipsel, apsel, baseiface, acctprofile, orig_active, curr_addr, schedule, connected, ipaddr, netmask, metric);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}		
	this.insertRecord = function(idx, name, selnd, comms, acctprotocol, acctuname, acctsname, 
						autoip, baseip, upbwm, downbwm, mtu, ripdir, multicase, active, 
						multicasever, bipsel, apsel, baseiface, acctprofile, orig_active, curr_addr, schedule, connected, ipaddr, netmask, metric)
	{	
		var data=new objPPPoERecord(name, selnd, comms, acctprotocol, acctuname, acctsname, 
						autoip, baseip, upbwm, downbwm, mtu, ripdir, multicase, active, 
						multicasever, bipsel, apsel, baseiface, acctprofile, orig_active, curr_addr, schedule, connected, ipaddr, netmask, metric);
		this.dataSet.splice(idx,0,data);		
	}
	this.insertafterRecord = function(idx, name, selnd, comms, acctprotocol, acctuname, acctsname, 
						autoip, baseip, upbwm, downbwm, mtu, ripdir, multicase, active, 
						multicasever, bipsel, apsel, baseiface, acctprofile, orig_active, curr_addr, schedule, connected, ipaddr, netmask, metric)
	{	
		var data=new objPPPoERecord(name, selnd, comms, acctprotocol, acctuname, acctsname, 
						autoip, baseip, upbwm, downbwm, mtu, ripdir, multicase, active, 
						multicasever, bipsel, apsel, baseiface, acctprofile, orig_active, curr_addr, schedule, connected, ipaddr, netmask, metric);
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
		
function objPPPoERecord(name, selnd, comms, acctprotocol, acctuname, acctsname, 
						autoip, baseip, upbwm, downbwm, mtu, ripdir, multicase, active, 
						multicasever, bipsel, apsel, baseiface, acctprofile, orig_active, curr_addr, schedule, connected, ipaddr, netmask, metric)
{
	this.name = name;
	this.selnd = selnd;	
	this.comms = comms;
	this.baseiface = baseiface;
	this.acctprofile = acctprofile;
	//this.baseiface = new objBifList();
	//this.acctprofile = new objBifList();				
	this.bipsel = bipsel;
	this.apsel = apsel;
	this.acctprotocol = acctprotocol;
	this.acctuname = acctuname;
	this.acctsname = acctsname;
	//
	this.autoip = autoip;
	this.baseip = baseip;
	//
	this.upbwm = upbwm;
	this.downbwm = downbwm;
	this.mtu = mtu;
	//
	this.ripdir = ripdir;
	this.multicase = multicase;
	this.multicasever = multicasever;
	this.active = active;	
	this.orig_active = orig_active;	
	this.curr_addr = curr_addr ;
	this.schedule = schedule;
	this.connected = connected;//yes or no
	
	this.ipaddr	= ipaddr;
	this.netmask = netmask;
	this.metric = metric;
}
function objBifList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(item){		
		var data=new objBifRecord(item);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, item){
		var data=new objBifRecord(item);
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, item){
		var data=new objBifRecord(item);
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
function objBifRecord(item){
	this.item = item;
}
function objAcctList()
{
	this.dataSet = new Array();
	this.addNewRecord = function(profilename, authtype, uname, pw, rtpw, cntid, svrname, comp, idletime, period, albudget, type, servername, encryption ){		
		var data=new objAcctRecord(profilename, authtype, uname, pw, rtpw, cntid, svrname, comp, idletime, period, albudget, type, servername, encryption );
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}	
	this.insertRecord = function(idx, profilename, authtype, uname, pw, rtpw, cntid, svrname, comp, idletime, period, albudget, type, servername , encryption){
		var data=new objAcctRecord(profilename, authtype, uname, pw, rtpw, cntid, svrname, comp, idletime, period, albudget, type, servername, encryption );
		this.dataSet.splice(idx,0,data);
	}
	this.insertafterRecord = function(idx, profilename, authtype, uname, pw, rtpw, cntid, svrname, comp, idletime, period, albudget, type, servername, encryption ){
		var data=new objAcctRecord(profilename, authtype, uname, pw, rtpw, cntid, svrname, comp, idletime, period, albudget, type, servername, encryption );
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
function objAcctRecord(profilename, authtype, uname, pw, rtpw, cntid, svrname, comp, idletime, period, albudget, type, servername, encryption ){
	this.profilename = profilename;
	this.authtype = authtype;
	this.uname = uname;
	this.pw = pw;
	this.rtpw = rtpw;
	this.cntid = cntid;
	this.svrname = svrname;
	this.comp = comp;
	this.idletime = idletime;
	//budget
	this.period = period;
	this.albudget = albudget;
	this.type = type;
	this.servername = servername ;
	this.encryption = encryption;
}

function objAuxList(){
	this.dataSet = new Array();
	this.addNewRecord = function( comments, port_speed, dialing_type, init_string, phone_number, user_name, password, 
										auth_type, timeout, retry_count, retry_interval, active, idletime)	
	{
	var data=new objAuxRecord( comments, port_speed, dialing_type, init_string, phone_number, user_name, password, 
										auth_type, timeout, retry_count, retry_interval, active, idletime );
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}		
	this.insertRecord = function(idx, comments, port_speed, dialing_type, init_string, phone_number, user_name, password, 
										auth_type, timeout, retry_count, retry_interval, active, idletime)
	{	var data=new objAuxRecord( comments, port_speed, dialing_type, init_string, phone_number, user_name, password, 
										auth_type, timeout, retry_count, retry_interval, active, idletime );
		this.dataSet.splice(idx,0,data);		
	}
	this.insertafterRecord = function(idx, comments, port_speed, dialing_type, init_string, phone_number, user_name, password, 
										auth_type, timeout, retry_count, retry_interval, active, idletime)
	{	var data=new objAuxRecord( comments, port_speed, dialing_type, init_string, phone_number, user_name, password, 
										auth_type, timeout, retry_count, retry_interval, active, idletime );				
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
		
function objAuxRecord( comments, port_speed, dialing_type, init_string, phone_number, user_name, password, 
										auth_type, timeout, retry_count, retry_interval, active, idletime )
{
	this.comments = comments;
	this.port_speed = port_speed;
	this.dialing_type = dialing_type;
	this.init_string = init_string;
	this.phone_number = phone_number;
	this.user_name = user_name;
	this.password = password;
	this.auth_type = auth_type;
	this.timeout = timeout;
	this.retry_count = retry_count;
	this.retry_interval = retry_interval;
	this.active = active;
	this.idletime = idletime;
}
function objASList(){
	this.dataSet = new Array();
	this.addNewRecord = function( asenable, dropmailenable, dropmailstr, Regstatus, type){
		var data=new objASRecord( asenable, dropmailenable, dropmailstr , Regstatus, type);
		this.dataSet.push(data); 	
		this.dataSet[0].sublink.addNewRecord(protocol="SMTP", action="drop", subject="SPAM", active=true );
		this.dataSet[0].sublink.addNewRecord(protocol="POP3", action="mark", subject="SPAM", active=true );
		//this.dataSet[0].sublink.addNewRecord(protocol="IMAP4", action="mark", subject="SPAM", active=true );
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}		
	this.insertRecord = function(idx, asenable, dropmailenable, dropmailstr, Regstatus, type){
		var data=new objASRecord( asenable, dropmailenable, dropmailstr, Regstatus, type);
		this.dataSet.splice(idx,0,data);		
	}
	this.insertafterRecord = function(idx, asenable, dropmailenable, dropmailstr, Regstatus, type){
		var data=new objASRecord( asenable, dropmailenable, dropmailstr, Regstatus, type);
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
		
function objASRecord( asenable, dropmailenable, dropmailstr, Regstatus, type )
{
	this.asenable = asenable;
	this.dropmailenable = dropmailenable;
	this.dropmailstr = dropmailstr;
	this.Regstatus = Regstatus;
	this.type = type;
	this.sublink = new objMailptlList();
	this.zonelink = new objASZoneList();
}
function objAVList(){
	this.dataSet = new Array();
	this.addNewRecord = function(mb1, mb2, mb3, mb4, mb5, mb6, mb7,	mb8, mb9, mb10, mb11, mb12,mb13, mb14, mb15){
		var data=new objAVRecord(mb1, mb2, mb3, mb4, mb5, mb6, mb7,	mb8, mb9, mb10, mb11, mb12,mb13, mb14, mb15);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}		
	this.insertRecord = function(idx, mb1, mb2, mb3, mb4, mb5, mb6, mb7,	mb8, mb9, mb10, mb11, mb12,mb13, mb14, mb15){
		var data=new objAVRecord(mb1, mb2, mb3, mb4, mb5, mb6, mb7,	mb8, mb9, mb10, mb11, mb12,mb13, mb14, mb15);
		this.dataSet.splice(idx,0,data);		
	}
	this.insertafterRecord = function(idx, mb1, mb2, mb3, mb4, mb5, mb6, mb7,	mb8, mb9, mb10, mb11, mb12,mb13, mb14, mb15){
		var data=new objAVRecord(mb1, mb2, mb3, mb4, mb5, mb6, mb7,	mb8, mb9, mb10, mb11, mb12,mb13, mb14, mb15);
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

function objAVRecord(mb1, mb2, mb3, mb4, mb5, mb6, mb7,	mb8, mb9, mb10, mb11, mb12,mb13, mb14, mb15)
{
	this.active = mb1;
	this.log    = mb2;
	this.fzone  = mb3;
	this.tzone  = mb4;
	this.sphttp = mb5;
	this.spftp  = mb6;
	this.sppop3 = mb7;
	this.spsmtp = mb8;
	this.mb9    = mb9;
	this.mb10   = mb10;
	this.mb11   = mb11;
	this.mb12   = mb12;
	this.mb13   = mb13;
	this.mb14   = mb14;
	this.spimap = mb15;
}
function objavSettingList(){
	this.dataSet = new Array();
	this.addNewRecord = function(fileexts, filesizelimit, oversize, infectedfile, uncleanfile, oversizeattach, virusattach){
		var data=new objavSettingRecord(fileexts, filesizelimit, oversize, infectedfile, uncleanfile, oversizeattach, virusattach);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}		
	this.insertRecord = function(idx, fileexts, filesizelimit, oversize, infectedfile, uncleanfile, oversizeattach, virusattach){
		var data=new objavSettingRecord(fileexts, filesizelimit, oversize, infectedfile, uncleanfile, oversizeattach, virusattach);
		this.dataSet.splice(idx,0,data);		
	}
	this.insertafterRecord = function(idx, fileexts, filesizelimit, oversize, infectedfile, uncleanfile, oversizeattach, virusattach){
		var data=new objavSettingRecord(fileexts, filesizelimit, oversize, infectedfile, uncleanfile, oversizeattach, virusattach);
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
		
function objavSettingRecord( fileexts, filesizelimit, oversize, infectedfile, uncleanfile, oversizeattach, virusattach)
{
	this.fileexts = fileexts;
	this.filesizelimit = filesizelimit;
	this.oversize = oversize;
	this.infectedfile = infectedfile;
	this.uncleanfile = uncleanfile;
	this.oversizeattach = oversizeattach;
	this.virusattach = virusattach;
}
function objASZoneList(){
	this.dataSet = new Array();
	this.addNewRecord = function(inzone, outzone, inspectmail, active, score, log){
		var data=new objASZoneRecord(inzone, outzone, inspectmail, active, score, log);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}		
	this.insertRecord = function(idx, inzone, outzone, inspectmail, active, score, log){	
		var data=new objASZoneRecord(inzone, outzone, inspectmail, active, score, log);
		this.dataSet.splice(idx,0,data);		
	}
	this.insertafterRecord = function(idx, inzone, outzone, inspectmail, active, score, log ){	
		var data=new objASZoneRecord(inzone, outzone, inspectmail, active, score, log);				
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

function objASZoneRecord( inzone, outzone, inspectmail, active, score, log ){
	this.inzone = inzone;
	this.outzone = outzone;
	this.inspectmail = inspectmail;
	this.score =score;
	this.log = log;
	this.active = active;
	this.orig_active = active;
}

function objMailptlList(){
	this.dataSet = new Array();
	this.addNewRecord = function(protocol, action, subject, active ){
		var data=new objMailptlRecord(protocol, action, subject, active );
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}		
	this.insertRecord = function(idx, protocol, action, subject, active){
		var data=new objMailptlRecord(protocol, action, subject, active);
		this.dataSet.splice(idx,0,data);		
	}
	this.insertafterRecord = function(idx, protocol, action, subject, active){	
		var data=new objMailptlRecord( protocol, action, subject, active);				
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
		
function objMailptlRecord( protocol, action, subject, active )
{
	this.protocol = protocol;
	this.action = action;
	this.subject = subject;
	this.active = active;
}
function objWBList(){
	this.dataSet = new Array();
	this.addNewRecord = function(ewhite, eblack ){
		var data=new objWBRecord(ewhite, eblack );
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}		
	this.insertRecord = function(idx, ewhite, eblack ){
		var data=new objWBRecord(ewhite, eblack );
		this.dataSet.splice(idx,0,data);		
	}
	this.insertafterRecord = function(idx, ewhite, eblack ){	
		var data=new objWBRecord(ewhite, eblack);				
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
		
function objWBRecord( ewhite, eblack )
{
	this.ewhite = ewhite;
	this.eblack = eblack;
	this.subwhite = new objsubbwList();// white list
	this.subblack = new objsubbwList();//black list
}
function objsubbwList(){
	this.dataSet = new Array();
	this.addNewRecord = function(list, type, emailaddr, ipaddr, network, mailheader, value, active, orig_active ){
		var data=new objsubbwRecord(list, type, emailaddr, ipaddr, network, mailheader, value, active, orig_active );
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}		
	this.insertRecord = function(idx, list, type, emailaddr, ipaddr, network, mailheader, value, active, orig_active ){
		var data=new objsubbwRecord(list, type, emailaddr, ipaddr, network, mailheader, value, active, orig_active );
		this.dataSet.splice(idx,0,data);		
	}
	this.insertafterRecord = function(idx, list, type, emailaddr, ipaddr, network, mailheader, value, active, orig_active ){	
		var data=new objsubbwRecord(list, type, emailaddr, ipaddr, network, mailheader, value, active, orig_active );				
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
		
function objsubbwRecord( list, type, emailaddr, ipaddr, network, mailheader, value, active, orig_active )
{
	this.list = list;
	this.type = type;
	this.emailaddr = emailaddr;
	this.ipaddr = ipaddr;
	this.network = network;
	this.mailheader = mailheader;
	this.value = value;
	this.active = active;
	this.orig_active = orig_active ;
}
function objRBList(){
	this.dataSet = new Array();
	this.addNewRecord = function(enale, maxquery){
		var data=new objrbRecord(enale, maxquery);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}		
	this.insertRecord = function(idx, enale, maxquery){
		var data=new objrbRecord(enale, maxquery);
		this.dataSet.splice(idx,0,data);		
	}
	this.insertafterRecord = function(idx, enale, maxquery){	
		var data=new objrbRecord(enale, maxquery);				
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
		
function objrbRecord( enale, maxquery)
{
	this.enale = enale;
	this.maxquery = maxquery;	
	this.subrbl = new objrbllist();// white list
}
function objrbllist(){
	this.dataSet = new Array();
	this.addNewRecord = function(svraddr, svrquery, active, orig_active){
		var data=new objrblRecord(svraddr, svrquery, active, orig_active);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}		
	this.insertRecord = function(idx, svraddr, svrquery, active, orig_active){
		var data=new objrblRecord(svraddr, svrquery, active, orig_active);
		this.dataSet.splice(idx,0,data);		
	}
	this.insertafterRecord = function(idx, svraddr, svrquery, active, orig_active){	
		var data=new objrblRecord(svraddr, svrquery, active, orig_active);				
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
		
function objrblRecord( svraddr, svrquery, active, orig_active)
{
	this.svraddr = svraddr;
	this.svrquery = svrquery;	
	this.active = active;
	this.orig_active = active;
}
function objAplPatrolList(){
	this.dataSet = new Array();
	this.addNewRecord = function(mb1, mb2, mb3, mb4, mb5, mb6, mb7, mb8, mb9, mb10, mb11){
		var data=new objAplPatrolRecord(mb1, mb2, mb3, mb4, mb5, mb6, mb7, mb8, mb9, mb10, mb11);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}		
	this.insertRecord = function(idx, mb1, mb2, mb3, mb4, mb5, mb6, mb7, mb8, mb9, mb10, mb11){
		var data=new objAplPatrolRecord(mb1, mb2, mb3, mb4, mb5, mb6, mb7, mb8, mb9, mb10, mb11);
		this.dataSet.splice(idx,0,data);		
	}
	this.insertafterRecord = function(idx, mb1, mb2, mb3, mb4, mb5, mb6, mb7, mb8, mb9, mb10, mb11){	
		var data=new objAplPatrolRecord(mb1, mb2, mb3, mb4, mb5, mb6, mb7, mb8, mb9, mb10, mb11);				
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
		
function objAplPatrolRecord(mb1, mb2, mb3, mb4, mb5, mb6, mb7, mb8, mb9, mb10, mb11)
{
	this.srv      = mb1;
	this.access   = mb2;
	this.classify = mb3;
	this.activate = mb4;
	this.bwGraph  = mb5;
	this.bwm      = mb6; // for Instant Messenger
	this.login    = mb7; // for Instant Messenger
	this.msg      = mb8; // for Instant Messenger
	this.audio    = mb9; // for Instant Messenger
	this.video    = mb10;// for Instant Messenger
	this.ft       = mb11;// for Instant Messenger
	this.rule     = new objexList();
	this.defaultPort = new objapList();
}
function objapList(){
	this.dataSet = new Array();
	this.addNewRecord = function(number, portnumber){
		var data=new objapRecord(number, portnumber);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}		
	this.insertRecord = function(idx, number, portnumber){
		var data=new objapRecord(number, portnumber);
		this.dataSet.splice(idx,0,data);		
	}
	this.insertafterRecord = function(idx, number, portnumber){	
		var data=new objapRecord(number, portnumber);				
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
		
function objapRecord( number, portnumber)
{
	this.number = number;
	this.portnumber = portnumber;	
}

function objexList(){
	this.dataSet = new Array();
	this.addNewRecord = function(mb1, mb2, mb3, mb4, mb5, mb6, mb7, mb8, mb9, mb10, mb11, mb12, mb13, mb14, mb15, mb16, mb17, mb18, mb19, mb20, mb21, mb22){
		var data=new objexRecord(mb1, mb2, mb3, mb4, mb5, mb6, mb7, mb8, mb9, mb10, mb11, mb12, mb13, mb14, mb15, mb16, mb17, mb18, mb19, mb20, mb21, mb22);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}		
	this.insertRecord = function(idx, mb1, mb2, mb3, mb4, mb5, mb6, mb7, mb8, mb9, mb10, mb11, mb12, mb13, mb14, mb15, mb16, mb17, mb18, mb19, mb20, mb21, mb22){
		var data=new objexRecord(mb1, mb2, mb3, mb4, mb5, mb6, mb7, mb8, mb9, mb10, mb11, mb12, mb13, mb14, mb15, mb16, mb17, mb18, mb19, mb20, mb21, mb22);
		this.dataSet.splice(idx,0,data);		
	}
	this.insertafterRecord = function(idx, mb1, mb2, mb3, mb4, mb5, mb6, mb7, mb8, mb9, mb10, mb11, mb12, mb13, mb14, mb15, mb16, mb17, mb18, mb19, mb20, mb21, mb22){	
		var data=new objexRecord(mb1, mb2, mb3, mb4, mb5, mb6, mb7, mb8, mb9, mb10, mb11, mb12, mb13, mb14, mb15, mb16, mb17, mb18, mb19, mb20, mb21, mb22);				
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
		
function objexRecord(mb1, mb2, mb3, mb4, mb5, mb6, mb7, mb8, mb9, mb10, mb11, mb12, mb13, mb14, mb15, mb16, mb17, mb18, mb19, mb20, mb21, mb22)
{
	this.name   = mb1;
	this.port   = mb2;
	this.sch    = mb3;
	this.usr    = mb4;
	this.fzone  = mb5;
	this.tzone  = mb6;
	this.src    = mb7;
	this.dest   = mb8;
	this.access = mb9;
	this.bwmIn  = mb10;
	this.bwmOut = mb11;
	this.bwmPri = mb12;
	this.bwmUsg = mb13;
	this.log    = mb14;
	this.login  = mb15;
	this.msg    = mb16;
	this.audio  = mb17;
	this.video  = mb18;
	this.ft     = mb19;
	this.proto  = mb20;
	this.activate  = mb21;
	this.activate2 = mb22;
}
function objAplOtherList(){
	this.dataSet = new Array();
	this.addNewRecord = function(number, runport, schedule, user, source, destination, protocol, access, log, alert){
		var data=new objAplOtherRecord(number, runport, schedule, user, source, destination, protocol, access, log, alert);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}		
	this.insertRecord = function(idx, number, runport, schedule, user, source, destination, protocol, access, log, alert){
		var data=new objAplOtherRecord(number, runport, schedule, user, source, destination, protocol, access, log, alert);
		this.dataSet.splice(idx,0,data);		
	}
	this.insertafterRecord = function(idx, number, runport, schedule, user, source, destination, protocol, access, log, alert){	
		var data=new objAplOtherRecord(number, runport, schedule, user, source, destination, protocol, access, log, alert);				
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
		
function objAplOtherRecord(number, runport, schedule, user, source, destination, protocol, access, log, alert)
{
	this.number = number;
	this.runport = runport;
	this.schedule = schedule;
	this.user = user;
	this.source = source;
	this.destination = destination;
	this.protocol = protocol;
	this.access = access;
	this.log = log;
	this.alert = alert;
}
function objHomeList(){
	this.dataSet = new Array();
	this.addNewRecord = function(name, status, hastatus, zone, ipaddr, subnetmask, ipassign, services, rndial, flag){
		var data=new objHomeRecord(name, status, hastatus, zone, ipaddr, subnetmask, ipassign, services, rndial, flag);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}		
	this.insertRecord = function(idx, name, status, hastatus, zone, ipaddr, subnetmask, ipassign, services, rndial, flag){
		var data=new objHomeRecord(name, status, hastatus, zone, ipaddr, subnetmask, ipassign, services, rndial, flag);
		this.dataSet.splice(idx,0,data);		
	}
	this.insertafterRecord = function(idx, name, status, hastatus, zone, ipaddr, subnetmask, ipassign, services, rndial, flag){	
		var data=new objHomeRecord(name, status, hastatus, zone, ipaddr, subnetmask, ipassign, services, rndial, flag);				
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
		
function objHomeRecord(name, status, hastatus, zone, ipaddr, subnetmask, ipassign, services, rndial, flag)
{
	this.name = name;
	this.status = status;
	this.hastatus = hastatus;
	this.zone = zone;
	this.ipaddr = ipaddr;
	this.subnetmask = subnetmask;
	this.ipassign = ipassign;
	this.services = services;
	this.rndial = rndial;
	this.flag = flag;
}
function objEditList(){
	this.dataSet = new Array();
	this.addNewRecord = function(objindex, subindex, sid, info){
		var data=new objEditRecord(objindex, subindex, sid, info);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}		
	this.insertRecord = function(idx, objindex, subindex, sid, info){
		var data=new objEditRecord(objindex, subindex, sid, info);
		this.dataSet.splice(idx,0,data);		
	}	
	this.deleteRecord = function(idx){				
		return this.dataSet.splice(idx,1); 
	}
	this.dataCount = function(){		
		return this.dataSet.length;
	}		
}
		
function objEditRecord(objindex, subindex, sid, info)
{
	this.objindex = objindex;
	this.subindex = subindex;
	this.sid = sid;
	this.info = info; /* activation, log, action */
}

function objVrrpGrpList(){
	this.dataSet = new Array();
	this.addNewRecord = function(name, vrid, role, iface, priority, preempt, manage_ip, manage_mask, 
							authentication, key_string, comment, devhastatus, active){
		var data=new objVrrpGrpRecord(name, vrid, role, iface, priority, preempt, manage_ip, manage_mask, 
							authentication, key_string, comment, devhastatus, active);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}		
	this.insertRecord = function(idx,name, vrid, role, iface, priority, preempt, manage_ip, manage_mask, 
							authentication, key_string, comment, devhastatus, active){
		var data=new objVrrpGrpRecord(name, vrid, role, iface, priority, preempt, manage_ip, manage_mask, 
							authentication, key_string, comment, devhastatus, active);
		this.dataSet.splice(idx,0,data);		
	}	
	this.deleteRecord = function(idx){				
		return this.dataSet.splice(idx,1); 
	}
	this.dataCount = function(){		
		return this.dataSet.length;
	}		
}
		
function objVrrpGrpRecord(name, vrid, role, iface, priority, preempt, manage_ip, manage_mask, 
							authentication, key_string, comment, devhastatus, active)
{
	this.name = name;
	this.vrid = vrid;
	this.role = role;
	this.iface = iface;
	this.priority = priority;
	this.preempt = preempt;
	this.manage_ip = manage_ip;
	this.authentication = authentication;
	this.key_string = key_string;
	this.manage_mask = manage_mask;
	this.comment = comment;
	this.devhastatus = devhastatus;
	this.active = active;
}
function objDevHaSyncList(){
	this.dataSet = new Array();
	this.addNewRecord = function(password, syncfrom, syncport, autosync, syncitv){
		var data=new objDevHaSyncRecord(password, syncfrom, syncport, autosync, syncitv);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}		
	this.insertRecord = function(idx,password, syncfrom, syncport, autosync, syncitv){
		var data=new objDevHaSyncRecord(password, syncfrom, syncport, autosync, syncitv);
		this.dataSet.splice(idx,0,data);		
	}	
	this.deleteRecord = function(idx){				
		return this.dataSet.splice(idx,1); 
	}
	this.dataCount = function(){		
		return this.dataSet.length;
	}		
}
		
function objDevHaSyncRecord(password, syncfrom, syncport, autosync, syncitv)
{
	this.password = password;
	this.syncfrom = syncfrom;
	this.syncport = syncport;
	this.autosync = autosync;
	this.syncitv = syncitv;
}
function objDDNSList(){
	this.dataSet = new Array();
	this.addNewRecord = function(profile,srvType,host,upPolicy,wildcard,waniface,haiface){
		var data=new objDDNSRecord(profile,srvType,host,upPolicy,wildcard,waniface,haiface);
		this.dataSet.push(data); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}		
	this.insertRecord = function(idx, profile,srvType,host,upPolicy,wildcard,waniface,haiface){
		var data=new objDDNSRecord(profile,srvType,host,upPolicy,wildcard,waniface,haiface);
		this.dataSet.splice(idx,0,data);		
	}
	this.insertafterRecord = function(idx, profile,srvType,host,upPolicy,wildcard,waniface,haiface){	
		var data=new objDDNSRecord(profile,srvType,host,upPolicy,wildcard,waniface,haiface);				
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

function objDDNSRecord(profile,srvType,host,upPolicy,wildcard,waniface,haiface)
{
	this.profile=profile;
	this.srvType=srvType;
	this.host=host;
	this.upPolicy=upPolicy;
	this.wildcard=wildcard;
	this.waniface=waniface;
	this.haiface=haiface;
}

function objDDNS(profile,username,password, srvType,host,upPolicy,myip,wildcard,waniface,haiface,backmx,mx)
{
	this.profile=profile ;
	this.username=username;
	this.password=password;
	this.srvType=srvType;
	this.host=host;
	this.upPolicy=upPolicy;
	this.myip=myip;
	this.wildcard=wildcard;
	this.waniface=waniface;
	this.haiface=haiface;
	this.backmx=backmx;
	this.mx=mx;
}

function objDDNSIfaceList(){
	this.dataSet = new Array();
	this.addNewRecord = function(iface){
		this.dataSet.push(iface); 	
	}
	this.getRecord = function(Idx){	
		return this.dataSet[Idx]; 				
	}		
	this.dataCount = function(){		
		return this.dataSet.length;
	}		
}

