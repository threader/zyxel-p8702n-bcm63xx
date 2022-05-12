
function checkOS() {
  if(navigator.userAgent.indexOf('Linux') != -1)
    { var OpSys = "Linux"; }
  else if((navigator.userAgent.indexOf('Win') != -1) &&
  (navigator.userAgent.indexOf('95') != -1))
    { var OpSys = "Windows95"; }
  else if(navigator.userAgent.indexOf('Win') != -1)
    { var OpSys = "Windows3.1 or NT"; }
  else if(navigator.userAgent.indexOf('Mac') != -1)
    { var OpSys = "Macintosh"; }
  else { var OpSys = "other"; }
  return OpSys;
}

var OpSys = checkOS();



// convert all characters to lowercase to simplify testing 
var agt=navigator.userAgent.toLowerCase(); 
// *** BROWSER VERSION *** 
// note: on IE5, these return 4, so use is_ie5up to detect IE5. 
var is_major = parseInt(navigator.appVersion); 
var is_minor = parseFloat(navigator.appVersion); 

var is_nav  = ((agt.indexOf('mozilla')!=-1) && (agt.indexOf('spoofer')==-1) 
            && (agt.indexOf('compatible') == -1) && (agt.indexOf('opera')==-1) 
            && (agt.indexOf('webtv')==-1)); 
var is_nav4 = (is_nav && (is_major == 4)); 
var is_nav4up = (is_nav && (is_major >= 4)); 
var is_navonly      = (is_nav && ((agt.indexOf(";nav") != -1) || 
                      (agt.indexOf("; nav") != -1)) ); 
var is_nav5 = (is_nav && (is_major == 5)); 
var is_nav5up = (is_nav && (is_major >= 5)); 

var is_ie   = (agt.indexOf("msie") != -1); 
var is_ie3  = (is_ie && (is_major < 4)); 
var is_ie4  = (is_ie && (is_major == 4) && (agt.indexOf("msie 5.0")==-1) ); 
var is_ie4up  = (is_ie  && (is_major >= 4)); 
var is_ie5  = (is_ie && (is_major == 4) && (agt.indexOf("msie 5.0")!=-1) ); 
var is_ie5up  = (is_ie  && !is_ie3 && !is_ie4); 
var isIE4, isIE4PC, isNav4PC, isNav4, isNav3;

if (is_ie4 || is_ie4up || is_nav5 || is_nav5up) {
        isIE4 = true;
        if (OpSys != "Macintosh") {
                isIE4PC = true;
        } 
} else if (is_nav4) {
        isNav4 = true;
        if (OpSys != "Macintosh") {
                isNav4PC = true;
        } 
} else if  (is_nav) {
        isNav3 = true;  
}


if (OpSys == "Linux") {
        document.write('<link rel="stylesheet" type="text/css" href="content_linux.css">');
} else {
        document.write('<link rel="stylesheet" type="text/css" href="content_ie.css">');
        if (isNav4PC) {
                document.write('<link rel="stylesheet" type="text/css" href="content_nc.css">');
        }
}

	function checkIPFormat(address) {
		var IP = address.value;
		if (IP == "N/A")
			return;
		IP = IP.replace(/ /g, "");
		address.value = IP;
		IPsplit = IP.split(".");
		if(IPsplit.length != 4 ) {
			if(is_ie) {
				alert("IP Format Error!!!");
				address.focus();
				address.value="0.0.0.0";
			}	
			return (false);
		}

		for(i = 0; i < 4; i++)
			if((isNaN(IPsplit[i])) || (IPsplit[i] == 'undefined') || (IPsplit[i] == "")) {
				if(is_ie) {
					alert("IP Format Error!!!");
					address.focus();
					address.value="0.0.0.0";
				}
				return (false);
			} else
				if((parseInt(IPsplit[i], 10) > 255) || (parseInt(IPsplit[i], 10) < 0)) {
					if(is_ie) {
						alert("IP Format Error!!!");
						address.focus();
						address.value="0.0.0.0";
					}	
					return (false);
				}
	}

	function SubnetCheck(mask)
	{	
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
		{		alert("Subnet Mask Format Error!!!");	    	 	mask.value = "0.0.0.0";
		}    
	}

    function doHexCheck(c)
    {
      if ( (c >= "0") && (c <= "9") )
        return 1;
      else if ( (c >= "A") && (c <= "F") )
        return 1;
      else if ( (c >= "a") && (c <= "f") )
        return 1;

      return -1;
    }

    function doMACcheck(object)
    {
      var szAddr = object.value;
      var len = szAddr.length;

      if ( len == 0 )
        return;

      if ( len == 12 )
      {
        var newAddr = "";
        var i = 0;

        for ( i = 0; i < len; i++ )
        {
          var c = szAddr.charAt(i);
          
          if ( doHexCheck(c) < 0 )
          {
          alert("Invalid MAC Address");          	object.value ="00:00:00:00:00:00";
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
      else if ( len == 17 )
      {
        var i = 2;
        var c0 = szAddr.charAt(0);
        var c1 = szAddr.charAt(1);

        if ( (doHexCheck(c0) < 0) || (doHexCheck(c1) < 0) )
        {
           alert("Invalid MAC Address");          	object.value ="00:00:00:00:00:00";	
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
             alert("Invalid MAC Address");          		object.value ="00:00:00:00:00:00";	
             	object.focus();
            return;
          }
          i = i + 3;
        }
        return; 
      }
      else
      {
      alert("Invalid MAC Address");      		object.value ="00:00:00:00:00:00";	
         	object.focus();
        return;
      }
    }	
    
function chk_num(event)
{
	if (event.keyCode < 48 || event.keyCode > 57 ) /* 0-9 */
		event.returnValue = false;
}

function chk_phonenum(event)
{
	 if(event.keyCode == 44)/* ,  */
     event.returnValue = true;
	else if (event.keyCode < 48 || event.keyCode > 57 ) /* 0-9 */
		event.returnValue = false;
}


function chk_chtNum(event)
{
return;
}

function chk_chtNumUserName(event)
{
return;
}	
function chk_CR(event)
{
	if (event.keyCode == 13) /* 0-9 */
		document.forms[0].submit();
}
function HourCheck(I)
{	d =parseInt(I.value, 10);
	if ( !(d<24 && d>=0) )
	{		alert('Hour value is out of range [0 - 23]');			I.value = I.defaultValue;
	}
}

function MinCheck(I)
{	d =parseInt(I.value, 10);
	if ( !(d<60 && d>=0) )
	{		alert('Minute value is out of range [0 - 59]');			I.value = I.defaultValue;
	}
}

