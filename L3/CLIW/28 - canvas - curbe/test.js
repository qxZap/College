let myCanvas = document.createElement('canvas');
document.body.appendChild(myCanvas);

myCanvas.width = 800;
myCanvas.height = 600;

myCanvas.id="myCanvas";
document.getElementById('myCanvas').style.border="1px dotted black";


if (myCanvas.getContext) {
	var context = myCanvas.getContext('2d');


	
	context.beginPath();
	context.moveTo(200,200);
	context.quadraticCurveTo(220, 400, 300, 200);
	context.quadraticCurveTo(380, 0, 400, 200); // cum  am calculat-o ?
	context.stroke();

	context.moveTo(200,400);
	context.bezierCurveTo(220, 600, 380, 200, 400, 400)
	context.stroke();	
}