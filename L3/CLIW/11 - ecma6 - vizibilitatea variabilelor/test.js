const s=10;
console.log(s);

var x = 'a';
let z = 'b';
{
	var x = 'A';
	let z = 'B';

	console.log(x);
	console.log(z);
}

console.log(x); // A
console.log(z); // b


let[a,,b] = [1,2,3];
console.log(a);
console.log(b);


function sumprod(a,b){
	return {i:a+b, j:a*b};
}
let {i,j} = sumprod(2,3);
console.log(i);
console.log(j);
