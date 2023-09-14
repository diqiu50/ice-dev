var proxy = "PROXY 112.81.83.18:14576; DIRECT";


var proxyRules = [
    ".openai.com",
];

function FindProxyForURL(url, host) {
    for (var i = 0; i < proxyRules.length; i++) {
        if (dnsDomainIs(host, proxyRules[i])) {
            return proxy;
        }
    }

    return "DIRECT";
}