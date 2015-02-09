var ip = '192.168.99.86';
var http = require('http');
http.createServer(function (req, res) {
	console.log(req.headers["host"]);
	res.writeHead(200, {'Content-Type': 'text/plain'});
	res.end('Hello World\n');
}).listen(1337, ip);

console.log('Server running at http://' + ip + ':1337/');
