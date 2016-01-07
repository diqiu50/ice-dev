#coding=utf-8
import json
import re
import os
import sys
import urllib.request

def geturi():
    file = open("a.har")
    data = json.load(file)
    entries = data["log"]["entries"]
    pattern = re.compile(r'http://192.168.99.2/(.+)/(\w+\.(\w+)).*')
    for entry in entries:
        url = entry["request"]["url"]
        print(url)
        match = pattern.search(url)
        if match:
            print(match.group(3));
            if match.group(3) == "css" or match.group(3) == "png" or match.group(3) == "js":
                if os.path.exists(match.group(1)) == False:
                    os.makedirs(match.group(1))
                file = open(match.group(1) + "/" + match.group(2),"wb");
                response = urllib.request.urlopen(url).read()
                file.write(response);
        print()


#geturi();
file = open(sys.argv[1], "wb");
response = urllib.request.urlopen("http://192.168.99.2/" + sys.argv[1]).read()
file.write(response);
