let myCanvas = document.createElement('canvas');
document.body.appendChild(myCanvas);

myCanvas.width = 800;
myCanvas.height = 600;

myCanvas.id="myCanvas";
document.getElementById('myCanvas').style.border="1px dotted darkgray";
var context = myCanvas.getContext('2d');	

let balls=[];

class Ball{
	constructor(){		
		this._x = Math.floor(Math.random()*myCanvas.width);
		this._y = Math.floor(Math.random()*myCanvas.height);
		this._speedx = Math.floor(Math.random()*4+3);
		this._speedy = Math.floor(Math.random()*4+3);		
	}
	move(){
		this._x += this._speedx;
		this._y += this._speedy;
		if ((this._x<0) || (this._x>myCanvas.width)) this._speedx *= -1;
		if ((this._y<0) || (this._y>myCanvas.height)) this._speedy *= -1;
	}
	paint(){		
		context.strokeStyle = 'rgba(255, 255, 255, 1)'; 
			context.beginPath();
			context.arc(this._x,this._y,3,0,2*Math.PI);
			context.closePath();
			context.stroke();				
	}
	get x() {return this._x};
	get y() {return this._y};

}

for(let i=0; i<3; i++)
	balls.push(new Ball());

function moveBalls() {
	for(let i=0; i<balls.length; i++)
		balls[i].move();
	
	context.fillStyle = 'rgba(0, 0, 0, .1)';
	context.fillRect(0, 0, myCanvas.width, myCanvas.height);	
	for(let i=0; i<balls.length; i++)
		balls[i].paint();	
	
	for(let i=0; i<balls.length; i++)
	{	
		context.moveTo(balls[i].x,balls[i].y);	
		context.lineTo(balls[(i+1)%3].x,balls[(i+1)%3].y);
	}	
	context.stroke();

	setTimeout(moveBalls, 20);
}

moveBalls();
