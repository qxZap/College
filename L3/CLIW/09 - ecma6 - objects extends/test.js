class Dreptunghi{
	constructor(lungime=0, latime=0){
		this.latime = latime;
		this.lungime = lungime;
	}
	aria(){
		if (this.latime*this.lungime<0) throw new RangeError('Arie negativa');
		//console.log (this.latime*this.lungime);
		return (this.latime*this.lungime);
	}
}


class Patrat extends Dreptunghi{
	constructor(latura){
		super(latura,latura)
	}
}


let dr = new Dreptunghi(10,5);
console.log(dr.aria());

let pt = new Patrat(10);
console.log(pt.aria());