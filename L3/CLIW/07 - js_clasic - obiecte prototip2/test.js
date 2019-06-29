function Persoana(nume, cnp) {
  this.nume = nume || 'Vasile';
  this.prenume = ["p1","p2"];
  this.cnp = cnp || '123123';
}


Persoana.prototype.vorbeste = function(txt)
{
	console.log(txt);
}


p = new Persoana();
p.vorbeste("bla bla bla");
document.write(p.nume);


p2 = new Persoana("Ionel","1");
p2.vorbeste("salut, sunt persoana 2");
document.write("<br>"+p2.nume);




function Student(){
	Persoana.call(this);
	this.an = 1;
}

Student.prototype = new Persoana("Gigi","-1");

s = new Student();
document.write("<hr>"+s.nume);
document.write("<br>"+s.cnp);
document.write("<br>"+s.an);
s.vorbeste("Sunt student");



function Profesor(){
	Persoana.call(this,arguments[0],arguments[1]);
}
Profesor.prototype = new Persoana;

prof = new Profesor("Mircea", "2");
document.write("<hr>"+prof.nume);
document.write("<br>"+prof.cnp);
document.write("<br>"+prof.an); // asta este numai la student
prof.vorbeste("Sunt profesor");




document.write("<hr>prof is instance of Profesor: "+ (prof instanceof Profesor));
document.write("<hr>s is instance of Student: "+ (s instanceof Student));
document.write("<hr>prof is instance of Persoana: "+ (prof instanceof Persoana));
document.write("<hr>s is instance of Profesor: "+ (s instanceof Profesor));
document.write("<hr>p is instance of Object: "+ (p instanceof Object));

document.write("<hr>p.__proto__ == Persoana.prototype : "+ (p.__proto__ == Persoana.prototype));
document.write("<hr>s.__proto__ == Persoana.prototype : "+ (s.__proto__ == Persoana.prototype));
document.write("<hr>s.__proto__.__proto__ == Persoana.prototype : "+ (s.__proto__.__proto__ == Persoana.prototype));


