var d = new Date();
var obj = {
 institutie: 'Facultatea de Informatica Iasi',
 dataCurenta: d.getDate()+'-'+(d.getMonth()+1)+'-'+d.getFullYear(),
 prezenti:['Gigel', 'Maria', 'Ionel', 'Gheorghe']
};

var {institutie, dataCurenta, prezenti} = obj;

var mesaj='Bine ati venit !';

var html=`<p> Astazi, ${dataCurenta}, la ${institutie}, facem laboratorul de CLIW</p><hr> 
<p><h4>Prezenti:</h4>
<ul>
	${prezenti.map(prezent => `<li>${prezent}</li>`).join('\n\t')}
</ul>
</p>
${mesaj}`;
document.getElementsByTagName("body")[0].innerHTML = html;

console.log(html);