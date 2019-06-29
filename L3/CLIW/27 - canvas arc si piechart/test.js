let myCanvas = document.createElement('canvas');
document.body.appendChild(myCanvas);

myCanvas.width = 800;
myCanvas.height = 600;

myCanvas.id="myCanvas";
document.getElementById('myCanvas').style.border="1px dotted black";


let radians = grade=>(Math.PI/180)*grade;


if (myCanvas.getContext) {
	var context = myCanvas.getContext('2d');

    // putem desena / sterge dreptunghiuri:

	//context.arc(x,y,raza,unghiStart, unghiSfarsit, anticlockwise);
	//context.arcTo(x1, y1, x2, y2, raza)
	
	context.beginPath();
	context.arc(200, 200, 50, 0, radians(90), 1);
	context.fill();
	context.stroke();
	//context.closePath();

	// sa facem un piechart (3 sectiuni): 10%,30%,60%;
	let valori=[10,30,40,20];

	if (valori.reduce((a,b)=>a+b)==100)
	{
		valori = valori.map(x=>x*3.6);	
		let s0=0,s1=0;
		for(let i=0; i<valori.length; i++)
		{
			s1+=valori[i];
			context.fillStyle = "RGB("+Math.floor(Math.random()*255)+","+Math.floor(Math.random()*255)+","+Math.floor(Math.random()*255)+")";
			context.beginPath();	
			context.moveTo(400,400);
			context.arc(400, 400, 100, radians(s0), radians(s1), 0);
			context.closePath();	
			context.fill();
			context.stroke();
			s0=s1;
		}
	}
}