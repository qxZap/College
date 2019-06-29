let myCanvas = document.createElement('canvas');
document.body.appendChild(myCanvas);

myCanvas.width = 800;
myCanvas.height = 600;

myCanvas.id="myCanvas";
document.getElementById('myCanvas').style.border="1px dotted black";


if (myCanvas.getContext) {
	var context = myCanvas.getContext('2d');

	var myPath = new Path2D();
	

	myPath.moveTo(200,200);
	myPath.bezierCurveTo(250, 400, 350, 120, 400, 200);
	context.stroke(myPath);


	// from SVG:

	//var myPath2 = new Path2D('M 100,100 h 50 v 50 h 50');
	var myPath2 = new Path2D('M 100,100 h 50 v 50 h 50');
	context.stroke(myPath2);
	
}