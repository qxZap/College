function sum(a=0, b=0, ...args) // parametri default respectiv lista de argumente
{
	let s=0;
	s+=a;
	s+=b;
	for(let i=0; i<args.length; i++)
		s+=args[i];
	return s;
}

document.getElementsByTagName("body")[0].innerHTML = `<p>${sum(1,2,3,4)}</p>`;






/*
let sir1 = [1,2,3];
let sir2 = [4,5,6, ...sir1];  // spread operator

document.getElementsByTagName("body")[0].innerHTML += `<ul>
	${sir2.map(element => `<li>${element}</li>`).join('\n\t')}
</ul>`;

*/