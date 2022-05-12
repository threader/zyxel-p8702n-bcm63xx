function checkIPFormat(address){
var IP = address.value;
if (IP == "N/A")
	return;
IP = IP.replace(/ /g, "");
address.value = IP;
IPsplit = IP.split(".");
if(IPsplit.length != 4 ){
		alert("IP Format Error!!!");
		address.focus();
		address.value="0.0.0.0";
	return (false);
}
for(i = 0; i < 4; i++)
	if((isNaN(IPsplit[i])) || (IPsplit[i] == 'undefined') || (IPsplit[i] == "")) {
			alert("IP Format Error!!!");
			address.focus();
			address.value="0.0.0.0";
		return (false);
	} else{
		if((parseInt(IPsplit[i], 10) > 255) || (parseInt(IPsplit[i], 10) < 0)) {
				alert("IP Format Error!!!");
				address.focus();
				address.value="0.0.0.0";	
			return (false);
		}
	}
}
function SubnetCheck(mask){
	if ( !((mask.value == "0.0.0.0")||
	(mask.value == "128.0.0.0")||
	(mask.value == "192.0.0.0")||
	(mask.value == "224.0.0.0")||
	(mask.value == "240.0.0.0")||
	(mask.value == "248.0.0.0")||
	(mask.value == "252.0.0.0")||
	(mask.value == "254.0.0.0")||
	(mask.value == "255.0.0.0")||
	(mask.value == "255.128.0.0")||
	(mask.value == "255.192.0.0")||
	(mask.value == "255.224.0.0")||
	(mask.value == "255.240.0.0")||
	(mask.value == "255.248.0.0")||
	(mask.value == "255.252.0.0")||
	(mask.value == "255.254.0.0")||
	(mask.value == "255.255.0.0")||
	(mask.value == "255.255.128.0")||
	(mask.value == "255.255.192.0")||
	(mask.value == "255.255.224.0")||
	(mask.value == "255.255.240.0")||
	(mask.value == "255.255.248.0")||
	(mask.value == "255.255.252.0")||
	(mask.value == "255.255.254.0")||
	(mask.value == "255.255.255.0")||
	(mask.value == "255.255.255.128")||
	(mask.value == "255.255.255.192")||
	(mask.value == "255.255.255.224")||
	(mask.value == "255.255.255.240")||
	(mask.value == "255.255.255.248")||
	(mask.value == "255.255.255.252")||
	(mask.value == "255.255.255.254")||
	(mask.value == "255.255.255.255")) )
	{	alert("Subnet Mask Format Error!!!");		mask.value = "0.0.0.0";
	}    
}

function doHexCheck(c){
 if ( (c >= "0") && (c <= "9") )
  return 1;
 else if ( (c >= "A") && (c <= "F") )
  return 1;
 else if ( (c >= "a") && (c <= "f") )
  return 1;

 return -1;
}
function doMACcheck(object){
var szAddr = object.value;
var len = szAddr.length;

if ( len == 0 )
return;

if ( len == 12 ){
	var newAddr = "";
	var i = 0;

	for ( i = 0; i < len; i++ )
	{
	var c = szAddr.charAt(i);
	
	if ( doHexCheck(c) < 0 )
	{
alert("Invalid MAC Address");		object.value ="00:00:00:00:00:00";
		object.focus();
		return;
	}
	if ( (i == 2) || (i == 4) || (i == 6) || (i == 8) || (i == 10) )
		newAddr = newAddr + ":";
		newAddr = newAddr + c;
	}
	object.value = newAddr;
	return;
}
else if ( len == 17 ){
	var i = 2;
	var c0 = szAddr.charAt(0);
	var c1 = szAddr.charAt(1);
	
	if ( (doHexCheck(c0) < 0) || (doHexCheck(c1) < 0) )
	{
		alert("Invalid MAC Address");		object.value ="00:00:00:00:00:00";
		object.focus();
		return;	
	}

	i = 2;
	while ( i < len )
	{
		var c0 = szAddr.charAt(i);
		var c1 = szAddr.charAt(i+1);
		var c2 = szAddr.charAt(i+2);  
		if ( (c0 != ":") || (doHexCheck(c1) < 0) || (doHexCheck(c2) < 0) )
		{
alert("Invalid MAC Address");		object.value ="00:00:00:00:00:00";
		object.focus();
		return;
		}
		i = i + 3;
	}
return; 
}
else{
alert("Invalid MAC Address");		object.value ="00:00:00:00:00:00";
	 	object.focus();
	return;
	}
}	
function chk_num(event){
	if (event.keyCode < 48 || event.keyCode > 57 ) /* 0-9 */
		event.returnValue = false;
}
function chk_phonenum(event){
	if(event.keyCode == 44)/* ,  */
		event.returnValue = true;
	else if (event.keyCode < 48 || event.keyCode > 57 ) /* 0-9 */
		event.returnValue = false;
}
function chk_chtNum(event){
return;
}
function chkName(event){
	var i=0;        
	var len=0;       
	var ch;          

	len=event.value.length;     
	if (event.value == "")
	{
		alert("Please input Name.");		} else{                  
		for(i=0;i<len;i++){
		ch=event.value.charAt(i);
		if (!(ch == '-' || (ch >= '0' && ch <= '9' )||(ch >= 'a' && ch <= 'z')||ch == '_' || (ch >= 'A' && ch <= 'Z' )||ch=='@') ) /* 0-9,a-z,A-Z,-,_,@ */
		{
	   		alert("format error,Please enter 0-9, a-z, A-Z, '-', '_'or'@'");
			event.value='';
			return false;
			}
		}  
	}
}

function chk_chtNumUserName(event){
return;
}	
function chk_CR(event){	if (event.keyCode == 13) /* 0-9 */
		document.forms[0].submit();
}
function HourCheck(I){	d =parseInt(I.value, 10);
	if ( !(d<24 && d>=0) )
	{		alert('Hour value is out of range [0 - 23]');			I.value = I.defaultValue;
	}
}
function MinCheck(I){	d =parseInt(I.value, 10);
	if ( !(d<60 && d>=0) )
	{		alert('Minute value is out of range [0 - 59]');			I.value = I.defaultValue;
	}
}
function showFullPath(str){	fr =2;
	parent.frames[fr].document.open();
	parent.frames[fr].document.writeln(' <html><head><meta http-equiv=\"Content-Type\" content=\"text\/html; charset=iso-8859-1\">\r\n<style type=\"text\/css\">\r\n<!-- ');
	parent.frames[fr].document.writeln(' body {\r\n\tfont-family: Verdana, Arial, Helvetica, sans-serif;\tcolor: #FFFFFF;\tmargin: 0px;\tfont-weight: bold;}');
	parent.frames[fr].document.writeln(' .path {\tfont-family: Verdana, Arial, Helvetica, sans-serif;\tfont-size: 11px;\tfont-weight: bold;\tcolor: #FFFFFF;}\r\n-->\r\n</style>');	
	parent.frames[fr].document.writeln(' <\/head><body background=\"Images\/full_path_bg.gif\"><table width=\"100%\"  border=\"0\" cellspacing=\"0\" cellpadding=\"0\"><tr><td height=\"19\" class=\"path\">&nbsp;<img src=\"Images\/dot_2.gif\" width=\"12\" height=\"12\"  align=\"absmiddle\">&nbsp;'+str+'<\/td><\/tr><\/table><\/body><\/html>');	
	parent.frames[fr].document.close();
}
function MM_swapImage(){
	var i,j=0,x,a=MM_swapImage.arguments; document.MM_sr=new Array; for(i=0;i<(a.length-2);i+=3)
	if ((x=MM_findObj(a[i]))!=null){document.MM_sr[j++]=x; if(!x.oSrc) x.oSrc=x.src; x.src=a[i+2];}
}
function MM_swapImgRestore(){
	var i,x,a=document.MM_sr; for(i=0;a&&i<a.length&&(x=a[i])&&x.oSrc;i++) x.src=x.oSrc;
}
function MM_preloadImages(){
	var d=document; if(d.images){ if(!d.MM_p) d.MM_p=new Array();
	var i,j=d.MM_p.length,a=MM_preloadImages.arguments; for(i=0; i<a.length; i++)
	if (a[i].indexOf("#")!=0){ d.MM_p[j]=new Image; d.MM_p[j++].src=a[i];}}
}
function MM_findObj(n, d){
	var p,i,x;  if(!d) d=document; if((p=n.indexOf("?"))>0&&parent.frames.length) {
	d=parent.frames[n.substring(p+1)].document; n=n.substring(0,p);}
	if(!(x=d[n])&&d.all) x=d.all[n]; for (i=0;!x&&i<d.forms.length;i++) x=d.forms[i][n];
	for(i=0;!x&&d.layers&&i<d.layers.length;i++) x=MM_findObj(n,d.layers[i].document);
	if(!x && d.getElementById) x=d.getElementById(n); return x;
}