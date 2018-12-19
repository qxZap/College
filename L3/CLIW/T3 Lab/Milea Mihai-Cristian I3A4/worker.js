onmessage = function(d) {
	let tryouts = d.data[0];
	let c=0;
	for(let i=0;i<tryouts;i++){
		let x = Math.random()*2-1;
		let y = Math.random()*2-1;
		if(Math.sqrt(x*x+y*y)<1)
			c++;
	}
	postMessage(["tryouts",c]);
}