/*
 * sapporo_subway javascript
 */

/*
 * APIのURL
 */
var API_HOST = 'http://sapporo-subway-webapi.herokuapp.com';

/*
 * 緯度経度取得のタイムアウト用タイマーのId
 */
var checkStationTimeoutId;

/*
 * 緯度経度取得のタイムアウト時間
 */
var TIMEOUT_CHECK_UPDATE = 60 * 1000; 

/*
 * 移動軽度取得時のオプション
 */
var locationOptions = { 
    "timeout": 15000, 
    "maximumAge": 60000
}; 


/**
 * decode utf8
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


/*
 * decode utf8
 */
function decodeUTF8(data){
    var result = {};
    result["stationInfoName"] = decode_utf8(data.stationInfoName);
    result["stationInfoDir1"] = decode_utf8(data.stationInfoDir1);
    result["stationInfoDir2"] = decode_utf8(data.stationInfoDir2);
    result["stationInfoDir1Departure"] = 
        decode_utf8(data.stationInfoDir1Departure);
    result["stationInfoDir1Departure"] = 
        decode_utf8(data.stationInfoDir1Departure);
    result["stationInfoDir2Departure"] = 
        decode_utf8(data.stationInfoDir2Departure);
    return result;
}


/*
 * 最寄りの駅を探してくる
 */
function checkClosestStation(lat, lng) {
    var req = new XMLHttpRequest();
    req.open('GET', API_HOST + '/stations/closest/'+lat+'/'+lng+'/');
    req.onload = function(e) {
        if (req.readyState == 4) {
            if(req.status == 200) {
                console.log(req.responseText);

                var data = JSON.parse(req.responseText);
                if (data) {
                    var result = decodeUTF8(data);
                    Pebble.sendAppMessage(result);
                }

            } else {
                console.log("Error");
            }
        }
    };
    req.send(null);
}


/**
 * 移動軽度取得処理が成功した時の処理
 */
function checkStationLocationSuccess(pos) {
    var coordinates = pos.coords;
    checkClosestStation(coordinates.latitude, coordinates.longitude);
}


/**
 * 緯度経度取得処理が失敗した時の処理
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
 * 緯度経度を取得する
 */
function getGeoForCheckClosestStation() {
    window.navigator.geolocation.getCurrentPosition(checkStationLocationSuccess, checkStationLocationError, locationOptions);
}


/**
 * 最寄りの駅の情報取得を開始する
 * 移動軽度取得処理を開始し、タイムアウト用のタイマーを開始する
 */
function checkStationTimer() {
    getGeoForCheckClosestStation();
    checkStationTimeoutId = setTimeout(checkStationTimer, TIMEOUT_CHECK_UPDATE);
}


/**
 * ready
 */
Pebble.addEventListener("ready", function(e) {
    console.log("connect! e.ready=" + e.ready + " e.type=" + e.type);
});


/**
 * メッセージが届いた時の処理
 */
Pebble.addEventListener("appmessage", function(e) {
    console.log("=== receive message === ");
    console.log(e.type);
    console.log(e.payload.command);
    console.log("");

    var command = e.payload.command;
    switch (e.payload.command) {
        /* start */
        case 20: 
            checkStationTimer();
            break;

        /* stop(cancel) */
        case 21: 
            clearTimeout(checkStationTimeoutId);
            break;
    }

});


/**
 * 設定画面を閉じた時の処理
 */
Pebble.addEventListener("webviewclosed", function(e) {
    console.log("webview closed");
    console.log(e.type);
    console.log(e.response);
});


/**
 * 設定画面を開いた時の処理
 */
Pebble.addEventListener("showConfiguration", function(e) {
    console.log("showConfiguration");
});

