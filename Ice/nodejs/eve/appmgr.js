'use strict'

var mysql = require('mysql');                    
var Application = require('./application');                    
var async = require('async')

var db = mysql.createConnection({
		host : '192.168.99.184',
		user : 'root',
		password : '12345',
		database : 'ice_db'
});

var AppMgr = {
	smApps:[],
	smMapping:[],
	start: function(callback) {
		db.connect(); 
		db.query("SELECT * from apps_t where app_id=100000",
			function(err, rows, fields){
				if (err) throw err;
				AppMgr.smApps[100000] = Application.create(100000, rows, db);
				callback();
			});
	},
	createApp: function(hostname, url, callback){
		var app;
		if (url == '/') {
			console.log("create def app:");
			app = AppMgr.smApps[100000];
			callback(app);
			return;
		}

		var key = hostname + "_" + url;	
		var appid = AppMgr.smMapping[key];
		if (appid) {
			callback(AppMgr.smApps[appid]);
		} else {
			//console.log("SELECT * from apps_t where hostname='" + hostname + "' and url='" + url + "'");
			db.query("SELECT * from apps_t where hostname='" + hostname + "' and url='" + url + "'",
				function(err, rows, fields){
					if (err) throw err;
					if (rows.length == 1) {
						console.log("create app: " + rows[0].app_id);
						app = Application.create(rows[0].app_id, rows[0], db);
						AppMgr.smApps[rows[0].app_id] = app;
					} else {
						console.log("can't create app: ");
					}
					callback(app);
				});
		}
	},
	getApp: function(appid){
		return AppMgr.smApps[appid];
	},
	stop : function() {
		db.end();
	}
}


module.exports = AppMgr;
