const connectButton = document.getElementById('connect-button');
const sendButton = document.getElementById('send-button');
const messageInput = document.getElementById('message-input');
const outputDiv = document.getElementById('output');

let socket = null;

connectButton.addEventListener('click', () => {
  const domainName = 'XX.XX'; // Remplacez par votre domaine ou votre service DDNS
  socket = new WebSocket(`ws://${domainName}`);

  socket.onopen = () => {
    console.log('Connexion établie');
    outputDiv.innerHTML += 'Connexion établie<br>';
  };

  socket.onmessage = (event) => {
    console.log(`Message reçu : ${event.data}`);
    outputDiv.innerHTML += `Message reçu : ${event.data}<br>`;
  };

  socket.onclose = () => {
    console.log('Connexion fermée');
    outputDiv.innerHTML += 'Connexion fermée<br>';
  };

  socket.onerror = (error) => {
    console.log(`Erreur : ${error}`);
    outputDiv.innerHTML += `Erreur : ${error}<br>`;
  };
});

sendButton.addEventListener('click', () => {
  const message = messageInput.value;
  socket.send(message);
  messageInput.value = '';
});