/* ZyWALL USG serial Mutilliugul */

/*
var countryCode = '';
var CC = new Array('zh-tw','en-us');

 if(navigator.appName == "Netscape") {
	countryCode = navigator.language.toLowerCase();  
}else{
	countryCode = navigator.browserLanguage.toLowerCase();    	
}
*/
function I18N(langFileName,nowPageLanguage) {
	this._langFileName    = langFileName;
	
	if(nowPageLanguage != undefined){
		this._nowPageLanguage = '_'+nowPageLanguage;
	}else{
		this._nowPageLanguage ='';
	}
};

I18N.prototype.loadLangFile = function (str) {
/*
	var cflag=false;	
	for(var i=0; i<CC.length; i++){
		if(CC[i] == countryCode){
			cflag=true;
		}
	}
	if(cflag){// find language 
		switch (str){
			case 'panel':
				var loadLangScripts = this._langFileName + "_panel_" + this._nowPageLanguage + ".js";
				break;	
			case 'access':
				var loadLangScripts = this._langFileName + "_panel_" + this._nowPageLanguage + ".js";
				break;
			default:
				var loadLangScripts = this._langFileName + "_" + this._nowPageLanguage + ".js";
				break;
		}  			
	}else{// default language 
*/		
		switch (str){
			case 'access':
				//var loadLangScripts = this._langFileName + "_panel.js";
				//break;
			case 'panel':
				var loadLangScripts = this._langFileName + "_panel.js";
				//var loadLangScripts = this._langFileName +  "_panel"+ this._nowPageLanguage + ".js";
				break;
			default:
				//var loadLangScripts = this._langFileName + ".js";
				var loadLangScripts = this._langFileName + this._nowPageLanguage + ".js";
				break;
		}  		
//	}
  //alert(loadLangScripts);

	var head = document.getElementsByTagName("head")[0];	
	var newScriptObj = document.createElement("script");
	newScriptObj.type = "text/javascript";
	newScriptObj.src = loadLangScripts;
	head.appendChild(newScriptObj);		
	
//alert(head.innerHTML);

};
function getLangFile(str){
	switch(str){
		case 'panel':
		case 'access':
			var i18N = new I18N("language","") ;
			i18N.loadLangFile(str);
			break;
		case 'user':
			var i18N = new I18N("language", top.frames[0].LangNow) ;
			i18N.loadLangFile(str);
			break;
		default:
			var i18N = new I18N("language", top.frames[1].ConCty);
    			i18N.loadLangFile(str);
			break;
	}

}

