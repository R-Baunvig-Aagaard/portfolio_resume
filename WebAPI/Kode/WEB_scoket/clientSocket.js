// NOTE: ROUTE IS CHAT
const socket = new WebSocket('ws://localhost:8080/chat');

// Connection opened
socket.addEventListener('open', function (event) {

// Sending a message to the web socket server...
socket.send('Hello Server!');
});

// Listen for messages
socket.addEventListener('message', function (message) {
console.log('Message from server ', message.data);

// DOM
document.getElementById("test").innerHTML = message.data;

});
