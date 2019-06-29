var lista=[1,2,3,4,5,6,7,8,9];

/* // sari peste asta
var cifre = nr => [...`${nr}`].map(i=>parseInt(i));
var lista = cifre(123456789);
*/


//lista = lista.map(v => v*2);
//lista = lista.map((v,i) => 'v['+i+']='+v);


/*
function fibo(i){
	if (i<2) return 1;
	else return fibo(i-1)+fibo(i-2);
}




//let fibo = (val) => {return val <2 ? 1 : fibo(val-1) + fibo(val-2);};

lista = lista.map((v,i) => fibo(i));
*/

// console.log(lista.reduce((a,b)=>a+b));




/*
function prim(i){
	if (i<2) return 0;
	for(j=2; j*j<=i; j++)
		if (i%j==0) return 0;
	return 1;
}


console.log(`every: toate elementele din ${lista.map(v => `${v}`).join()} sunt prime: ${lista.every(prim)}`); // testeaza daca toate numerele fac ca functia sa returneze true
console.log(`some: exista elemente din ${lista.map(v => `${v}`).join()} care prime: ${lista.some(prim)}`); // testeaza daca toate numerele fac ca functia sa returneze true

lista = lista.filter(v => prim(v)); // filtram si ramanem doar cu cele prime

console.log(`every: toate elementele din ${lista.map(v => `${v}`).join()} sunt prime: ${lista.every(prim)}`); // testeaza daca toate numerele fac ca functia sa returneze true
*/



document.getElementsByTagName("body")[0].innerHTML = `<ul>
	${lista.map(element => `<li>${element}</li>`).join('\n\t')}
</ul>`;

