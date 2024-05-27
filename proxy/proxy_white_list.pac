var proxy = 'SOCKS5 112.81.83.18:14578; SOCKS 112.81.83.18:14578; DIRECT;';

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
