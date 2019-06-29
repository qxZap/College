let myCanvas = document.createElement('canvas');
document.body.appendChild(myCanvas);



// myCanvas.width = 800;
// myCanvas.height = 600;

// myCanvas.width = window.innerWidth;;
// myCanvas.height = window.innerHeight;


myCanvas.id="myCanvas";
document.getElementById('myCanvas').style.border="3px dotted red";
// default este 300px X 150px

// sa desenam pe el doua patrate :D
// pentru asta avem nevoie de un rendering context care poate fi 2D, 3d, WebGL

let context = myCanvas.getContext('2d');
context.fillStyle = 'rgb(0, 170, 0)';
context.fillRect(10, 10, 50, 50);
context.fillStyle = 'rgba(100, 0, 200, 0.5)';
context.fillRect(30, 30, 50, 50);

// document.getElementById('myCanvas').style.width="800px";
// document.getElementById('myCanvas').style.height="600px";

