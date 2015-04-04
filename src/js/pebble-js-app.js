var API_ROOT = 'http://api.deserthacks.org';
var oauth = 'XXXXXXXXXXXXXXXXXXXXXXXXXXXX';

Pebble.addEventListener('topics', function(e) {
  // Show config page
  Pebble.openURL('https://www.deserthacks.org/pebble');
});


Pebble.addEventListener("Added", function() {
  console.log("Adding to Timeline");
  initialized = true;
});

Pebble.addEventListener("showConfiguration", function() {
  console.log("showing configuration");
  Pebble.openURL('http://deserthacks.org/pebble'+encodeURIComponent(JSON.stringify(options)));
});

Pebble.addEventListener("webviewclosed", function(e) {
  console.log("configuration closed");
  // webview closed
  //Using primitive JSON validity and non-empty check
  if (e.response.charAt(0) == "{" && e.response.slice(-1) == "}" && e.response.length > 5) {
    options = JSON.parse(decodeURIComponent(e.response));
    console.log("Options = " + JSON.stringify(options));
  } else {
    console.log("Cancelled");
  }
});