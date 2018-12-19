var t0 = performance.now();
function printPI() {
  alert(tryouts*4/incercari);
}

function sleep(milliseconds) {
  var start = new Date().getTime();
  for (var i = 0; i < 1e7; i++) {
    if ((new Date().getTime() - start) > milliseconds){
      break;
    }
  }
}


function printAll(){
var t1 = performance.now();
var elem = document.getElementById("wait");
elem.parentNode.removeChild(elem);

document.body.innerHTML += "PI: "+Math.PI;
document.body.innerHTML += "<br>";
document.body.innerHTML += "Found PI: "+tryouts*4/incercari;
document.body.innerHTML += "<br>";
document.body.innerHTML += "time: "+(t1-t0)/1000+" seconds";
document.body.innerHTML += "<br>";
}

let dones=0;
let incercari = 1000000000;
let workers = 10;
let tryouts = 0;
for(let i=0;i<workers;i++){
	let worker = new Worker('worker.js');
	worker.postMessage([incercari/workers]);

	worker.onmessage = event =>{
		tryouts+=event.data[1];
		dones++;
		if(dones===workers){
			printAll();
		}
	}
}

