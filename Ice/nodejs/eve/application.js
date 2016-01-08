'use strict'

var fs = require('fs');
var AppConfig = require('./config');

var Application = {
	create : function(appid, app_conf, db) {
		var app = {};
		app.appid = appid;
		app.appname = app_conf["appname"];
		app.mDbConn = db;
		app.mResources = [],
		app.loadResource = function(url, callback){
			var path = "/" + this.appid + "/";
			if (url.indexOf(path) == 0) {
				url = url.substring(path.length-1);
			}
			if (url == "/") {
				url = "/index.html"
			}

			this.mDbConn.query("SELECT uri, contents from res_t where uri='" + 
					url + "' and app_id='" + this.appid + "'", function(err, rows, fields) {
				if (err) throw err;	
				if (rows.length != 1) {
					callback(false, "")
					return;
				}
				var filename = AppConfig.web_base + "/" + app.appid + rows[0].uri;
				var dirname = filename.substring(0, filename.lastIndexOf('/'));
				if (!fs.existsSync(dirname)) {
					fs.mkdirSync(dirname);
				}
				fs.writeFile(filename, rows[0].contents, function(err) {
					if (err) throw err;	
					console.log("write file: " + filename); 
					callback(true, rows[0]);
				});
			});
		};
		return app;
	}
};

module.exports = Application;
