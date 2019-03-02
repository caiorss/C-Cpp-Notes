

function hideNavBar(){
    var toc = document.querySelector("#table-of-contents");
    toc.style.display = "none";
    // document.documentElement.style.setProperty('--main-width', '90%');
    document.querySelector("body")
        .style
        .setProperty("margin-left", "100px");
    
    // button.style.left = "10px";
}

function showNavBar(){
    var toc = document.querySelector("#table-of-contents");
    toc.style.display = "block";
    // document.documentElement.style.setProperty('--main-width', '100%');
    document.querySelector("body")
        .style
        .setProperty("margin-left", "250px");
    
    // button.style.left = "25%";
}

var buttonFlag = false; 

// button.addEventListener("click", function(){
//     if(buttonFlag == true) {
//         hideNavBar();
//         buttonFlag = false;
//     } else {
//         showNavBar();
//         buttonFlag = true;
//     }
// });


// var init = function(){
//     hideNavBar();
//     document.body.appendChild(button);
// }


var openTOCMenu = function(){
    var toc = document.querySelector("#table-of-contents");
    toc.style.setProperty("height", "100%");
    toc.style.setProperty("overflow-y", "scroll");
}

var closeTOCMenu = function(){
   
    var toc = document.querySelector("#table-of-contents");
    toc.scrollTop = 0 ;
    toc.style.setProperty("height", "50px");
    toc.style.setProperty("overflow-y", "hidden");
}

var initMobileTocMenu = function(){
    var toc = document.querySelector("#table-of-contents");
    var to  = document.querySelector("#table-of-contents h2");
    
	// Button top - scroll to page top 
	//-----------------------------------------------
    var btnTop = document.createElement("button")
    btnTop.textContent     = "Top";
    btnTop.style.top       = "20px";    
    btnTop.style.left      = "100px";   
    btnTop.style.position  = "fixed";
    // button.style.zIndex = 10000;

    btnTop.addEventListener("click", function(){
        document.querySelector("#content").scrollTop = 0;
    });   
    toc.appendChild(btnTop);
    
	// Button bottom - scroll to page bottom 
	//-------------------------------------------
    var btnBottom = document.createElement("button")
    btnBottom.textContent     = "Bottom";
    btnBottom.style.top       = "20px";    
    btnBottom.style.left      = "200px";   
    btnBottom.style.position  = "fixed";
    // button.style.zIndex = 10000;

    btnBottom.addEventListener("click", function(){        
        var elem =  document.querySelector("#content");
        elem.scrollTop = elem.scrollHeight;        
    });   
    toc.appendChild(btnBottom);    
    

    // Button index - go to index page
    //-------------------------------------------------
    var btnIndex = document.createElement("button")
    btnIndex.textContent     = "Index";
    btnIndex.style.top       = "20px";    
    btnIndex.style.left      = "300px";   
    btnIndex.style.position  = "fixed";
    // button.style.zIndex = 10000;
    btnIndex.addEventListener("click", function(){
        window.location = "index.html";
    });   
    toc.appendChild(btnIndex);

    
    var tocTitle = document.querySelector("#table-of-contents h2");
    tocTitle.textContent = "TOC";
    tocTitle.style.color = "black";

    var tocOpened = false;
    
    to.addEventListener("click", function(){
        // console.log("tocOpened = " + tocOpened);
        if(!tocOpened){
            openTOCMenu();
            tocOpened = true;
        }
        else {            
            closeTOCMenu();
            tocOpened = false;
        }
    });

    
    var tocLinks = document.querySelectorAll("#table-of-contents a");
    tocLinks.forEach(a => a.addEventListener("click", closeTOCMenu));

} // --- End of Function initMobileTocMenu() ---- //


var initDesktopMenu = function () {
    var xoffset  =  50;
    var btnWidth = "60px";

    var cont = document.querySelector("#content");
    var boundRectangle = cont.getBoundingClientRect();
    var xpos = boundRectangle.x + boundRectangle.width + xoffset + "px";
    var body = document.querySelector("body");

    var makeMenuButton = function(caption, ypos){
        var button = document.createElement("button")
        body.appendChild(button);
        button.textContent     = caption;
        button.style.width     = btnWidth;
        button.style.top       = ypos;
        button.style.left      = xpos;
        button.style.position  = "fixed";
        button.style.backgroundColor = "black";
        button.style.color = "white";
        return button
    };

    // Button index - go to index page
    //-------------------------------------------------
    var btnIndex = makeMenuButton("Index", "200px");
    // button.style.zIndex = 10000;
    btnIndex.addEventListener("click", function(){
        window.location = "index.html";
    });

    var btnTop = makeMenuButton("Top", "260px");
    btnTop.addEventListener("click", function(){
        var doc = document.documentElement || document;
        doc.scrollTop = 0;
    });

    // Button bottom - scroll to page bottom
    //-------------------------------------------
    var btnBottom = makeMenuButton("Bottom", "320px");
    btnBottom.addEventListener("click", function(){
        var elem =  document.querySelector("#content");
        var doc = document.documentElement || document;
        doc.scrollTop = elem.scrollHeight;
        // elem.scrollTop = elem.scrollHeight;
    });


} // --- End of function() initDesktopMenu() ---- /q


var init = function(){
    if (window.screen.width <= 960)
        initMobileTocMenu();
    if(window.screen.width > 960)
        initDesktopMenu();
}


document.addEventListener("DOMContentLoaded", init, false);

// document.addEventListener("DOMContentLoaded", init, false);

