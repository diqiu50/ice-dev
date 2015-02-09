var dgram = require('dgram');
var message = new Buffer("<request><item name='operation'><![CDATA[retrieveFullVpd]]></item><item name='subOpr'><![CDATA[publish]]></item><item name='sendto'><![CDATA[htmlserver]]></item><item name='userurl'><![CDATA[http://192.168.99.96:8083/cuteEditor.yyy你好]]></item><item name='url'><![CDATA[cuteEditor.yyy]]></item></request>");
var client = dgram.createSocket("udp4");
client.send(message, 0, message.length, 5555, "192.168.99.87", 
		function(err, bytes){
			//console.log("err:" + err + " , bytes:" + bytes );
		});

client.on("message", function(msg, rinfo) {
			console.log("server got: " + msg + " from " +
				    rinfo.address + ":" + rinfo.port);
			client.close();
		});

client.on("error", function(msg, rinfo) {
			console.log("server got: " + msg + " from " +
				    rinfo.address + ":" + rinfo.port);
			client.close();
		});
