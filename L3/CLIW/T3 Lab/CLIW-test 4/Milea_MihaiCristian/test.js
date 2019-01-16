
var obj;

function cleanList(){

	let keys = Object.keys(obj);
	let listLength = Object.keys(obj).length;
	if (listLength===0){
		alert("List already empty");
	}
	else{
		localStorage.setItem("myToDoList", JSON.stringify(new Object()));
		location.reload();
	}
}


function deleteFromList(who)
{

	let keys = Object.keys(obj);
	let listLength = Object.keys(obj).length;
	for(let i=0; i<listLength; i++)
	{	
		if(obj[keys[i]].reminder===obj[Object.keys(obj)[who]].reminder){
			delete obj[keys[i]];
			setLocal();
			location.reload();
			break;
		}
	}
}

function updateFromList(who)
{
	var withWhat = prompt("Change reminder", obj[Object.keys(obj)[who]].reminder);

	let keys = Object.keys(obj);
	let listLength = Object.keys(obj).length;
	for(let i=0; i<listLength; i++)
	{	
		if(obj[keys[i]].reminder===obj[Object.keys(obj)[who]].reminder){
			obj[Object.keys(obj)[who]].reminder=withWhat;
			setLocal();
			location.reload();
			break;
		}
	}
}


function displayReminders(){
	let listLength = Object.keys(obj).length;
	console.log(listLength + " objects:");
	let displayedList = document.getElementById("list");
	let keys = Object.keys(obj);
	if (listLength===0){
		displayedList.innerHTML = "The list is empty";
	}
	else{
		for(let i=0; i<listLength; i++)
		{		
			var listItem = document.createElement("li");
	        listItem.innerHTML = obj[keys[i]].reminder;	
	        listItem.innerHTML += " <button onclick='updateFromList(" + '"' + + i + '"' + ")'>&#x270e;</button>"
	        listItem.innerHTML +="<button onclick='deleteFromList(" + '"' + + i + '"' + ")'>&#x274C;</button>";


			displayedList.appendChild(listItem);


			console.log(i+": "+obj[keys[i]].reminder);
		}
	}
}

function setLocal()
{
	localStorage.setItem("myToDoList", JSON.stringify(obj));
}

function getLocal()
{
	var intro = localStorage.getItem("myToDoList");
	if (intro!=null) obj = JSON.parse(intro);
				else obj = new Object();
}

let buton = document.getElementById("btn");
buton.onclick = function()
{
	if(document.getElementById("txt").value===""){
		alert("You can`t add an empty note");
	}
	else{
		let listLength = Object.keys(obj).length;
		listLength++;
		while(obj["o"+listLength]!=null){
			listLength++;
		}
		let newObj = { status: "offline", timestamp: (new Date()).getTime(), reminder: document.getElementById("txt").value};
		obj["o"+listLength] = newObj;
		setLocal(JSON.stringify(obj));		
	}
}


getLocal();
displayReminders();