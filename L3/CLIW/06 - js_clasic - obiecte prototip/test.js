function Persoana() {
  this.nume = '';
  this.cnp = '123123';
}
Persoana.prototype.nationalitate = "Romana";


function Profesor(){
	Persoana.call(this);
    this.cursuri = [];
}

Profesor.prototype = Object.create(Persoana.prototype);
Profesor.prototype.constructor = Profesor;

function Student(){
	Persoana.call(this);
    this.an = 1;
}


//Student.prototype = Object.create(Persoana.prototype);
//Student.prototype.constructor = Student();
Student.prototype = new Persoana();


var p = new Persoana();
document.write("p is of type " + typeof(p));
p.prop = "test";
document.write("<br>p.prop = " + p.prop);


var s = new Student();
document.write("<hr>s is of type " + typeof(s));
document.write("<br>s.an = " + s.an);
document.write("<br>s.cnp = " + s.cnp);
document.write("<br>s.nationalitate = " + s.nationalitate);
s.nationalitate = "turca";
document.write("<br>s.nationalitate = " + s.nationalitate);

Persoana.prototype.rasa = "caucazian";
document.write("<br>s.rasa = " + s.rasa);



Persoana.prototype.vorbeste = function(txt)
{
	console.log(txt);
}
s.vorbeste("bla bla bla");

