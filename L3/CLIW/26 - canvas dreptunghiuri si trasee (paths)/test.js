let myCanvas = document.createElement('canvas');
document.body.appendChild(myCanvas);

myCanvas.width = 800;
myCanvas.height = 600;

myCanvas.id="myCanvas";
document.getElementById('myCanvas').style.border="1px dotted black";


if (myCanvas.getContext) {
	var context = myCanvas.getContext('2d');

    // putem desena / sterge dreptunghiuri:
	context.fillStyle = 'rgba(200, 170, 0, .9)'; 
	context.fillRect(30, 50, 350, 280);
	context.strokeRect(100, 100, 100, 150);
	context.clearRect(40, 60, 40, 40);



	// sau putem trasa cu ajutorul unui "path":
	context.strokeStyle = "#FF0000";	
	context.lineWidth = 20;

    //context.lineJoin = "bevel"; // "bevel", "mitter", "round"

	context.beginPath();
	context.moveTo(500,300);
	context.lineTo(750,500);
	context.lineTo(500,500);
	context.fill();
	context.closePath();
	context.stroke();


	context.strokeStyle = "#000000";	
	context.lineWidth = 1;
	context.font="50px Verdana";
	context.strokeText("Salutare din canvas!",100,570);	
}