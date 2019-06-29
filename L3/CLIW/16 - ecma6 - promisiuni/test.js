

function http(url, method){

	var promise = new Promise( function(resolve, reject){ // resolve = ok, reject = fail
			
			setTimeout(function()
			{
				var raspuns='raspuns'; // incearca sa scoti asta
				if(raspuns){
					resolve(raspuns);
				} 
				else
				{
					reject('Nu am primit raspuns');
				}
			}, 1000);										
	});
	return promise;
}

http('http://google.com', 'GET')
   .then(function(rasp){console.log(rasp)})
   .catch(function(mesaj){console.log(mesaj)});




/*
function http(url, method){

	var promise = new Promise( function(resolve, reject){ // resolve = ok, reject = fail
			
			let myFunc = function()
			{
				var raspuns='raspuns';
				if(raspuns){
					resolve(raspuns);
				} 
				else
				{
					reject('Nu am primit raspuns');
				}
			}

			setTimeout(myFunc, 1000);										
	});
	return promise;
}

http('http://google.com', 'GET')
   .then(function(rasp){return rasp.toUpperCase()})
   .then(function(raspunsModificat){console.log(raspunsModificat)})
   .catch(function(mesaj){console.log(mesaj)});
*/



 /* 
  // source: https://jsfiddle.net/fracz/kf8c6t1v/
  function loadImage(url) {
    return new Promise((resolve, reject) => {
      let img = new Image();
      img.addEventListener('load', e => resolve(img));
      img.addEventListener('error', () => {
        reject(new Error(`Failed to load image's URL: ${url}`));
      });
      img.src = url;      
    });    
  }

 
  loadImage('http://thecatapi.com/api/images/get?format=src&type=jpg&size=big')
    .then(img => document.getElementById('matze').appendChild(img))
    .catch(error => console.error(error));
*/	



























 /*
  loadImage('http://thecatapi.com/api/images/get?format=src&type=jpg&size=big')
    .then(function(img){document.getElementById('matze').appendChild(img); console.log(img.width+" X "+img.height);})
    .catch(error => console.error(error));
*/