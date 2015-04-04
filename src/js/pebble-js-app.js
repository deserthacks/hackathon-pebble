var API_ROOT = 'http://api.deserthacks.org';
var oauth = 'XXXXXXXXXXXXXXXXXXXXXXXXXXXX';
var timelineToken;

Pebble.addEventListener('ready', function() {
  timelineToken = Pebble.getTimelineToken(
    function(token) {
      timelineToken = token;
    },
    function(err) {
      console.log('Couldn\'t get token');
    });
});

Pebble.addEventListener('showConfiguration', function() {
  console.log('showing configuration');
  Pebble.openURL('http://deserthacks.org/login?pebbleTimeline='+encodeURIComponent(JSON.stringify(timelineToken)));
});

Pebble.addEventListener('webviewclosed', function(e) {
  console.log('configuration closed');
  var res = JSON.parse(decodeURIComponent(e.response));
  localStorage.setItem(0, res);
});

Pebble.addEventListener('appmessage', function(e) {
  console.log('Received message: ' + JSON.stringify(e.payload));

  if (e.payload.action) {
    switch(e.payload.action) {
      case 3:

        break;
      case 4:

        break;
      case 5:

        break;
      case 6:

        break;
    }
  }
});