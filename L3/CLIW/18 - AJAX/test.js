
function loadDoc() {
  var xhttp = new XMLHttpRequest();

  xhttp.timeout = 2000;

  xhttp.onreadystatechange = function() {
  	// readyState: 0=unsent, 1=opened, 2=headers_received, 3=loading, 4=done
  	console.log(this.readyState);
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("continut").innerHTML = this.responseText;
    }
  };

  xhttp.onload = function(){
  	console.log("S-a incarcat !");
  }

  xhttp.ontimeout = function(){
  	console.log("Au trecut 2 secunde si ... NIMIC !");
  }

  xhttp.open("GET", "test.txt", true); // de fapt aici este o adresa de genul c:/.... si nu are voie sa incarce
 
  xhttp.send();
}

loadDoc();
document.getElementById("continut").innerHTML = "dispare ?";
