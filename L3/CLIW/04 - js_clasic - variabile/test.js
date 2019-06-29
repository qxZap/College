var a = "string";

document.write("a= " + a);

document.write("<br>a este: "+typeof(a)+"<br><br>");



a = 2 + 3;

document.write("a= " + a);

document.write("<br>a este: "+typeof(a)+"<br><br>");



a = function(x, y){ return x+y;}

document.write(a(5,8));

document.write("<br>a este: "+typeof(a)+"<br><br>");



a = 5/0;

document.write("a= " + a);

document.write("<br>a este: "+typeof(a)+"<br><br>");




var nedefinit;

document.write(nedefinit);

document.write("<br>a este: "+typeof(nedefinit)+"<br><br>");




a = 5/nedefinit;

document.write(a);

document.write("<br>a este: "+typeof(a)+"<br><br>");




a = new String("String Object"); // de fapt si la inceput era String

document.write(a);

document.write("<br>a este: "+typeof(a)+"<br><br>");

//https://developer.mozilla.org/ro/docs/Web/JavaScript/Reference/Global_Objects/String





a = [1,2,3,4];

document.write(a);

document.write("<br>a este: "+typeof(a)+"<br><br>");




a = new Object(); // de fapt si la inceput era String

document.write(a);

document.write("<br>a este: "+typeof(a)+"<br><br>");






a = {attr1:"val1", attr2:"val2"};

document.write(a);

document.write("<br>a este: "+typeof(a)+"<br><br>");






document.write("Operatori: ca cei din C++ / Java"+"<br><br>");

document.write("Sa citim <a href=\"https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Details_of_the_Object_Model\">asta</a>"); 