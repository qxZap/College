
var w = Math.max(document.documentElement.clientWidth, window.innerWidth || 0)-60;
var h = Math.max(document.documentElement.clientHeight, window.innerHeight || 0)-60;


let balls=[];

class Ball{
	constructor(nr){
		this._raza = Math.floor(Math.random()*30+30);
		this._x = Math.floor(Math.random()*w);
		this._y = Math.floor(Math.random()*h);
		this._speed = Math.floor(Math.random()*3+1);
		
		let red = Math.floor(Math.random()*255);
		let green = Math.floor(Math.random()*255);
		let blue = Math.floor(Math.random()*255);


		let ball = document.createElement('div');
		ball.id = nr;
		ball.style.width = ""+this._raza*2+"px";
		ball.style.height = ""+this._raza*2+"px";
		ball.style.zIndex = ""+nr;
		ball.style.position = "absolute";
		ball.style.backgroundColor = "rgb("+red+","+green+","+blue+")";
		ball.style.top = ""+this._y+"px";
		ball.style.left = ""+this._x+"px";
		ball.style.borderRadius = this._raza+"px";
		document.body.appendChild(ball);		

		ball.onmouseenter = function(event){

				balls[event.target.id]._x=-200;
				balls[event.target.id]._speed=Math.floor(Math.random()*3+1);
				let red = Math.floor(Math.random()*255);
				let green = Math.floor(Math.random()*255);
				let blue = Math.floor(Math.random()*255);

				document.getElementById(event.target.id).style.left = balls[event.target.id]._x+"px";
				document.getElementById(event.target.id).style.top = Math.floor(Math.random()*h)+"px";
				document.getElementById(event.target.id).style.backgroundColor = "rgb("+red+","+green+","+blue+")";			
		}
	}
}



for(let i=0; i<10; i++)
{
	let b = new Ball(i);
	balls.push(b);
}


let move = function(){
	for(let i=0; i<balls.length; i++)
	{
		balls[i]._x += balls[i]._speed;
		if (balls[i]._x > w-100) 
			{
				balls[i]._x=-100;
				balls[i]._speed=Math.floor(Math.random()*3+1);
				let red = Math.floor(Math.random()*255);
				let green = Math.floor(Math.random()*255);
				let blue = Math.floor(Math.random()*255);				
				document.getElementById(i).style.top = Math.floor(Math.random()*h)+"px";
				document.getElementById(i).style.backgroundColor = "rgb("+red+","+green+","+blue+")";

			}
		document.getElementById(i).style.left = balls[i]._x+"px";
		//console.log(balls[i]._x);
	}
}

let myInterval = setInterval(move);


