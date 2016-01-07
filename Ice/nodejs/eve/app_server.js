var express = require('express');
var bodyParser = require('body-parser');
var session = require('express-session');
var cookieParser = require('cookie-parser');
var AppMgr = require('./appmgr');

var app_svr = express();

app_svr.use(session({
	resave:false,
	saveUninitialized:true,
	secret:"keyboard cat",
}));
app_svr.use(bodyParser.json({limit: '10mb'})); 
app_svr.use(bodyParser.urlencoded({extended: true}));

app_svr.use(function(req, res, next) {
	console.log("url:" + req.originalUrl);
	if (req.session.app) {
		console.log("find app:" + req.session.app.appid);
		next();
	} else {
		AppMgr.getApp(req.hostname, req.originalUrl, function(app){
			console.log(444444);
			req.session.app = app;
			console.log(app);
			next();
		});
	}
});

app_svr.use(express.static("webapp",{index:"index.html"}));


app_svr.all('/svrcmd/:reqid', function(req, res) {
	if (req.params.reqid == "login"){
		req.session.userinfo= req.body;
		res.json({status:"success"});
	}
	if (req.params.reqid == "userinfo")
	{
		console.log(req.session.userinfo);
		res.json(req.session.userinfo);
	}
	if (req.params.reqid == "createdata")
	{
		console.log(req.session.userinfo);
		res.json(req.session.userinfo);
	}
	if (req.params.reqid == "modifydata")
	{
		console.log(req.session.userinfo);
		res.json(req.session.userinfo);
	}
	if (req.params.reqid == "retrievedata")
	{
		console.log(req.session.userinfo);
		res.json(req.session.userinfo);
	}
	if (req.params.reqid == "querydata")
	{
		console.log(req.session.userinfo);
		res.json(req.session.userinfo);
	}
});

app_svr.use(function(req, res, next) {
	console.log("404 url:" + req.originalUrl);
	if (req.session.app) {
		var app = req.session.app;
		app.loadResource(req.originalUrl, function (file){
			if (err) res.send('404');
			res.sendFile(file);
		});
	}
	res.send("404");
});


AppMgr.start(function() {
	var server = app_svr.listen(3000, function() {
		var host = server.address().address;
		var port = server.address().port;
		console.log("started")
	});
	server.on('close', function() {
		AppMgr.stop();
	});
});
