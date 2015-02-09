#!/usr/bin/python
# Filename: helloworld.py

import socket;
import struct;
import sys;


filename = "run.xml";
if (len(sys.argv) == 2):
	filename = sys.argv[1];
file = open(filename);
print filename;
request = file.read();
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM);
sock.connect(('127.0.0.1', 5555));
#request = r"<request><item name='operation'><![CDATA[retrieveFullVpd]]></item><item name='subOpr'><![CDATA[publish]]></item><item name='sendto'><![CDATA[htmlserver]]></item><item name='userurl'><![CDATA[http://192.168.99.96:8083/cuteEditor.yyy]]></item><item name='url'><![CDATA[cuteEditor.yyy]]></item></request>";
slen = socket.htonl(len(request));
print "Request:"
print request;
bytes=struct.pack('I',slen);
sock.send(bytes);
sock.send(request);
rlen, = struct.unpack('I',sock.recv(4));
rlen = socket.ntohl(rlen);
response = sock.recv(rlen);
print "\nResponse:"
print response;
sock.close();
