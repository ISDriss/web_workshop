const connectButton = document.getElementById('connectBtn');
const sendButton = document.getElementById('sendBtn');
const messageInput = document.getElementById('clientInput');
const outputDiv = document.getElementById('serverOutput');

let socket = null;

connectButton.addEventListener('click', () => {
  const domainName = '10.1.224.116'; // Replace with your domain or DDNS service
  socket = new WebSocket(`ws://${domainName}`);

  socket.onopen = () => {
    console.log('Connection established');
    appendMessageToOutput('Connection established', '#FF4D00',);
  };

  socket.onclose = () => {
    console.log('Connection closed');
    appendMessageToOutput('Connection closed', '#FF4D00',);
  };

  socket.onmessage = (event) => {
    console.log(`Server_Message: ${event.data}`);
    appendMessageToOutput(event.data, '#009FFD', 'server');
  };

  socket.onerror = (error) => {
    console.log(`Error: ${error}`);
    appendMessageToOutput(`Error: ${error}`, '#FF0000', 'server');
  };
});

sendButton.addEventListener('click', () => {
  const message = messageInput.value;

  // Only send if the message isn't empty and the connection is open
  if (message.trim() !== '' && socket && socket.readyState === WebSocket.OPEN) {
    socket.send(message);

    // Append the client's message to the output div
    appendMessageToOutput(message, '#0CCA4A', 'client');
    messageInput.value = ''; // Clear the input after sending the message
  }
});

/**
 * Function to append messages to the output div
 * @param {string} message - The message to display
 * @param {string} color - font color
 * @param {string} type - 'client' for client's messages, 'server' for server's messages
 */
function appendMessageToOutput(message, color, type) {
  // Create a new span element to hold the message
  const messageSpan = document.createElement('span');
  messageSpan.style.color = color;
  message = message + '\r\n';

  // Apply different styles based on the message type
  if (type === 'client') { message = 'C> ' + message;} 
  else if (type === 'server') { message = 'S> ' + message;}
  messageSpan.textContent = message;

  // Append the message span to the output div
  outputDiv.appendChild(messageSpan);

  // Ensure the latest message is always visible by scrolling to the bottom
  outputDiv.scrollTop = outputDiv.scrollHeight;
}
