var config = {
	address: "localhost", 	// Address to listen on, can be:
							// - "localhost", "127.0.0.1", "::1" to listen on loopback interface
							// - another specific IPv4/6 to listen on a specific interface
							// - "0.0.0.0", "::" to listen on any interface
							// Default, when address config is left out or empty, is "localhost"
	port: 8081,
	basePath: "/", 	// The URL path where MagicMirror is hosted. If you are using a Reverse proxy
					// you must set the sub path here. basePath must end with a /
	ipWhitelist: ["127.0.0.1", "::ffff:127.0.0.1", "::1"], 	// Set [] to allow all IP addresses
															// or add a specific IPv4 of 192.168.1.5 :
															// ["127.0.0.1", "::ffff:127.0.0.1", "::1", "::ffff:192.168.1.5"],
															// or IPv4 range of 192.168.3.0 --> 192.168.3.15 use CIDR format :
															// ["127.0.0.1", "::ffff:127.0.0.1", "::1", "::ffff:192.168.3.0/28"],

	useHttps: false, 		// Support HTTPS or not, default "false" will use HTTP
	httpsPrivateKey: "", 	// HTTPS private key path, only require when useHttps is true
	httpsCertificate: "", 	// HTTPS Certificate path, only require when useHttps is true

	language: "en",
	locale: "en-US",
	logLevel: ["INFO", "LOG", "WARN", "ERROR"], // Add "DEBUG" for even more logging
	timeFormat: 12,
	units: "imperial",
	// serverOnly:  true/false/"local" ,
	// local for armv6l processors, default
	//   starts serveronly and then starts chrome browser
	// false, default for all NON-armv6l devices
	// true, force serveronly mode, because you want to.. no UI on this device

	modules: [
		{
			module: "alert",
		},
		{
			module: "updatenotification",
			position: "top_bar"
		},
		{
			module: "clock",
			position: "top_left"
		},
		{
			module: "calendar",
			header: "Calendar",
			position: "top_left",
			config: {
				calendars: [
					{
						symbol: "calendar-check",
						url: "https://calendar.google.com/calendar/u/0?cid=c3VoYXNyYWphMUBnbWFpbC5jb20"					}
				]
			}
		},
		{
			module: "weather",
			position: "top_right",
			header: "Weather Forecast",
			config: {
				weatherProvider: "openweathermap",
				type: "forecast",
				location: "Cambridge",
				locationID: "4931972", //ID from http://bulk.openweathermap.org/sample/city.list.json.gz; unzip the gz file and find your city
				apiKey: "c70dcbaffd0f60b2ff4b15cf3b49bfec"
			}
		},
		{
			module: "newsfeed",
			position: "bottom_bar",
			config: {
				feeds: [
					{
						title: "New York Times",
						url: "https://rss.nytimes.com/services/xml/rss/nyt/HomePage.xml"
					}
				],
				showSourceTitle: true,
				showPublishDate: true,
				broadcastNewsFeeds: true,
				broadcastNewsUpdates: true
			}
		},
		{
			module: 'MMM-MBTA',
			position: 'top_right', // This can be any of the regions.
			header: "MBTA:",
			config: {
				apikey: '9e415cb34b1f498187a3c9dc3a71c090',
				stations: [ "Davis Square" ]
			}
		},
/*		{
			module: 'MMM-Todoist',
			position: 'top_left',	// This can be any of the regions. Best results in left or right regions.
			header: 'Todoist', // This is optional
			config: { // See 'Configuration options' for more information.
				hideWhenEmpty: false,
				accessToken: '5a3709d31da6dde7a45b89e26e269cca1e631fa9',
				maximumEntries: 60,
				updateInterval: 10*60*1000, // Update every 10 minutes
				fade: false,      
			// projects and/or labels is mandatory:
				projects: [ "1490442406"], // Tasks for any projects with these labels will be shown.
				labels: ["WF"]
      			}
		},
*/		{
			module: "MMM-NowPlayingOnSpotify",
			position: "bottom_left",
			config: {
				clientID: "7d019b741f7e45baabbc0217e07dcd69",
				clientSecret: "83538911b8df42ddbd82b2bdd353d88c",
				accessToken: "BQCy46syFj57GmNQb12ZqTtO_ck7PtBnPZKDTITgRu-Zr0vg8vxWq4Rr0NYUqtr1MsDwxVtcrcK77nZk6qMyyKi2u7zZ6TwkdpduVUQMEd2g7ItC9O_0mlq9oc0zhTxSbAatzL4vql0FyZoH0tX4VpeCqLJZvQTe3OblhXz035OIDw",
				refreshToken: "AQAsAMei0ebiM8tlcIcjyvyzPdEGDu1rYtlSVjNh1DLRKwK8tZLjCZ3xDpub9Sm4-LgeaMxFj6iwjMbPM7dL4VsCVOFuZUYuSft7I8GpI2jHpVRWwbh1FiY3OmZ6Wv2Y3tg"
			}
		},
		{
			module: 'MMM-zoom',
			config: {
				zoom: 1
			}
		},
/*		{
			module: 'DailyXKCD',
			position: 'top_left',
			config: {
				invertColors: true,
				showTitle: false,
				limitComicHeight:300,
				scrollInterval:30000,
				showAltText: false
			}
		},*/
	]
};

/*************** DO NOT EDIT THE LINE BELOW ***************/
if (typeof module !== "undefined") {module.exports = config;}
