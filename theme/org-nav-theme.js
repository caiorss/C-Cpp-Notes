
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

var isOverflown = function(element) {
	// Vertical overflow
    return element.scrollHeight > element.clientHeight; 
    // Horizontal overflow
    // || element.scrollWidth > element.clientWidth;
}

var addCodeExpansionButton = function(){
    codes = document.querySelectorAll("pre");   
	
	if(window.screen.width <= 960)
	{  // Mobile	   	
	   maxHeight = "200px";	
	   btnLeftPosition = "70%";	   
	} else {
		// Desktop
	   maxHeight = "400px";
	   btnLeftPosition = "90%";
	}		
    
    codes.forEach(x => {
		
		if(!isOverflown(x)) return;
		
		var button = document.createElement("button");
		button.textContent     = "Expand";						
		button.style.position  = "relative";
		button.style.top       = "5px";
		button.style.bottom    = "5px";
		button.style.left      = btnLeftPosition;
		button.style.backgroundColor = "black";
        button.style.color = "white";
		x.parentNode.insertBefore(button, x);		
		
		flag = false;
		button.addEventListener("click", () => {
			
			if(flag == false){
				x.style.maxHeight = "none";			
				flag = true;
			} else {
				flag = false;
				x.style.maxHeight = maxHeight;			
			}
		});
		
	});		
}

function append_html(dom_elem, html)
{
   var temp = document.createElement("template");
   temp.innerHTML = html.trim(); 
   var elem = temp.content.firstChild;
   dom_elem.appendChild(elem);
   return elem; 
}

class LocalStorageString {
    
    constructor(name, value) {
        this.name = name;
        this.observers = [];
        this._dummy = (function () {
            var q = localStorage.getItem(name);
            if (q == null || q == "undefined") {
                localStorage.setItem(name, value);
            }
        } ());
    }

    add_observer(observer_function)
    {
        this.observers.push(observer_function);
    };

    notify(){
        for(let callback of this.observers) callback(this);
    };

    get(){
        var result = localStorage.getItem(this.name);
        console.assert( result != "undefined"  );
        return result;
    };
    
    set(value){ 
       localStorage.setItem(this.name, value);
       this.notify();
    }


};


function set_light_theme()
{
    var dom_content = document.querySelector("#content");
    const theme_dark  = "theme-dark-content";
    const theme_light = "theme-light-content";

    var dom_content = document.querySelector("#content");   
    var cls = dom_content.classList;
    if( cls.contains(theme_dark))  cls.remove(theme_dark);
    if(!cls.contains(theme_light)) cls.add(theme_light);  
}

function set_dark_theme()
{
    const theme_dark  = "theme-dark-content";
    const theme_light = "theme-light-content";

    var dom_content = document.querySelector("#content");   
    var cls = dom_content.classList;
    if(!cls.contains(theme_dark))  cls.add(theme_dark);
    if( cls.contains(theme_light)) cls.remove(theme_light);       
}

const theme_mode_dark = "dark_mode";
const theme_mode_light = "light_mode";
const storage_theme = new LocalStorageString("theme", theme_mode_light);

storage_theme.add_observer( sender => {
    console.trace("Theme updated. Ok");
    var theme = sender.get();

    var selector = document.querySelector("#theme-selector")

    if(theme == theme_mode_dark){ 
        set_dark_theme(); 
        selector.selectedIndex = 1;
    }
    if(theme == theme_mode_light){
        set_light_theme();
        selector.selectedIndex = 0;
    }
});

const storage_code_font_size = new LocalStorageString("code-font-size", "8pt");

storage_code_font_size.add_observer(sender => {
    var font_size = sender.get(); 
    document.documentElement.style.setProperty('--font-source-size', font_size);

    
    const fonts_index = {"8pt": 0, "10pt": 1, "12pt": 2};
    var selector = document.querySelector("#selector-font-code");
    selector.selectedIndex = fonts_index[font_size];
});

const storage_text_font_size = new LocalStorageString("text-font-size", "10pt"); 

storage_text_font_size.add_observer(sender => {
    var font_size = sender.get(); 
    document.documentElement.style.setProperty('--text-font-size', font_size);
    
    const fonts_index = {"8pt": 0, "10pt": 1, "12pt": 2, "14pt": 3};
    var selector = document.querySelector("#selector-font-text");
    selector.selectedIndex = fonts_index[font_size];
});

const id_selector_content_font = "selector-content-font";
const storage_content_font = new LocalStorageString("content-font", "spectral"); 

storage_content_font.add_observer(sender => {
    let font = sender.get(); 
    document.documentElement.style.setProperty('--content-font', font);
    
    const fonts_index = {"spectral": 0, "arial": 1, "monospace": 2};
    var selector = document.querySelector("#" + id_selector_content_font);
    selector.selectedIndex = fonts_index[font];
});





function startControlPanel(isMobile) {
    var toc = document.querySelector("#table-of-contents");
    var to  = document.querySelector("#table-of-contents h2");

    var el = document.createElement("template");
    el.innerHTML = `
    <div class="control-panel"> 

        <button class="util-btn" id="btn-config" title="User settings">
            <svg height="21" viewBox="0 0 21 21" width="21" xmlns="http://www.w3.org/2000/svg"><g fill="none" fill-rule="evenodd" transform="translate(3 5)"><g stroke="#2a2e3b" stroke-linecap="round" stroke-linejoin="round"><path d="m4.5 1.5h8"/><path d="m4.5 5.498h5"/><path d="m4.5 9.5h8"/></g><path d="m1.49884033 2.5c.5 0 1-.5 1-1s-.5-1-1-1-.99884033.5-.99884033 1 .49884033 1 .99884033 1zm0 4c.5 0 1-.5 1-1s-.5-1-1-1-.99884033.5-.99884033 1 .49884033 1 .99884033 1zm0 4c.5 0 1-.5 1-1s-.5-1-1-1-.99884033.5-.99884033 1 .49884033 1 .99884033 1z" fill="#2a2e3b"/></g></svg>
        </button>
        
        <button class="util-btn" id="btn-top" title="Go to top of page">
            <svg height="21" viewBox="0 0 21 21" width="21" xmlns="http://www.w3.org/2000/svg"><g fill="none" fill-rule="evenodd" stroke="#2a2e3b" stroke-linecap="round" stroke-linejoin="round" transform="matrix(0 -1 1 0 2 19)"><circle cx="8.5" cy="8.5" r="8"/><path d="m11.621 6.379v4.242h-4.242" transform="matrix(.70710678 .70710678 .70710678 -.70710678 -3.227683 7.792317)"/><path d="m8.5 4.5v8" transform="matrix(0 1 -1 0 17 0)"/></g></svg>
        </button>
        
        <button class="util-btn" id="btn-bottom" title="Go to bottom of page">
            <svg height="21" viewBox="0 0 21 21" width="21" xmlns="http://www.w3.org/2000/svg"><g fill="none" fill-rule="evenodd" stroke="#2a2e3b" stroke-linecap="round" stroke-linejoin="round" transform="matrix(0 1 -1 0 20 2)"><circle cx="8.5" cy="8.5" r="8"/><path d="m11.621 6.379v4.242h-4.242" transform="matrix(.70710678 .70710678 .70710678 -.70710678 -3.227683 7.792317)"/><path d="m8.5 4.5v8" transform="matrix(0 1 -1 0 17 0)"/></g></svg>
        </button>
        
        <button  class="util-btn" id="btn-index" title="Go to index page">
            <svg height="21" viewBox="0 0 21 21" width="21" xmlns="http://www.w3.org/2000/svg"><g fill="none" fill-rule="evenodd" stroke="#2a2e3b" stroke-linecap="round" stroke-linejoin="round" transform="translate(2 2)"><path d="m2.5.5h12c1.1045695 0 2 .8954305 2 2v12c0 1.1045695-.8954305 2-2 2h-12c-1.1045695 0-2-.8954305-2-2v-12c0-1.1045695.8954305-2 2-2z"/><path d="m.5 4.5h16"/></g></svg>
        </button>
    <div>
    `.trim();        

    var elem = el.content.firstChild;
    var parent = document.body;
    parent.appendChild(elem);

   var dialog_code = `
    <dialog id="user-settings" class="config-dialog"> 
        <h3>User Settings</h3>

        <table> 
            <tr> 
                <th class="row-label">Theme mode</th> 
                <th class="row-item"> 
                    <select id="theme-selector" title="Select theme mode light/dark">
                        <option value="light_mode">Light mode</option>
                        <option value="dark_mode">Dark mode</option>           
                    </select>
                </th> 
            </t> 

            <tr> 
                <th class="row-label">Text's font size</th>
                <th class="row-item"> 
                    <select id="selector-font-text" title="Select code block font size">
                        <option value="8pt">8pt</option>
                        <option value="10pt">10pt</option>
                        <option value="12pt">12pt</option>           
                        <option value="14pt">14pt</option>                       
                    </select>
                </th> 
            </tr> 

            <tr> 
                <th class="row-label">Source code's font size</th>
                <th class="row-item"> 
                    <select id="selector-font-code" title="Select code block font size">
                        <option value="8pt">8pt</option>
                        <option value="10pt">10pt</option>           
                        <option value="12pt">12pt</option>                       
                    </select>
                </th> 
            </tr> 

            <tr> 
                <th class="row-label">Content font type</th>
                <th class="row-item"> 
                    <select id="${id_selector_content_font}" title="Select font for content">
                        <option value="spectral">Spectral</option>
                        <option value="arial">Arial</option>           
                        <option value="monospace">Monospace</option>                       
                    </select>
                </th> 
            </tr>             

        </table>

        <button id="btn-close">Close</button>

    </dialog>
    `;
    dialog = append_html(document.body, dialog_code);
    dialog.querySelector("#btn-close").addEventListener("click", () => { 
        dialog.close();
        console.log(" [INFO] Dialog closed Ok.")
    });
    dialog.querySelector("#theme-selector").addEventListener("change", () => { 
        var sender = dialog.querySelector("#theme-selector");
        if(sender.value == "dark_mode" ) storage_theme.set(theme_mode_dark);
        if(sender.value == "light_mode") storage_theme.set(theme_mode_light);
        // alert(" Theme changed Ok to " + sender.value);
    });

    dialog.querySelector("#selector-font-code").addEventListener("change", () => { 
        var sender = dialog.querySelector("#selector-font-code");
        storage_code_font_size.set(sender.value);
    });

    dialog.querySelector("#selector-font-text").addEventListener("change", () => { 
         var sender = dialog.querySelector("#selector-font-text");
         storage_text_font_size.set(sender.value);
    });

    dialog.querySelector("#" + id_selector_content_font).addEventListener("change", () => {
        var sender = dialog.querySelector("#" + id_selector_content_font);
        storage_content_font.set(sender.value);
    });
 

    elem.querySelector("#btn-config").addEventListener("click", () => {
        dialog.showModal();
    });

    elem.querySelector("#btn-top").addEventListener("click", () => {
        console.log("Click Button TOP");

        var elem =  document.querySelector("#content");
        // var doc = document.documentElement || document;
        // doc.scrollTop = elem.scrollHeight; 
        elem.scrollTop = 0;

        var doc = document.documentElement || document;
        doc.scrollTop = 0;

   /*      var doc = document.documentElement || document;
        doc.scrollTop = 0;

        window.scrollTo(0, 0); */
    });

    elem.querySelector("#btn-bottom").addEventListener("click", () => {
        console.log("Click button bottom");
        // window.scrollTo(0,document.body.scrollHeight);

        var elem =  document.querySelector("#content");
        // var doc = document.documentElement || document;
        // doc.scrollTop = elem.scrollHeight; 
        elem.scrollTop = elem.scrollHeight;

        var elem =  document.querySelector("#content");
        var doc = document.documentElement || document;
        doc.scrollTop = elem.scrollHeight;
    
    });
    
    elem.querySelector("#btn-index").addEventListener("click", () => {
        console.log("Click button index");
        window.location = "index.html";
    });
    
    addCodeExpansionButton();        


    /** ---- Table of Content Folding button -------------- */
    if(isMobile){
        // alert(" MOBILE MODE");

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
        
    }
    

}; // --- End of function initMobileTocMenu() ------------ //

function toggle_theme() {
    var dom_content = document.querySelector("#content");
    dom_content.classList.toggle("theme-light-content");    
    dom_content.classList.toggle("theme-dark-content");    

}

var init = function(){

    var isMobile = window.screen.width <= 960;
    startControlPanel(isMobile);
    
    if(window.screen.width > 960) {
        const xoffset = 50;
        var ypos = 50;

        var cont = document.querySelector("#content");
        var boundRectangle = cont.getBoundingClientRect();
        var xpos = boundRectangle.x + boundRectangle.width + xoffset + "px";
        var body = document.querySelector("body");

        var ctpanel = document.querySelector(".control-panel");        
        ctpanel.style.width  = "50px";
        ctpanel.style.top  = "200px"; 
        ctpanel.style.left = "1080px";
        console.log("Control panel style = ", ctpanel.style);
    }

    storage_theme.notify();
    storage_code_font_size.notify();
    storage_text_font_size.notify();
    storage_content_font.notify();
        
  /*   var dom_content = document.querySelector("#content");
    dom_content.classList.toggle("theme-light-content");     */
}


document.addEventListener("DOMContentLoaded", init, false);

// document.addEventListener("DOMContentLoaded", init, false);

