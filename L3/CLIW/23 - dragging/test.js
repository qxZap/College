let element = document.getElementById("myDiv");



var pozx=0;
var pozy=0;
var mousex=0, mousey=0;


element.ondragstart = function(event){
	mousex = event.clientX;
	mousey = event.clientY;	
}

element.ondrag = function(event){
	//console.log(mousex, mousey);
	; 
}

element.ondragend = function(event){
	let mousex2 = event.clientX;
	let mousey2 = event.clientY;
	let deltax = mousex2 - mousex;
	let deltay = mousey2 - mousey;

	pozx = pozx + deltax;
	pozy = pozy + deltay;

	element.style.top = pozy + "px";	
	element.style.left = pozx + "px";	
}
