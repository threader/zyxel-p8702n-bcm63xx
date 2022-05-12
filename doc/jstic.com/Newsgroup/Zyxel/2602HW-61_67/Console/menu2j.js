// get browser version
var appName = navigator.appName;
var appVersion = navigator.appVersion;
var isIE4 = false;
var whereami = window.location.href;
if( ((appVersion.substring(0,1) >= 4) && (appName == "Microsoft Internet Explorer")) ||
((appVersion.substring(0,1) >= 5) && (appName == "Netscape"))// Netscape 6.x
)
isIE4 = true;
else
isIE4 = false;
function menuItem(text, pan, url) {
var sub = null;
if( whereami.toLowerCase().indexOf( url.toLowerCase() ) < 0 )
menuWrite(text, pan, url, sub);
else
menuHere(text, sub);
}
function subMenuItem(text, pan, url) {
var sub = '<img src=\"Images/nav_light.gif\" border=0 width=7 height=7 hspace=2  align=absmiddle>&nbsp\;';
if( whereami.toLowerCase().indexOf( url.toLowerCase() ) < 0 )
menuWrite(text, pan, url, sub);
else
menuHereSmall(text, sub);
}
function menuWrite(text, pan,url, sub) {
if( isIE4 ) {
document.write('<tr>\n');
document.write('<td align=center class="shadow"><img ');
document.write('src="Images/spacer.gif" width="1" height="1"></td>\n');
document.write('</tr>\n');
document.write('<tr>\n');
document.write('<td bgcolor="#999999" align=center class="highlight"><img src="Images/spacer.gif" width="1" height="1"></td>\n');
document.write('</tr>');
document.write('<tr>\n');
document.write('<td class="off" ');
document.write('onMouseOver="this.className=\'on\'; window.status=\'' + text + '\'; return true" '); // onMouseOver
document.write('onMouseOut="this.className=\'off\'; window.status=\'\'; return true" '); // onMouseOut
document.write('valign="middle" height="20" ');
if( sub == null && pan == "" )// Logout.html
	document.write('onClick="top.location.href=\'' + url + '\';">'); // write the URL
	else
document.write('onClick="top.main.location.href=\'' + url + '\'; top.panel.location.href=\'' + pan + '\';">'); // write the sub-menu URL
document.write('&nbsp; ');
if( sub != null )
document.write(sub); // if sub-menu item
document.write('<span class="navLink">' + text + '</span></td>\n'); // write the text
document.write('</tr>');
} else {
document.write('<tr>\n');
document.write('<td bgcolor="#999999" class="shadow"><img src="Images/spacer.gif" ');
document.write('width="1" height="1"></td>\n');
document.write('</tr>\n');
document.write('<tr>\n');
document.write('<td align=center class="highlight">');
document.write('<img src="Images/spacer.gif" ');
document.write('width="1" height="1"></td>\n');
document.write('</tr>\n');
document.write('<tr>\n');
document.write('<td class="off" valign=middle height="20">&nbsp; ');
document.write('<a href="');
document.write( url );
if( sub == null && pan == "" )// Logout.html
document.write('" target="_top" class="navLink">');
else
document.write('" target="main" class="navLink" onClick="window.location=\'' + pan + '\';">');
document.write('&nbsp; ');
if( sub != null )
document.write(sub); // if sub-menu item
document.write( text );
document.write('</a></td>\n');
document.write('</tr>\n');
}
}
function menuHere(text, sub) {
document.write('<tr>\n');
document.write('<td align=center class="shadow"><img src="Images/spacer.gif" width="1" height="1"></td>\n');
document.write('</tr>');
document.write('<tr>\n');
document.write('<td align=center class="contentCell2"><img src="Images/spacer.gif" width="1" height="1"></td>');
document.write('</tr>');
document.write('<tr>\n');
document.write('<td background="Images/left_menu_bg.gif" ');
document.write('valign="middle" height="20" class="navNull">&nbsp;&nbsp; ');
if( sub != null )
document.write(' &nbsp; '); // if sub-menu item
document.write( text ); // write the text
document.write('</td>\n');
document.write('</tr>\n');
}
function menuHereSmall(text, sub) {
document.write('<tr>\n');
document.write('<td align=center class="shadow"><img src="Images/spacer.gif" width="1" height="1"></td>\n');
document.write('</tr>');
document.write('<tr>\n');
document.write('<td align=center class="contentCell2"><img src="Images/spacer.gif" width="1" height="1"></td>');
document.write('</tr>');
document.write('<tr>\n');
document.write('<td background="Images/left_menu_bg.gif" ');
document.write('valign="middle" height="20" class="navNull2">&nbsp;&nbsp; ');
if( sub != null )
document.write(' <img src=\"Images/nav_light_on.gif\" border=0 width=7 height=7 hspace=2  align=absmiddle> '); // if sub-menu item
//	document.write( '<span style="font-size: 10px !important">' + text + '</span>'); // write the text
document.write( text ); // write the text
document.write('</td>\n');
document.write('</tr>\n');
}