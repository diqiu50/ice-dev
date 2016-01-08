'use strict'

var express = require('express');
var bodyParser = require('body-parser');
var session = require('express-session');
var cookieParser = require('cookie-parser');
var AppMgr = require('./appmgr');
var AppConfig = require('./config');

var app_svr = express();

app_svr.use(session({
	resave:false,
	saveUninitialized:true,
	secret:"keyboard cat",
}));
app_svr.use(bodyParser.json({limit: '10mb'})); 
app_svr.use(bodyParser.urlencoded({extended: true}));

app_svr.use(function(req, res, next) {
	console.log("access url:" + req.originalUrl);
	if (req.session.appid) {
		console.log("find app:");
		next();
		return;
	} else {
		console.log("create app:");
		AppMgr.createApp(req.hostname, req.originalUrl, function(app){
			if (app) req.session.appid = app.appid;
			next();
		});
	}
	return;
});

app_svr.use(express.static(AppConfig.web_base, {index:"index.html"}));


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
	if (req.session.appid) {
		var app = AppMgr.getApp(req.session.appid);
		app.loadResource(req.originalUrl, function (success, file){
			if (success) {
				var str = file["contents"];
				res.send(str);
			} else {
				res.send("404 " + req.originalUrl);
			}
		});
	}
	else
	{
		res.send("404 " + req.originalUrl);
	}
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
