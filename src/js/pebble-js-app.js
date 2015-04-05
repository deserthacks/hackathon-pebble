var API_ROOT = 'http://api.deserthacks.org';

var timelineToken;

Pebble.addEventListener('ready', function() {
  timelineToken = Pebble.getTimelineToken(
    function(token) {
      timelineToken = token;
      console.log(token);
    },
    function(err) {
      console.log('Couldn\'t get token');
    });
});

Pebble.addEventListener('showConfiguration', function() {
  console.log('showing configuration');
  Pebble.openURL(API_ROOT + '/login?pebbleTimeline='+encodeURIComponent(JSON.stringify(timelineToken)));
});

Pebble.addEventListener('webviewclosed', function(e) {
  console.log('configuration closed');
  var res = JSON.parse(decodeURIComponent(e.response));
  localStorage.setItem(0, res);
});

Pebble.addEventListener('appmessage', function(e) {
  console.log('Received message: ' + JSON.stringify(e.payload));
  if(e.payload.SUB_TOPIC_TALKS) {
    Pebble.timelineSubscribe('talk',
      function() {
        console.log('Great success!');
      },
      function(err) {
        if(err) return next(err);
      });
  }
  if(e.payload.SUB_TOPIC_EVENT) {
    Pebble.timelineSubscribe('event',
      function() {
        console.log('Great success!');
      },
      function(err) {
        if(err) return next(err);
      });
  }
  if(e.payload.SUB_TOPIC_FOOD) {
    Pebble.timelineSubscribe('food',
      function() {
        console.log('Great success!');
      },
      function(err) {
        if(err) return next(err);
      });
  }
  if(e.payload.SUB_TOPIC_REMINDER) {
    Pebble.timelineSubscribe('reminder',
      function() {
        console.log('Great success!');
      },
      function(err) {
        if(err) return next(err);
      });
  }
  if(e.payload.UNSUB_TOPIC_TALKS) {
    Pebble.timelineUnsubscribe('talk',
      function() {
        console.log('Great success!');
      },
      function(err) {
        if(err) return next(err);
      });
  }
  if(e.payload.UNSUB_TOPIC_EVENT) {
    Pebble.timelineUnsubscribe('event',
      function() {
        console.log('Great success!');
      },
      function(err) {
        if(err) return next(err);
      });
  }
  if(e.payload.UNSUB_TOPIC_FOOD) {
    Pebble.timelineUnsubscribe('food',
      function() {
        console.log('Great success!');
      },
      function(err) {
        if(err) return next(err);
      });
  }
  if(e.payload.UNSUB_TOPIC_REMINDER) {
    Pebble.timelineUnsubscribe('reminder',
      function() {
        console.log('Great success!');
      },
      function(err) {
        if(err) return next(err);
      });
  }
});