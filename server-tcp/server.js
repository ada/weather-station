var net = require('net');
var binary = require('binary');
var serialport = require('serialport');
var WebSocketServer = require('ws').Server;
var HOST = '127.0.0.1';
var PORT = 81;
var vars;
var SerialPort = serialport.SerialPort;

// Web socket Server
var wss = new WebSocketServer({ port: 82 });
wss.on('connection', function connection(ws) {
  ws.send(JSON.stringify(vars));
});

// Connect to MCU
serialport.list(function (err, ports) {
  ports.forEach(function(port) {
    console.log(port.comName);
  });
});

var myPort = new SerialPort("/dev/cu.usbmodem1411", {
   baudRate: 115200,
   parser: serialport.parsers.readline("\n")
 });
 myPort.on('open', function showPortOpen() {
   console.log('port open. Data rate: ' + myPort.options.baudRate);
});
 myPort.on('data', function sendSerialData(data) {
   console.log(data);
   wss.clients.forEach(function each(client) {
     //client.send(JSON.stringify(vars));
     client.send(data);
   });
});
 myPort.on('close', function showPortClose() {
   console.log('port closed.');
});
 myPort.on('error', function showError(error) {
   console.log('Serial port error: ' + error);
});








const server = net.createServer(function (socket){
  socket.pipe(socket).on('data', function (data) {
    /*var buf = new Buffer(data);
    vars = binary.parse(buf)
        .word8('Kind_1')
        .word8('Kind_2')
        .word8('Kind_3')
        .word8('Kind_4')
        .word32ls('Value_Int')
        .vars;
*/

    wss.clients.forEach(function each(client) {
      //client.send(JSON.stringify(vars));
    });
    console.log(socketInfo(socket), vars);
  })
  .on('end', function () {
    console.log('Client disconnected');
  })
})

.on('connection', function(socket) {
  console.log(socketInfo(socket) + ' Connected.');
  //socket.write('Welcome!\r\n');
})

.on('error', function (err) {
  console.log(err);
})

.listen(PORT, HOST, function () {
  console.log('TCP Server listening on ' + server.address().address +':'+
      server.address().port);
});

function socketInfo(s){
  return s.remoteAddress +':'+ s.remotePort+': ';
}

/*
var HTTP_SERVER_HOST = "127.0.0.1";
var HTTP_SERVER_PORT = 7777;
var raw_request = "GET http://localhost:7777/ HTTP/1.1\nUser-Agent: Mozilla 5.0\nhost: localhost\nCookie: \ncontent-length: 0\nConnection: keep-alive";

var client = new net.Socket();
client.connect(HTTP_SERVER_PORT, HTTP_SERVER_HOST)
.on('data', function(data) {
    console.log(data);
    //client.destroy();
}).on('close', function() {
  console.log('socket Connection closed');
}).on('error', function (err) {
  console.log(HTTP_SERVER_HOST + ':' + HTTP_SERVER_PORT + " socket error.");
  console.log(err);
  //socket.write(data.toString()+' ACK\r\n');
  //client.write('GET /api/weather/post?device=1& HTTP/1.1\r\n\r\n');
});
*/
