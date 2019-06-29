for(let i of [2,3,7])
	console.log(i);





function* range (start, end, step) {  // functie generator (are steluta :D )
    while (start < end) {
        yield start                   // asta imi opreste generatorul si imi da o noua valoare
        start += step
    }
}



for (let n of range(1,30,4)) {
    document.getElementsByTagName("body")[0].innerHTML += `<p>${n}</p>`;
}

// sau 
/*
let sir = range(1,30,4);
for(let i of sir)
	console.log(i);
*/





/*

function* fibo(maxim)
{
	let a=0, b=1;
	while(b < maxim)
	{
		let nou = a+b;
		a = b;
		b = nou;
		// ce e mai sus in acest while poate fi inlocuit cu
		// [a,b] = [b,a+b];
		yield b;
	}
}


let sir = fibo(1000);
for(let i of sir)
	console.log(i);
*/	