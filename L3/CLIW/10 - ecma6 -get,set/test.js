class Dreptunghi{
	constructor(lungime=0, latime=0){
		this._latime = latime;
		this._lungime = lungime;
	}
	get aria(){
		if (this._latime*this._lungime<0) throw new RangeError('Arie negativa');
		//console.log (this.latime*this.lungime);
		return (this._latime*this._lungime);
	}
	static func(){
		console.log('functie');
	}
}


class Patrat extends Dreptunghi{
	constructor(latura=0){		
		super(latura,latura)		
		this._latura = latura;
	}
	set latura(lungime) {
		this._latura = lungime; 
		this._latime=lungime; 
		this._lungime=lungime;
	}
}


let dr = new Dreptunghi(10,5);
console.log(dr.aria);

let pt = new Patrat(10);
console.log(pt.aria);

console.log(pt._latura);
console.log(pt);
Patrat.func();

console.log("setez latura egala cu 5");
pt.latura = 5;
console.log(pt.aria);
