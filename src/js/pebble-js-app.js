/*
 * pebble-planset javascript
 *
 */

/**
 *
 */
var checkMessageTimeoutId;
var TIMEOUT_CHECK_UPDATE = 60 * 1000;
var prev_message;

var checkStationTimeoutId;
var TIMEOUT_CHECK_UPDATE = 60 * 1000; 

var locationOptions = { "timeout": 15000, "maximumAge": 60000 }; 


/**
 * 
 */
function decode_utf8(s) {
    try {
        s = unescape(encodeURIComponent(s));
    } catch(e){
        console.log('Error: s = unescape(encodeURIComponent(s))');
    }
    try {
        return decodeURIComponent(escape(s));
    }catch(e){
        console.log('Error: decode_utf8: ' + s);
        return s
    }
}

/**
 * 
 */
function checkUpdateMessages() {
    var response;
    var req = new XMLHttpRequest();
    req.open('GET', 'http://dkpyn.com/pebble/messages');
    req.onload = function(e) {
        if (req.readyState == 4) {
            if(req.status == 200) {
                console.log(req.responseText);

                var messages = JSON.parse(req.responseText);
                if (messages) {
                    var result = {};
                    console.log(messages.length);
                    result["message"] = decode_utf8(messages[0].message);
                    console.log(result["message"]);
                    Pebble.sendAppMessage(result);
                }

            } else {
                console.log("Error");
            }
        }
    }
    req.send(null);
}

/**
 * 
 */
function checkUpdateTimer() {
    checkUpdateMessages();
    checkMessageTimeoutId = setTimeout(checkUpdateTimer, TIMEOUT_CHECK_UPDATE);
}


/**
 * 
 */
function checkClosestStation(lat, lng) {
    var response;
    var req = new XMLHttpRequest();
    req.open('GET', 'http://dkpyn.com/pebble/stations/closest/'+lat+'/'+lng+'/');
    req.onload = function(e) {
        if (req.readyState == 4) {
            if(req.status == 200) {
                console.log(req.responseText);

                var data = JSON.parse(req.responseText);
                if (data) {
                    var result = {};
                    result["stationInfoName"] = decode_utf8(data.stationInfoName);
                    result["stationInfoDir1"] = decode_utf8(data.stationInfoDir1);
                    result["stationInfoDir2"] = decode_utf8(data.stationInfoDir2);
                    result["stationInfoDir1Departure"] = decode_utf8(data.stationInfoDir1Departure);
                    result["stationInfoDir1Departure"] = decode_utf8(data.stationInfoDir1Departure);
                    result["stationInfoDir2Departure"] = decode_utf8(data.stationInfoDir2Departure);
                    Pebble.sendAppMessage(result);
                }

            } else {
                console.log("Error");
            }
        }
    }
    req.send(null);
}

/**
 * 
 */
function checkStationLocationSuccess(pos) {
    var coordinates = pos.coords;
    checkClosestStation(coordinates.latitude, coordinates.longitude);
}

/**
 * 
 */
function checkStationLocationError(err) {
    console.warn('location error (' + err.code + '): ' + err.message);
    var result = {};
    result["stationInfoName"] = '';
    result["stationInfoDir1"] = '';
    result["stationInfoDir2"] = '';
    result["stationInfoDir1Departure"] = 'error';
    result["stationInfoDir2Departure"] = '';
    Pebble.sendAppMessage(result);
}

/**
 * 
 */
function getGeoForCheckClosestStation() {
    window.navigator.geolocation.getCurrentPosition(checkStationLocationSuccess, checkStationLocationError, locationOptions);
}

/**
 * 
 */
function checkStationTimer() {
    getGeoForCheckClosestStation();
    checkMessageTimeoutId = setTimeout(checkStationTimer, TIMEOUT_CHECK_UPDATE);
}


/**
 * 
 */
function locationSuccess(pos) {
    var coordinates = pos.coords;
    fetchWeather(coordinates.latitude, coordinates.longitude);
}

/**
 * 
 */
function locationError(err) {
    console.warn('location error (' + err.code + '): ' + err.message);
    Pebble.sendAppMessage({
        "city":"Loc Unavailable",
        "temperature":"N/A"
    });
}

/**
 * 
 */
function downloadBinaryResource(imageURL, callback, errorCallback) {
    var req = new XMLHttpRequest();
    req.open("GET", imageURL,true);
    req.responseType = "arraybuffer";
    req.onload = function(e) {
        console.log("loaded");
        var buf = req.response;
        if(req.status == 200 && buf) {
            var byteArray = new Uint8Array(buf);
            var arr = [];
            for(var i=0; i<byteArray.byteLength; i++) {
                arr.push(byteArray[i]);
            }

            console.log("Received image with " + byteArray.length + " bytes.");
            callback(arr);
        }
        else {
          errorCallback("Request status is " + req.status);
        }
    }
    req.onerror = function(e) {
      errorCallback(e);
    }
    req.send(null);
}

/**
 * 
 */
function transferImageBytes(bytes, chunkSize, successCb, failureCb) {
  var retries = 0;

  success = function() {
    console.log("Success cb=" + successCb);
    if (successCb != undefined) {
      successCb();
    }
  };
  failure = function(e) {
    console.log("Failure cb=" + failureCb);
    if (failureCb != undefined) {
      failureCb(e);
    }
  };

  // This function sends chunks of data.
  sendChunk = function(start) {
    var txbuf = bytes.slice(start, start + chunkSize);
    console.log("Sending " + txbuf.length + " bytes - starting at offset " + start);
    Pebble.sendAppMessage({ "NETIMAGE_DATA": txbuf },
      function(e) {
        // If there is more data to send - send it.
        if (bytes.length > start + chunkSize) {
          sendChunk(start + chunkSize);
        }
        // Otherwise we are done sending. Send closing message.
        else {
          Pebble.sendAppMessage({"NETIMAGE_END": "done" }, success, failure);
        }
      },
      // Failed to send message - Retry a few times.
      function (e) {
        if (retries++ < 3) {
          console.log("Got a nack for chunk #" + start + " - Retry...");
          sendChunk(start);
        }
        else {
          failure(e);
        }
      }
    );
  };

  // Let the pebble app know how much data we want to send.
  Pebble.sendAppMessage({"NETIMAGE_BEGIN": bytes.length },
    function (e) {
      // success - start sending
      sendChunk(0);
    }, failure);

}


/**
 * 
 */
Pebble.addEventListener("ready", function(e) {
    console.log("connect! e.ready=" + e.ready + " e.type=" + e.type);
    //locationWatcher = window.navigator.geolocation.watchPosition(locationSuccess, locationError, locationOptions);
});

/**
 * 
 */
Pebble.addEventListener("appmessage", function(e) {
    //window.navigator.geolocation.getCurrentPosition(locationSuccess, locationError, locationOptions);

    console.log("=== receive message === ");
    console.log(e.type);
    console.log(e.payload.command);
    console.log("");

    var command = e.payload.command;
    switch(e.payload.command) {
        case 10: checkUpdateTimer(); break;
        case 11: clearTimeout(checkMessageTimeoutId); break;
        case 20: checkStationTimer(); break;
        case 21: clearTimeout(checkStationTimeoutId); break;
        case 30: 
            if (transferInProgress == false) {
                transferInProgress = true;
                downloadBinaryResource(e.payload['NETIMAGE_URL'], function(bytes) {
                    transferImageBytes(bytes, e.payload['NETIMAGE_CHUNK_SIZE'],
                    function() { console.log("Done!"); transferInProgress = false; },
                    function(e) { console.log("Failed! " + e); transferInProgress = false; }
                    );
                });
            } else {
                console.log("Ignoring request to download " + e.payload['NETIMAGE_URL'] + " because another download is in progress.");
            }
            break;
    }

});

/**
 * 
 */
Pebble.addEventListener("webviewclosed", function(e) {
    console.log("webview closed");
    console.log(e.type);
    console.log(e.response);
});

/**
 * 
 */
Pebble.addEventListener("showConfiguration", function(e) {
    console.log("showConfiguration");
});

