var net = require('net');

var client = net.connect({port:5565},
                         function(){
                             console.log('client connected');
                             var str = "<request><item name='operation'><![CDATA[retrieveFullVpd]]></item><item name='subOpr'><![CDATA[publish]]></item><item name='sendto'><![CDATA[htmlserver]]></item><item name='userurl'><![CDATA[http://192.168.99.96:8083/cuteEditor.yyy你好]]></item><item name='url'><![CDATA[cuteEditor.yyy]]></item></request>";
        					 console.log(str.length);
        					 console.log(Buffer.byteLength(str));
                             var buff = new Buffer(Buffer.byteLength(str)+4);
                             buff.writeInt32BE(Buffer.byteLength(str), 0);
                             buff.write(str, 4);
        					 console.log(buff.length);
                             client.write(buff);                             
                         });

client.on('data', function(data){
		var length = data.readInt32BE(0);
        console.log(length);
        console.log(data.toString('utf-8',4, 4+length));
        client.end();
    });

client.on('end', function() {
        console.log('msg read!');
	});

client.on('error', function(){
        console.log('error');
    });
