
var Application = {

	create : function(appid, app_conf, db) {
		var app = {};
		app.appid = appid;
		app.appname = app_conf["appname"];
		app.mDbConn = db;
		app.mResources = [],
		app.loadResource: function(url, callback) {
						
		}
		return app;
	},
}

module.exports = Application;
