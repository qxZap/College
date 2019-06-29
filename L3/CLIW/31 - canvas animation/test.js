let myCanvas = document.createElement('canvas');
document.body.appendChild(myCanvas);

myCanvas.width = 800;
myCanvas.height = 600;

myCanvas.id="myCanvas";
document.getElementById('myCanvas').style.border="1px dotted black";
var context = myCanvas.getContext('2d');	

let balls=[];

class Ball{
	constructor(){		
		this._x = Math.floor(Math.random()*myCanvas.width);
		this._y = Math.floor(Math.random()*myCanvas.height);
		this._speedx = Math.floor(Math.random()*2+2);
		this._speedy = Math.floor(Math.random()*2+2);		
		this._oldx = [];
		this._oldy = [];
		this._pres=0;
	}
	move(){
		this._pres++;
		this._pres%=10;

		this._x += this._speedx;
		this._y += this._speedy;
		if ((this._x<0) || (this._x>myCanvas.width)) this._speedx *= -1;
		if ((this._y<0) || (this._y>myCanvas.height)) this._speedy *= -1;

		this._oldx[this._pres]=this._x;
		this._oldy[this._pres]=this._y;
		console.log(this._x,this._y);
	}
	paint(){		
		context.strokeStyle = 'rgba(255, 255, 255, 1)'; 
		for(let i=9; i>=0; i--)
		{
			context.beginPath();
			context.arc(this._oldx[(this._pres+i)%10],this._oldy[(this._pres+i)%10],10,0,2*Math.PI);
			context.closePath();
			context.stroke();		
		}
		
	}

}

for(let i=0; i<3; i++)
	balls.push(new Ball());

function moveBalls() {
	for(let i=0; i<balls.length; i++)
		balls[i].move();
	
	context.fillStyle = 'rgba(0, 0, 0, .3)';
	context.fillRect(0, 0, myCanvas.width, myCanvas.height);	
	for(let i=0; i<balls.length; i++)
		balls[i].paint();	
	setTimeout(moveBalls, 20);
}

moveBalls();
