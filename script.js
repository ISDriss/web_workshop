const connectButton = document.getElementById('connectBtn');
const sendButton = document.getElementById('sendBtn');
const messageInput = document.getElementById('clientInput');
const outputDiv = document.getElementById('serverOutput');

let socket = null;

connectButton.addEventListener('click', () => {
  const domainName = '10.1.224.116'; // Remplacez par votre domaine ou votre service DDNS
  socket = new WebSocket(`ws://${domainName}`);

  socket.onopen = () => {
    console.log('Connexion établie');
    outputDiv.innerHTML += 'Connexion établie\r\n';
  };

  socket.onmessage = (event) => {
    console.log(`Message reçu : ${event.data}`);
    outputDiv.innerHTML += `S> ${event.data}\r\n`;
  };

  socket.onclose = () => {
    console.log('Connexion fermée');
    outputDiv.innerHTML += 'Connexion fermée\r\n';
  };

  socket.onerror = (error) => {
    console.log(`Erreur : ${error}`);
    outputDiv.innerHTML += `Erreur : ${error}\r\n`;
  };
});

sendButton.addEventListener('click', () => {
  const message = messageInput.value;
  outputDiv.innerHTML += `C> ${message}\r\n`
  socket.send(message);
  messageInput.value = '';
});