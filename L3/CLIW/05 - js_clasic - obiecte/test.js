function Persoana() {
  this.nume = '';
  this.cnp = '123123';
}


function Student(){
    this.an = 1;
}


var p = new Persoana();
document.write("p is of type " + typeof(p));
p.prop = "test";
document.write("<br>p.prop = " + p.prop);
document.write("<br>p.cnp = " + p.cnp);


var s = new Student();
document.write("<hr>s is of type " + typeof(s));

