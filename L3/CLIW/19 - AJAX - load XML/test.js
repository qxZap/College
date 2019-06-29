

function loadDoc() {
  var xhttp = new XMLHttpRequest();

  xhttp.onreadystatechange = function() {
  	console.log(this.readyState);
    if (this.readyState == 4 && this.status == 200) {
        var parser = new DOMParser();
		var xmlDoc = parser.parseFromString(this.responseText,"text/xml");
		//console.log(xmlDoc.getElementsByTagName("img")[0].childNodes[3].childNodes[0].nodeValue);

		document.getElementById("b1").src=xmlDoc.getElementsByTagName("img")[0].childNodes[3].childNodes[0].nodeValue;
		document.getElementById("b2").src=xmlDoc.getElementsByTagName("img")[1].childNodes[3].childNodes[0].nodeValue;

		/*  //better approach:
		let pictures = xmlDoc.getElementsByTagName("img");
		for(i=0; i<pictures.length; i++)
		{
			let img = document.createElement('img');
			img.src = xmlDoc.getElementsByTagName("img")[i].childNodes[3].childNodes[0].nodeValue;
			img.style.width="640px";
			document.getElementById("container").appendChild(img);
		}
		*/
    }
  };

  xhttp.open("GET", "test.xml", true); // de fapt aici este o adresa de genul c:/.... si nu are voie sa incarce 
  xhttp.send();
}

loadDoc();
