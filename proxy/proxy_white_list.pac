var proxy = 'SOCKS5 71.136.70.163:14578; SOCKS 71.136.70.163:14578; DIRECT;';

var proxyRules = [
    "openai.com",
    "chatgpt.com",
];

function FindProxyForURL(url, host) {
    for (var i = 0; i < proxyRules.length; i++) {
        if (host == proxyRules[i]|| host.endsWith('.' + proxyRules[i])) {
            return proxy;
        }
    }

    return "DIRECT";
}
