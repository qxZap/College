let myCanvas = document.createElement('canvas');
document.body.appendChild(myCanvas);

myCanvas.width = 800;
myCanvas.height = 600;

myCanvas.id="myCanvas";

var context = myCanvas.getContext('2d');
var offset = 0;

function draw() {
  context.clearRect(0, 0, myCanvas.width, myCanvas.height);
  context.setLineDash([8, 4]);
  context.lineDashOffset = -offset;
  context.strokeRect(10, 10, 100, 100);
}

function march() {
  offset++;
  if (offset > 32) {
    offset = 0;
  }
  draw();
  setTimeout(march, 20);
}

march();










