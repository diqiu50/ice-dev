var date = new Date();
var start = date.getTime()/1000;
var str = "";
for (var i=0; i<20000000; i++)
{
    str+=i;
}
date = new Date();
//console.log(str);
console.log(date.getTime()/1000-start);