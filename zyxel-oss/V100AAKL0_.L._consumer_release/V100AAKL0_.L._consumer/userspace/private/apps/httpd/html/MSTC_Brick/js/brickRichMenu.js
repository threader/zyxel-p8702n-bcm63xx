     
     var currentSubMenu=null;
     $.fn.caaShow = function(time, callback){
           currentSubMenu =  $(this);
           var animateTop = $(this).position().top;
           
           if( $(this).css('visibility')!= 'visible')
           $(this).css("top",animateTop-30);
           
           return $(this).css({'visibility': 'visible'})
           .animate({
               'top': animateTop,
               opacity: 1
           }, time, 'easeOutBack' ,callback);
        }

        $.fn.caaHide = function(time, callback){
            currentSubMenu=null;
            var animateLeft = $(this).position().left;
            return $(this).animate(
            {
            opacity: 0}, 
            time, 
            
            function(){
                $(this).css('visibility', 'hidden');
                callback;
            });
        }

        $.zyBRMenu = {
            modifySubMenuPos: function(panel, liObj){
                var pos = liObj.position();
                var scrollTop = liObj.parents('.scrollable')[0].scrollTop
                var atop = liObj.parents('.items').position().top;
                var scrollableTop = liObj.parents('.scrollable').position().top;
                var panelTop = pos.top - scrollTop + scrollableTop + atop -14;
                
                if (panelTop + panel.height() > $('.outer-west').height()) {
                    var Hoffset = 5;
                    if ($.browser.msie && ($.browser.version == "6.0")) 
                        Hoffset = 15
                    
                    panelTop = panelTop - panel.height() + liObj.height() + Hoffset;
                }
                
                panel.css('top', panelTop).css('left', liObj.width() + liObj.parent().parent().position().left);

            },
            afterSetScrollable: function(scrollSize){
            },
            getScrollSize:function(){
                var ss = Math.ceil(($('.outer-west').height() - 140) / $('.menuItem').height());
                ss = ss > 1 ? ss : 1;
                return ss > $('.menuItem').length ? $('.menuItem').length : ss;
            },
            getScrollOption: function(scrollSize){
                var scrollOption = {
                    size: scrollSize,
                    keyboard: false,
                    //activeClass: 'caaActive',
                    //easing:'easeOutQuad',//'easeOutElastic', //'easeInBack',//'easeOutElastic',
                    api: true,
                    speed: 500,
                    vertical: true,
                    clickable: false,
                    //  hoverClass: 'menuItemHover',
                    onSeek: function(){
                        scrollApi = this;
                        var index = scrollApi.getIndex();
                        
                        var size = scrollApi.getSize();
                        var visibleSize = scrollApi.getVisibleItems().length;
                      
                        if (index + visibleSize == size) 
                            $('.browse.right').addClass('disabled');
                        else 
                            $('.browse.right').removeClass('disabled');
                        
                        if (index == 0) 
                            $('.browse.left').addClass('disabled');
                        else 
                            $('.browse.left').removeClass('disabled');
                    }
                };
               
			   var scrollPanel = $('.scrollable');
			   var menuItems = scrollPanel.find('.menuItem');
			   if(menuItems.length<scrollSize)
			    scrollPanel.height(menuItems.length*menuItems.height());
			   else
			    scrollPanel.height(scrollSize*menuItems.height());
				
                return scrollOption;
            },
            layoutOption: {
                center__paneSelector: ".outer-center",
                north__paneSelector: ".outer-north",
                west__paneSelector: ".outer-west",
                west__size: 120,
                north__size: 56,
                collapsable: false,
                slidable: false,
                resizable: false,
                spacing_open:    0,       // no resizer-bar when open (zero height)
                spacing_closed:  0,
                center__onresize: function(){}
            },
            
            modifyLayout: function(){
                
           /*
            <div style='height:100%; border: solid 1px white; overflow: auto; position:relative; '>
                <div style='margin-top:50px; border: solid 1px blue;'>
                    <div class="browse left disabled" id="arrowright">
                        <a href="#"></a>
                    </div>
                    <div id="menuDiv" class="scrollable">
                    </div>
                    <div class="browse right" id="arrowleft">
                        <a href="#"></a>
                    </div>
                </div>
            </div>
            <div style='position:absolute; left:0px; top:0px;'>
                <div id="subMenuDiv" style="margin: 1px auto 0 auto;  position:relative;  border: solid 1px blue;">
                </div>
                <div>
                </div>
            </div>
           
           */
              var htmlObj = 
              '<div id="scrollDiv" style="height:100%; overflow: auto; position:relative; background:url(images/menu_bg.jpg); ">'+
                '<div style="margin-top:10px;">'+
                        '<div class="browse left disabled" id="arrowright">'+
                               '<a href="#"></a>'+
                         '</div>'+
                    '<div id="menuDiv" class="scrollable">'+
                    '</div>'+
                    '<div class="browse right disabled" id="arrowleft">'+
                        '<a href="#"></a>'+
                    '</div>'+
                '</div>'+
              '</div>'+
           
                '<div style="position:absolute; left:5px; top:0px;">'+
                  '<div id="subMenuDiv" style="margin: 1px auto 0 auto;  position:relative;">'+
                  '</div>'+
                
           ' </div>';
              
                $('.outer-west').append($(htmlObj));
                
                $('#scrollDiv').scroll(function(){
                    if (currentSubMenu != null && currentSubMenu.attr('targetId') != undefined) {
                        var liObj = $('#' + currentSubMenu.attr('targetId'));
                        currentSubMenu.caaHide(200, function(){
                            $(this).attr('isAnimation', 'false');
                            $(this).css('display', 'none');
                        });
                        liObj.removeClass('hover');
                    }
                });
                
            }
        };
       
       

